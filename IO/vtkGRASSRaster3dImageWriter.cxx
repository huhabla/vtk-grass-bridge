/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include "vtkGRASSRaster3dImageWriter.h"

#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSRaster3dMapReader.h"
#include "vtkDoubleArray.h"

extern "C"
{
#include <grass/gis.h>
#include <math.h>
}

vtkStandardNewMacro(vtkGRASSRaster3dImageWriter);

//----------------------------------------------------------------------------

vtkGRASSRaster3dImageWriter::vtkGRASSRaster3dImageWriter()
{
    this->Raster3dName = NULL;
    this->Mapset = NULL;
    this->RegionUsage = VTK_GRASS_REGION_CURRENT;
    this->Raster3dMap = vtkGRASSRaster3dMapWriter::New();
}

//----------------------------------------------------------------------------

vtkGRASSRaster3dImageWriter::~vtkGRASSRaster3dImageWriter()
{
    if (this->Raster3dName)
        delete [] this->Raster3dName;
    if (this->Mapset)
        delete [] this->Mapset;
    this->Raster3dMap->Delete();
}

//----------------------------------------------------------------------------

template<class T>
void
vtkGRASSRaster3dImageWriterExecute(vtkGRASSRaster3dImageWriter *self, vtkImageData* input, T *ptr)
{
    int z, y, x;
    int dims[3];

    input->GetDimensions(dims);

    vtkGRASSRaster3dMapWriter *map = self->GetRaster3dMap();

    // Loop through output pixel
    for (z = 0; z < dims[2]; z++)
    {
        for (y =  0; !self->GetAbortExecute() && y < dims[1]; y++)
        {
            for (x = 0; x < dims[0]; x++)
            {
                map->PutValue(x, y, z, (double) (ptr[z * dims[1] * dims[0] + y * dims[0] + x]));
            }
        }
    }
}

//----------------------------------------------------------------------------

void
vtkGRASSRaster3dImageWriter::SimpleExecute(vtkImageData* input, vtkImageData* output)
{

    void *ptr = input->GetScalarPointer();
    int dims[3];

    input->GetDimensions(dims);

    VGB_CREATE(vtkGRASSRegion, region);

    // SetThe region of the raster map
    if (this->RegionUsage == VTK_GRASS_REGION_CURRENT)
    {
        region->ReadCurrentRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT)
    {
        region->ReadDefaultRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_USER)
    {
        region->DeepCopy(this->Raster3dMap->GetRegion());
    }

    // We need to adjust the region to the image dimension
    region->SetDepths(dims[2]);
    region->SetRows3d(dims[1]);
    region->SetCols3d(dims[0]);
    region->AdjustRegion3dResolution();

    this->Raster3dMap->SetRegion(region);
    this->Raster3dMap->UseUserDefinedRegion();

    // Set the map data type
    if (input->GetScalarType() == VTK_CHAR ||
        input->GetScalarType() == VTK_UNSIGNED_CHAR ||
        input->GetScalarType() == VTK_INT ||
        input->GetScalarType() == VTK_UNSIGNED_INT)
        this->Raster3dMap->SetMapTypeToDCELL();
    else if (input->GetScalarType() == VTK_FLOAT)
        this->Raster3dMap->SetMapTypeToFCELL();
    else
        this->Raster3dMap->SetMapTypeToDCELL();

    // Now open the map
    if (!this->Raster3dMap->OpenMap(this->Raster3dName))
    {
        vtkErrorMacro( << "Error in vtkGRASSRaster3dMap: " << this->Raster3dMap->GetError());
        return;
    }

    switch (input->GetScalarType()) {
        // This is simply a #define for a big case list. It handles all
        // data types VTK supports.
        vtkTemplateMacro(
                         vtkGRASSRaster3dImageWriterExecute(this, input, static_cast<VTK_TT *> (ptr)));
    default:
        vtkGenericWarningMacro("Execute: Unknown input ScalarType");
    }

    // All work is done. now we can close the map
    this->Raster3dMap->CloseMap();

    return;
}

//----------------------------------------------------------------------------

void
vtkGRASSRaster3dImageWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

}
