/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2012 by Soeren Gebbert, soerengebbert@googlemail.com
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


#include <vtkDataSetAttributes.h>
#include "vtkGRASSMultiVectorPolyDataLineReader.h"

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkGRASSVectorMapTopoReader.h>
#include <vtkGRASSVectorMapNoTopoReader.h>
#include <vtkGRASSVectorMapBase.h>
#include <vtkGRASSVectorFeaturePoints.h>
#include <vtkIdList.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkShortArray.h>
#include <vtkGRASSVectorFeatureCats.h>
#include <vtkGRASSDbmiInterface.h>
#include <vtkGRASSDefines.h>
#include <vtkGRASSDbmiTable.h>
#include <vtkGRASSDbmiCatValueArray.h>
#include <vtkGRASSDbmiColumn.h>
#include <vtkGRASSDbmiValue.h>

vtkStandardNewMacro(vtkGRASSMultiVectorPolyDataLineReader);

//----------------------------------------------------------------------------

vtkGRASSMultiVectorPolyDataLineReader::vtkGRASSMultiVectorPolyDataLineReader()
{
  this->NewArrayNames = NULL;
  this->LineLengths = NULL;
  this->VectorNames = NULL;
  this->SetFeatureTypeToPoint();
}

//----------------------------------------------------------------------------

vtkGRASSMultiVectorPolyDataLineReader::~vtkGRASSMultiVectorPolyDataLineReader()
{
  if (this->NewArrayNames)
    this->NewArrayNames->Delete();
  if (this->LineLengths)
    this->LineLengths->Delete();
  if (this->VectorNames)
    this->VectorNames->Delete();
}

//----------------------------------------------------------------------------

void vtkGRASSMultiVectorPolyDataLineReader::PrintSelf(ostream& os,
                                                      vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------

int vtkGRASSMultiVectorPolyDataLineReader::RequestData(
    vtkInformation*, vtkInformationVector**, vtkInformationVector* outputVector)
{

  int i, j, n;
  int featureCount = 0;
  double length;
  vtkIdType id;

  if (this->VectorNames == NULL)
    {
    vtkErrorMacro( << "Vector names not set.");
    return -1;
    }

  if (this->LineLengths == NULL)
    {
    vtkErrorMacro( << "Line lengths not set.");
    return -1;
    }

  if (this->VectorNames->GetNumberOfTuples()
      != this->LineLengths->GetNumberOfTuples())
    {
    vtkErrorMacro( << "The number of line length entries is not equal "
    "the number of vector names");
    return -1;
    }

  if (this->NewArrayNames != NULL && this->ColumnNames != NULL)
    {
    if (this->ColumnNames->GetNumberOfTuples()
        != this->NewArrayNames->GetNumberOfTuples())
      {
      vtkErrorMacro( << "The number of column names and "
      "the number of array names is unequal");
      return -1;
      }
    }

	VGB_CREATE(vtkShortArray, layer);
	layer->SetNumberOfComponents(1);
	layer->SetName("Layer");

  vtkPolyData* output = vtkPolyData::GetData(outputVector);
  output->Allocate(1);
  VGB_CREATE(vtkPoints, points);
  VGB_CREATE(vtkIdList, ids);

  length = 0.0;

  for (n = 0; n < this->VectorNames->GetNumberOfValues(); n++)
    {
    length += this->LineLengths->GetValue(n);

    VGB_CREATE(vtkGRASSVectorMapTopoReader, reader);

    if (!reader->OpenMap(this->VectorNames->GetValue(n)))
      {
      vtkErrorMacro(<< "Unable to open vector map "
                    << this->VectorNames->GetValue(n));
      return -1;
      }

    this->SetMapset(reader->GetMapset());


    VGB_CREATE(vtkIntArray, categories);
    categories->SetNumberOfComponents(1);
    categories->SetName(this->CategoryArrayName);

    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, feature);

    int featureCount2 = 0;
    while (reader->ReadNextFeature(feature, cats) > 0)
      {
      if (feature->GetFeatureType() == this->FeatureType)
        {
        ids->Initialize();
        // We expect only a single point
        double *point = feature->GetPoint(0);
        // We safe the layer number
		layer->InsertNextValue(n + 1);
        // We create two points for the first layer
        if (n == 0)
          {
          // Fist point
          id = points->InsertNextPoint(point[0], point[1], 0);
          ids->InsertNextId(id);
          // second point
          id = points->InsertNextPoint(point[0], point[1], length);
          ids->InsertNextId(id);
          featureCount++;
          featureCount2++;
          categories->InsertNextValue(cats->GetCat(this->Layer));
          }
        else
          {
          // New lower point with accumulated line length
          id = points->InsertNextPoint(point[0], point[1], length);
          // The id of the lower points of the layer before this
          // This assures connected lines
          if(n == 1)
            {
            /* Numbering is based on:
             *
             *  Top layer
             *
             *  0 o   2 o   4 o   6 o   8 o
             *    |     |     |     |     |
             *  1 o   3 o   5 o   7 o   9 o
             *    |     |     |     |     |
             * 10 o  11 o  12 o  13 o  14 o
             *    |     |     |     |     |
             * 15 o  16 o  17 o  18 o  19 o
             *
             * Bottom layer
             *
             */
            ids->InsertNextId(id - (2*featureCount - featureCount2 - 1));
            //std::cout << "First point id: " << id << "  Second point id: "
            //    << id - (2*featureCount - featureCount2 - 1) << std::endl;
            }
          else
            {
            ids->InsertNextId(id - featureCount);
            //std::cout << "First point id: " << id << "  Second point id: "
            //    << id - featureCount << std::endl;
            }
          // Second point
          ids->InsertNextId(id);
          featureCount2++;
          categories->InsertNextValue(cats->GetCat(this->Layer));
          }
        output->InsertNextCell(VTK_LINE, ids);
        }
      }
    if (featureCount != featureCount2)
      {
      vtkErrorMacro(
          << "Number of features are different in input vector maps");
      return -1;
      }
    // Read all data from the database into cell arrays
    vtkCellData *cellData = vtkCellData::New();
    this->ReadDatabaseData(reader, categories, cellData);

    // Add the cell data to the output
    for (i = 0; i < cellData->GetNumberOfArrays(); i++)
      {
      // Simply add the arrays
      if (n == 0)
        {
        output->GetCellData()->AddArray(cellData->GetArray(i));
        }
      else
        {
        // Add the new data to the arrays
        vtkDataArray *source = cellData->GetArray(i);
        if(source == NULL)
          {
          vtkErrorMacro(
              << "Data array not available in input");
          return -1;
          }
        vtkDataArray *target = output->GetCellData()->GetArray(
            source->GetName());
        for (j = 0; j < source->GetNumberOfTuples(); j++)
          {
          target->InsertNextTuple(source->GetTuple(j));
          }
        }
      }
    reader->CloseMap();
    cellData->Delete();
    }

  // Replace the array names with new names
  if (this->NewArrayNames != NULL && this->ColumnNames != NULL)
    {
    for (i = 0; i < this->ColumnNames->GetNumberOfValues(); i++)
      {
      output->GetCellData()->GetArray(this->ColumnNames->GetValue(i))->SetName(
          this->NewArrayNames->GetValue(i));
      }
    }

  // Store the points in the output data object.
  output->GetCellData()->AddArray(layer);
  output->SetPoints(points);

  return 1;
}
