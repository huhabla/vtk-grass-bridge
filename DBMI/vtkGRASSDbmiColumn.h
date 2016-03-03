
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
 * \brief This class provides read/write access to a database tables column of a vector map
 *
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Oct. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSDbmiColumn_h
#define	_vtkGRASSDbmiColumn_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSDefines.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiColumn : public vtkObjectGRASSErrorHandler {
public:

    //BTX
    friend class vtkGRASSDbmiTable;
    //ETX

    static vtkGRASSDbmiColumn *New();
    vtkTypeMacro(vtkGRASSDbmiColumn, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    //! \brief Reset the column and fill with default values
    void Initialize();

    const char *GetDescription() {
        if (this->column)
            return db_get_column_description(this->column);
        return NULL;
    }

    int GetLength() {
        if (this->column)
            return db_get_column_length(this->column);
        return -1;
    }

    int GetHostType() {
        if (this->column)
            return db_get_column_host_type(this->column);
        return -1;
    }

    const char *GetName() {
        if (this->column)
            return db_get_column_name(this->column);
        return NULL;
    }

    int GetPrecision() {
        if (this->column)
            return db_get_column_precision(this->column);
        return -1;
    }

    int GetScale() {
        if (this->column)
            return db_get_column_scale(this->column);
        return -1;
    }

    int GetSelectPrivileg() {
        if (this->column)
            return db_get_column_select_priv(this->column);
        return -1;
    }

    int GetSQLType() {
        if (this->column)
            return db_get_column_sqltype(this->column);
        return -1;
    }

    const char *GetSQLTypeName() {
        if (this->column)
            return db_sqltype_name(db_get_column_sqltype(this->column));
        return NULL;
    }

    int GetUpdatePrivileg() {
        if (this->column)
            return db_get_column_update_priv(this->column);
        return -1;
    }

    bool IsValueDouble() {
        if (this->column)
            if (db_sqltype_to_Ctype(db_get_column_sqltype(this->column)) == DB_C_TYPE_DOUBLE)
                return true;
        return false;
    }

    bool IsValueInteger() {
        if (this->column)
            if (db_sqltype_to_Ctype(db_get_column_sqltype(this->column)) == DB_C_TYPE_INT)
                return true;
        return false;
    }

    bool IsValueString() {
        if (this->column)
            if (db_sqltype_to_Ctype(db_get_column_sqltype(this->column)) == DB_C_TYPE_STRING)
                return true;
        return false;
    }

    bool IsValueDatetime() {
        if (this->column)
            if (db_sqltype_to_Ctype(db_get_column_sqltype(this->column)) == DB_C_TYPE_DATETIME)
                return true;
        return false;
    }

    int GetValueInteger() {
        if (this->column)
            return db_get_value_int(db_get_column_value(this->column));
        return 0;
    }

    double GetValueDouble() {
        if (this->column)
            return db_get_value_double(db_get_column_value(this->column));
        return 0;
    }

    const char *GetValueString() {
        if (this->column)
            return db_get_value_string(db_get_column_value(this->column));
        return NULL;
    }

    void SetValueInteger(int value) {
        if (this->column)
            db_set_value_int(db_get_column_value(this->column), value);
    }

    void SetValueDouble(double value) {
        if (this->column)
            db_set_value_double(db_get_column_value(this->column), value);
    }

    void SetValueString(const char* string) {
        if (this->column)
            db_set_value_string(db_get_column_value(this->column), string);
    }

    char *GetValueAsString() {
        if (this->column) {
            dbString *valstr = (dbString*) (calloc(1, sizeof (dbString)));
            db_init_string(valstr);
            db_convert_column_value_to_string(this->column, valstr);
            return db_get_string(valstr);
        }
        return NULL;
    }

    void SetHasDefinedDefaultValue() {
        if (this->column)
            db_set_column_has_defined_default_value(this->column);
    }

    void SetHasUndefinedDefaultValue() {
        if (this->column)
            db_set_column_has_undefined_default_value(this->column);
    }

    void SetNullAllowed() {
        if (this->column)
            db_set_column_null_allowed(this->column);
    }

    void SetUseDefaultValue() {
        if (this->column)
            db_set_column_use_default_value(this->column);
    }

    void SetSelectPrivilegNotGranted() {
        if (this->column)
            db_set_column_select_priv_not_granted(this->column);
    }

    void SetSelectPrivilegGranted() {
        if (this->column)
            db_set_column_select_priv_granted(this->column);
    }

    void SetDescription(const char*description) {
        if (this->column)
            db_set_column_description(this->column, description);
    }

    void SetHostType(int host_type) {
        if (this->column)
            db_set_column_host_type(this->column, host_type);
    }

    void SetLength(int length) {
        if (this->column)
            db_set_column_length(this->column, length);
    }

    void SetName(const char* name) {
        if (this->column)
            db_set_column_name(this->column, name);
    }

    void SetPrecision(int precision) {
        if (this->column)
            db_set_column_precision(this->column, precision);
    }

    void SetScale(int scale) {
        if (this->column)
            db_set_column_scale(this->column, scale);
    }

    void SetSQLTypeToCharacter() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_CHARACTER);
    }
    void SetSQLTypeToDecimal() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_DECIMAL);
    }
    void SetSQLTypeToDoublePrecision() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_DOUBLE_PRECISION);
    }
    void SetSQLTypeToInteger() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_INTEGER);
    }
    void SetSQLTypeToReal() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_REAL);
    }
    void SetSQLTypeToSmallInt() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_SMALLINT);
    }
    void SetSQLTypeToUnknown() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_UNKNOWN);
    }
    void SetSQLTypeToString() {
        if (this->column)
            db_set_column_sqltype(this->column, DB_SQL_TYPE_TEXT);
    }

    //!\brief Make a deep copy of column
    virtual void DeepCopy(vtkGRASSDbmiColumn *column);

    //BTX
    dbColumn *GetPointer() {
        return this->column;
    }
    virtual void DeepCopy(dbColumn *column);
    // ETX

protected:
    vtkGRASSDbmiColumn();
    ~vtkGRASSDbmiColumn();

    //BTX
    dbColumn *column;
    //ETX

private:
    vtkGRASSDbmiColumn(const vtkGRASSDbmiColumn&); // Not implemented.
    void operator=(const vtkGRASSDbmiColumn&); // Not implemented.
};

#endif
