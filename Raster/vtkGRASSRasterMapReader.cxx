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

#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkCharArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include "vtkGRASSRasterMapReader.h"
#include "vtkGRASSHistory.h"
#include "vtkGRASSRasterRow.h"

vtkStandardNewMacro(vtkGRASSRasterMapReader);

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapReader::OpenMap(const char *name)
{
    const char *mapset;
    char buff[1024];
    int error = 0;

    // Check if the same map is already opened
    if (this->Open == true && strcmp(name, this->RasterName) == 0)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Raster map %s is already open.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->InsertNextError(buff);
        return false;
    }
    else if (this->Open == true)
    {
        // If a new name is given, the open map will be closed
        if(!this->CloseMap())
            return false;
    }

    this->SetRasterName(name);

    mapset = G_find_raster2(this->GetRasterName(), "");
    if (mapset == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Raster map %s not found.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->InsertNextError(buff);
        return false;
    }

    this->SetMapset(mapset);

    if (this->RegionUsage == VTK_GRASS_REGION_RASTER)
    {
        struct Cell_head head;
        Rast_get_cellhd(this->GetRasterName(), this->GetMapset(), &head);
        G_set_window(&head);
        this->Region->CopyRegionFrom(&head);
        this->NumberOfRows = head.rows;
        this->NumberOfCols = head.cols;
    } else {
        // Set the region for the map
        this->SetRegion();
    }

    if (!setjmp(vgb_stack_buffer))
    {
        /* open raster map */
        this->Map = Rast_open_old(this->RasterName, this->Mapset);
        if (this->Map < 0)
        {
            error = 1;
        }
    }
    else
    {
        this->InsertNextError(vgb_error_message);
        return false;
    }

    if (error == 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to open raster map %s.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->InsertNextError(buff);
        return false;
    }

    if (!this->History->ReadHistory(this->GetRasterName()))
    {
        G_snprintf(buff, 1024, "class: %s line: %i %s.",
                   this->GetClassName(), __LINE__, this->History->GetError());
        this->InsertNextError(buff);
        return false;
    }

    // Get the map type
    TRY this->MapType = Rast_get_map_type(this->Map);
    CATCH_BOOL

    this->Open = true;
    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapReader::GetRange(double range[2])
{

    char buff[1024];
    if (this->Open == false)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to read raster map range. Map is not open.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }
    if (this->MapType == CELL_TYPE)
    {
        struct Range r;
        if (Rast_read_range(this->RasterName, this->Mapset, &r) == -1)
        {
            G_snprintf(buff, 1024, "class: %s line: %i Unable to read raster map range.",
                       this->GetClassName(), __LINE__);
            this->InsertNextError(buff);
            return false;
        }
        range[0] = (double) r.min;
        range[1] = (double) r.max;
    }
    else
    {
        struct FPRange r;
        if (Rast_read_fp_range(this->RasterName, this->Mapset, &r) == -1)
        {
            G_snprintf(buff, 1024, "class: %s line: %i Unable to read raster map range.",
                       this->GetClassName(), __LINE__);
            this->InsertNextError(buff);
            return false;
        }
        range[0] = (double) r.min;
        range[1] = (double) r.max;
    }
    return true;
}


//----------------------------------------------------------------------------

bool vtkGRASSRasterMapReader::GetRow(int idx, vtkGRASSRasterRow *row)
{

    char buff[1024];

    if (idx < 0 || idx > this->NumberOfRows - 1) {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                   this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return false;
    }

    //Check if raster row is of correct type and allocated
    if((row->GetRowType() == this->GetMapType()) && row->GetNumberOfCols() == this->NumberOfCols)
    {
        ;
    } else {
        // Allocate the raster buffer with the map specific type
        row->Allocate(this->NumberOfCols, this->MapType);
    }

    TRY Rast_get_row(this->Map, row->GetBuffer(), idx, this->MapType);
    CATCH_BOOL

    return true;
}

//----------------------------------------------------------------------------

vtkDataArray *
vtkGRASSRasterMapReader::GetRow(int idx)
{
    int i;
    char buff[1024];

    if (idx < 0 || idx > this->NumberOfRows - 1) {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                   this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    // Allocate the raster buffer with the map specific type
    this->SetUpRasterBuffer();

    TRY Rast_get_row(this->Map, this->RasterBuff, idx, this->MapType);
    CATCH_NULL

    for (i = 0; i < this->NumberOfCols; i++) {

        if (this->UseNullValue) {
            if ((this->MapType == CELL_TYPE && Rast_is_c_null_value(&((CELL*)this->RasterBuff)[i])) ||
                (this->MapType == FCELL_TYPE && Rast_is_f_null_value(&((FCELL*)this->RasterBuff)[i])) ||
                (this->MapType == DCELL_TYPE && Rast_is_d_null_value(&((DCELL*)this->RasterBuff)[i]))) {
                this->Row->SetTuple1(i, this->NullValue);
            } else {

                if (this->MapType == CELL_TYPE) {
                    this->Row->SetTuple1(i, (double) ((CELL*)this->RasterBuff)[i]);
                } else if (this->MapType == FCELL_TYPE) {
                    this->Row->SetTuple1(i, (double) ((FCELL*)this->RasterBuff)[i]);
                } else if (this->MapType == DCELL_TYPE) {
                    this->Row->SetTuple1(i, (double) ((DCELL*)this->RasterBuff)[i]);
                }
            }
        } else {
            if (this->MapType == CELL_TYPE) {
                this->Row->SetTuple1(i, (double) ((CELL*)this->RasterBuff)[i]);
            } else if (this->MapType == FCELL_TYPE) {
                this->Row->SetTuple1(i, (double) ((FCELL*)this->RasterBuff)[i]);
            } else if (this->MapType == DCELL_TYPE) {
                this->Row->SetTuple1(i, (double) ((DCELL*)this->RasterBuff)[i]);

            }
        }
    }

    return this->Row;
}

//----------------------------------------------------------------------------

vtkCharArray *
vtkGRASSRasterMapReader::GetNullRow(int idx)
{
    int i;
    char buff[1024];

    if (idx < 0 || idx > this->NumberOfRows - 1) {
        G_snprintf(buff, 1024, "class: %s line: %i The index %i is out of range.",
                   this->GetClassName(), __LINE__, idx);
        this->InsertNextError(buff);
        return NULL;
    }

    this->SetUpRasterBuffer();
    TRY Rast_get_null_value_row(this->Map, this->NullBuff, idx);
    CATCH_NULL

    for (i = 0; i < this->NumberOfCols; i++) {
        this->NullRow->SetValue(i, this->NullBuff[i]);
    }

    return this->NullRow;
}
