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
 * \brief This is the base class to list database files located in a mapset
 *
 * The list of files is stored in a vtkStringArray.
 * 
 * \TODO: The grass7 api for file access changed and must be implemented 
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSFileLister_h
#define __vtkGRASSFileLister_h

#include "vtkGRASSBridgeCommonWin32Header.h"
#include "vtkObjectGRASSErrorHandler.h"

class vtkStringArray;

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSFileLister : public vtkObjectGRASSErrorHandler
{
public:
  static vtkGRASSFileLister *New();
  vtkTypeMacro(vtkGRASSFileLister,vtkObjectGRASSErrorHandler);
  virtual void PrintSelf(ostream& os, vtkIndent indent);
  
  vtkGetStringMacro(MapsetName);
  //! \brief Set the mapset which should be searched for grass files. Default is the current mapset.
  vtkSetStringMacro(MapsetName);
  //! \brief The grass file names are sorted and stored in a vtkStringArray
  vtkGetObjectMacro(FileNames, vtkStringArray);

  //! \brief Use Refresh() to re-read the files in case the maspet has changed, or new files have been added to the database.
  virtual bool Refresh();

  void SetElementToRaster(){this->SetElement("cell");}
  void SetElementToVector(){this->SetElement("vector");}
  void SetElementToGrid3(){this->SetElement("grid3");}
  void SetElementToGroup(){this->SetElement("group");}
  void SetElementToRegion(){this->SetElement("windows");}
  void SetElementToRegion3d(){this->SetElement("windows3d");}

protected:
  vtkGRASSFileLister();
  ~vtkGRASSFileLister();
  
  //! \brief Set the Element of which the grass files schould be listed
  vtkSetStringMacro(Element);
  char *MapsetName;
  char *Element;
  vtkStringArray *FileNames;

private:
  vtkGRASSFileLister(const vtkGRASSFileLister&);  // Not implemented.
  void operator=(const vtkGRASSFileLister&);  // Not implemented.
};


#endif
