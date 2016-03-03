
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

#include "vtkGRASSDbmiValue.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>

vtkStandardNewMacro(vtkGRASSDbmiValue);

//----------------------------------------------------------------------------

vtkGRASSDbmiValue::vtkGRASSDbmiValue() {
    this->value = (dbValue*)db_calloc(sizeof(dbValue), 1);
}

//----------------------------------------------------------------------------

vtkGRASSDbmiValue::~vtkGRASSDbmiValue() {

    if(this->value)
        G_free(this->value);
    this->value = NULL;
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiValue::DeepCopy(vtkGRASSDbmiValue* value)
{
    this->DeepCopy(value->GetPointer());
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiValue::DeepCopy(dbValue* value)
{
    if(this->value)
        db_copy_value(this->value, value);
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiValue::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
        os << indent <<  "Table double  value: " <<  db_get_value_double(this->value) << endl;
        os << indent <<  "Table integer value: " <<  db_get_value_int(this->value) << endl;
}

