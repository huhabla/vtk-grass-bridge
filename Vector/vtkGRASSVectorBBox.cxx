
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

#include "vtkGRASSVectorBBox.h"
#include <vtkObjectFactory.h>

vtkStandardNewMacro(vtkGRASSVectorBBox);

//------------------------------------------------------------------------------

double* vtkGRASSVectorBBox::GetBoxAsArray()
{
    double *a = (double*)G_calloc(6, sizeof(double));

    a[0] = this->box.N;
    a[1] = this->box.S;
    a[2] = this->box.E;
    a[3] = this->box.W;
    a[4] = this->box.T;
    a[5] = this->box.B;

    return a;
}

//----------------------------------------------------------------------------

void
vtkGRASSVectorBBox::PrintSelf(ostream& os, vtkIndent indent)
{

    this->Superclass::PrintSelf(os, indent);

    os << indent << "North: " << this->box.N << "\n";
    os << indent << "South: " << this->box.S << "\n";
    os << indent << "East: " << this->box.E << "\n";
    os << indent << "West: " << this->box.W << "\n";
    os << indent << "Top: " << this->box.T << "\n";
    os << indent << "Bottom: " << this->box.B << "\n";
}
