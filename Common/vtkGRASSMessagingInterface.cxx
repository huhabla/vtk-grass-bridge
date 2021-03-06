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

#include "vtkGRASSMessagingInterface.h"
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"


vtkCxxRevisionMacro(vtkGRASSMessagingInterface, "$Revision: 1.18 $");
vtkStandardNewMacro(vtkGRASSMessagingInterface);

//----------------------------------------------------------------------------

void vtkGRASSMessagingInterface::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);
}
