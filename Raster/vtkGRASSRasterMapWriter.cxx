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

#include "vtkGRASSRasterMapWriter.h"
#include "vtkGRASSHistory.h"
#include "vtkGRASSRasterRow.h"
#include "vtkGRASSRasterMemoryMap.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>

vtkStandardNewMacro(vtkGRASSRasterMapWriter);

//----------------------------------------------------------------------------

vtkGRASSRasterMapWriter::vtkGRASSRasterMapWriter()
{
    this->MapTitle = NULL;
    this->SetMapTitle("Raster map title");
    this->Empty = true;
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterMapWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "Is map empty: " << (this->Empty ? "YES" : "NO") << endl;

    return;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapWriter::OpenMap(const char *name)
{
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
        G_snprintf(buff, 1024, "class: %s line: %i Raster map %s is already open.",
        this->GetClassName(), __LINE__, this->RasterName);
        this->InsertNextError(buff);
        if(!this->CloseMap())
            return false;
    }

    this->SetRasterName(name);


    if (G_legal_filename(this->RasterName) != 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Raster map name %s is not valid.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->InsertNextError(buff);
        return false;
    }

    this->SetMapset(G_mapset());

    // Set the region for the map
    this->SetRegion();

    if (!setjmp(vgb_stack_buffer))
    {
        /* open raster map */
        this->Map = Rast_open_new(this->RasterName, this->MapType);
        if (this->Map < 0)
        {
            error = 1;
        }
    }
    else
    {
        this->InsertNextError(vgb_error_message);
        this->Open = false;
        return false;
    }

    if (error == 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to open new raster map %s.",
                   this->GetClassName(), __LINE__, this->RasterName);
        this->Open = false;
        this->InsertNextError(buff);
        return false;
    }

    this->Open = true;
    this->Empty = true;
    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapWriter::PutNextRow(vtkDataArray *data)
{

    int i;
    char buff[1024];

    if (data == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The data array is NULL.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (data->GetNumberOfTuples() < this->NumberOfCols)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The wrong number of tupples in data array.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (!this->SetUpRasterBuffer())
        return false;

    // copy data
    for (i = 0; i < this->NumberOfCols; i++)
    {
        if(this->UseNullValue && data->GetTuple1(i) == this->NullValue)
        {
            if (this->MapType == CELL_TYPE)
                Rast_set_c_null_value(&((CELL*)this->RasterBuff)[i], 1);
            else if (this->MapType == FCELL_TYPE)
                Rast_set_f_null_value(&((FCELL*)this->RasterBuff)[i], 1);
            else if (this->MapType == DCELL_TYPE)
                Rast_set_d_null_value(&((DCELL*)this->RasterBuff)[i], 1);
        } else {
            if (this->MapType == CELL_TYPE) {
                ((CELL*)this->RasterBuff)[i] = (CELL) data->GetTuple1(i);
            }
            else if (this->MapType == FCELL_TYPE) {
                ((FCELL*)this->RasterBuff)[i] = (FCELL) data->GetTuple1(i);
            }
            else if (this->MapType == DCELL_TYPE) {
                ((DCELL*)this->RasterBuff)[i] = (DCELL) data->GetTuple1(i);
            }
        }
    }

    TRY Rast_put_row(this->Map, this->RasterBuff, this->MapType);
    CATCH_BOOL

    this->Empty = false;

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapWriter::PutNextRow(vtkGRASSRasterRow *row)
{
    char buff[1024];

    if (row == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The row is NULL.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (row->GetNumberOfCols() < this->NumberOfCols)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The number of cols differs.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (row->GetRowType() < this->MapType)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The row type and map type differ.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    TRY Rast_put_row(this->Map, row->GetBuffer(), this->MapType);
    CATCH_BOOL

    this->Empty = false;

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapWriter::WriteMemoryMap(vtkGRASSRasterMemoryMap *mmap)
{
    char buff[1024];
    int i;
    VGB_CREATE(vtkGRASSRasterRow, row);

    if (mmap == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The memory map is NULL.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (mmap->GetNumberOfCols() < this->NumberOfCols)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The number of cols differs.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (mmap->GetNumberOfRows() < this->NumberOfRows)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The number of rows differs.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (mmap->GetMapType() < this->MapType)
    {
        G_snprintf(buff, 1024, "class: %s line: %i The map types are different.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    for(i = 0; i < this->GetNumberOfRows(); i++) {
        mmap->GetRow(i, row);
        TRY Rast_put_row(this->Map, row->GetBuffer(), this->MapType);
        CATCH_BOOL
    }

    this->Empty = false;

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapWriter::CloseMap()
{
    char buff[1024];
    // We close them only when its not empty
    if (this->Empty)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Map is empty, unable to close.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if(!this->Superclass::CloseMap())
        return false;

    if (!this->History->WriteHistory(this->RasterName))
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i %s.",
                   this->GetClassName(), __LINE__, this->History->GetError());
        this->InsertNextError(buff);
        return false;
    }

    return true;
}
