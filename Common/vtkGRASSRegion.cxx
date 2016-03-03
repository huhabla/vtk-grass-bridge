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

#include "vtkGRASSRegion.h"
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"

vtkStandardNewMacro(vtkGRASSRegion);

//----------------------------------------------------------------------------

vtkGRASSRegion::vtkGRASSRegion() {
    this->BytesPerCell = 0;
    this->CompressionFlag = 0;
    this->Rows = 0;
    this->Cols = 0;
    this->Rows3d = 0;
    this->Cols3d = 0;
    this->Depths = 0;
    this->Top = 0;
    this->Bottom = 0;
    this->Projection = 0;
    this->Zone = 0;
    this->EastWestResolution = 0;
    this->NorthSouthResolution = 0;
    this->EastWestResolution3d = 0;
    this->NorthSouthResolution3d = 0;
    this->TopBottomResolution = 0;
    this->NorthernEdge = 0;
    this->SouthernEdge = 0;
    this->EasternEdge = 0;
    this->WesternEdge = 0;

    this->Name = NULL;

    this->ReadCurrentRegion();
}

//----------------------------------------------------------------------------

vtkGRASSRegion::~vtkGRASSRegion() {

    if(this->Name)
        delete [] this->Name;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::SetCurrentRegion() {
    this->CopyRegionTo(&this->head);
    TRY G_set_window(&this->head);
    CATCH_BOOL
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::ReadCurrentRegion() {
    TRY G_get_set_window(&this->head);
    CATCH_BOOL
    this->CopyRegionFrom(&this->head);
    this->Modified();
    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::ReadDefaultRegion() {
    TRY
    G_get_default_window(&this->head);
    CATCH_BOOL
    this->CopyRegionFrom(&this->head);
    this->Modified();

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::ReadRegion(char *regionName) {

    const char * mapset;
    char buff[1024];

    this->SetName(regionName);

    if (this->Name == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Please set the region name first",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (G_legal_filename(this->Name) < 0) {
        G_snprintf(buff, 1024, "class: %s line: %i Region name does not fit GRASS naming convecntion",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    mapset = G_find_file("windows", this->Name, "");
    if (!mapset) {
        G_snprintf(buff, 1024, "class: %s line: %i Region file <%s> not found",
                    this->GetClassName(), __LINE__, this->Name);
        this->InsertNextError(buff);
        return false;
    }
    TRY
    G_get_element_window(&this->head, "windows", this->Name, mapset);
    CATCH_BOOL

    this->CopyRegionFrom(&this->head);
    this->Modified();

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::SaveRegion(char *regionName) {

    char buff[1024];

    this->SetName(regionName);
    this->CopyRegionTo(&this->head);

    if (this->Name == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Please set the region name first",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    if (G_legal_filename(this->Name) < 0) {
        G_snprintf(buff, 1024, "class: %s line: %i Region name does not fit GRASS naming convecntion",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    TRY
    G_adjust_Cell_head(&this->head, 1, 1);
    G_adjust_Cell_head3(&this->head, 1, 1, 1);
    CATCH_BOOL


    this->CopyRegionTo(&this->head);
    if(G_put_element_window(&this->head, "windows", this->Name) != 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to save the region",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::SaveRegionAsDefault() {
    char buff[1024];

    this->CopyRegionTo(&this->head);

    TRY G_adjust_Cell_head(&this->head, 1, 1);
    G_adjust_Cell_head3(&this->head, 1, 1, 1);
    CATCH_BOOL

    if(G_put_window(&this->head) != 1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Error while writing default region",
                    this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::AdjustRegionResolution() {

    this->CopyRegionTo(&this->head);

    TRY G_adjust_Cell_head(&this->head, 1, 1);
    CATCH_BOOL

    this->CopyRegionFrom(&this->head);

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::AdjustRegionFromResolution() {

    this->CopyRegionTo(&this->head);

    TRY G_adjust_Cell_head(&this->head, 0, 0);
    CATCH_BOOL

    this->CopyRegionFrom(&this->head);

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::AdjustRegion3dResolution() {

    this->CopyRegionTo(&this->head);

    TRY G_adjust_Cell_head3(&this->head, 1, 1, 1);
    CATCH_BOOL

    this->CopyRegionFrom(&this->head);

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::AdjustRegion3dFromResolution() {

    this->CopyRegionTo(&this->head);

    TRY G_adjust_Cell_head3(&this->head, 0, 0, 0);
    CATCH_BOOL

    this->CopyRegionFrom(&this->head);

    return true;
}

//----------------------------------------------------------------------------

bool vtkGRASSRegion::CopyRegionFrom(struct Cell_head *head) {
    this->BytesPerCell = head->format;
    this->CompressionFlag = head->compressed;
    this->Rows = head->rows;
    this->Cols = head->cols;
    this->Rows3d = head->rows3;
    this->Cols3d = head->cols3;
    this->Depths = head->depths;
    this->Top = head->top;
    this->Bottom = head->bottom;
    this->Projection = head->proj;
    this->Zone = head->zone;
    this->EastWestResolution = head->ew_res;
    this->NorthSouthResolution = head->ns_res;
    this->EastWestResolution3d = head->ew_res3;
    this->NorthSouthResolution3d = head->ns_res3;
    this->TopBottomResolution = head->tb_res;
    this->NorthernEdge = head->north;
    this->SouthernEdge = head->south;
    this->EasternEdge = head->east;
    this->WesternEdge = head->west;
    this->Modified();
    return true;
}
//----------------------------------------------------------------------------

bool vtkGRASSRegion::CopyRegionTo(struct Cell_head *head) {
    head->format = this->BytesPerCell;
    head->compressed = this->CompressionFlag;
    head->rows = this->Rows;
    head->cols = this->Cols;
    head->rows3 = this->Rows3d;
    head->cols3 = this->Cols3d;
    head->depths = this->Depths;
    head->top = this->Top;
    head->bottom = this->Bottom;
    head->proj = this->Projection;
    head->zone = this->Zone;
    head->ew_res = this->EastWestResolution;
    head->ns_res = this->NorthSouthResolution;
    head->ew_res3 = this->EastWestResolution3d;
    head->ns_res3 = this->NorthSouthResolution3d;
    head->tb_res = this->TopBottomResolution;
    head->north = this->NorthernEdge;
    head->south = this->SouthernEdge;
    head->east = this->EasternEdge;
    head->west = this->WesternEdge;

    return true;
}

//----------------------------------------------------------------------------

void vtkGRASSRegion::DeepCopy(vtkGRASSRegion *region) {
    this->BytesPerCell = region->BytesPerCell;
    this->CompressionFlag = region->CompressionFlag;
    this->Rows = region->Rows;
    this->Cols = region->Cols;
    this->Rows3d = region->Rows3d;
    this->Cols3d = region->Cols3d;
    this->Depths = region->Depths;
    this->Top = region->Top;
    this->Bottom = region->Bottom;
    this->Projection = region->Projection;
    this->Zone = region->Zone;
    this->EastWestResolution = region->EastWestResolution;
    this->NorthSouthResolution = region->NorthSouthResolution;
    this->EastWestResolution3d = region->EastWestResolution3d;
    this->NorthSouthResolution3d = region->NorthSouthResolution3d;
    this->TopBottomResolution = region->TopBottomResolution;
    this->NorthernEdge = region->NorthernEdge;
    this->SouthernEdge = region->SouthernEdge;
    this->EasternEdge = region->EasternEdge;
    this->WesternEdge = region->WesternEdge;
    this->CopyRegionTo(&this->head);
    this->Modified();
    return;
}

//----------------------------------------------------------------------------

void vtkGRASSRegion::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Region name: " << (this->Name?this->Name:"(none)") << endl;
    os << indent << "BytesPerCell: " << this->head.format << endl;
    os << indent << "CompressionFlag: " << this->head.compressed << endl;
    os << indent << "Rows: " << this->head.rows << endl;
    os << indent << "Cols: " << this->head.cols << endl;
    os << indent << "Rows3d: " << this->head.rows3 << endl;
    os << indent << "Cols3d: " << this->head.cols3 << endl;
    os << indent << "Depths: " << this->head.depths << endl;
    os << indent << "Top: " << this->head.top << endl;
    os << indent << "Bottom: " << this->head.bottom << endl;
    os << indent << "Projection: " << this->head.proj << endl;
    os << indent << "Zone: " << this->head.zone << endl;
    os << indent << "EastWestResolution: " << this->head.ew_res << endl;
    os << indent << "NorthSouthResolution: " << this->head.ns_res << endl;
    os << indent << "EastWestResolution3d: " << this->head.ew_res3 << endl;
    os << indent << "NorthSouthResolution3d: " << this->head.ns_res3 << endl;
    os << indent << "TopBottomResolution: " << this->head.tb_res << endl;
    os << indent << "NorthernEdge: " << this->head.north << endl;
    os << indent << "SouthernEdge: " << this->head.south << endl;
    os << indent << "EasternEdge: " << this->head.east << endl;
    os << indent << "WesternEdge: " << this->head.west << endl;
    return;
}
