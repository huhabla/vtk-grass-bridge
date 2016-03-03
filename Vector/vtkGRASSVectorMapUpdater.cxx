
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

#include "vtkGRASSVectorMapUpdater.h"
#include <vtkObjectFactory.h>
#include <vtkGRASSDefines.h>

vtkStandardNewMacro(vtkGRASSVectorMapUpdater);

//----------------------------------------------------------------------------

vtkGRASSVectorMapUpdater::vtkGRASSVectorMapUpdater()
{
    this->SetVectorLevelToTopo();
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapUpdater::RewriteFeature(int feature, int type, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats)
{
    if (this->Open)
    {
        TRY return Vect_rewrite_line(&this->map, feature, type, points->GetPointer(), cats->GetPointer());
        CATCH_INT
    }
    return 0;
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorMapUpdater::RestoreFeature(int line, int offset)
{
    if (this->Open)
    {
        TRY return Vect_restore_line(&this->map, line, (off_t) offset);
        CATCH_INT
    }
    return 0;
}

//----------------------------------------------------------------------------

bool
vtkGRASSVectorMapUpdater::OpenMap(const char *name, int with_z)
{
    char buff[1024];

    // Check if the same map is already opened
    if (this->Open == true && strcmp(name, this->VectorName) == 0)
    {
        G_snprintf(buff, 1024, "class: %s line: %i Vector map %s is already open.",
                   this->GetClassName(), __LINE__, this->VectorName);
        this->InsertNextError(buff);
        return false;
    }
    else if (this->Open == true)
    {
        // If a new name is given, the open map will be closed
        this->CloseMap(0);
    }

    this->SetVectorName(name);

    Vect_set_open_level(this->VectorLevel);
    if (!setjmp(vgb_stack_buffer))
    {
        if (1 > Vect_open_update(&this->map, name, ""))
        {
            G_snprintf(buff, 1024, "class: %s line: %i Unable to open vector map <%s>.",
                       this->GetClassName(), __LINE__, name);
            this->InsertNextError(buff);
            this->Open = false;
            return false;
        }
    }
    else
    {
        this->InsertNextError(vgb_error_message);
        this->Open = false;
        return false;
    }

    this->Open = true;

    // Create the Dbmi interface for database operations
    if(this->DbmiInterface == NULL)
        this->DbmiInterface = vtkGRASSDbmiInterface::New();
    this->DbmiInterface->SetVectorMap(this);

    return true;
}
