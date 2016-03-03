
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

#ifndef _vtkGRASSDbmiValue_h
#define	_vtkGRASSDbmiValue_h

extern "C" {
#include <grass/dbmi.h>
}

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeDbmiWin32Header.h"
#include "vtkGRASSDefines.h"



class VTK_GRASS_BRIDGE_DBMI_EXPORT vtkGRASSDbmiValue : public vtkObjectGRASSErrorHandler {
public:

    //BTX
    friend class vtkGRASSDbmiInterface;
    //ETX

    static vtkGRASSDbmiValue *New();
    vtkTypeMacro(vtkGRASSDbmiValue, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    //!\brief Return the double value
    double GetDouble(){return db_get_value_double(this->value);}

    //!\brief Return the integer value
    int GetInteger(){return db_get_value_int(this->value);}

    //!\brief Return the string value
    const char* GetString(){return db_get_value_string(this->value);}
    
    //!\brief Test if value is null
    bool IsNull(){return (db_test_value_isnull(this->value)?true:false);}

  virtual void DeepCopy(vtkGRASSDbmiValue *value);
  //BTX
  virtual dbValue * GetPointer(){return this->value;}
  virtual void DeepCopy(dbValue *value);
  //ETX

protected:
    vtkGRASSDbmiValue();
    ~vtkGRASSDbmiValue();

    //BTX
    dbValue *value;
    //ETX

private:
    vtkGRASSDbmiValue(const vtkGRASSDbmiValue&); // Not implemented.
    void operator=(const vtkGRASSDbmiValue&); // Not implemented.
};

#endif
