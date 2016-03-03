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
 * \brief This class lists all available raster maps of a mapset
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRaster3dMapLister_h
#define __vtkGRASSRaster3dMapLister_h

#include <vtkGRASSFileLister.h>
#include "vtkGRASSBridgeCommonWin32Header.h"

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSRaster3dMapLister : public vtkGRASSFileLister
{
public:
  static vtkGRASSRaster3dMapLister *New();
  vtkTypeMacro(vtkGRASSRaster3dMapLister,vtkGRASSFileLister);

protected:
  vtkGRASSRaster3dMapLister();
  ~vtkGRASSRaster3dMapLister() {};


private:
  vtkGRASSRaster3dMapLister(const vtkGRASSRaster3dMapLister&);  // Not implemented.
  void operator=(const vtkGRASSRaster3dMapLister&);  // Not implemented.
};


#endif
