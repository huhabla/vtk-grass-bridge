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
 * \brief This class reads a grass raster map from a valid location/mapset
 * and creates a vtkImageData as output.
 *
 * To use this class make sure you are in a grass session and vtkGRASSInit was called first.
 *
 * The user can choose the current region, the default region, the raster map region or
 * a user defined region to open the raster map.
 * The position of the lower left edge of the created image will be set to the
 * east/south coordinates of the choosen region. The data spacing in x and y
 * direction will be set by the ew and ns resolution of the choosen region.
 * The raster map types CELL, FCELL or DCELL
 * will be conveted into int, float or double types.
 *
 * This class uses the vtkGRASSRaster3dMapReader to read the raster map rows into the memory.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSRaster3dImageReader_h
#define __vtkGRASSRaster3dImageReader_h

#include <vtkImageAlgorithm.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSRaster3dMapReader.h"

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSRaster3dImageReader : public vtkImageAlgorithm
{
public:
  static vtkGRASSRaster3dImageReader *New();
  vtkTypeMacro(vtkGRASSRaster3dImageReader,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //! \brief Get the data type of pixels in the imported data.
  //! As a convenience, the OutputScalarType is set to the same value.
  vtkGetMacro(DataScalarType, int);
  const char *GetDataScalarTypeAsString() {
    return vtkImageScalarTypeNameMacro(this->DataScalarType);
  }

  vtkSetStringMacro(Raster3dName);
  vtkGetStringMacro(Raster3dName);
  vtkGetStringMacro(Mapset);

  void UseDefaultRegion(){this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);}
  void UseCurrentRegion(){this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);}
  void UseRasterRegion(){this->SetRegionUsage(VTK_GRASS_REGION_RASTER);}
  void UseUserDefinedRegion(){this->SetRegionUsage(VTK_GRASS_REGION_USER);}
  vtkGetMacro(RegionUsage, int);

  /*! \brief Set the region which should be used to open the raster map
   *
   * */
  void SetRegion(vtkGRASSRegion *region) {this->Raster3dMap->SetRegion(region);}
  vtkGRASSRegion *GetRegion() {return this->Raster3dMap->GetRegion();}

  /*! \brief Return the Raster3dMap object
   *
   * */
  vtkGetObjectMacro(Raster3dMap, vtkGRASSRaster3dMapReader);

    //! \brief Null value which should replace the default grass null value for CELL, FCELL andDCELL maps
  //! to enable the NullValue, set the this->UseGRASSNulleValueOff()
  vtkSetMacro(NullValue, double);
  //! \brief Null value which should replace the default grass null value for CELL, FCELL andDCELL maps
  //! to enable the NullValue, set the this->UseGRASSNulleValueOff()
  vtkGetMacro(NullValue, double);

  //! \brief Read the GRASS raster image values as cell data rather then point data which is the default.
  //! Is set true the layout of the image will change (number of raster pixels are now number of cells)
  vtkSetMacro(AsCellData, int);
  //! \brief Read the GRASS raster image values as cell data rather then point data which is the default
  vtkGetMacro(AsCellData, int);
  //! \brief Read the GRASS raster image values as cell data rather then point data which is the default.
  //! Is set true the layout of the image will change (number of raster pixels are now number of cells)
  vtkBooleanMacro(AsCellData, int);

protected:
  vtkGRASSRaster3dImageReader();
  ~vtkGRASSRaster3dImageReader();

  vtkSetStringMacro(Mapset);
  vtkSetMacro(RegionUsage, int);

  char *Raster3dName;
  char *Mapset;
  int DataScalarType;
  int RegionUsage;
  double NullValue;

  int AsCellData;

  int DataExtent[6];
  double DataSpacing[3];
  double DataOrigin[3];

  vtkGRASSRaster3dMapReader *Raster3dMap;

  vtkSetMacro(DataScalarType,int);
  void SetDataScalarTypeToDouble(){this->SetDataScalarType(VTK_DOUBLE);}
  void SetDataScalarTypeToFloat(){this->SetDataScalarType(VTK_FLOAT);}

  virtual int RequestInformation (vtkInformation*,
                                  vtkInformationVector**,
                                  vtkInformationVector*);

  virtual int RequestData(vtkInformation*,
                          vtkInformationVector**,
                          vtkInformationVector*);


private:
  vtkGRASSRaster3dImageReader(const vtkGRASSRaster3dImageReader&);  // Not implemented.
  void operator=(const vtkGRASSRaster3dImageReader&);  // Not implemented.
};


#endif
