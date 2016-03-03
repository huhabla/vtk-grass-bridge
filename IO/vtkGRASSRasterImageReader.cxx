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

#include "vtkGRASSRasterImageReader.h"

#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSRasterMapReader.h"
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

extern "C"
{
#include <grass/gis.h>
#include <math.h>
}

vtkStandardNewMacro(vtkGRASSRasterImageReader);

//----------------------------------------------------------------------------

vtkGRASSRasterImageReader::vtkGRASSRasterImageReader()
{
    this->DataExtent[0] = 0;
    this->DataExtent[1] = 255;
    this->DataExtent[2] = 0;
    this->DataExtent[3] = 255;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = 0;

    this->DataScalarType = VTK_DOUBLE;
    this->MapTypeConversion = 0; // Use the maptype as scalar type

    this->DataOrigin[0] = this->DataOrigin[1] = this->DataOrigin[2] = 0.0;
    this->DataSpacing[0] = this->DataSpacing[1] = this->DataSpacing[2] = 1.0;

    this->RasterName = NULL;
    this->Mapset = NULL;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);

    this->RegionUsage = VTK_GRASS_REGION_CURRENT;

    this->RasterMap = vtkGRASSRasterMapReader::New();

    this->NullValue = this->RasterMap->GetNullValue();
    this->UseNullValue = 1;

    this->AsCellDataOff();
}

//----------------------------------------------------------------------------

vtkGRASSRasterImageReader::~vtkGRASSRasterImageReader()
{
    if (this->RasterName)
        delete [] this->RasterName;
    if (this->Mapset)
        delete [] this->Mapset;
    this->RasterMap->Delete();
}

//----------------------------------------------------------------------------

int
vtkGRASSRasterImageReader::RequestInformation(vtkInformation * vtkNotUsed(request),
                                              vtkInformationVector ** vtkNotUsed(inputVector),
                                              vtkInformationVector *outputVector)
{


    if (this->RegionUsage == VTK_GRASS_REGION_CURRENT)
    {
        this->RasterMap->UseCurrentRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT)
    {

        this->RasterMap->UseDefaultRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_RASTER)
    {
        this->RasterMap->UseRasterRegion();
    }
    else if (this->RegionUsage == VTK_GRASS_REGION_USER)
    {
        this->RasterMap->UseUserDefinedRegion();
    }

    this->RasterMap->SetNullValue(this->NullValue);
    if(this->UseNullValue)
        this->RasterMap->UseNullValueOn();

    // The region settings are made in the OpenMap method.
    // So we need to open the map here to get the correct
    // region settings.
    if (!this->RasterMap->OpenMap(this->RasterName))
    {
        vtkErrorMacro( << "Error in vtkGRASSRasterMap: " << this->RasterMap->GetError());
        return 0;
    }


    if(this->MapTypeConversion == 0) {
        if (this->RasterMap->GetMapType() == CELL_TYPE)
        {
            this->SetDataScalarTypeToInt();
        }
        else if (this->RasterMap->GetMapType() == FCELL_TYPE)
        {
            this->SetDataScalarTypeToFloat();
        }
        else
        {
            this->SetDataScalarTypeToDouble();
        }
    }

    if(this->MapTypeConversion == 1)
        this->SetDataScalarTypeToInt();
    if(this->MapTypeConversion == 2)
        this->SetDataScalarTypeToFloat();
    if(this->MapTypeConversion == 3)
        this->SetDataScalarTypeToDouble();

    // Init the image extent with the grass region settings
    this->DataExtent[0] = 0;
    this->DataExtent[2] = 0;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = 0;

    this->DataSpacing[0] = this->RasterMap->GetRegion()->GetEastWestResolution();
    this->DataSpacing[1] = this->RasterMap->GetRegion()->GetNorthSouthResolution();

    this->DataOrigin[0] = this->RasterMap->GetRegion()->GetWesternEdge();
    this->DataOrigin[1] = this->RasterMap->GetRegion()->GetSouthernEdge();
    this->DataOrigin[2] = 0;

    // The grass raster cells can be represented as points or as pixel in the vtkImageData
    if(this->AsCellData == 1) {
      this->DataExtent[1] = this->RasterMap->GetNumberOfCols();
      this->DataExtent[3] = this->RasterMap->GetNumberOfRows();
    } else {
      this->DataExtent[1] = this->RasterMap->GetNumberOfCols() - 1;
      this->DataExtent[3] = this->RasterMap->GetNumberOfRows() - 1;
    }

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
vtkGRASSRasterImageReaderExecute(vtkGRASSRasterImageReader *self,
                                 vtkImageData *data, T *outPtr,
                                 int outExt[6])
{

    int idxX, idxY, idxZ;
    vtkIdType outIncX, outIncY, outIncZ;
    int rowcount = outExt[3];
    vtkGRASSRasterMapReader *map = self->GetRasterMap();
    vtkDataArray *row;

    // Get increments to march through data
    data->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);

    // Loop through output pixel
    for (idxZ = outExt[4]; idxZ <= outExt[5]; idxZ++)
    {
        for (idxY = outExt[2]; !self->GetAbortExecute() && idxY <= outExt[3]; idxY++)
        {

            row = map->GetRow(rowcount);
            rowcount--;

            for (idxX = outExt[0]; idxX <= outExt[1]; idxX++)
            {
                *outPtr++ = (T)row->GetTuple1(idxX);
            }
            outPtr += outIncY;
        }
        outPtr += outIncZ;
    }
}

