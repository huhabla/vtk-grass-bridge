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
 * \brief The vtkObjectGRASSErrorHandler class is the base class of all
 *  classes in vtkGRASSBridge which are derived from vtkObject.
 * It provides an mechanism to log and store errors and warning.
 *
 * It can hold a maximum number of warnings and errors.
 * In case more errors or warnings appear, the
 * oldest error or warning messages will be overwritten by the new ones.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkObjectGRASSErrorHandler_h
#define __vtkObjectGRASSErrorHandler_h

#include <vtkObject.h>
#include "vtkGRASSBridgeCommonWin32Header.h"
#include <vtkStringArray.h>

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkObjectGRASSErrorHandler : public vtkObject
{
public:
  static  vtkObjectGRASSErrorHandler *New();
  vtkTypeMacro(vtkObjectGRASSErrorHandler,vtkObject);

  vtkGetMacro(NumberOfErrors, int);
  vtkGetMacro(NumberOfWarnings, int);

  vtkGetObjectMacro(WarningList, vtkStringArray);
  vtkGetObjectMacro(ErrorList, vtkStringArray);

  //! \brief Returns the last error message
  const char *GetError();
  //! \brief Retrurns the last warning message
  const char *GetWarning();

  bool HasErrors();
  bool HasWarnings();

  //! \brief Removes all Errors and Warnings from the list and set the counter to zero
  void ResetErrors();


protected:
  vtkObjectGRASSErrorHandler();
  ~vtkObjectGRASSErrorHandler();

  vtkSetMacro(NumberOfErrors, int);
  vtkSetMacro(NumberOfWarnings, int);

  void InsertNextError(char *message);
  void InsertNextWarning(char *message);
  void InsertNextError(const char *message){this->InsertNextError((char*)message);}
  void InsertNextWarning(const char *message){this->InsertNextWarning((char*)message);}

  int NumberOfErrors;
  int NumberOfWarnings;
  int LastErrorNumber;
  int LastWarningNumber;

  //BTX
  static const int MaxNumberOfWarnings = 50;
  static const int MaxNumberOfErrors = 50;
  //ETX

  vtkStringArray *WarningList;
  vtkStringArray *ErrorList;

private:
  vtkObjectGRASSErrorHandler(const vtkObjectGRASSErrorHandler&);  // Not implemented.
  void operator=(const vtkObjectGRASSErrorHandler&);  // Not implemented.
};


#endif
