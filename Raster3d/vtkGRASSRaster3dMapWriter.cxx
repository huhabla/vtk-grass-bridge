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

#include "vtkGRASSRaster3dMapWriter.h"
#include "vtkGRASSHistory.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>

vtkStandardNewMacro(vtkGRASSRaster3dMapWriter);

//----------------------------------------------------------------------------

vtkGRASSRaster3dMapWriter::vtkGRASSRaster3dMapWriter()
{

}

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapWriter::OpenMap(char *name) {
    char buff[1024];
    int error = 0;
    RASTER3D_Region region;

    // Check if the same map is already opened
    if (this->Open == true && strcmp(name, this->Raster3dName) == 0) {
        G_snprintf(buff, 1024, "class: %s line: %i Raster 3d map %s is already open.",
                this->GetClassName(), __LINE__, this->Raster3dName);
        this->InsertNextError(buff);
        return false;
    } else if (this->Open == true) {
        // If a new name is given, the open map will be closed
        if (!this->CloseMap())
            return false;
    }

    this->SetRaster3dName(name);
    this->Mapset = G_store(G_mapset());

    Rast3d_init_defaults();

    // Set the region for the 3d map based on the region settings
    this->SetRegion();
    this->Region->CopyRegionTo(this->Region->GetPointer());
    Rast3d_region_from_to_cell_head(this->Region->GetPointer(), &region);

    this->NumberOfRows = region.rows;
    this->NumberOfCols = region.cols;
    this->NumberOfDepths = region.depths;

    if (!setjmp(vgb_stack_buffer)) {
        /* open 3d raster map */
        this->Map = (RASTER3D_Map*)Rast3d_open_cell_new(this->Raster3dName, this->MapType,
                            RASTER3D_USE_CACHE_DEFAULT, &region);
        if (this->Map == NULL) {
            error = 1;
        }
    } else {
        this->InsertNextError(vgb_error_message);
        return false;
    }

    if (error == 1) {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to open raster 3d map %s.",
                this->GetClassName(), __LINE__, this->Raster3dName);
        this->InsertNextError(buff);
        return false;
    }

    this->History->SetMaptype("g3d");

    this->Open = true;
    return true;
}

//----------------------------------------------------------------------------

int
vtkGRASSRaster3dMapWriter::PutValue(int x, int y, int z, double value)
{
    if(this->Open)
        return Rast3d_put_double(this->Map, x, y, z, value);

    return 1;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapWriter::CloseMap()
{
    if(!this->Superclass::CloseMap())
        return false;

    if (!this->History->Write3dHistory(this->Raster3dName))
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i %s.",
                   this->GetClassName(), __LINE__, this->History->GetError());
        this->InsertNextError(buff);
        return false;
    }

    return true;
}
