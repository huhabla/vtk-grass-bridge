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

#include "vtkGRASSRasterMapBase.h"
#include <vtkStringArray.h>
#include <vtkCharArray.h>
#include <vtkObjectFactory.h>
#include <vtkDataArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include "vtkGRASSRegion.h"
#include "vtkGRASSHistory.h"
#include "vtkCELL.h"
#include "vtkFCELL.h"
#include <vtkGRASSDefines.h>

vtkStandardNewMacro(vtkGRASSRasterMapBase);

//----------------------------------------------------------------------------

vtkGRASSRasterMapBase::vtkGRASSRasterMapBase()
{
    this->RasterName = NULL;
    this->Mapset = NULL;
    this->Region = vtkGRASSRegion::New();
    this->History = vtkGRASSHistory::New();
    this->Map = -1;
    this->MapType = CELL_TYPE;
    this->NumberOfRows = 0;
    this->NumberOfCols = 0;
    this->Open = false;
    this->Row = NULL;
    this->NullRow = vtkSmartPointer<vtkCharArray>::New();
    this->RegionUsage = VTK_GRASS_REGION_CURRENT;
    this->RasterBuff = (void*) NULL;
    this->NullBuff = (char*) NULL;
    this->NullValue = -999999;
    this->UseNullValue = 1;
}

//----------------------------------------------------------------------------

vtkGRASSRasterMapBase::~vtkGRASSRasterMapBase()
{
    if (this->RasterName)
        delete [] this->RasterName;
    if (this->Mapset)
        delete [] this->Mapset;

    this->Region->Delete();
    this->History->Delete();

    if (this->RasterBuff)
        G_free(this->RasterBuff);
    if (this->NullBuff)
        G_free(this->NullBuff);
}

//----------------------------------------------------------------------------

void
vtkGRASSRasterMapBase::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Is map open: " << (this->Open ? "YES" : "NO") << endl;
    if (this->Open) {
        os << indent << "Name   : " << this->RasterName << endl;
        os << indent << "Mapset : " << this->Mapset << endl;
        os << indent << "Maptype: " << this->GetMapTypeAsString() << endl;
        os << indent << "Cols   : " << this->NumberOfCols << endl;
        os << indent << "Rows   : " << this->NumberOfRows << endl;
        os << indent << "UseNullValue: " << this->UseNullValue << endl;
        this->History->PrintSelf(os, indent.GetNextIndent());
    }
    return;
}

//----------------------------------------------------------------------------

const char* vtkGRASSRasterMapBase::GetMapTypeAsString()
{
    if (this->MapType == CELL_TYPE)
        return "CELL_TYPE";
    if (this->MapType == FCELL_TYPE)
        return "FCELL_TYPE";
    if (this->MapType == DCELL_TYPE)
        return "DCELL_TYPE";

    return NULL;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapBase::SetRegion()
{
    struct Cell_head head;

    if (this->RasterName == NULL || this->Mapset == NULL) {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to set the region. RasterName is not set.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }
    TRY
    if (this->RegionUsage == VTK_GRASS_REGION_CURRENT) {
        G_get_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    } else if (this->RegionUsage == VTK_GRASS_REGION_DEFAULT) {
        G_get_default_window(&head);
        G_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    } else if (this->RegionUsage == VTK_GRASS_REGION_USER && this->Region != NULL) {
        this->Region->CopyRegionTo(&head);
        G_set_window(&head);
    } else {
        // Use current region as default
        G_get_set_window(&head);
        this->Region->CopyRegionFrom(&head);
    }
    CATCH_BOOL

    this->NumberOfRows = head.rows;
    this->NumberOfCols = head.cols;

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapBase::SetUpRasterBuffer()
{

    // The region must be set and map must be open to allocate the buffer
    if (this->Open == false || this->Map < 0) {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate raster buffer. Raster map is not open",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (this->Row == NULL) {
        if (this->MapType == CELL_TYPE)
            this->Row = vtkSmartPointer<vtkIntArray>::New();
        else if (this->MapType == FCELL_TYPE)
            this->Row = vtkSmartPointer<vtkFloatArray>::New();
        else if (this->MapType == DCELL_TYPE)
            this->Row = vtkSmartPointer<vtkDoubleArray>::New();

        this->Row->SetNumberOfTuples(this->NumberOfCols);
    }

    TRY
    if (this->RasterBuff == NULL) {
        if (this->MapType == CELL_TYPE)
            this->RasterBuff = (CELL*) G_calloc(this->NumberOfCols, sizeof (CELL));
        else if (this->MapType == FCELL_TYPE)
            this->RasterBuff = (FCELL*) G_calloc(this->NumberOfCols, sizeof (FCELL));
        else if (this->MapType == DCELL_TYPE)
            this->RasterBuff = (DCELL*) G_calloc(this->NumberOfCols, sizeof (DCELL));

    }
    CATCH_BOOL

    return true;
}


//----------------------------------------------------------------------------

bool
vtkGRASSRasterMapBase::CloseMap()
{
    // Cleaning up the null buffer for reuse
    if (this->NullBuff) {
        G_free(this->NullBuff);
        this->NullBuff = (char*) NULL;
    }

    // Cleaning up the raster buffer for reuse
    if (this->RasterBuff) {
        G_free(this->RasterBuff);
        this->RasterBuff = (void*) NULL;
    }

    if (this->Open == true && this->Map != -1) {
        TRY Rast_close(this->Map);
        CATCH_BOOL
    }
    // This flag is important and must be set by open and close methods
    this->Open = false;

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetSampleValue(double north, double east, vtkDCELL *value, int type)
{
    if (!this->IsOpen()){
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to compute sample because map is not open.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    value->Value = Rast_get_sample(this->Map, this->Region->GetPointer(), NULL, north, east, 0, type);

    return !value->IsNull();
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetNearestSampleValue(double north, double east, vtkDCELL *value)
{
    return this->GetSampleValue(north, east, value, INTERP_NEAREST);
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetBilinearSampleValue(double north, double east, vtkDCELL *value)
{
    return this->GetSampleValue(north, east, value, INTERP_BILINEAR);
}

//----------------------------------------------------------------------------

bool vtkGRASSRasterMapBase::GetBicubicSampleValue(double north, double east, vtkDCELL *value)
{
    return this->GetSampleValue(north, east, value, INTERP_BICUBIC);
}
