
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

#include "vtkGRASSDbmiCatValueArray.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>
#include <vtkIntArray.h>
#include <vtkDoubleArray.h>

vtkStandardNewMacro(vtkGRASSDbmiCatValueArray);

//----------------------------------------------------------------------------

vtkGRASSDbmiCatValueArray::vtkGRASSDbmiCatValueArray() {
    db_CatValArray_init(&this->catval);
}

//----------------------------------------------------------------------------

vtkGRASSDbmiCatValueArray::~vtkGRASSDbmiCatValueArray() {
}

//----------------------------------------------------------------------------

double vtkGRASSDbmiCatValueArray::GetValueDouble(int cat)
{
    double val = 0.0;
    if (DB_OK == db_CatValArray_get_value_double(&this->catval, cat, &val)) {
        return val;
    } else {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to find double value for cat %i..",
                this->GetClassName(), __LINE__, cat);
        this->InsertNextError(buff);
        return 0.0;
    }
}

//----------------------------------------------------------------------------

int vtkGRASSDbmiCatValueArray::GetValueInteger(int cat)
{
    int val = 0;
    if (DB_OK == db_CatValArray_get_value_int(&this->catval, cat, &val)) {
        return val;
    } else {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to find int value for cat %i..",
                this->GetClassName(), __LINE__, cat);
        this->InsertNextError(buff);
        return 0;
    }
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiCatValueArray::HasValueDouble(int cat)
{
    double val = 0.0;
    if (DB_OK == db_CatValArray_get_value_double(&this->catval, cat, &val))
        return true;
   return false;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiCatValueArray::HasValueInteger(int cat)
{
    int val = 0;
    if (DB_OK == db_CatValArray_get_value_int(&this->catval, cat, &val))
        return true;
    return false;
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiCatValueArray::ValuesToDoubleArray(vtkIntArray *cats, vtkDoubleArray *values, double notFoundValue)
{
    int i;
    for(i = 0; i < cats->GetNumberOfTuples(); i++)
    {
        if(this->HasValueDouble(cats->GetValue(i)))
            values->InsertValue(i, this->GetValueDouble(cats->GetValue(i)));
        else
            values->InsertValue(i, notFoundValue);

    }
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiCatValueArray::ValuesToIntegerArray(vtkIntArray *cats, vtkIntArray *values, int notFoundValue)
{
    int i;
    for(i = 0; i < cats->GetNumberOfTuples(); i++)
    {
        if(this->HasValueInteger(cats->GetValue(i)))
            values->InsertValue(i, this->GetValueInteger(cats->GetValue(i)));
        else
            values->InsertValue(i, notFoundValue);
    }
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiCatValueArray::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
}

