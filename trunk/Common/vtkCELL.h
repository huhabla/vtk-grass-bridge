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
 * \brief This class should be used when a double value is a mutable 
 *        argument of a method and other approaches are not available
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkCELL_h
#define __vtkCELL_h

#include "vtkObject.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster.h>
}


class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkCELL : public vtkObject
{
public:
  static  vtkCELL *New();
  vtkTypeRevisionMacro(vtkCELL,vtkObject);

  double GetValueAsInt(){return (int)this->Value;}
  double GetValueAsFloat(){return (float)this->Value;}
  double GetValueAsDouble(){return (double)this->Value;}

  void SetIntValue(int value){this->Value = (CELL)value;}

  bool IsNull(){return (Rast_is_c_null_value(&(this->Value))?true:false);}
  void SetNull(){Rast_set_c_null_value(&(this->Value), 1);}

  //BTX
  CELL Value;
  //ETX
  
protected:
  vtkCELL(){;}
  ~vtkCELL() {};

private:
  vtkCELL(const vtkCELL&);  // Not implemented.
  void operator=(const vtkCELL&);  // Not implemented.
};

#endif