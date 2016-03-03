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

#include "vtkGRASSRasterImageWriter.h"

#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSRasterMapReader.h"
#include "vtkDoubleArray.h"
#include <vtkPointData.h>

extern "C"
{
#include <grass/gis.h>
#include <math.h>
}

vtkStandardNewMacro(vtkGRASSRasterImageWriter);

//----------------------------------------------------------------------------

vtkGRASSRasterImageWriter::vtkGRASSRasterImageWriter()
{
    this->RasterName = NULL;
    this->Mapset = NULL;
    this->RegionUsage = VTK_GRASS_REGION_CURRENT;
    this->RasterMap = vtkGRASSRasterMapWriter::New();
    this->NullValue = -999999;
    this->UseNullValue = 1;
}

//----------------------------------------------------------------------------

vtkGRASSRasterImageWriter::~vtkGRASSRasterImageWriter()
{
    if (this->RasterName)
        delete [] this->RasterName;
    if (this->Mapset)
        delete [] this->Mapset;
    this->RasterMap->Delete();
}

//----------------------------------------------------------------------------

template<class T>
void
vtkGRASSRasterImageWriterExecute(vtkGRASSRasterImageWriter *self, vtkImageData* input, T *ptr)
{
    int z, y, x;
    int dims[3];

    input->GetDimensions(dims);

    vtkGRASSRasterMapWriter *map = self->GetRasterMap();
    // Largest array available
    VGB_CREATE(vtkDoubleArray, row);
    row->SetNumberOfTuples(map->GetNumberOfCols());

    // Loop through output pixel
    for (z = 0; z < dims[2]; z++)
    {
        // We need to count from the top
        for (y = dims[1] - 1; !self->GetAbortExecute() && y >= 0; y--)
        {
            for (x = 0; x < dims[0]; x++)
            {
                //printf("z %i y %i x %i :: %g\n", z, y, x, (double) (ptr[z * dims[1] * dims[0] + y * dims[0] + x]));
                row->SetTuple1(x, (double) (ptr[z * dims[1] * dims[0] + y * dims[0] + x]));
            }
            map->PutNextRow(row);
        }
    }
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterImageWriter::SimpleExecute(vtkImageData* input, vtkImageData* output)
{

    void *ptr = input->GetPointData()->GetScalars()->GetVoidPointer(0);
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

    // We need to adjust the region to the image dimension
    region->SetRows(dims[1]);
    region->SetCols(dims[0]);
    region->AdjustRegionResolution();

    this->RasterMap->SetRegion(region);
    this->RasterMap->UseUserDefinedRegion();
    this->RasterMap->SetNullValue(this->NullValue);
    if(this->UseNullValue)
        this->RasterMap->UseNullValueOn();

    // Set the map data type
    if (input->GetScalarType() == VTK_CHAR ||
        input->GetScalarType() == VTK_UNSIGNED_CHAR ||
        input->GetScalarType() == VTK_INT ||
        input->GetScalarType() == VTK_UNSIGNED_INT)
        this->RasterMap->SetMapTypeToCELL();
    else if (input->GetScalarType() == VTK_FLOAT)
        this->RasterMap->SetMapTypeToFCELL();
    else if (input->GetScalarType() == VTK_DOUBLE)
        this->RasterMap->SetMapTypeToDCELL();
    else
        this->RasterMap->SetMapTypeToDCELL();

    // cout << input->GetScalarTypeAsString() << endl;

    // Now open the map
    if (!this->RasterMap->OpenMap(this->RasterName))
    {
        vtkErrorMacro( << "Error in vtkGRASSRasterMap: " << this->RasterMap->GetError());
        return;
    }

    // this->RasterMap->Print(cout);

    switch (input->GetScalarType()) {
        // This is simply a #define for a big case list. It handles all
        // data types VTK supports.
        vtkTemplateMacro(
                         vtkGRASSRasterImageWriterExecute(this, input, static_cast<VTK_TT *> (ptr)));
    default:
        vtkGenericWarningMacro("Execute: Unknown input ScalarType");
    }

    // All work is done. now we can close the map
    this->RasterMap->CloseMap();

    return;
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterImageWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

}
