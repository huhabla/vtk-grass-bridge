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
#include "vtkGRASSVectorPolyDataAreaWriter.h"

#include <vtkCellArray.h>
#include <vtkDataArray.h>
#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "vtkGRASSVectorMapWriter.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include "vtkGRASSVectorFeatureCats.h"
#include "vtkGRASSVectorMapWriter.h"
#include "vtkGRASSRasterMapWriter.h"
#include <vtkInformationVector.h>
#include <vtkInformation.h>

vtkStandardNewMacro(vtkGRASSVectorPolyDataAreaWriter);

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataAreaWriter::vtkGRASSVectorPolyDataAreaWriter()
{
    this->SetNumberOfInputPorts(2);
    this->SetNumberOfOutputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSVectorPolyDataAreaWriter::~vtkGRASSVectorPolyDataAreaWriter()
{
    ;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorPolyDataAreaWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Vector name: "
        << (this->VectorName ? this->VectorName : "(none)") << "\n";
    os << indent << "Mapset: "
        << (this->Mapset ? this->Mapset : "(none)") << "\n";

}

//----------------------------------------------------------------------------

int
vtkGRASSVectorPolyDataAreaWriter::RequestData(
                                          vtkInformation *vtkNotUsed(request),
                                          vtkInformationVector **inputVector,
                                          vtkInformationVector *vtkNotUsed(outputVector))
{
    // get the info objects
    vtkInformation *in1Info = inputVector[0]->GetInformationObject(0);
    vtkInformation *in2Info = inputVector[1]->GetInformationObject(0);

    // get the input and ouptut
    vtkDataSet *boundaries = vtkDataSet::SafeDownCast(in1Info->Get(vtkDataObject::DATA_OBJECT()));
    vtkDataSet *centroids = vtkDataSet::SafeDownCast(in2Info->Get(vtkDataObject::DATA_OBJECT()));
    
    int i, j, cat;
    double point[3];
    
    if (this->VectorName == NULL)
    {
        vtkErrorMacro( << "Vector name not set.");
        return -1;
    }

    VGB_CREATE(vtkGRASSVectorMapWriter, writer);
    
    if (!writer->OpenMap(this->VectorName, 1))
    {
        vtkErrorMacro( << "Unable to open vector map " << this->VectorName);
        return -1;
    }

    this->SetMapset(writer->GetMapset());
    
    // Check for centroid caregories
    vtkDataArray *categories = NULL;
    if (centroids->GetCellData()->HasArray(this->CategoryArrayName))
        categories = centroids->GetCellData()->GetArray(this->CategoryArrayName);
    else {
        vtkErrorMacro( << "Missing category array " << this->CategoryArrayName << " in second input");
        return -1;
    }
    
    VGB_CREATE(vtkGRASSVectorFeatureCats, cats);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, centroid);
    VGB_CREATE(vtkGRASSVectorFeaturePoints, feature);
    
    // Write the boundaries
    for (i = 0; i < boundaries->GetNumberOfCells(); i++)
    {
        vtkCell *cell = boundaries->GetCell(i);

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
    
    // Writen the centroids
    for(i = 0; i < centroids->GetNumberOfCells(); i++) {
        vtkCell *cell = centroids->GetCell(i);
        // Centroids must be of type VTK_VERTEX
        if (cell->GetCellType() == VTK_VERTEX) {
            cat = (int)categories->GetTuple1(i);
            
            centroid->Reset();
            cats->Reset();
            
            // Get the centroid coordiantes, always the first point
            cell->GetPoints()->GetPoint(0, point);

            centroid->SetFeatureTypeToCentroid();
            centroid->AppendPoint(point[0], point[1], point[2]);

            // Attach the centroid
            if (1 > writer->WriteFeature(centroid, cats))
            {
                vtkErrorMacro( << "Error writing centroid to vector map");
                return -1;
            }
        }
    }

    // Now add any cell data to the vector database table
    this->AddCellDataToVectorMap(centroids->GetCellData(), categories, writer);
    
    if (this->BuildTopo > 0)
        writer->CloseMap(true);
    else
        writer->CloseMap(false);

    return 1;
}
