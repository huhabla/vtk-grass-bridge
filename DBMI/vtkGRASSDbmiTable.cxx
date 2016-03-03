
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

#include "vtkGRASSDbmiTable.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>

vtkStandardNewMacro(vtkGRASSDbmiTable);

//----------------------------------------------------------------------------

vtkGRASSDbmiTable::vtkGRASSDbmiTable() {
    this->table = db_alloc_table(0);
}

//----------------------------------------------------------------------------

vtkGRASSDbmiTable::~vtkGRASSDbmiTable() {
    if(this->table)
        db_free_table(this->table);
    this->table = NULL;
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiTable::DeepCopy(dbTable *table) {

    if (this->table)
        db_free_table(this->table);

    this->table = db_clone_table(table);

    return;
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiTable::DeepCopy(vtkGRASSDbmiTable *table) {

    this->DeepCopy(table->GetPointer());
    return;
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiTable::SetColumn(vtkGRASSDbmiColumn *column, int idx)
{
    db_set_table_column(this->table, idx, column->GetPointer());
    return;
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiTable::AppendColumn(vtkGRASSDbmiColumn *column) {

    db_append_table_column(this->table, column->GetPointer());
    return;
}

//----------------------------------------------------------------------------
bool vtkGRASSDbmiTable::GetColumn(int idx, vtkGRASSDbmiColumn *column)
{
    dbColumn *c = NULL;
    c = db_get_table_column(this->table, idx);
    if(c == NULL)
        return true;

    column->DeepCopy(c);
    return true;
}

//----------------------------------------------------------------------------
bool vtkGRASSDbmiTable::GetColumn(const char* name, vtkGRASSDbmiColumn *column)
{
    dbColumn *c = NULL;

    c = db_get_table_column_by_name(this->table, name);
    if(c == NULL)
        return false;

    column->DeepCopy(c);
    
    return true;
}

//----------------------------------------------------------------------------
const char* vtkGRASSDbmiTable::TableToSQL()
{
    dbString sql;
    db_init_string(&sql);
    if(db_table_to_sql(this->table, &sql) != DB_OK)
        return NULL;

    char *s = db_get_string(&sql);
    return s;
}

//----------------------------------------------------------------------------
void
vtkGRASSDbmiTable::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
    os << indent << "Table name:  " << this->GetName() << "\n";
    os << indent << "Table description:  " << this->GetTableDescription() << "\n";
    os << indent << "Table number of columns:  " << this->GetNumberOfColumns() << "\n";
    int i;
    for (i = 0; i < this->GetNumberOfColumns(); i++) {
        //this->GetColumn(i)->PrintSelf(os, indent.GetNextIndent());
    }
}

