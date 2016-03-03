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

#include "vtkGRASSRaster3dImageReader.h"

#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSRaster3dMapReader.h"
#include "vtkDataArray.h"
#include "vtkGRASSRaster3dMapBase.h"
#include "vtkGRASSMessagingInterface.h"

extern "C"
{
#include <grass/gis.h>
#include <math.h>
}

vtkStandardNewMacro(vtkGRASSRaster3dImageReader);

//----------------------------------------------------------------------------

vtkGRASSRaster3dImageReader::vtkGRASSRaster3dImageReader()
{
    this->DataExtent[0] = 0;
    this->DataExtent[1] = 255;
    this->DataExtent[2] = 0;
    this->DataExtent[3] = 255;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = 255;

    this->DataScalarType = VTK_DOUBLE;

    this->DataOrigin[0] = this->DataOrigin[1] = this->DataOrigin[2] = 0.0;
    this->DataSpacing[0] = this->DataSpacing[1] = this->DataSpacing[2] = 1.0;

    this->Raster3dName = NULL;
    this->Mapset = NULL;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);

    this->RegionUsage = VTK_GRASS_REGION_CURRENT;

    this->Raster3dMap = vtkGRASSRaster3dMapReader::New();
}

//----------------------------------------------------------------------------

vtkGRASSRaster3dImageReader::~vtkGRASSRaster3dImageReader()
{
    if (this->Raster3dName)
        delete [] this->Raster3dName;
    if (this->Mapset)
        delete [] this->Mapset;
    this->Raster3dMap->Delete();
}

//----------------------------------------------------------------------------

int
vtkGRASSRaster3dImageReader::RequestInformation(
                                              vtkInformation * vtkNotUsed(request),
                                              vtkInformationVector ** vtkNotUsed(inputVector),
                                              vtkInformationVector *outputVector)
{


    if (this->RegionUsage == VTK_GRASS_REGION_CURRENT)
    {
        this->Raster3dMap->UseCurrentRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT)
    {

        this->Raster3dMap->UseDefaultRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_RASTER)
    {
        this->Raster3dMap->UseRasterRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_USER)
    {
        this->Raster3dMap->UseUserDefinedRegion();
    }

    // The region settings are made in the OpenMap method.
    // So we need to open the map here to get the correct
    // region settings.
    if (!this->Raster3dMap->OpenMap(this->Raster3dName))
    {
        vtkErrorMacro( << "Error in vtkGRASSRaster3dMap: " << this->Raster3dMap->GetError());
        return 0;
    }

    if (this->Raster3dMap->GetMapType() == FCELL_TYPE)
    {
        this->SetDataScalarTypeToFloat();
    }
    else
    {
        this->SetDataScalarTypeToDouble();
    }

    // Init the image extent with the grass region settings
    this->DataExtent[0] = 0;
    this->DataExtent[1] = this->Raster3dMap->GetNumberOfCols() - 1;
    this->DataExtent[2] = 0;
    this->DataExtent[3] = this->Raster3dMap->GetNumberOfRows() - 1;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = this->Raster3dMap->GetNumberOfDepths() - 1;

    this->DataSpacing[0] = this->Raster3dMap->GetRegion()->GetEastWestResolution();
    this->DataSpacing[1] = this->Raster3dMap->GetRegion()->GetNorthSouthResolution();
    this->DataSpacing[2] = this->Raster3dMap->GetRegion()->GetTopBottomResolution();

    this->DataOrigin[0] = this->Raster3dMap->GetRegion()->GetWesternEdge() + this->Raster3dMap->GetRegion()->GetEastWestResolution()/2.0;
    this->DataOrigin[1] = this->Raster3dMap->GetRegion()->GetSouthernEdge() + this->Raster3dMap->GetRegion()->GetNorthSouthResolution()/2.0;
    this->DataOrigin[2] = this->Raster3dMap->GetRegion()->GetBottom() + this->Raster3dMap->GetRegion()->GetTopBottomResolution()/2.0;

    // get the info objects
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    outInfo->Set(vtkDataObject::SPACING(), this->DataSpacing, 3);
    outInfo->Set(vtkDataObject::ORIGIN(), this->DataOrigin, 3);
    outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
                 this->DataExtent, 6);
    vtkDataObject::SetPointDataActiveScalarInfo(outInfo, this->DataScalarType, 1);
    return 1;
}

//----------------------------------------------------------------------------

template<class T>
void
vtkGRASSRaster3dImageReaderExecute(vtkGRASSRaster3dImageReader *self,
                                 vtkImageData *data, T *outPtr,
                                 int outExt[6])
{

    int idxX, idxY, idxZ;
    vtkIdType outIncX, outIncY, outIncZ;
    vtkGRASSRaster3dMapReader *map = self->GetRaster3dMap();

    vtkGRASSMessagingInterface *gm = vtkGRASSMessagingInterface::New();

    // Get increments to march through data
    data->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);

    int count = 0;
    // Loop through output pixel
    for (idxZ = outExt[4]; idxZ <= outExt[5]; idxZ++)
    {
        gm->Percent(count, outExt[5], 1);
        count++;

        for (idxY = outExt[2]; !self->GetAbortExecute() && idxY <= outExt[3]; idxY++)
        {
            for (idxX = outExt[0]; idxX <= outExt[1]; idxX++)
            {
                *outPtr++ = (T)map->GetValue(idxX, idxY, idxZ);
            }
            outPtr += outIncY;
        }
        outPtr += outIncZ;
    }

    gm->Delete();
}

//----------------------------------------------------------------------------

int
vtkGRASSRaster3dImageReader::RequestData(vtkInformation*,
                                         vtkInformationVector**,
                                         vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    vtkImageData *data = this->AllocateOutputData(outInfo->Get(vtkDataObject::DATA_OBJECT()),
                                                  outInfo);
    int *outExt = data->GetExtent();
    void *outPtr = data->GetScalarPointerForExtent(outExt);


    // Call the correct templated function for the output
    switch (this->GetDataScalarType()) {
        vtkTemplateMacro(vtkGRASSRaster3dImageReaderExecute(this, data,
                                                          static_cast<VTK_TT *> (outPtr),
                                                          outExt));
    default:
        vtkErrorMacro( << "Execute: Unknown data type");
    }

    // All work is done. now we can close the map
    this->Raster3dMap->CloseMap();
}

//----------------------------------------------------------------------------

void
vtkGRASSRaster3dImageReader::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "DataScalarType: " <<
        vtkImageScalarTypeNameMacro(this->DataScalarType) << "\n";
    os << indent << "DataExtent: (" << this->DataExtent[0] << ", "
        << this->DataExtent[1] << ", "
        << this->DataExtent[2] << ", "
        << this->DataExtent[3] << ", "
        << this->DataExtent[4] << ", "
        << this->DataExtent[5] << ")\n";
    os << indent << "DataSpacing: (" << this->DataSpacing[0] << ", "
        << this->DataSpacing[1] << ", "
        << this->DataSpacing[2] << ")\n";
    os << indent << "DataOrigin: (" << this->DataOrigin[0] << ", "
        << this->DataOrigin[1] << ", "
        << this->DataOrigin[2] << ")\n";
}
