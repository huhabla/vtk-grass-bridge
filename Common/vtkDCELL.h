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
 * \brief This is a simple wrapper class to have DCELL value support in Python and Java code. 
 * 
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkDCELL_h
#define __vtkDCELL_h

#include "vtkObject.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster.h>
}


class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkDCELL : public vtkObject
{
public:
  static  vtkDCELL *New();
  vtkTypeMacro(vtkDCELL,vtkObject);

  //!\brief The internal DCELL value is cast to int and returned
  double GetValueAsInt(){return (int)this->Value;}
  //!\brief The internal DCELL value is cast to float and returned
  double GetValueAsFloat(){return (float)this->Value;}
  //!\brief The internal DCELL value is cast to double and returned
  double GetValueAsDouble(){return (double)this->Value;}

  //!\brief Set the value using a double value which is casted to DCELL internally
  void SetDoubleValue(double value){this->Value = (DCELL)value;}
  //!\brief Check if the DCELL value is of type null
  bool IsNull(){return (Rast_is_d_null_value(&(this->Value))?true:false);}
  //!\brief Set the DCELL value to null
  void SetNull(){Rast_set_d_null_value(&(this->Value), 1);}

  //BTX
  DCELL Value;
  //ETX

protected:
  vtkDCELL(){;}
  ~vtkDCELL() {};

private:
  vtkDCELL(const vtkDCELL&);  // Not implemented.
  void operator=(const vtkDCELL&);  // Not implemented.
};

#endif
