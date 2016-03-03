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

#include "vtkGRASSFileLister.h"
#include "vtkGRASSFileLister.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>


extern "C" {
#include <math.h>
#include <unistd.h>
#include <grass/gis.h>
}

vtkStandardNewMacro(vtkGRASSFileLister);

//----------------------------------------------------------------------------

vtkGRASSFileLister::vtkGRASSFileLister() {

    this->MapsetName = NULL;
    this->Element = NULL;
    this->FileNames = vtkStringArray::New();

    //G_gisinit("vtkGRASSFileLister");
}

//----------------------------------------------------------------------------

vtkGRASSFileLister::~vtkGRASSFileLister() {

    if(this->MapsetName)
        delete [] this->MapsetName;
    if(this->Element)
        delete [] this->Element;
    this->FileNames->Delete();
}

//----------------------------------------------------------------------------

bool vtkGRASSFileLister::Refresh(){

  char **mapArray = NULL;
  char path[GMAPSET_MAX];
  int count = 0, i;
  char buff[1024];

  this->FileNames->Initialize();

  if(this->MapsetName == NULL)
    this->SetMapsetName(G_mapset());
  // Create the full path to the directoy
  G_file_name(path, this->Element, "", this->MapsetName);
  if (access(path, 0) != 0) {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to access path %s",
                    this->GetClassName(), __LINE__, path);
        this->InsertNextError(buff);
        return false;
  }
  //Check Path

  // List the conetent of the directory
  mapArray = G_ls2(path, &count);


  for(i = 0; i < count; i++)
  {
          this->FileNames->InsertNextValue(mapArray[i]);
          G_free(mapArray[i]);
  }

  G_free(mapArray);


  return true;

}

//----------------------------------------------------------------------------

void vtkGRASSFileLister::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);

    os << indent << "MapsetName: " << (this->MapsetName?this->MapsetName : "none") << endl;
    this->FileNames->PrintSelf(os, indent);

}
