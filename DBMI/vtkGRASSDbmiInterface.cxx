
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

#include "vtkGRASSDbmiInterface.h"
#include "vtkGRASSDbmiValue.h"
#include <vtkObjectFactory.h>
#include "vtkGRASSDbmiCatValueArray.h"
#include "vtkGRASSDbmiTable.h"

vtkStandardNewMacro(vtkGRASSDbmiInterface);

//----------------------------------------------------------------------------

vtkGRASSDbmiInterface::vtkGRASSDbmiInterface() {
    this->driver =  NULL;
    this->Connected = false;
    this->FieldNumber = 1;
    this->VectorMap = NULL;
}

//----------------------------------------------------------------------------

vtkGRASSDbmiInterface::~vtkGRASSDbmiInterface() {

    this->DisconnectDB();
    
    if(this->driver)
        G_free(this->driver);
}

//----------------------------------------------------------------------------

int vtkGRASSDbmiInterface::GetNumberOfRows()
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        char buff[1024];
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi == NULL) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to get field info for map %s",
                    this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
            this->InsertNextError(buff);
            return false;
        }
        G_snprintf(buff, 1024, "SELECT * FROM %s", Fi->table);
        dbString sql;
        db_init_string(&sql);
        db_set_string(&sql, buff);
        return db_get_table_number_of_rows(this->driver, &sql);
    }
    return -1;
}

//----------------------------------------------------------------------------

void vtkGRASSDbmiInterface::DisconnectDB()
{
    if(this->driver && this->Connected) {
        db_close_database_shutdown_driver(this->driver);
    }
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::SelectValue(int cat, const char* column, vtkGRASSDbmiValue* value)
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi == NULL) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to get field info for map %s",
                    this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
            this->InsertNextError(buff);
            return false;
        }
        if(db_select_value(this->driver, Fi->table, Fi->key, cat, column, value->GetPointer())<0) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to select value",
                    this->GetClassName(), __LINE__);
            this->InsertNextError(buff);
            return false;
        }
    }else {
        return false;
    }
    return true;
}

