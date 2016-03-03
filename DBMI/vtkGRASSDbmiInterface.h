
/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2009 - 2010 by Soeren Gebbert, soerengebbert@googlemail.com
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
 * \brief This class provides read and write access to vector map database tables in grass
 *
 * Objects of this class and subclasses will only be created by subclasses of vtkGRASSVectorMapBase
 * to ensure that database tables are always related to vector maps
 *
 * This class provides methods to open and close the database connection of
 * grass vector maps as well as functions to create tables,
 * to select single values or an array of values
 * from a table column. The selection can be modified using where statements.
 *
 * \TODO Rewrite this class to be independent from an existing vecor map. So it can
 *       be used to access SQL databases without the need of a vector map.
 * \TODO Use the field_info structure internally to identify the database, driver,
 *       key and table. Use a vector map to set these values or create a ability to 
 *       do this by hand.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Oct. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSDbmiInterface_h
#define	_vtkGRASSDbmiInterface_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSVectorMapBase.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSDbmiColumn.h"

class vtkIntArray;
class vtkDataArray;
class vtkGRASSDbmiValue;
class vtkGRASSDbmiCatValueArray;
class vtkGRASSDbmiTable;

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
#include <grass/dbmi.h>
}

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiInterface : public vtkObjectGRASSErrorHandler {
public:
    //BTX
    friend class vtkGRASSVectorMapNoTopoReader;
    friend class vtkGRASSVectorMapTopoReader;
    friend class vtkGRASSVectorMapWriter;
    friend class vtkGRASSVectorMapUpdater;
    //ETX

    static vtkGRASSDbmiInterface *New();
    vtkTypeMacro(vtkGRASSDbmiInterface, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);


    //!\brief Open a connection to the database. A databse connection can only
    //!be established when a VectorMap was specified and opened.
    virtual bool ConnectDB();

    //!\brief Open a connection to the database and create a table. A databse connection can only
    //! be established when a VectorMap was specified and opened.
    //! The provided table must have the same name as the vector map, if not the name will be
    //! changed to the vector map name. In case a database connection is already
    //! available no new table will be created.
    virtual bool ConnectDBCreateTable(vtkGRASSDbmiTable *table);

    //!\brief Close the database connection
    virtual void DisconnectDB();

    //!\brief Select a value from the vector map table. This works only when the
    //!database connection is established.
    //!\param cat The category
    //!\param column The name of the column to select values from
    //!\brief value The value object which stores the result
    //!\return true on success or false on error
    virtual bool SelectValue(int cat, const char *column, vtkGRASSDbmiValue *value);

    //!\brief Get the number of rows of the entire table
    virtual int GetNumberOfRows();

    //!\brief Select all values of a specific column and store the result in a category value array
    //!For each category a specific value is stored. Currently only integer and double values are supported.
    //!The selection can be modified with a where statement i.e. "pH < 6.5" in case pH is a column name
    //!\param column The name of the column to select values from
    //!\param where The where statement to modify the selection
    //!\param catval The category value array to store the results
    //!\return true on success or false on error
    virtual bool SelectCatValueArray(const char *column, const char *where, vtkGRASSDbmiCatValueArray *catval);

    //!\brief Select all values of a specific column and store the result in a category value array
    //!For each category a specific value is stored. Currently only integer and double values are supported.
    //!\param column The name of the column to select values from
    //!\param catval The category value array to store the results
    //!\return true on success or false on error
    virtual bool SelectCatValueArray(const char *column, vtkGRASSDbmiCatValueArray *catval){return this->SelectCatValueArray(column, NULL, catval);}

    //!\brief Start a database transaction request to execute SQL statements
    virtual bool BeginTransaction();
    
    //!\brief Immediately execute an SQL statement within a transaction request
    //! This statemanet can only be executed if BeginTransaction() was called first
    //! To finish the execution, CommitTransaction() must be called.
    //! Any SQL steatement can be executed which is supported by the underlying DB
    //! INSERT, UPDATE, ALTER, CREATE and so on
    //!\param sql The SQL statement as string
    virtual bool ExecuteImmediate(const char *sql);

    //!\brief Commit the database transaction request
    virtual bool CommitTransaction();

    //!\brief Add a column to the existing vector table
    //!\attentio Do not use this method within a transaction request
    virtual bool AddColumn(vtkGRASSDbmiColumn *column);

    //!\brief Dropcd a column to the existing vector table
    //!\attention This method does not work with sqlite databases
    //!\attention Do not use this method within a transaction request
    virtual bool DropColumn(const char *column);

    //!\brief Get the vector table
    virtual bool GetTable(vtkGRASSDbmiTable *table);

    //!\brief Which field number should be used to access the vector database
    vtkSetMacro(FieldNumber, int);
    //!\brief Which field number should be used to access the vector database
    vtkGetMacro(FieldNumber, int);

    //!\brief Get the vector map from which the database connection is based on
    vtkGetObjectMacro(VectorMap, vtkGRASSVectorMapBase);

    //!\brief Check if the database connection is established
    bool IsConnected(){return this->Connected;}

protected:
    vtkGRASSDbmiInterface();
    ~vtkGRASSDbmiInterface();

    vtkSetObjectMacro(VectorMap, vtkGRASSVectorMapBase);

    int FieldNumber;
    bool Connected;
    //BTX
    dbDriver *driver;
    //ETX

    //The vector map which the database connection is based on
    vtkGRASSVectorMapBase *VectorMap;

private:
    vtkGRASSDbmiInterface(const vtkGRASSDbmiInterface&); // Not implemented.
    void operator=(const vtkGRASSDbmiInterface&); // Not implemented.
};

#endif
