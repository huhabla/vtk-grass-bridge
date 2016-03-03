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
#include "vtkGRASSVectorPolyDataWriter.h"

#include <vtkCellArray.h>
#include <vtkShortArray.h>
#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "vtkGRASSVectorMapWriter.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include <vtkIdList.h>
#include "vtkGRASSVectorFeatureCats.h"
#include "vtkGRASSVectorMapWriter.h"
#include "vtkGRASSRasterMapWriter.h"
#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkCleanPolyData.h>
#include <vtkExtractEdges.h>
#include <vtkDataArrayCollection.h>
#include "vtkGRASSDbmiInterface.h"
#include "vtkGRASSDbmiCatValueArray.h"
#include "vtkGRASSDbmiTable.h"
#include "vtkGRASSDbmiColumn.h"
#include "vtkGRASSDbmiValue.h"
#include <iostream>
#include <string>
#include <sstream>

vtkStandardNewMacro(vtkGRASSVectorPolyDataWriter);

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataWriter::vtkGRASSVectorPolyDataWriter()
{
    this->Mapset = NULL;
    this->VectorName = NULL;
    this->CategoryArrayName = NULL;
    this->BuildTopoOff();
    this->SetCategoryArrayName("cat");
    this->SetNumberOfInputPorts(1);
    this->SetNumberOfOutputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataWriter::~vtkGRASSVectorPolyDataWriter()
{
    if (this->VectorName)
        delete [] this->VectorName;
    if (this->Mapset)
        delete [] this->Mapset;
    if (this->CategoryArrayName)
        delete [] this->CategoryArrayName;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorPolyDataWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
        << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
        << (this->Mapset ? this->Mapset : "(none)") << "\n";

}

//----------------------------------------------------------------------------

int
vtkGRASSVectorPolyDataWriter::RequestData(
                                          vtkInformation *vtkNotUsed(request),
                                          vtkInformationVector **inputVector,
                                          vtkInformationVector *vtkNotUsed(outputVector))
{
    // get the info objects
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);

    // get the input and ouptut
    vtkDataSet *input = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    int i, j;
    double point[3], pcoords[3];
    bool hasAreas = false;

    if (this->VectorName == NULL)
    {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    VGB_CREATE(vtkGRASSVectorMapWriter, writer);

    if (!writer->OpenMap(this->VectorName, 1))
    {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        writer->Delete();
        return -1;
    }

    this->SetMapset(writer->GetMapset());

    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, feature);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, centroid);

    vtkDataArray *categories = NULL;
    if (input->GetCellData()->HasArray(this->CategoryArrayName))
        categories = input->GetCellData()->GetArray(this->CategoryArrayName);
    else {
        categories = vtkIntArray::New();
        categories->SetName("cats");
        categories->SetNumberOfComponents(1);
        categories->SetNumberOfTuples(input->GetNumberOfCells());
        for (i = 0; i < input->GetNumberOfCells(); i++)
            categories->InsertTuple1(i, (double)(i + 1));
    }

    // We write the data per cell
    for (i = 0; i < input->GetNumberOfCells(); i++)
    {
        vtkCell *cell = input->GetCell(i);

        if (cell->GetCellType() == VTK_POLY_VERTEX)
        {
            vtkErrorMacro( << "VTK_POLY_VERTEX is currently not supported.");
            continue;
        }

        // Only linear 2d cells are supported
        if (cell->IsLinear() && cell->GetCellDimension() < 3)
        {
            bool writeFeature = true;
            // Reset the feature and the categorie arrays
            feature->Reset();
            centroid->Reset();
            cats->Reset();

            // Get the point coordiantes
            vtkPoints *points = cell->GetPoints();
            for (j = 0; j < cell->GetNumberOfPoints(); j++)
            {
                points->GetPoint(j, point);
                feature->AppendPoint(point[0], point[1], point[2]);
            }

            cats->AddCat(1, (int) categories->GetTuple1(i));

            // Only a subset of cells are supported
            if (cell->GetCellType() == VTK_VERTEX)
                feature->SetFeatureTypeToPoint();
            else if (cell->GetCellType() == VTK_TRIANGLE)
                feature->SetFeatureTypeToFace();
            else if (cell->GetCellType() == VTK_POLY_LINE || cell->GetCellType() == VTK_LINE)
                feature->SetFeatureTypeToLine();
            else if (cell->GetCellType() == VTK_POLYGON || cell->GetCellType() == VTK_QUAD)
            {
                // ATTENTION!!! This method does not work with nested polygons
                // We write only the centroid, areas a treated separately
                writeFeature = false;
                hasAreas = true;

                double *weights = new double [cell->GetNumberOfPoints()];
                int subId;

                centroid->SetFeatureTypeToCentroid();
                // Compute the centroid
                subId = cell->GetParametricCenter(pcoords);
                cell->EvaluateLocation(subId, pcoords, point, weights);
                // Append the centroid
                centroid->AppendPoint(point[0], point[1], point[2]);

                if (1 > writer->WriteFeature(centroid, cats))
                {
                    vtkErrorMacro( << "Error writing centroid to vector map");
                    delete [] weights;
                    return -1;
                }
                delete [] weights;
            }
            else
                continue;

            if (writeFeature && (1 > writer->WriteFeature(feature, cats)))
            {
                vtkErrorMacro( << "Error writing feature to vector map");
                return -1;
            }
        }
        else
        {
            vtkWarningMacro( << "cell type " << cell->GetCellType() << " not supported");
            continue;
        }
    }

    // Areas are treated separately
    if(hasAreas) {
        // Clean the data, remove redundant points
        VGB_CREATE(vtkCleanPolyData, cleaner);
        // Extract edges of polygons
        VGB_CREATE(vtkExtractEdges, edges);

        cleaner->SetInputData(input);
        cleaner->ConvertLinesToPointsOn();
        cleaner->ConvertPolysToLinesOn();
        cleaner->Update();

        edges->SetInputConnection(cleaner->GetOutputPort());
        edges->Update();

        vtkPolyData *areas = edges->GetOutput();

        // We write the data per cell
        for (i = 0; i < areas->GetNumberOfCells(); i++)
        {
            vtkCell *cell = areas->GetCell(i);

            if (cell->GetCellType() == VTK_POLY_LINE || cell->GetCellType() == VTK_LINE)
            {
                feature->Reset();
                // Get the point coordiantes
                vtkPoints *points = cell->GetPoints();
                for (j = 0; j < cell->GetNumberOfPoints(); j++)
                {
                    points->GetPoint(j, point);
                    feature->AppendPoint(point[0], point[1], point[2]);
                }

                feature->SetFeatureTypeToBoundary();

                if (1 > writer->WriteFeature(feature, cats))
                {
                    vtkErrorMacro( << "Error writing boundary feature to vector map");
                    return -1;
                }
            }
        }
    }

    // Now add any cell data to the vector database table
    this->AddCellDataToVectorMap(input->GetCellData(), categories, writer);

    if (this->BuildTopo > 0)
        writer->CloseMap(true);
    else
        writer->CloseMap(false);

    return 1;
}

