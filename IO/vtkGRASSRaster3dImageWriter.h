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

/**
 * \brief This class writes a 3d vtkImageData into a grass raster3d map.
 *
 * To use this class make sure you are in a valid grass session and vtkGRASSInit was called first.
 *
 * The user can choose the current region, the default region or
 * a user defined region to write the 3d image as grass raster3d map.
 * Based on the dimension of the vtkImageData,
 * the region resolution will be adjusted accordingly.
 *
 * This class uses the vtkGRASSRaster3dMapWriter to write the 3d image data into
 * the grass raster3d map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSRaster3dImageWriter_h
#define __vtkGRASSRaster3dImageWriter_h

#include <vtkSimpleImageToImageFilter.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSRaster3dMapWriter.h"
#include "vtkGRASSDefines.h"

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSRaster3dImageWriter : public vtkSimpleImageToImageFilter {
public:
    static vtkGRASSRaster3dImageWriter *New();
    vtkTypeMacro(vtkGRASSRaster3dImageWriter, vtkSimpleImageToImageFilter);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkSetStringMacro(Raster3dName);
    vtkGetStringMacro(Raster3dName);
    vtkGetStringMacro(Mapset);

    void UseDefaultRegion() {
        this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);
    }

    void UseCurrentRegion() {
        this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);
    }

    void UseUserDefinedRegion() {
        this->SetRegionUsage(VTK_GRASS_REGION_USER);
    }
    vtkGetMacro(RegionUsage, int);

    void SetRegion(vtkGRASSRegion *region) {
        this->Raster3dMap->SetRegion(region);
    }

    vtkGRASSRegion *GetRegion() {
        return this->Raster3dMap->GetRegion();
    }

    vtkGetObjectMacro(Raster3dMap, vtkGRASSRaster3dMapWriter);

protected:
    vtkGRASSRaster3dImageWriter();
    ~vtkGRASSRaster3dImageWriter();

    vtkSetStringMacro(Mapset);
    vtkSetMacro(RegionUsage, int);

    char *Raster3dName;
    char *Mapset;
    int RegionUsage;

    vtkGRASSRaster3dMapWriter *Raster3dMap;

    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

private:
    vtkGRASSRaster3dImageWriter(const vtkGRASSRaster3dImageWriter&); // Not implemented.
    void operator=(const vtkGRASSRaster3dImageWriter&); // Not implemented.
};


#endif
