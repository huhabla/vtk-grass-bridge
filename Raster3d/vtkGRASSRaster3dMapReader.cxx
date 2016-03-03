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

#include "vtkGRASSRaster3dMapReader.h"
#include "vtkGRASSHistory.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>

vtkStandardNewMacro(vtkGRASSRaster3dMapReader);

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapReader::OpenMap(char *name) {
    const char *mapset;
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

    mapset = G_find_raster3d(this->GetRaster3dName(), "");
    if (mapset == NULL) {
        G_snprintf(buff, 1024, "class: %s line: %i Raster map %s not found.",
                this->GetClassName(), __LINE__, this->Raster3dName);
        this->InsertNextError(buff);
        return false;
    }

    /* Make a copy of the mapset string */
    if (!setjmp(vgb_stack_buffer)) {
        this->Mapset = G_store(mapset);
    } else {
        this->InsertNextError(vgb_error_message);
        return false;
    }

    // Set the region for the 3d map based on the region settings
    if (this->RegionUsage == VTK_GRASS_REGION_RASTER) {
        Rast3d_read_region_map(this->Raster3dName, this->Mapset, &region);
        Rast3d_region_to_cell_head(&region, this->Region->GetPointer());
        this->Region->CopyRegionFrom(this->Region->GetPointer());
    } else {
        this->SetRegion();
        this->Region->CopyRegionTo(this->Region->GetPointer());
        Rast3d_region_from_to_cell_head(this->Region->GetPointer(), &region);
    }

    this->NumberOfRows = region.rows;
    this->NumberOfCols = region.cols;
    this->NumberOfDepths = region.depths;

    if (!setjmp(vgb_stack_buffer)) {
        /* open 3d raster map */
        this->Map = (RASTER3D_Map*)Rast3d_open_cell_old(this->Raster3dName, this->Mapset,
                &region, RASTER3D_TILE_SAME_AS_FILE, RASTER3D_USE_CACHE_DEFAULT);
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

    if (!this->History->Read3dHistory(this->GetRaster3dName())) {
        G_snprintf(buff, 1024, "class: %s line: %i %s.",
                this->GetClassName(), __LINE__, this->History->GetError());
        this->InsertNextError(buff);
        return false;
    }

    // Get the map type
    TRY this->MapType = Rast3d_file_type_map(this->Map);
    CATCH_BOOL

    this->Open = true;
    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRaster3dMapReader::GetRange(double range[2]) {

    char buff[1024];
    if (this->Open == false) {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to read raster map range. Map is not open.",
                this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    TRY Rast3d_range_min_max(this->Map, &range[0], &range[1]);
    CATCH_BOOL

    return true;
}
