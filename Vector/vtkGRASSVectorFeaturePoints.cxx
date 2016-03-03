
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

#include "vtkGRASSVectorFeaturePoints.h"
#include "vtkGRASSVectorBBox.h"
#include "vtkGRASSDefines.h"
#include <vtkObjectFactory.h>
#include <vtkCellType.h>

vtkStandardNewMacro(vtkGRASSVectorFeaturePoints);

//----------------------------------------------------------------------------

vtkGRASSVectorFeaturePoints::vtkGRASSVectorFeaturePoints()
{
    this->FeatureType = -1;
    this->points = Vect_new_line_struct();
    if (this->points == NULL)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate the internal structure.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
    }
}

//----------------------------------------------------------------------------

vtkGRASSVectorFeaturePoints::~vtkGRASSVectorFeaturePoints()
{
    if (this->points)
        Vect_destroy_line_struct(this->points);
}

//----------------------------------------------------------------------------

void vtkGRASSVectorFeaturePoints::DeepCopy(vtkGRASSVectorFeaturePoints *points)
{
    int i;
    double *p;
    this->Reset();
    for(i = 0; i < points->GetNumberOfPoints(); i++) {
        p = points->GetPoint(i);
        this->AppendPoint(p[0], p[1], p[2]);
    }
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorFeaturePoints::AppendPoints(vtkGRASSVectorFeaturePoints *Points, int direction)
{
    if (direction == 1)
        direction = GV_FORWARD;
    if (direction == -1)
        direction = GV_BACKWARD;
    return Vect_append_points(this->points, Points->GetPointer(), direction);
}

//----------------------------------------------------------------------------

double
vtkGRASSVectorFeaturePoints::GetPointDistanceFromLine(double x, double y, double z)
{
    double distance;
    Vect_line_distance(this->points, x, y, z, 1, NULL, NULL, NULL, &distance, NULL, NULL);

    return distance;
}

//----------------------------------------------------------------------------

vtkGRASSVectorBBox *vtkGRASSVectorFeaturePoints::GetBoundingBox()
{
    VGB_CREATE(vtkGRASSVectorBBox, box);
    //vtkGRASSVectorBBox *box = vtkGRASSVectorBBox::New();
    Vect_line_box(this->points, box->GetPointer());
    return box;
}

//----------------------------------------------------------------------------

double *
vtkGRASSVectorFeaturePoints::GetPoint(int index)
{
    double *point = (double*)G_calloc(3, sizeof(double));

    if (index < 0 || index > this->points->n_points - 1)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate the internal structure.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return NULL;
    }

    point[0] = this->points->x[index];
    point[1] = this->points->y[index];
    point[2] = this->points->z[index];

    return point;
}

//----------------------------------------------------------------------------


bool vtkGRASSVectorFeaturePoints::SetPoint(int index, double x, double y, double z)
{
    if (index < 0 || index > this->points->n_points - 1)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Index is out of range.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    this->points->x[index] = x;
    this->points->y[index] = y;
    this->points->z[index] = z;

    return true;
}

//----------------------------------------------------------------------------

char *vtkGRASSVectorFeaturePoints::GetFeatureTypeAsString()
{
    if(this->IsFeatureTypeBoundary())
        return G_store("boundary");
    if(this->IsFeatureTypeCentroid())
        return G_store("centroid");
    if(this->IsFeatureTypeFace())
        return G_store("face");
    if(this->IsFeatureTypeArea())
        return G_store("area");
    if(this->IsFeatureTypeKernel())
        return G_store("kernel");
    if(this->IsFeatureTypeLine())
        return G_store("line");
    if(this->IsFeatureTypePoint())
        return G_store("point");

    return NULL;
}


//----------------------------------------------------------------------------

int vtkGRASSVectorFeaturePoints::GetVTKCellId()
{
    if(this->IsFeatureTypeBoundary())
        return VTK_POLY_LINE;
    if(this->IsFeatureTypeCentroid())
        return VTK_VERTEX;
    if(this->IsFeatureTypeFace())
        return VTK_POLYGON;
    if(this->IsFeatureTypeArea())
        return VTK_POLYGON;
    if(this->IsFeatureTypeKernel())
        return VTK_VERTEX;
    if(this->IsFeatureTypeLine())
        return VTK_POLY_LINE;
    if(this->IsFeatureTypePoint())
        return VTK_VERTEX;

    return VTK_EMPTY_CELL;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorFeaturePoints::PrintSelf(ostream& os, vtkIndent indent)
{

    this->Superclass::PrintSelf(os, indent);

    os << indent << "Number of points: " << this->points->n_points << "\n";
    os << indent << "Number of segments: " << this->points->n_points - 1 <<"\n";
    os << indent << "Feature type: " << this->GetFeatureTypeAsString() <<"\n";
    os << indent << "Point coordinates: " << "\n";
    int i;
    for (i = 0; i < this->points->n_points; i++)
    {
        os << indent << this->points->x[i] << " " << this->points->y[i] << " " << this->points->z[i] << endl;
    }
}
