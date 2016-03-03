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

#include "vtkObjectGRASSErrorHandler.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>


extern "C"
{
#include <grass/gis.h>
#include <math.h>
}

vtkStandardNewMacro(vtkObjectGRASSErrorHandler);

//----------------------------------------------------------------------------

vtkObjectGRASSErrorHandler::vtkObjectGRASSErrorHandler()
{

    this->NumberOfErrors = 0;
    this->NumberOfWarnings = 0;
    this->LastErrorNumber = 0;
    this->LastWarningNumber = 0;
    this->ErrorList = vtkStringArray::New();
    this->ErrorList->SetNumberOfValues(this->MaxNumberOfErrors);
    this->ErrorList->SetName("ErrorList");
    this->WarningList = vtkStringArray::New();
    this->WarningList->SetNumberOfValues(this->MaxNumberOfWarnings);
    this->WarningList->SetName("WarningList");
}

//----------------------------------------------------------------------------

vtkObjectGRASSErrorHandler::~vtkObjectGRASSErrorHandler()
{
    this->ErrorList->Delete();
    this->WarningList->Delete();
}

//----------------------------------------------------------------------------

void
vtkObjectGRASSErrorHandler::ResetErrors()
{
    this->NumberOfErrors = 0;
    this->NumberOfWarnings = 0;
    this->LastErrorNumber = 0;
    this->LastWarningNumber = 0;
    this->ErrorList->Reset();
    this->WarningList->Reset();
}

//----------------------------------------------------------------------------

void
vtkObjectGRASSErrorHandler::InsertNextError(char *message)
{
    if (this->LastErrorNumber == 0 || (this->LastErrorNumber == this->MaxNumberOfErrors - 1))
    {
        this->LastErrorNumber = 0;
        this->ErrorList->InsertValue(this->LastErrorNumber, message);
    }
    else
    {
        this->LastErrorNumber++;
        this->ErrorList->InsertValue(this->LastErrorNumber, message);
    }

    if (this->NumberOfErrors < this->MaxNumberOfErrors)
        this->NumberOfErrors++;

    vtkErrorMacro( << message);

    return;
}

//----------------------------------------------------------------------------

void
vtkObjectGRASSErrorHandler::InsertNextWarning(char *message)
{
    if (this->LastWarningNumber == 0 || (this->LastWarningNumber == this->MaxNumberOfWarnings - 1))
    {
        this->LastWarningNumber = 0;
        this->WarningList->InsertValue(this->LastWarningNumber, message);
    }
    else
    {
        this->LastWarningNumber++;
        this->WarningList->InsertValue(this->LastWarningNumber, message);
    }

    if (this->NumberOfWarnings < this->MaxNumberOfWarnings)
        this->NumberOfWarnings++;

    vtkWarningMacro( << message);

    return;
}

//----------------------------------------------------------------------------

const char *
vtkObjectGRASSErrorHandler::GetError()
{
    if (this->NumberOfErrors)
        return this->ErrorList->GetValue(this->LastErrorNumber);

    return NULL;
}

//----------------------------------------------------------------------------

const char *
vtkObjectGRASSErrorHandler::GetWarning()
{
    if (this->NumberOfWarnings)
        return this->WarningList->GetValue(this->LastWarningNumber);

    return NULL;
}

//----------------------------------------------------------------------------

bool
vtkObjectGRASSErrorHandler::HasErrors()
{
    if (this->NumberOfErrors)
        return true;

    return false;
}

//----------------------------------------------------------------------------

bool
vtkObjectGRASSErrorHandler::HasWarnings()
{
    if (this->NumberOfWarnings)
        return true;

    return false;
}
