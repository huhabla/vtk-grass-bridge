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

#include "vtkGRASSOption.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"
#include <vtkStringArray.h>

vtkStandardNewMacro(vtkGRASSOption);


//----------------------------------------------------------------------------

vtkGRASSOption::vtkGRASSOption()
{
    this->option = NULL;

    this->Key = NULL;
    this->Description = NULL;
    this->Label = NULL;
    this->Answer = NULL;
    this->Options = NULL;
    this->GuiSection = NULL;
    this->GuiDependency = NULL;
    this->GisPrompt = NULL;
}

bool vtkGRASSOption::CreateOption(int OptionType)
{
    if (this->option == NULL) {
        this->option = G_define_standard_option(OptionType);
        return true;
    } else {
        return false;
    }
}

//----------------------------------------------------------------------------

void vtkGRASSOption::GetAnswers(vtkStringArray *array)
{
    int count = 0;

    array->Initialize();
    array->SetNumberOfComponents(1);

    if (this->option->answers != NULL) {
        while (this->option->answers[count] != NULL) {
            array->InsertNextValue(this->option->answers[count]);
            count++;
        }
    }

    return;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetKey(const char *key)
{
    VGB_CHECK_OPTION 
        
    if(this->Key)
        free(this->Key);
    
    this->Key = strdup(key);
    
    this->option->key = (const char*)this->Key;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetDescription(const char *description)
{
    VGB_CHECK_OPTION
        
    if(this->Description)
        free(this->Description);
    
    this->Description = strdup(description);
    
    this->option->description = (const char*)this->Description;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetLabel(const char *label)
{
    VGB_CHECK_OPTION
        
    if(this->Label)
        free(this->Label);
    
    this->Label = strdup(label);
    
    this->option->label = (const char*)this->Label;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetGuiSection(const char *guisection)
{
    VGB_CHECK_OPTION
        
    if(this->GuiSection)
        free(this->GuiSection);
    
    this->GuiSection = strdup(guisection);
    
    this->option->guisection = (const char*)this->GuiSection;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetGuiDependency(const char *key)
{
    VGB_CHECK_OPTION

    if(this->GuiDependency)
        free(this->GuiDependency);

    this->GuiDependency = strdup(key);

    this->option->guidependency = (const char*)this->GuiDependency;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetGisprompt(const char *gisprompt)
{
    VGB_CHECK_OPTION 
        
    if(this->GisPrompt)
        free(this->GisPrompt);
    
    this->GisPrompt = strdup(gisprompt);
    
    this->option->gisprompt = (const char*)this->GisPrompt;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetDefaultOptions(const char *options)
{
    VGB_CHECK_OPTION
        
    if(this->Options)
        free(this->Options);
    
    this->Options = strdup(options);
    
    this->option->options = (const char*)this->Options;
}

//----------------------------------------------------------------------------

void vtkGRASSOption::SetDefaultAnswer(char *answer)
{
    VGB_CHECK_OPTION;
        
    if(this->Answer)
        free(this->Answer);
    
    this->Answer = strdup(answer);
    
    this->option->answer = this->Answer;
}
