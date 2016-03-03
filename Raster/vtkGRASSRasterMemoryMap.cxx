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

#include "vtkGRASSRasterMemoryMap.h"
#include "vtkGRASSRasterMapBase.h"
#include "vtkGRASSRasterRow.h"
#include <vtkObjectFactory.h>

vtkStandardNewMacro(vtkGRASSRasterMemoryMap);

//----------------------------------------------------------------------------

vtkGRASSRasterMemoryMap::vtkGRASSRasterMemoryMap()
{
    this->Allocated = false;
    this->CELLBuff = NULL;
    this->FCELLBuff = NULL;
    this->DCELLBuff = NULL;
    this->NumberOfCols = 0;
    this->NumberOfRows = 0;
    this->MapType = -1;
}

//----------------------------------------------------------------------------

vtkGRASSRasterMemoryMap::~vtkGRASSRasterMemoryMap()
{
    this->Reset();
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterMemoryMap::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Allocated   : " << (this->Allocated ? "YES" : "NO") << endl;
    os << indent << "CELLBuff : " << this->CELLBuff << endl;
    os << indent << "FCELLBuff : " << this->FCELLBuff << endl;
    os << indent << "DCELLBuff : " << this->DCELLBuff << endl;
    os << indent << "Cols   : " << this->NumberOfCols << endl;
    os << indent << "Rows   : " << this->NumberOfRows << endl;
    os << indent << "MapType: " << this->MapType << endl;

    return;
}

//----------------------------------------------------------------------------

