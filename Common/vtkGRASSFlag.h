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
 * \brief This class creates grass command line flags
 *
 * Example python code how to use this class:
 *
 * \verbatim
   flag = vtkGRASSFlag()
   flag.SetDescription("This is a flag")
   flag.SetKey('f')
   flag.SetGuiSection("Flags")
 * \endverbatim
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2010
 * \author soerengebbert@googlemail.com
 *
 *
 * \TODO 
 *
 * */

#ifndef __vtkGRASSFlag_h
#define __vtkGRASSFlag_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"
{
#include <grass/gis.h>
}

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSFlag : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSFlag *New();
  vtkTypeMacro(vtkGRASSFlag,vtkObjectGRASSErrorHandler);

  //!\brief Set the key.
  //! This will set the identifier
  void SetKey(char key){this->flag->key = key;}
  //!\brief Set the description of the flag
  //! This description will be used within the help and the manual page
  void SetDescription(const char *description);
  //!\brief Set the label of the module
  void SetLabel(const char *label);
  //!\brief Set the gui section identifier. This string is used to create
  //! specific tabs in the generated gui
  void SetGuiSection(const char *guisection);
  //!\brief Return the flag desription
  const char * GetDescription(){return this->flag->description;}
  //!\brief Return the guisection string
  const char* GetGuiSection() {return this->flag->guisection;}
  //!\brief Return the label string
  const char* GetLabel() {return this->flag->label;}
  //!\brief Return the answer (true or false)
  bool GetAnswer() {return (this->flag->answer?true:false);}
  
protected:
  vtkGRASSFlag();
  ~vtkGRASSFlag() {};

  //BTX
  struct Flag *flag;
  char *Description;
  char *Label;
  char *GuiSection;
  //ETX

private:
  vtkGRASSFlag(const vtkGRASSFlag&);  // Not implemented.
  void operator=(const vtkGRASSFlag&);  // Not implemented.
};


#endif
