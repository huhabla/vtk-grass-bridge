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

#include "vtkGRASSRaster3dMapBase.h"
#include "vtkGRASSRasterToImageReader.h"
#include <vtkStringArray.h>
#include <vtkCharArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSHistory.h"
#include <vtkGRASSDefines.h>

vtkStandardNewMacro(vtkGRASSRaster3dMapBase);

//----------------------------------------------------------------------------

vtkGRASSRaster3dMapBase::vtkGRASSRaster3dMapBase()
{
    this->Raster3dName = NULL;
    this->Mapset = NULL;
    this->Region = vtkGRASSRegion::New();
    this->History = vtkGRASSHistory::New();
    this->Map = NULL;
    this->MapType = DCELL_TYPE;
    this->NumberOfRows = 0;
    this->NumberOfCols = 0;
    this->Open = false;
    this->RegionUsage = VTK_GRASS_REGION_CURRENT;
}

//----------------------------------------------------------------------------

vtkGRASSRaster3dMapBase::~vtkGRASSRaster3dMapBase()
{
    this->CloseMap();

    if (this->Raster3dName)
        delete [] this->Raster3dName;
    if (this->Mapset)
        G_free(this->Mapset);

    this->Region->Delete();
    this->History->Delete();
}

//----------------------------------------------------------------------------

void
vtkGRASSRaster3dMapBase::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Is map open: " << (this->Open ? "YES" : "NO") << endl;
    if(this->Open) {
        os << indent << "Name   : " << this->Raster3dName << endl;
        os << indent << "Mapset : " << this->Mapset << endl;
        os << indent << "Maptype: " << this->GetMapTypeAsString() << endl;
        os << indent << "Cols   : " << this->NumberOfCols << endl;
        os << indent << "Rows   : " << this->NumberOfRows << endl;
        os << indent << "Depths : " << this->NumberOfDepths << endl;
        this->History->PrintSelf(os, indent.GetNextIndent());
    }
    return;
}

//----------------------------------------------------------------------------

const char* vtkGRASSRaster3dMapBase::GetMapTypeAsString()
{
    if(this->MapType == FCELL_TYPE)
        return "FCELL_TYPE";
    if(this->MapType == DCELL_TYPE)
        return "DCELL_TYPE";

    return NULL;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapBase::SetRegion()
{
    struct Cell_head head;

    if (this->Raster3dName == NULL || this->Mapset == NULL)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to set the region. RasterName is not set.",
                   this->GetClassName(), __LINE__);
        return false;
    }
    TRY
        if (this->RegionUsage == VTK_GRASS_REGION_CURRENT)
        {
            G_get_set_window(&head);
            this->Region->CopyRegionFrom(&head);
        }
        else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT && this->Region != NULL)
        {
            G_get_default_window(&head);
            G_set_window(&head);
            this->Region->CopyRegionFrom(&head);
        }
        else if (this->RegionUsage == VTK_GRASS_REGION_USER && this->Region != NULL)
        {
            this->Region->CopyRegionTo(&head);
            G_set_window(&head);
        }

    CATCH_BOOL

    return true;
}

//----------------------------------------------------------------------------

double
vtkGRASSRaster3dMapBase::GetValue(int x, int y, int z)
{
    if(this->Open)
        return Rast3d_get_double(this->Map, x, y, z);

    double nan;
    Rast_set_d_null_value(&nan, 1);
    return nan;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapBase::GetValue(int x, int y, int z, vtkDCELL *value)
{
    if(!this->Open)
        return false;

    value->Value = (DCELL)Rast3d_get_double(this->Map, x, y, z);

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapBase::GetValue(double north, double east, double top, vtkDCELL *value)
{
    if(!this->Open)
        return false;

    Rast3d_get_region_value(this->Map, north, east, top, &(value->Value), DCELL_TYPE);

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapBase::CloseMap()
{
    if (this->Open == true && this->Map != NULL)
    {
         TRY Rast3d_close(this->Map);
         CATCH_BOOL
    }
    // This flag is important and must be set by open and close methods
    this->Open = false;

    return true;
}
