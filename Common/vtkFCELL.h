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

/**
 * \brief This is a simple wrapper class to have FCELL value support in Python and Java code. 
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkFCELL_h
#define __vtkFCELL_h

#include "vtkObject.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster.h>
}


class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkFCELL : public vtkObject
{
public:
  static  vtkFCELL *New();
  vtkTypeMacro(vtkFCELL,vtkObject);

  //!\brief The internal FCELL value is cast to int and returned
  double GetValueAsInt(){return (int)this->Value;}
  //!\brief The internal FCELL value is cast to float and returned
  double GetValueAsFloat(){return (float)this->Value;}
  //!\brief The internal FCELL value is cast to double and returned
  double GetValueAsDouble(){return (double)this->Value;}

  //!\brief Set the value using a value value which is casted to FCELL internally
  void SetFloatValue(float value){this->Value = (FCELL)value;}
  //!\brief Check if the FCELL value is of type null
  bool IsNull(){return (Rast_is_f_null_value(&(this->Value))?true:false);}
  //!\brief Set the FCELL value to null
  void SetNull(){Rast_set_f_null_value(&(this->Value), 1);}

  //BTX
  FCELL Value;
  //ETX
  
protected:
  vtkFCELL(){;}
  ~vtkFCELL() {};

private:
  vtkFCELL(const vtkFCELL&);  // Not implemented.
  void operator=(const vtkFCELL&);  // Not implemented.
};

#endif