bool vtkGRASSDbmiInterface::SelectCatValueArray(const char *column, const char *where, vtkGRASSDbmiCatValueArray *catval)
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi == NULL) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to get field info for map %s",
                    this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
            this->InsertNextError(buff);
            return false;
        }
        if(db_select_CatValArray(this->driver, Fi->table, Fi->key, column, where, catval->GetPointer())<0) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to select CatValueArray",
                    this->GetClassName(), __LINE__);
            this->InsertNextError(buff);
            return false;
        }
    }else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::BeginTransaction()
{
    if(this->Connected) {
        if(db_begin_transaction(this->driver) != DB_OK) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to begin transaction",
                    this->GetClassName(), __LINE__);
            this->InsertNextError(buff);
            return false;
        }
    } else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::ExecuteImmediate(const char *sql)
{
    if(this->Connected) {
        dbString s;
        db_init_string(&s);
        db_set_string(&s, sql);
        if(db_execute_immediate(this->driver, &s) != DB_OK) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to execute sql statement",
                    this->GetClassName(), __LINE__);
            this->InsertNextError(buff);
            return false;
        }
    } else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::CommitTransaction()
{
    if(this->Connected) {
        if(db_commit_transaction(this->driver) != DB_OK) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to commit transaction",
                    this->GetClassName(), __LINE__);
            this->InsertNextError(buff);
            return false;
        }
    } else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::AddColumn(vtkGRASSDbmiColumn *column)
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi == NULL) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to get field info for map %s",
                    this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
            this->InsertNextError(buff);
            return false;
        }
        char sql[1024];

        G_snprintf(sql, 1024, "ALTER TABLE %s ADD %s %s", Fi->table,
            column->GetName(), column->GetSQLTypeName());
        
        this->BeginTransaction();
        this->ExecuteImmediate(sql);
        this->CommitTransaction();
    }else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::DropColumn(const char *column)
{
    if(this->VectorMap && this->VectorMap->IsOpen() && this->Connected) {
        struct field_info *Fi;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi == NULL) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to get field info for map %s",
                    this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
            this->InsertNextError(buff);
            return false;
        }
        char sql[1024];
        G_snprintf(sql, 1024, "ALTER TABLE %s DROP COLUMN %s", Fi->table, column);

        this->BeginTransaction();
        this->ExecuteImmediate(sql);
        this->CommitTransaction();

    }else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::ConnectDB()
{
    this->Connected = false;

    if(this->VectorMap && this->VectorMap->IsOpen() && this->VectorMap->GetNumberOfDBLinks()) {
        struct field_info *Fi = NULL;
        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi) {
            this->driver = db_start_driver_open_database(Fi->driver, Fi->database);
            if(this->driver == NULL) {
                char buff[1024];
                G_snprintf(buff, 1024, "class: %s line: %i Unable open database connection for map %s",
                        this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
                this->InsertNextError(buff);
                return false;
            }
        } else
            return false;
        return this->Connected = true;
    }
    return this->Connected = false;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::GetTable(vtkGRASSDbmiTable *table)
{
    dbTable *t;
    dbString table_name;
    struct field_info *Fi = NULL;

    db_init_string(&table_name);

    if(this->VectorMap && this->VectorMap->IsOpen() && this->VectorMap->GetNumberOfDBLinks()) {

        if(table == NULL)
            return false;

        Fi = Vect_get_field(this->VectorMap->GetPointer(), this->FieldNumber);
        if(Fi == NULL) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to get field info for map %s",
                    this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
            this->InsertNextError(buff);
            return false;
        }

	db_set_string(&table_name, Fi->table);
        if(db_describe_table(this->driver, &table_name, &t) != DB_OK) {
            char buff[1024];
            G_snprintf(buff, 1024, "class: %s line: %i Unable to describe table %s",
                    this->GetClassName(), __LINE__, Fi->table);
            this->InsertNextError(buff);
            return false;
        }
        table->DeepCopy(t);
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------

bool vtkGRASSDbmiInterface::ConnectDBCreateTable(vtkGRASSDbmiTable *table)
{
    this->Connected = false;

    if(this->VectorMap && this->VectorMap->IsOpen() && this->VectorMap->GetNumberOfDBLinks() == 0) {
        struct field_info *Fi = NULL;
        Fi = Vect_default_field_info(this->VectorMap->GetPointer(), this->FieldNumber, NULL, GV_1TABLE);
        if(Fi) {
            //Open database
            this->driver = db_start_driver_open_database(Fi->driver, Fi->database);
            if(this->driver == NULL) {
                char buff[1024];
                G_snprintf(buff, 1024, "class: %s line: %i Unable open database connection for map %s",
                        this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
                this->InsertNextError(buff);
                return this->Connected = false;
            }

            // Connection established
            this->Connected = true;
            
            if(Vect_map_add_dblink(this->VectorMap->GetPointer(), this->FieldNumber,
                                NULL, Fi->table, "cat", Fi->database, Fi->driver)< 0) {
                char buff[1024];
                G_snprintf(buff, 1024, "class: %s line: %i Unable add database link to map %s",
                        this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
                this->InsertNextError(buff);
                return this->Connected = false;
            }

            if(!Vect_map_check_dblink(this->VectorMap->GetPointer(), this->FieldNumber, Fi->table)) {
                char buff[1024];
                G_snprintf(buff, 1024, "class: %s line: %i Error checking dblink of map %s",
                        this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
                this->InsertNextError(buff);
                return this->Connected = false;
            }

            if(!this->BeginTransaction())
                return false;

            // Make sure the name of the table is the name of the vector map
            table->SetName(Fi->table);

            if(!this->ExecuteImmediate(table->TableToSQL()))
                return false;

            if(!this->CommitTransaction())
                return false;

            //Check the created table
            VGB_CREATE(vtkGRASSDbmiTable, t);
            this->GetTable(t);
            if(t->GetNumberOfColumns() != t->GetNumberOfColumns()) {
                char buff[1024];
                G_snprintf(buff, 1024, "class: %s line: %i Unable to create valid table for map %s",
                        this->GetClassName(), __LINE__, this->VectorMap->GetFullName());
                this->InsertNextError(buff);
                return this->Connected = false;
            }
        } else {
            return false;
        }
        return this->Connected = true;
    } else {
        this->ConnectDB();
    }
    return this->Connected = false;
}

//----------------------------------------------------------------------------

void
vtkGRASSDbmiInterface::PrintSelf(ostream& os, vtkIndent indent) {

    this->Superclass::PrintSelf(os, indent);
    os << indent << "Field number: " << this->FieldNumber << endl;
    os << indent << "Connected: " << this->Connected << endl;
    os << indent << "Registered vector map: " << (this->VectorMap?this->VectorMap->GetFullName():"none") << endl;
    if(this->VectorMap)
        this->VectorMap->PrintSelf(os, indent.GetNextIndent());
}

