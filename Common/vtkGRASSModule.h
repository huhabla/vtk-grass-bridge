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
 * \brief 
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 *
 * */

#ifndef __vtkGRASSModule_h
#define __vtkGRASSModule_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"
{
#include <grass/gis.h>
}

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSModule : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSModule *New();
  vtkTypeMacro(vtkGRASSModule,vtkObjectGRASSErrorHandler);

  //!\brief Adds a keyword to the module description
  void AddKeyword(const char *keyword);
  //!\brief Set the module description
  //! This description will be used within the help and the manual page
  void SetDescription(const char *description);
  //!\brief Set the label of the module
  void SetLabel(const char *label);
  //!\brief Return the module desription
  const char * GetDescription(){return this->module->description;}
  //!\brief Return the label string
  const char* GetLabel() {return this->module->label;}

protected:
  vtkGRASSModule();
  ~vtkGRASSModule(){;}

  //BTX
  struct GModule *module;
  char *Description;
  char *Label;
  //ETX


private:
  vtkGRASSModule(const vtkGRASSModule&);  // Not implemented.
  void operator=(const vtkGRASSModule&);  // Not implemented.
};


#endif