//----------------------------------------------------------------------------

void vtkGRASSVectorPolyDataWriter::AddCellDataToVectorMap(vtkCellData *celldata, vtkDataArray *categories, vtkGRASSVectorMapWriter *writer)
{
    int i, j, numcols, numcells;
    double range[2];
    VGB_CREATE(vtkCellData, cd);

    numcols = celldata->GetNumberOfArrays();

    // Return if nothing todo
    if(numcols == 0)
        return;

    // Grass expects the name cat as category array
    categories->SetName("cat");
    categories->GetRange(range);

    if(range[1] < 1.0)
        return;

    // To make sure only unequal categories are added to the database table
    // We use a selection array
    VGB_CREATE(vtkShortArray, catselect);
    catselect->SetNumberOfComponents(1);
    catselect->SetNumberOfTuples((long long)range[1] + 1);
    catselect->FillComponent(0, 0.0);

    // The first item is the category array
    cd->AddArray(categories);

    // Add all available arrays to the collection
    for(i = 0; i < numcols; i++)
    {
        vtkDataArray *array = celldata->GetArray(i);

        // Skip arrays with more than one component
        if(array->GetNumberOfComponents() > 1)
        {
            vtkWarningMacro(<< "Skipping array " << array->GetName()
                            << " with " <<  array->GetNumberOfComponents()
                            << " components");
            continue;
        }

        if(!cd->HasArray(array->GetName()))
            cd->AddArray(array);
    }

    numcols = cd->GetNumberOfArrays();

    vtkGRASSDbmiInterface *db = writer->GetDbmiInterface();

    VGB_CREATE(vtkGRASSDbmiTable, table);
    table->SetName(this->GetVectorName());

    // Cats are already set, start with 1
    for(i = 0; i < numcols; i++)
    {
        VGB_CREATE(vtkGRASSDbmiColumn, col);
        vtkDataArray *array = cd->GetArray(i);

        col->SetName(array->GetName());

        if(array->GetDataType() == VTK_DOUBLE)
            col->SetSQLTypeToDoublePrecision();
        else if(array->GetDataType() == VTK_CHAR)
            col->SetSQLTypeToCharacter();
        else if(array->GetDataType() == VTK_INT)
            col->SetSQLTypeToInteger();
        else if(array->GetDataType() == VTK_LONG)
            col->SetSQLTypeToInteger();
        else if(array->GetDataType() == VTK_SHORT)
            col->SetSQLTypeToSmallInt();
        else
            col->SetSQLTypeToDoublePrecision();

        table->AppendColumn(col);
    }

    vtkDebugMacro(<< table->TableToSQL());

    // Create the table
    db->ConnectDBCreateTable(table);

    db->BeginTransaction();

    numcells = categories->GetNumberOfTuples();

    for(i = 0; i < numcells; i++)
    {
        // Add only values with categories > 0
        if(categories->GetTuple1(i) < 1)
            continue;
        // Do nothing if this category was already inserted
        if(catselect->GetValue((int)categories->GetTuple1(i)) == 1)
            continue;
        // Create the SQL string
        std::ostringstream os;
        os << "INSERT INTO " << table->GetName() << " VALUES (";

        for(j = 0; j < numcols; j++)
        {
            vtkDataArray *array = cd->GetArray(j);

            if(array->GetDataType() == VTK_DOUBLE)
                os << (double)array->GetTuple1(i);
            else if(array->GetDataType() == VTK_CHAR)
                os << (char)array->GetTuple1(i);
            else if(array->GetDataType() == VTK_INT)
                os << (int)array->GetTuple1(i);
            else if(array->GetDataType() == VTK_LONG)
                os << (long)array->GetTuple1(i);
            else if(array->GetDataType() == VTK_SHORT)
                os << (short)array->GetTuple1(i);
            else
                os << (double)array->GetTuple1(i);

            if(j < numcols - 1)
                os << ",";
        }
        os << ")";
        vtkDebugMacro(<< os.str());
        db->ExecuteImmediate(os.str().c_str());
        // The category was created
        catselect->SetValue((int)categories->GetTuple1(i), 1);
    }

    db->CommitTransaction();
    db->DisconnectDB();


    return;
}
