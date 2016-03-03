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

#include "vtkGRASSFlag.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"

vtkStandardNewMacro(vtkGRASSFlag);


//----------------------------------------------------------------------------

vtkGRASSFlag::vtkGRASSFlag()
{
	this->flag = G_define_flag();

    this->Description = NULL;
    this->Label = NULL;
    this->GuiSection = NULL;
}

//----------------------------------------------------------------------------

void vtkGRASSFlag::SetDescription(const char *description)
{
    if(this->Description)
        free(this->Description);
    
    this->Description = strdup(description);
    
    this->flag->description = (const char*)this->Description;
}

//----------------------------------------------------------------------------

void vtkGRASSFlag::SetLabel(const char *label)
{        
    if(this->Label)
        free(this->Label);
    
    this->Label = strdup(label);
    
    this->flag->label = (const char*)this->Label;
}

//----------------------------------------------------------------------------

void vtkGRASSFlag::SetGuiSection(const char *guisection)
{
    if(this->GuiSection)
        free(this->GuiSection);
    
    this->GuiSection = strdup(guisection);
    
    this->flag->guisection = (const char*)this->GuiSection;
}
