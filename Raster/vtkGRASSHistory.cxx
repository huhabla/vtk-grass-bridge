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

#include "vtkGRASSHistory.h"
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"

extern "C"
{
#include <string.h>
}

vtkStandardNewMacro(vtkGRASSHistory);

//----------------------------------------------------------------------------

vtkGRASSHistory::vtkGRASSHistory()
{

    this->hist = (History*)G_calloc(1, sizeof(struct History));

    this->Is3d = false;

    Rast_short_history("Title", "raster", this->hist);
    Rast_set_history(this->hist, HIST_KEYWRD, G_program_name());

    this->RasterName = NULL;

}

//----------------------------------------------------------------------------

vtkGRASSHistory::~vtkGRASSHistory()
{
    if (this->RasterName)
        delete [] this->RasterName;

    TRY
        Rast_free_history(this->hist);
        G_free(this->hist);
    CATCH_VOID
}

//----------------------------------------------------------------------------

bool
vtkGRASSHistory::ReadHistory(const char *rastername)
{
    const char *mapset;
    char buff[1024];


    mapset = G_find_raster2(rastername, "");
    if (mapset == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to find map %s.",
                   this->GetClassName(), __LINE__, rastername);
        this->InsertNextError(buff);
        return false;
    }

    if (Rast_read_history(rastername, mapset, this->hist) == -1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to read history file of map %s.",
                   this->GetClassName(), __LINE__, rastername);
        this->InsertNextError(buff);
        return false;
    }

    this->SetRasterName(rastername);
    this->Is3d = false;

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSHistory::Read3dHistory(const char *rastername)
{
    const char *mapset;
    char buff[1024];


    mapset = G_find_raster3d(rastername, "");
    if (mapset == NULL)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to find 3d raster map %s.",
                   this->GetClassName(), __LINE__, rastername);
        this->InsertNextError(buff);
        return false;
    }

    if (Rast3d_read_history(rastername, mapset, this->hist) == -1)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Unable to read history file of 3d raster map %s.",
                   this->GetClassName(), __LINE__, rastername);
        this->InsertNextError(buff);
        return false;
    }

    this->SetRasterName(rastername);
    this->Is3d = true;

    return true;
}


//----------------------------------------------------------------------------

bool
vtkGRASSHistory::WriteHistory(const char *rastername)
{
    TRY Rast_write_history(rastername, this->hist);
    CATCH_BOOL

    this->SetRasterName(rastername);
    this->Is3d = false;

    return true;
}

//----------------------------------------------------------------------------

bool
vtkGRASSHistory::Write3dHistory(const char *rastername)
{
    TRY Rast3d_write_history(rastername, this->hist);
    CATCH_BOOL

    this->SetRasterName(rastername);
    this->Is3d = true;

    return true;
}

//----------------------------------------------------------------------------

void
vtkGRASSHistory::PrintSelf(ostream& os, vtkIndent indent)
{
    int i;
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Raster name: " << (this->RasterName ? this->RasterName : "(none)") << endl;
    os << indent << "mapid: " << Rast_get_history(this->hist, HIST_MAPID) << endl;
    os << indent << "title: " << Rast_get_history(this->hist, HIST_TITLE) << endl;
    os << indent << "mapset: " << Rast_get_history(this->hist, HIST_MAPSET) << endl;
    os << indent << "creator: " << Rast_get_history(this->hist, HIST_CREATOR) << endl;
    os << indent << "maptype: " << Rast_get_history(this->hist, HIST_MAPTYPE) << endl;
    os << indent << "datsrc_1: " << Rast_get_history(this->hist, HIST_DATSRC_1) << endl;
    os << indent << "datsrc_2: " << Rast_get_history(this->hist, HIST_DATSRC_2) << endl;
    os << indent << "keywrd: " << Rast_get_history(this->hist, HIST_KEYWRD) << endl;

    for(i = 0; i < this->hist->nlines; i++)
        os << indent.GetNextIndent() << "Comment " << i << ": " << this->hist->lines[i] << endl;

    return;
}
