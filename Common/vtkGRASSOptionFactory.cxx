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

#include "vtkGRASSOptionFactory.h"
#include <vtkObjectFactory.h>
#include "vtkGRASSOption.h"

vtkStandardNewMacro(vtkGRASSOptionFactory);

vtkGRASSOptionFactory::vtkGRASSOptionFactory() {
    ;
}

vtkGRASSOption* vtkGRASSOptionFactory::CreateInstance(int OptionType) {
    vtkGRASSOption *option = vtkGRASSOption::New();
    option->CreateOption(OptionType);
    return option;
}

vtkGRASSOption* vtkGRASSOptionFactory::CreateInstance(int OptionType, const char* key) {
    vtkGRASSOption *option = vtkGRASSOption::New();
    option->CreateOption(OptionType);
    option->SetKey(key);
    return option;
}

vtkGRASSOption* vtkGRASSOptionFactory::CreateInstance(int OptionType, const char *key, const char *description) {
    vtkGRASSOption *option = vtkGRASSOption::New();
    option->CreateOption(OptionType);
    option->SetKey(key);
    option->SetDescription(description);
    return option;
}
