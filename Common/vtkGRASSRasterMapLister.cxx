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

#include "vtkGRASSRasterMapLister.h"
#include "vtkGRASSRasterMapLister.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>

extern "C" {
#include <grass/gis.h>
#include <math.h>
}

vtkStandardNewMacro(vtkGRASSRasterMapLister);

//----------------------------------------------------------------------------

vtkGRASSRasterMapLister::vtkGRASSRasterMapLister() {

    this->SetElementToRaster();
    this->Refresh();
}
