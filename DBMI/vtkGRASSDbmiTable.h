
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

/*!
 * \brief This is the base class for read/write access to a vector map database table
 *
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Oct. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSDbmiTable_h
#define	_vtkGRASSDbmiTable_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSDbmiColumn.h"

extern "C" {
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiTable : public vtkObjectGRASSErrorHandler {
public:

    //BTX
    friend class vtkGRASSDbmiInterface;
    //ETX

    static vtkGRASSDbmiTable *New();
    vtkTypeMacro(vtkGRASSDbmiTable, vtkObjectGRASSErrorHandler);
    virtual void PrintSelf(ostream& os, vtkIndent indent);

    //!\brief Return the number of columns of this table

    virtual int GetNumberOfColumns() {
        if (this->table)
            return db_get_table_number_of_columns(this->table);
        return -1;
    }

    //!\brief Set the name of the table
    //! return 0 if success and 1 in case of failure

    virtual int SetName(const char *name) {
        if (this->table)
            return db_set_table_name(this->table, name);
        return DB_FAILED;
    }

    //!\brief Get the name of the table
    //! return NULL in case of failure

    virtual const char *GetName() {
        if (this->table)
            return db_get_table_name(this->table);
        return NULL;
    }

    //!\brief Set the description of the table
    //! return 0 if success and 1 in case of failure

    virtual int SetTableDescription(const char *name) {
        if (this->table)
            return db_set_table_name(this->table, name);
        return DB_FAILED;
    }

    //!\brief Get the description of the table
    //! return NULL in case of failure

    virtual const char *GetTableDescription() {
        if (this->table)
            return db_get_table_description(this->table);
        return NULL;
    }

    //!\brief Reses the table to an initial state

    virtual void Reset() {
        if (this->table)
            db_init_table(this->table);
    }

    //!\brief Reset the existing and allocate a specific number of columns

    virtual void Allocate(int ncols) {
        if (this->table)
            db_free_table(this->table);
        this->table = db_alloc_table(ncols);
    }

    //!\brief Set a column at a specific index
    virtual void SetColumn(vtkGRASSDbmiColumn *column, int idx);

    //!\brief Return a column of a specific name
    virtual bool GetColumn(const char* name, vtkGRASSDbmiColumn *column);

    //!\brief Return a column at a specific index
    virtual bool GetColumn(int idx, vtkGRASSDbmiColumn *column);

    //!\brief Append a vtkGRASSDbmiColumn
    virtual void AppendColumn(vtkGRASSDbmiColumn *column);

    //!\brief Make a deep copy of an existing table
    virtual void DeepCopy(vtkGRASSDbmiTable *table);

    //!\brief Create SQL CREATE sring from table definition
    virtual const char *TableToSQL();
    //int db_table_to_sql(dbTable * table, dbString * sql)

    //BTX
    virtual dbTable * GetPointer() {
        return this->table;
    }
    virtual void DeepCopy(dbTable *table);
    //ETX

protected:
    vtkGRASSDbmiTable();
    ~vtkGRASSDbmiTable();

    //BTX
    dbTable *table;
    //ETX


private:
    vtkGRASSDbmiTable(const vtkGRASSDbmiTable&); // Not implemented.
    void operator=(const vtkGRASSDbmiTable&); // Not implemented.
};

#endif