void vtkGRASSRasterMemoryMap::Reset()
{
    if(this->Allocated) {
        TRY
        if(this->CELLBuff) {
            G_free(this->CELLBuff);
            this->CELLBuff = NULL;
        } else if(this->FCELLBuff) {
            G_free(this->FCELLBuff);
            this->FCELLBuff = NULL;
        } if(this->DCELLBuff) {
            G_free(this->DCELLBuff);
            this->DCELLBuff = NULL;
        }
        CATCH_VOID
    }

    this->NumberOfCols = 0;
    this->NumberOfRows = 0;
    this->MapType = -1;
    this->Allocated = false;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::Allocate(int rows, int cols, int MapType)
{
    if(MapType == CELL_TYPE)
        return this->AllocateCELL(rows, cols);
    if(MapType == FCELL_TYPE)
        return this->AllocateFCELL(rows, cols);
    if(MapType == DCELL_TYPE)
        return this->AllocateDCELL(rows, cols);

    this->InsertNextError("vtkGRASSRasterMemoryMap::Allocate() Unable to allocate memory");
    return false;
}
//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::AllocateCELL(int rows, int cols)
{
    if(rows < 1) {
        this->InsertNextError("Number of rows is lower than 1");
        return false;
    }
    if(cols < 1) {
        this->InsertNextError("Number of columns is lower than 1");
        return false;
    }

    this->Reset();

    TRY
    this->CELLBuff = (CELL*)G_calloc(rows*cols, sizeof(CELL));
    CATCH_BOOL

    this->NumberOfCols = cols;
    this->NumberOfRows = rows;
    this->Allocated = true;
    this->MapType = CELL_TYPE;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::AllocateFCELL(int rows, int cols)
{
    if(rows < 1) {
        this->InsertNextError("Number of rows is lower than 1");
        return false;
    }
    if(cols < 1) {
        this->InsertNextError("Number of columns is lower than 1");
        return false;
    }

    this->Reset();

    TRY
    this->FCELLBuff = (FCELL*)G_calloc(rows*cols, sizeof(FCELL));
    CATCH_BOOL


    this->NumberOfCols = cols;
    this->NumberOfRows = rows;
    this->Allocated = true;
    this->MapType = FCELL_TYPE;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::AllocateDCELL(int rows, int cols)
{
    if(rows < 1) {
        this->InsertNextError("Number of rows is lower than 1");
        return false;
    }
    if(cols < 1) {
        this->InsertNextError("Number of columns is lower than 1");
        return false;
    }

    this->Reset();

    TRY
    this->DCELLBuff = (DCELL*)G_calloc(rows*cols, sizeof(DCELL));
    CATCH_BOOL


    this->NumberOfCols = cols;
    this->NumberOfRows = rows;
    this->Allocated = true;
    this->MapType = DCELL_TYPE;

    return true;
}

//----------------------------------------------------------------------------

void *vtkGRASSRasterMemoryMap::GetBuffer()
{
    if(this->Allocated == false)
        return NULL;

    if(this->MapType == CELL_TYPE)
        return this->CELLBuff;
    if(this->MapType == FCELL_TYPE)
        return this->FCELLBuff;
    if(this->MapType == DCELL_TYPE)
        return this->DCELLBuff;

    return NULL;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::GetRow(int row, vtkGRASSRasterRow *buff)
{
    if(this->MapType == CELL_TYPE){
        buff->SetBuffer(this->NumberOfCols, this->MapType,
                (void *)&(this->CELLBuff[row*this->NumberOfCols]));
        return true;
    } else if(this->MapType == FCELL_TYPE){
        buff->SetBuffer(this->NumberOfCols, this->MapType,
                (void *)&(this->FCELLBuff[row*this->NumberOfCols]));
        return true;
    } else if(this->MapType == DCELL_TYPE) {
        buff->SetBuffer(this->NumberOfCols, this->MapType,
                (void *)&(this->DCELLBuff[row*this->NumberOfCols]));
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::GetCELLValue(int row, int col, vtkCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    if(this->MapType == CELL_TYPE)
        value->Value = (CELL)this->CELLBuff[row * this->NumberOfCols + col];
    else  if(this->MapType == DCELL_TYPE)
        value->Value = (CELL)this->DCELLBuff[row * this->NumberOfCols + col];
    else  if(this->MapType == FCELL_TYPE)
        value->Value = (CELL)this->FCELLBuff[row * this->NumberOfCols + col];

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::GetFCELLValue(int row, int col, vtkFCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    if(this->MapType == FCELL_TYPE)
        value->Value = (FCELL)this->FCELLBuff[row * this->NumberOfCols + col];
    else  if(this->MapType == DCELL_TYPE)
        value->Value = (FCELL)this->DCELLBuff[row * this->NumberOfCols + col];
    else  if(this->MapType == CELL_TYPE)
        value->Value = (FCELL)this->CELLBuff[row * this->NumberOfCols + col];


    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::GetDCELLValue(int row, int col, vtkDCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    if(this->MapType == DCELL_TYPE)
        value->Value = (DCELL)this->DCELLBuff[row * this->NumberOfCols + col];
    else  if(this->MapType == FCELL_TYPE)
        value->Value = (DCELL)this->FCELLBuff[row * this->NumberOfCols + col];
    else  if(this->MapType == CELL_TYPE)
        value->Value = (DCELL)this->CELLBuff[row * this->NumberOfCols + col];

    return true;
}


//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::SetCELLValue(int row, int col, vtkCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    if(this->MapType == CELL_TYPE)
        this->CELLBuff[row * this->NumberOfCols + col] = (CELL)value->Value;
    else if(this->MapType == FCELL_TYPE)
        this->FCELLBuff[row * this->NumberOfCols + col] = (FCELL)value->Value;
    else if(this->MapType == DCELL_TYPE)
        this->DCELLBuff[row * this->NumberOfCols + col] = (DCELL)value->Value;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::SetFCELLValue(int row, int col, vtkFCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    if(this->MapType == FCELL_TYPE)
        this->FCELLBuff[row * this->NumberOfCols + col] = (FCELL)value->Value;
    else if(this->MapType == DCELL_TYPE)
        this->DCELLBuff[row * this->NumberOfCols + col] = (DCELL)value->Value;
    else if(this->MapType == CELL_TYPE)
        this->CELLBuff[row * this->NumberOfCols + col] = (CELL)value->Value;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::SetDCELLValue(int row, int col, vtkDCELL* value)
{
    if(this->Allocated == false)
        return false;

    if(value == NULL)
        return false;

    if(this->MapType == DCELL_TYPE)
        this->DCELLBuff[row * this->NumberOfCols + col] = (DCELL)value->Value;
    else if(this->MapType == FCELL_TYPE)
        this->FCELLBuff[row * this->NumberOfCols + col] = (FCELL)value->Value;
    else if(this->MapType == CELL_TYPE)
        this->CELLBuff[row * this->NumberOfCols + col] = (CELL)value->Value;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::IsNull(int row, int col)
{
    if(this->Allocated == false)
        return false;

    if(this->MapType == CELL_TYPE)
        return (Rast_is_c_null_value(&(this->CELLBuff[row * this->NumberOfCols + col]))?true:false);
    if(this->MapType == FCELL_TYPE)
        return (Rast_is_f_null_value(&(this->FCELLBuff[row * this->NumberOfCols + col]))?true:false);
    if(this->MapType == DCELL_TYPE)
        return (Rast_is_d_null_value(&(this->DCELLBuff[row * this->NumberOfCols + col]))?true:false);

    return false;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::SetToNull(int row, int col)
{
    if(this->Allocated == false)
        return false;

    if(this->MapType == CELL_TYPE)
        Rast_set_c_null_value(&(this->CELLBuff[row * this->NumberOfCols + col]), 1);
    if(this->MapType == FCELL_TYPE)
        Rast_set_f_null_value(&(this->FCELLBuff[row * this->NumberOfCols + col]), 1);
    if(this->MapType == DCELL_TYPE)
        Rast_set_d_null_value(&(this->DCELLBuff[row * this->NumberOfCols + col]), 1);

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMemoryMap::SetToNull()
{
    int size = this->NumberOfCols * this->NumberOfRows;

    if(this->Allocated == false)
        return false;

    TRY
    if(this->MapType == CELL_TYPE)
        Rast_set_c_null_value(this->CELLBuff, size);
    else if(this->MapType == FCELL_TYPE)
        Rast_set_f_null_value(this->FCELLBuff, size);
    else if(this->MapType == DCELL_TYPE)
        Rast_set_d_null_value(this->DCELLBuff, size);
    else
        return false;
    CATCH_BOOL;

    return true;
}