//----------------------------------------------------------------------------

int
vtkGRASSRasterImageReader::RequestData(vtkInformation*,
                                       vtkInformationVector**,
                                       vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    vtkImageData *data = this->AllocateOutputData(outInfo->Get(vtkDataObject::DATA_OBJECT()),
                                                  outInfo);
    int *outExt = data->GetExtent();
    void *outPtr = NULL;

    if(this->AsCellData)
    {
      int row, col, rowcount;

      vtkDataArray *cellscalars;
      cellscalars = vtkDataArray::CreateDataArray(this->GetDataScalarType());
      cellscalars->SetNumberOfComponents(1);
      cellscalars->SetNumberOfTuples((this->DataExtent[1] + 0)*(this->DataExtent[3] + 0));
      cellscalars->SetName(this->RasterName);

      vtkDataArray *pointscalars;
      pointscalars = vtkDataArray::CreateDataArray(this->GetDataScalarType());
      pointscalars->SetNumberOfComponents(1);
      pointscalars->SetNumberOfTuples((this->DataExtent[1] + 1)*(this->DataExtent[3] + 1));
      pointscalars->SetName(this->RasterName);
      pointscalars->FillComponent(0, 0.0);

      vtkGRASSRasterMapReader *map = this->GetRasterMap();
      vtkDataArray *rasterrow;

      // Read the data row by row. GRASS counts the rows from the top left corner,
      // While VTK counts from bottom left
      rowcount = 0;
      for(row = 0; row < this->DataExtent[3]; row ++)
      {
        rasterrow = map->GetRow(row);

        for(col = 0; col < this->DataExtent[1]; col++)
        {
          cellscalars->SetTuple1(row*this->DataExtent[1] + col, rasterrow->GetTuple1(col));
          //pointscalars->SetTuple1(row*(this->DataExtent[1] + 1) + col + 1, rasterrow->GetTuple1(col));
        }
      }

      data->GetCellData()->SetScalars(cellscalars);
      data->GetPointData()->SetScalars(pointscalars);
      cellscalars->Delete();
      pointscalars->Delete();

    } else {
      outPtr = data->GetScalarPointerForExtent(outExt);

      // Call the correct templated function for the output
      switch (this->GetDataScalarType()) {
          vtkTemplateMacro(vtkGRASSRasterImageReaderExecute(this, data,
                                                            static_cast<VTK_TT *> (outPtr),
                                                            outExt));
      default:
          vtkErrorMacro( << "Execute: Unknown data type");
      }
      // Set the name of the scalar array to the raster file name
      data->GetPointData()->GetScalars()->SetName(this->RasterName);
    }

    // All work is done. now we can close the map
    this->RasterMap->CloseMap();
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterImageReader::PrintSelf(ostream& os, vtkIndent indent)
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
