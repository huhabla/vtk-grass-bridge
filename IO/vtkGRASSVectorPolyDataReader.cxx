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
#include <vtkDataSetAttributes.h>
#include "vtkGRASSVectorPolyDataReader.h"

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDataSetAttributes.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "vtkGRASSVectorMapTopoReader.h"
#include "vtkGRASSVectorMapNoTopoReader.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include <vtkIdList.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include "vtkGRASSVectorFeatureCats.h"
#include "vtkGRASSDbmiInterface.h"
#include "vtkGRASSDbmiTable.h"
#include "vtkGRASSDbmiCatValueArray.h"
#include "vtkGRASSDbmiColumn.h"
#include "vtkGRASSDbmiValue.h"

vtkStandardNewMacro(vtkGRASSVectorPolyDataReader);

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataReader::vtkGRASSVectorPolyDataReader()
{
    this->Mapset = NULL;
    this->VectorName = NULL;
    this->CategoryArrayName = NULL;
    this->SetCategoryArrayName("cat");
    this->SetNumberOfInputPorts(0);

    this->Layer = 1;
    this->NoDataValue = -999999;
    this->ReadDBTable = 1;

    this->ColumnNames = vtkStringArray::New();
}

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataReader::~vtkGRASSVectorPolyDataReader()
{
    if (this->VectorName)
        delete [] this->VectorName;
    if (this->Mapset)
        delete [] this->Mapset;
    if (this->CategoryArrayName)
        delete [] this->CategoryArrayName;

    this->ColumnNames->Delete();
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorPolyDataReader::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
        << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
        << (this->Mapset ? this->Mapset : "(none)") << "\n";
    os << indent << "CategoryArrayName: "
        << (this->CategoryArrayName ? this->CategoryArrayName : "(none)") << "\n";

}

//----------------------------------------------------------------------------

int
vtkGRASSVectorPolyDataReader::RequestData(vtkInformation*,
                                          vtkInformationVector**,
                                          vtkInformationVector* outputVector)
{
    int i;
    vtkIdType id;

    if (this->VectorName == NULL)
    {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    VGB_CREATE(vtkGRASSVectorMapNoTopoReader, reader);
    //vtkGRASSVectorMapNoTopoReader *reader = vtkGRASSVectorMapNoTopoReader::New();


    if (!reader->OpenMap(this->VectorName))
    {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        //reader->Delete();
        return -1;
    }

    this->SetMapset(reader->GetMapset());

    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);
    //vtkGRASSVectorFeatureCats *cats = vtkGRASSVectorFeatureCats::New();
    VGB_CREATE(vtkGRASSVectorFeaturePoints, feature);
    //vtkGRASSVectorFeaturePoints *feature = vtkGRASSVectorFeaturePoints::New();

    vtkPolyData* output = vtkPolyData::GetData(outputVector);
    output->Allocate(1);

    VGB_CREATE(vtkPoints, points);
    //vtkPoints *points = vtkPoints::New();

    VGB_CREATE(vtkIntArray, categories);
    //vtkIntArray *categories = vtkIntArray::New();
    categories->SetNumberOfComponents(1);
    categories->SetName(this->CategoryArrayName);

    VGB_CREATE(vtkIdList, ids);
    //vtkIdList *ids = vtkIdList::New();

    // Read every feature in vector map
    while (reader->ReadNextFeature(feature, cats) > 0)
    {
        for(i = 0; i < feature->GetNumberOfPoints(); i++)
        {
            double *point = feature->GetPoint(i);
            id = points->InsertNextPoint(point[0], point[1], point[2]);
            ids->InsertNextId(id);
        }
        output->InsertNextCell(feature->GetVTKCellId(), ids);
        ids->Initialize();

        categories->InsertNextValue(cats->GetCat(1));
    }

    // Store the points in the output data object.
    output->SetPoints(points);
    output->GetCellData()->SetScalars(categories);

    // Read all data from the database into cell arrays
    this->ReadDatabaseData(reader, categories, output->GetCellData());

    reader->CloseMap();

    return 1;
}

//----------------------------------------------------------------------------

void vtkGRASSVectorPolyDataReader::ReadDatabaseData(vtkGRASSVectorMapNoTopoReader *map,
                                                    vtkIntArray *cats,
                                                    vtkDataSetAttributes *cdata)
{
    int i;
    int ncols;

    if(this->ReadDBTable == 0)
        return;
    
    VGB_CREATE(vtkGRASSDbmiCatValueArray, catval);
    //vtkGRASSDbmiCatValueArray *catval = vtkGRASSDbmiCatValueArray::New();
    VGB_CREATE(vtkGRASSDbmiTable, table);
    //vtkGRASSDbmiTable *table = vtkGRASSDbmiTable::New();
    VGB_CREATE(vtkGRASSDbmiColumn, column);
    //vtkGRASSDbmiColumn *column = vtkGRASSDbmiColumn::New();
 
    vtkGRASSDbmiInterface *db = map->GetDbmiInterface();
    db->ConnectDB();
    db->SetFieldNumber(this->Layer);
    db->GetTable(table);
        
    // In case the ColumnNames array is empty, fill it with all valid table columns
    if(this->ColumnNames->GetNumberOfValues() == 0) {
        ncols = table->GetNumberOfColumns();
        for(i = 0; i < ncols; i++) {
	    table->GetColumn(i, column);
	    if(column->IsValueInteger() || column->IsValueDouble()) {
                this->ColumnNames->InsertNextValue(column->GetName());
	    }
        }
    }

    // Read all data from the database into cell arrays
    for(i = 0; i < this->ColumnNames->GetNumberOfValues(); i++) {
        if(table->GetColumn(this->ColumnNames->GetValue(i), column) == false) {
	    vtkErrorMacro(<< "Database column " << (const char*)this->ColumnNames->GetValue(i) << " not available");
	    continue;
	}

	// The categories are already readed
        if(strcmp(column->GetName(), "cat") == 0)
            continue;

        // Read data into the cat value array 
        db->SelectCatValueArray(this->ColumnNames->GetValue(i), catval);

	if(column->IsValueDouble()) {
            vtkDoubleArray *array = vtkDoubleArray::New();
	    array->SetNumberOfComponents(1);
	    array->SetName(column->GetName());
	    catval->ValuesToDoubleArray(cats, array, this->NoDataValue);
            cdata->AddArray(array);
	    array->Delete();
	} else if(column->IsValueInteger()) {
            vtkIntArray *array = vtkIntArray::New();
	    array->SetNumberOfComponents(1);
	    array->SetName(column->GetName());
	    catval->ValuesToIntegerArray(cats, array, this->NoDataValue);
            cdata->AddArray(array);
	    array->Delete();
	} else {
            vtkErrorMacro(<< "Databse column " << column->GetName() << " has unsupported data type");
	    continue;
	}
    }

    db->DisconnectDB();
    return;
}


