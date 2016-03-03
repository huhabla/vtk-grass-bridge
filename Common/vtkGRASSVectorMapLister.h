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
 * \brief This class lists all available vector maps of a mapset
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSVectorMapLister_h
#define __vtkGRASSVectorMapLister_h

#include <vtkGRASSFileLister.h>
#include "vtkGRASSBridgeCommonWin32Header.h"

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSVectorMapLister : public vtkGRASSFileLister
{
public:
  static  vtkGRASSVectorMapLister *New();
  vtkTypeMacro(vtkGRASSVectorMapLister,vtkGRASSFileLister);

protected:
  vtkGRASSVectorMapLister();
  ~vtkGRASSVectorMapLister() {};


private:
  vtkGRASSVectorMapLister(const vtkGRASSVectorMapLister&);  // Not implemented.
  void operator=(const vtkGRASSVectorMapLister&);  // Not implemented.
};


#endif
