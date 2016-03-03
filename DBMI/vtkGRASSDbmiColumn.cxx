
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

#include "vtkGRASSDbmiColumn.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>

vtkStandardNewMacro(vtkGRASSDbmiColumn);

//----------------------------------------------------------------------------

vtkGRASSDbmiColumn::vtkGRASSDbmiColumn()
{
    this->column = (dbColumn *) db_calloc(sizeof(dbColumn), 1);
    db_init_column(this->column);
}

//----------------------------------------------------------------------------

vtkGRASSDbmiColumn::~vtkGRASSDbmiColumn()
{

    if (this->column)
        db_free_column(this->column);
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiColumn::Initialize()
{

    if (this->column)
        db_free_column(this->column);

    this->column = (dbColumn *) db_calloc(sizeof(dbColumn), 1);
    db_init_column(this->column);
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiColumn::DeepCopy(dbColumn *column)
{

    if (this->column == column)
        return;

    this->Initialize();

    db_copy_column(this->column, column);
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiColumn::DeepCopy(vtkGRASSDbmiColumn *column)
{
    this->DeepCopy(column->GetPointer());
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiColumn::PrintSelf(ostream& os, vtkIndent indent)
{

    this->Superclass::PrintSelf(os, indent);
    if (this->column)
    {
        os << indent << "Name: " << this->GetName() << endl;
        os << indent << "Description: " << this->GetDescription() << endl;
        os << indent << "HostType: " << this->GetHostType() << endl;
        os << indent << "Length: " << this->GetLength() << endl;
        os << indent << "SQLType: " << this->GetSQLType() << endl;
        os << indent << "Precision: " << this->GetPrecision() << endl;
        os << indent << "Scale: " << this->GetScale() << endl;
        os << indent << "SelectPrivileg: " << this->GetSelectPrivileg() << endl;
        os << indent << "ValueAsString: " << this->GetValueAsString() << endl;
        os << indent << "UpdatePrivileg: " << this->GetUpdatePrivileg() << endl;
    }
}

