
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
 * \brief This class provides an interface to the raster3d map io library to
 * write a raster3d map.
 *
 * The user can choose the current region, the default region, the raster3d map region or
 * a user defined region to open the new raster3d map.
 *
 * This class does not use the vtk update mechanism.
 * Before writing the new raster3d map you need to open it.
 * After writing you must close the raster3d map or delete the object. The
 * destructor will close the map.
 * 
 * The user can provide a vtkGRASSHistory object, which will be used to write the history
 * of the map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */
#ifndef __vtkGRASSRaster3dMapWriter_h
#define __vtkGRASSRaster3dMapWriter_h

#include <vtkGRASSRaster3dMapBase.h>
#include "vtkGRASSBridgeRaster3dWin32Header.h"

class vtkDataArray;

class VTK_GRASS_BRIDGE_RASTER3D_EXPORT vtkGRASSRaster3dMapWriter : public vtkGRASSRaster3dMapBase
{
public:
  static  vtkGRASSRaster3dMapWriter *New();
  vtkTypeMacro(vtkGRASSRaster3dMapWriter,vtkGRASSRaster3dMapBase);

  virtual void SetMapTypeToFCELL() {this->SetMapType(FCELL_TYPE);}
  virtual void SetMapTypeToDCELL() {this->SetMapType(DCELL_TYPE);}

  //! \brief Set the histroy which should be written to the raster3d map
  vtkSetObjectMacro(History, vtkGRASSHistory);

  //! \brief Open new raster3d map for writing
  virtual bool OpenMap(char *name);
  //! \brief Write a single value at position x, y, and z, returns 1 on success, -1 in case of an error
  virtual int PutValue(int x, int y, int z, double value);
  //! \brief close Map and write a short history
  virtual bool CloseMap();
  
protected:
  vtkGRASSRaster3dMapWriter();
  ~vtkGRASSRaster3dMapWriter(){};

private:

    vtkGRASSRaster3dMapWriter(const vtkGRASSRaster3dMapWriter&);  // Not implemented.
  void operator=(const vtkGRASSRaster3dMapWriter&);  // Not implemented.
};


#endif
