
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
 * \brief This category value array is used to store category - value pairs
 * of database requests
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2010
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSDbmiCatValueArray_h
#define	_vtkGRASSDbmiCatValueArray_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSDefines.h"

extern "C" {
#include <grass/dbmi.h>
}

class vtkIntArray;
class vtkDoubleArray;

class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiCatValueArray : public vtkObjectGRASSErrorHandler {
public:

    static vtkGRASSDbmiCatValueArray *New();
    vtkTypeMacro(vtkGRASSDbmiCatValueArray, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);


    //!\brief Return true if the values are of type double
    virtual bool IsValueTypeDouble(){return ((this->catval.ctype == DB_C_TYPE_DOUBLE)?true:false);}
    //!\brief Return true if the values are of type integer
    virtual bool IsValueTypeInteger(){return ((this->catval.ctype == DB_C_TYPE_INT)?true:false);}

    //!\brief Get the double value for category cat
    virtual double GetValueDouble(int cat);
    //!\brief Get the integer value for category cat
    virtual int GetValueInteger(int cat);

    //!\brief Check if the double value for category cat exists
    virtual bool HasValueDouble(int cat);
    //!\brief Check if the integer value for category cat exists
    virtual bool HasValueInteger(int cat);

    //!\brief Return the number of allocated values
    virtual int GetNumberOfValues(){return this->catval.n_values;}

    //!\brief Write the values of the cat  value array into a double array, based
    //! on the categories which must be provided as integer array.
    //!\param cats the integer array with categories
    //!\param values the double array in which the found values are written in the order of the category array
    //!\param Value which should be used to identify Cat - value pairs which where not found
    virtual void ValuesToDoubleArray(vtkIntArray *cats, vtkDoubleArray *values, double notFoundValue);
    //!\brief Write the values of the cat  value array into a integer array, based
    //! on the categories which must be provided as integer array.
    //!\param cats the integer array with categories
    //!\param values the integer array in which the found values are written in the order of the category array
    //!\param Value which should be used to identify Cat - value pairs which where not found
    virtual void ValuesToIntegerArray(vtkIntArray *cats, vtkIntArray *values, int notFoundValue);

  //BTX
  virtual dbCatValArray * GetPointer(){return &this->catval;}
  //ETX

protected:
    vtkGRASSDbmiCatValueArray();
    ~vtkGRASSDbmiCatValueArray();

    //BTX
    dbCatValArray catval;
    //ETX


    //!\brief Allocate n cat value pairs
    virtual int Allocate(int n){return db_CatValArray_alloc(&this->catval, n);}
    //!\brief Resize the cat value array to n pairs
    virtual int Reallocate(int n){return db_CatValArray_realloc(&this->catval, n);}

    //!\brief Set the type of the array values to double
    virtual void SetValueTypeToDouble(){this->catval.ctype = DB_C_TYPE_DOUBLE;}
    //!\brief Set the type of the array values to double
    virtual void SetValueTypeToInteger(){this->catval.ctype = DB_C_TYPE_INT;}

    //!\brief Set the double value for category cat
    //!\param idx the index of the cat value pair
    //!\param cat the category
    //!\param value the double value
    //! no array size check is performed, make sure the array is alloacted
    //! to the correct size
    virtual void SetCatValueDouble(int idx, int cat, double value){
        this->catval.value[idx].cat = cat;
        this->catval.value[idx].val.d = value;
    }
    //!\brief Set the integer value for category cat
    //!\param idx the index of the cat value pair
    //!\param cat the category
    //!\param value the integer value
    //! no array size check is performed, make sure the array is alloacted
    //! to the correct size
    virtual void SetCatValueInteger(int idx, int cat, int value){
        this->catval.value[idx].cat = cat;
        this->catval.value[idx].val.i = value;
    }

    virtual int GetCatAtIndex(int idx){return this->catval.value[idx].cat;}
    virtual double GetDoubleValueAtIndex(int idx){return this->catval.value[idx].val.d;}
    virtual int GetIntegerValueAtIndex(int idx){return this->catval.value[idx].val.i;}

    //!\brief Sort the cat value array by categories
    virtual void SortByCats(){db_CatValArray_sort(&this->catval);}
    //!\brief Sort the cat value array by values
    virtual void SortByValues(){db_CatValArray_sort_by_value(&this->catval);}

private:
    vtkGRASSDbmiCatValueArray(const vtkGRASSDbmiCatValueArray&); // Not implemented.
    void operator=(const vtkGRASSDbmiCatValueArray&); // Not implemented.
};

#endif
