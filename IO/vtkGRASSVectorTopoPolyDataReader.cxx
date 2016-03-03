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
#include "vtkGRASSVectorTopoPolyDataReader.h"

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "vtkGRASSVectorMapTopoReader.h"
#include "vtkGRASSVectorMapNoTopoReader.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include <vtkIdList.h>
#include "vtkGRASSVectorFeatureCats.h"
#include <vtkDoubleArray.h>
#include <vtkShortArray.h>
#include <vtkPointData.h>
#include "vtkGRASSDefines.h"

vtkStandardNewMacro(vtkGRASSVectorTopoPolyDataReader);

//----------------------------------------------------------------------------

vtkGRASSVectorTopoPolyDataReader::vtkGRASSVectorTopoPolyDataReader() {
    this->Mapset = NULL;
    this->VectorName = NULL;
    this->FeatureType = GV_POINTS;
    this->CategoryArrayName = NULL;
    this->SetCategoryArrayName("cat");
    this->SetNumberOfInputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSVectorTopoPolyDataReader::~vtkGRASSVectorTopoPolyDataReader() {
    ;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorTopoPolyDataReader::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
            << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
            << (this->Mapset ? this->Mapset : "(none)") << "\n";

}

//----------------------------------------------------------------------------

int
vtkGRASSVectorTopoPolyDataReader::RequestData(vtkInformation*,
        vtkInformationVector**,
        vtkInformationVector* outputVector) {

    int i, area, isle;
    vtkIdType id;

    if (this->VectorName == NULL) {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    VGB_CREATE(vtkGRASSVectorMapTopoReader, reader);
    //vtkGRASSVectorMapTopoReader *reader = vtkGRASSVectorMapTopoReader::New();

    if (!reader->OpenMap(this->VectorName)) {
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

    // Read only the requested feature in vector map
    if (this->FeatureType != GV_AREA) {
        while (reader->ReadNextFeature(feature, cats) > 0) {
            if (feature->GetFeatureType() == this->FeatureType) {
                for (i = 0; i < feature->GetNumberOfPoints(); i++) {
                    double *point = feature->GetPoint(i);
                    id = points->InsertNextPoint(point[0], point[1], point[2]);
                    ids->InsertNextId(id);
                }
                output->InsertNextCell(feature->GetVTKCellId(), ids);
                ids->Initialize();

                categories->InsertNextValue(cats->GetCat(1));
            }
        }
    } else {
        // Save the area of the area
        VGB_CREATE(vtkDoubleArray, a);
        //vtkDoubleArray *a = vtkDoubleArray::New();
        a->SetNumberOfComponents(1);
        a->SetNumberOfValues(reader->GetNumberOfAreas());
        a->SetName("Area");
        // Save the number of islands for each area
        VGB_CREATE(vtkIntArray, isles);
        //vtkIntArray *isles = vtkIntArray::New();
        isles->SetNumberOfComponents(1);
        isles->SetNumberOfValues(reader->GetNumberOfAreas());
        isles->SetName("IslandsPerArea");
        // Save the state of an area (island, no island)
        VGB_CREATE(vtkShortArray, isIsle);
        //vtkShortArray *isIsle = vtkShortArray::New();
        isIsle->SetNumberOfComponents(1);
        isIsle->SetNumberOfValues(reader->GetNumberOfAreas());
        isIsle->SetName("isIsle");
        isIsle->FillComponent(0, 0.0);

        // Save each area as polygone
        for (area = 1; area <= reader->GetNumberOfAreas(); area++) {
            reader->GetArea(area, feature, cats);
            for (i = 0; i < feature->GetNumberOfPoints(); i++) {
                double *point = feature->GetPoint(i);
                id = points->InsertNextPoint(point[0], point[1], point[2]);
                ids->InsertNextId(id);
            }
            // Save the area of the area
            a->SetValue(area - 1, reader->GetAreaOfArea(area));
            // Insert cell in polydataset
            output->InsertNextCell(feature->GetVTKCellId(), ids);
            ids->Initialize();
            // Add categories and number of islands per area
            categories->InsertNextValue(cats->GetCat(1));
            isles->SetValue(area - 1, reader->GetNumberOfAreaIsles(area));
        }

         // Save if an area is an isle
        for (isle = 1; isle <= reader->GetNumberOfIsles(); isle++) {
            area = reader->GetIsleArea(isle);
            if(area > 0)
                isIsle->SetValue(area - 1, 1);
        }

        // Add the area as scalars
        output->GetCellData()->AddArray(a);
        output->GetCellData()->AddArray(isles);
        output->GetCellData()->AddArray(isIsle);
    }

    output->GetCellData()->AddArray(categories);
    output->GetCellData()->SetActiveScalars(categories->GetName());

    // Read all data from the database into cell arrays
    this->ReadDatabaseData(reader, categories, output->GetCellData());

    // Store the points in the output data object.
    output->SetPoints(points);

    return 1;
}
