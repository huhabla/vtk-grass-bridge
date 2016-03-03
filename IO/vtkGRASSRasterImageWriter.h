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
 * \brief This class writes a vtkImageData into a grass raster map.
 *
 * To use this class make sure you are in a grass session and vtkGRASSInit was called first.
 *
 * The user can choose the current region, the default region or
 * a user defined region to write the image as grass raster map.
 * Based on the dimension of the vtkImageData,
 * the region resolution will be adjusted accordingly.
 *
 * This class uses the vtkGRASSRasterMapWriter to write the image data into
 * the grass raster map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSRasterImageWriter_h
#define __vtkGRASSRasterImageWriter_h

#include <vtkSimpleImageToImageFilter.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSRasterMapWriter.h"
#include "vtkGRASSDefines.h"

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSRasterImageWriter : public vtkSimpleImageToImageFilter {
public:
    static vtkGRASSRasterImageWriter *New();
    vtkTypeMacro(vtkGRASSRasterImageWriter, vtkSimpleImageToImageFilter);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkSetStringMacro(RasterName);
    vtkGetStringMacro(RasterName);
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
        this->RasterMap->SetRegion(region);
    }

    vtkGRASSRegion *GetRegion() {
        return this->RasterMap->GetRegion();
    }

    //! Enable or disable the interpretation of a NullValue
    vtkSetMacro(UseNullValue, int);
    //! Enable or disable the interpretation of a NullValue
    vtkGetMacro(UseNullValue, int);
    //! Enable or disable the interpretation of a NullValue 
    vtkBooleanMacro(UseNullValue, int);

    //! \brief Null value which represent the default grass null value for CELL, FCELL and DCELL maps.
    //! In case a NullValue is found, the grass null value will be set in the raster map
    //! to enable the NullValue, set this->InterpretNulleValueOn()
    vtkSetMacro(NullValue, double);
    //! \brief Null value which should replace the default grass null value for CELL, FCELL and DCELL maps
    //! In case a NullValue is found, the grass null value will be set in the raster map
    //! to enable the NullValue, set this->InterpretNulleValueOn()
    vtkGetMacro(NullValue, double);

    vtkGetObjectMacro(RasterMap, vtkGRASSRasterMapWriter);

protected:
    vtkGRASSRasterImageWriter();
    ~vtkGRASSRasterImageWriter();

    vtkSetStringMacro(Mapset);
    vtkSetMacro(RegionUsage, int);

    char *RasterName;
    char *Mapset;
    int RegionUsage;
    double NullValue;
    int UseNullValue;

    vtkGRASSRasterMapWriter *RasterMap;

    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

private:
    vtkGRASSRasterImageWriter(const vtkGRASSRasterImageWriter&); // Not implemented.
    void operator=(const vtkGRASSRasterImageWriter&); // Not implemented.
};


#endif
