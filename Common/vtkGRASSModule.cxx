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

#include "vtkGRASSModule.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"

extern "C" {
#include <grass/gis.h>
#include <string.h>
}

vtkStandardNewMacro(vtkGRASSModule);


//----------------------------------------------------------------------------

vtkGRASSModule::vtkGRASSModule()
{
    this->module = G_define_module();
    this->Description = NULL;
    this->Label = NULL;
}

//----------------------------------------------------------------------------

void vtkGRASSModule::AddKeyword(const char *keyword)
{
    G_add_keyword(keyword);
}

//----------------------------------------------------------------------------

void vtkGRASSModule::SetDescription(const char *description)
{
    if(this->Description)
        free(this->Description);
    
    this->Description = strdup(description);
    
    this->module->description = (const char*)this->Description;
}

//----------------------------------------------------------------------------

void vtkGRASSModule::SetLabel(const char *label)
{
    if(this->Label)
        free(this->Label);
    
    this->Label = strdup(label);
    
    this->module->label = (const char*)this->Label;
}
