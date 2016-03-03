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
 * \brief This class provides an interface to the grass message anf error functions
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSMessagingInterface_h
#define __vtkGRASSMessagingInterface_h

#include "vtkGRASSBridgeCommonWin32Header.h"
#include "vtkObjectGRASSErrorHandler.h"

extern "C" {
#include <grass/gis.h>
#include <math.h>
}

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSMessagingInterface : public vtkObjectGRASSErrorHandler
{
public:
  static vtkGRASSMessagingInterface *New();
  vtkTypeMacro(vtkGRASSMessagingInterface,vtkObjectGRASSErrorHandler);
  void PrintSelf(ostream& os, vtkIndent indent);

  static void Message(char *message){G_message("%s",message);}
  static void VerboseMessage(char *message){G_verbose_message("%s",message);}
  static void ImportantMessage(char *message){G_important_message("%s",message);}
  static void Warning(char *message){G_warning("%s",message);}
  static void FatalError(char *message){G_fatal_error("%s",message);}
  static void Percent(int current_element, int total_number_of_elements, int increment_size){G_percent(current_element, total_number_of_elements, increment_size);}
  static void PercentReset(){G_percent_reset();}
  static void SetVerbosityLevel(int level){G_set_verbose(level);}
  static int GetVerbositiyLevel(){return G_verbose();}
  static int GetMinVerbositiyLevel(){return G_verbose_min();}
  static int GetMaxVerbositiyLevel(){return G_verbose_max();}
  static int GetStandardVerbositiyLevel(){return G_verbose_std();}
  static int DebugMessage(int level, char* message){return G_debug(level, "%s", message);}

protected:
  vtkGRASSMessagingInterface(){};
  ~vtkGRASSMessagingInterface(){};

private:
  vtkGRASSMessagingInterface(const vtkGRASSMessagingInterface&);  // Not implemented.
  void operator=(const vtkGRASSMessagingInterface&);  // Not implemented.
};


#endif
