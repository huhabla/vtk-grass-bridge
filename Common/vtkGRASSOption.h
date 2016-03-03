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
 * \brief This class creates grass command line options
 *
 * Several methods are available to set and get specific option variables.
 * To create standard options you must use the vtkGRASSOptionFactory. Python
 * code example how to use this class:
 * \verbatim
    option1 = vtkGRASSOption()
    option1.SetKey("option1")
    option1.SetDescription("This is option one")
    option1.SetGuiSection("Options")
    option1.SetGisprompt("old,raster,cell")
    option1.RequiredOff()
    option1.MultipleOff()
    option1.SetTypeToString()
 * \endverbatim
 *
 *
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 *
 * \TODO
 *
 * */

#ifndef __vtkGRASSOption_h
#define __vtkGRASSOption_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"
{
#include <grass/gis.h>
}

class vtkStringArray;
class vtkGRASSOptionFactory;
#define VGB_CHECK_OPTION if(this->option == NULL){ this->option = G_define_option();}

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSOption : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSOption *New();
  vtkTypeMacro(vtkGRASSOption,vtkObjectGRASSErrorHandler);

  //BTX
  friend class vtkGRASSOptionFactory;
  //ETX

  //!\brief Set the key.
  //! This will set the identifier string
  void SetKey(const char *key);
  //!\brief Set the description of the option
  //! This description will be used within the help and the manual page
  void SetDescription(const char *description);
  //!\brief Set the label of the module
  void SetLabel(const char *label);
  //!\brief Set the gui section identifier. This string is used to create
  //! specific tabs in the generated gui
  //! GUI Layout guidance: ';' delimited heirarchical tree position
  void SetGuiSection(const char *guisection);
  //!\brief Set the gispompt string ("old,raster,cell" or "new,vector,vector")
  void SetGisprompt(const char *gisprompt);
  //!\brief Set default options ("a,b,c,d")
  void SetDefaultOptions(const char *options);
  //!\brief Set the default answer
  void SetDefaultAnswer(char *answer);
  //!\brief GUI dependency, list of options
  //! (separated by commas) to be updated
  //! if the value is changed
  void SetGuiDependency(const char *guidependency);

  //!\brief Set if the option support multiple answers
  void MultipleOn() {VGB_CHECK_OPTION this->option->multiple = 1;}
  //!\brief Set if the option support multiple answers
  void MultipleOff(){VGB_CHECK_OPTION this->option->multiple = 0;}
  //!\brief Set if the option is required
  void RequiredOn() {VGB_CHECK_OPTION this->option->required = 1;}
  //!\brief Set if the option is required
  void RequiredOff(){VGB_CHECK_OPTION this->option->required = 0;}
  //!\brief Set the type of the option to integer
  void SetTypeToInteger(){VGB_CHECK_OPTION this->option->type = TYPE_INTEGER;}
  //!\brief Set the type of the option to double
  void SetTypeToDouble() {VGB_CHECK_OPTION this->option->type = TYPE_DOUBLE;}
  //!\brief Set the type of the option to string
  void SetTypeToString() {VGB_CHECK_OPTION this->option->type = TYPE_STRING;}
  
  //!\brief Return the option desription
  const char * GetDescription(){return (this->option?this->option->description:NULL);}
  //!\brief Return the guisection string
  const char* GetGuiSection() {return (this->option?this->option->guisection:NULL);}
  //!\brief Return the label string
  const char* GetLabel() {return (this->option?this->option->label:NULL);}
  //!\brief Return the gisprompt string
  const char* GetGisprompt() {return (this->option?this->option->gisprompt:NULL);}
  //!\brief Return the answer string
  const char* GetAnswer() {return (this->option?this->option->answer:NULL);}
  //!\brief Return the answer strings in a vtkStringArray
  void GetAnswers(vtkStringArray *answers);
  //!\brief Return the options string
  const char* GetOptions() {return (this->option?this->option->options:NULL);}
  //!\brief Check if the option is multiple (true/false)
  bool IsMultiple() {return ((this->option && this->option->multiple)?true:false);}
  //!\brief Check if the option is required (true/false)
  bool IsRequired() {return ((this->option && this->option->required)?true:false);}
  //!\brief Check if the type is integer (true/false)
  bool IsTypeInteger() {return ((this->option && this->option->type == TYPE_INTEGER)?true:false);}
  //!\brief Check if the type is double (true/false)
  bool IsTypeDouble() {return ((this->option && this->option->type == TYPE_DOUBLE) ?true:false);}
  //!\brief Check if the type is string (true/false)
  bool IsTypeString() {return ((this->option && this->option->type == TYPE_STRING)?true:false);}

protected:
  vtkGRASSOption();
  ~vtkGRASSOption() {};

  //!\brief Create a standard option. This methd is used by the option factory to create default options
  bool CreateOption(int OptionType);
  
  //BTX
  struct Option *option;
  char *Key;
  char *Description;
  char *Label;
  char *Answer;
  char *Options;
  char *GuiSection;
  char *GisPrompt;
  char *GuiDependency;
  //ETX

private:
  vtkGRASSOption(const vtkGRASSOption&);  // Not implemented.
  void operator=(const vtkGRASSOption&);  // Not implemented.
};


#endif
