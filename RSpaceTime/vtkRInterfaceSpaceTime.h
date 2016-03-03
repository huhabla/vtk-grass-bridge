/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2011 by Soeren Gebbert, soerengebbert@googlemail.com
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
 * \brief This class helps to converts vtkPointData and vtkImageData into R spatial objects 
 * 
 * Only Point coordiantes and point data are currently supported. Use the VTK class vtkCellDataToPointData
 * to convert cell data into point data. Make sure correct and unique array names are used.
 * The spatial objects can be accessed in the VTK R instance by an identifier which must
 * be specified. 
 * 
 * */

#ifndef __vtkRInterfaceSpaceTime_h
#define __vtkRInterfaceSpaceTime_h

#include <vtkRInterfaceSpatial.h>
#include "vtkGRASSBridgeRSpaceTimeWin32Header.h"

class vtkDoubleArray;
class vtkTemporalDataSet;

class VTK_GRASS_BRIDGE_RSPACETIME_EXPORT vtkRInterfaceSpaceTime : public vtkRInterfaceSpatial
{
public:
  static  vtkRInterfaceSpaceTime *New();
  vtkTypeMacro(vtkRInterfaceSpaceTime, vtkRInterface);
  
  //! \brief Assign data arrays present in a dataset attribute to R using the array names as R variable names. Make sure correct and unique array names are used.
  virtual bool AssignVTKTemporalDataSetToRSTFDF(vtkTemporalDataSet *data, const char *RVariable, vtkDataArray *timesteps, const char *startDate);

protected:
  vtkRInterfaceSpaceTime();
  ~vtkRInterfaceSpaceTime(){;}

private:
  vtkRInterfaceSpaceTime(const vtkRInterfaceSpaceTime&);  // Not implemented.
  void operator=(const vtkRInterfaceSpaceTime&);  // Not implemented.
};


#endif
