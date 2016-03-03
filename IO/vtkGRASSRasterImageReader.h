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
 * This class uses the vtkGRASSRasterMapReader to read the raster map rows into the memory.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSRasterImageReader_h
#define __vtkGRASSRasterImageReader_h

#include <vtkImageAlgorithm.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSRasterMapReader.h"

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSRasterImageReader : public vtkImageAlgorithm
{
public:
  static vtkGRASSRasterImageReader *New();
  vtkTypeMacro(vtkGRASSRasterImageReader,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //! \brief Get the data type of pixels in the imported data.
  //! As a convenience, the OutputScalarType is set to the same value.
  vtkGetMacro(DataScalarType, int);
  const char *GetDataScalarTypeAsString() {
    return vtkImageScalarTypeNameMacro(this->DataScalarType);
  }

  vtkSetStringMacro(RasterName);
  vtkGetStringMacro(RasterName);
  vtkGetStringMacro(Mapset);

  void UseDefaultRegion(){this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);}
  void UseCurrentRegion(){this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);}
  void UseRasterRegion(){this->SetRegionUsage(VTK_GRASS_REGION_RASTER);}
  void UseUserDefinedRegion(){this->SetRegionUsage(VTK_GRASS_REGION_USER);}
  vtkGetMacro(RegionUsage, int);

  /*! \brief Set the region which should be used to open the raster map
   *
   * */
  void SetRegion(vtkGRASSRegion *region) {this->RasterMap->SetRegion(region);}
  vtkGRASSRegion *GetRegion() {return this->RasterMap->GetRegion();}

  /*! \brief Return the RasterMap object
   *
   * */
  vtkGetObjectMacro(RasterMap, vtkGRASSRasterMapReader);

    //! \brief Null value which should replace the default grass null value for CELL, FCELL and DCELL maps
  //! to enable the NullValue, set this->UseGRASSNulleValueOff()
  vtkSetMacro(NullValue, double);
  //! \brief Null value which should replace the default grass null value for CELL, FCELL and DCELL maps
  //! to enable the NullValue, set this->UseGRASSNulleValueOff()
  vtkGetMacro(NullValue, double);


    //! Enable or disable the interpretation of a NullValue
    vtkSetMacro(UseNullValue, int);
    //! Enable or disable the interpretation of a NullValue
    vtkGetMacro(UseNullValue, int);
    //! Enable or disable the interpretation of a NullValue
    vtkBooleanMacro(UseNullValue, int);

    //! \brief Create a vtkImagedata with the same scalar type as the raster map
    void ReadMapAsMapType(){this->SetMapTypeConversion(0);}
    //! \brief Create a vtkImagedata with scalar type integer and convert the raster map type
    void ReadMapAsInt(){this->SetMapTypeConversion(1);}
    //! \brief Create a vtkImagedata with scalar type float and convert the raster map type
    void ReadMapAsFloat(){this->SetMapTypeConversion(2);}
    //! \brief Create a vtkImagedata with scalar type double and convert the raster map type
    void ReadMapAsDouble(){this->SetMapTypeConversion(3);}

    //! \brief Read the GRASS raster image values as cell data rather then point data which is the default.
    //! Is set true the layout of the image will change (number of raster pixels are now number of cells)
    vtkSetMacro(AsCellData, int);
    //! \brief Read the GRASS raster image values as cell data rather then point data which is the default
    vtkGetMacro(AsCellData, int);
    //! \brief Read the GRASS raster image values as cell data rather then point data which is the default.
    //! Is set true the layout of the image will change (number of raster pixels are now number of cells)
    vtkBooleanMacro(AsCellData, int);

protected:
  vtkGRASSRasterImageReader();
  ~vtkGRASSRasterImageReader();

  char *RasterName;
  char *Mapset;
  int DataScalarType;
  int RegionUsage;
  double NullValue;
  int UseNullValue;

  int AsCellData;

  int MapTypeConversion; // 0 == same as map, 1 == int, 2 == float, 3 == double

  int DataExtent[6];
  double DataSpacing[3];
  double DataOrigin[3];

  vtkGRASSRasterMapReader *RasterMap;

  vtkSetStringMacro(Mapset);
  vtkSetMacro(RegionUsage, int);

  vtkSetMacro(MapTypeConversion, int);

  vtkSetMacro(DataScalarType,int);
  void SetDataScalarTypeToDouble(){this->SetDataScalarType(VTK_DOUBLE);}
  void SetDataScalarTypeToFloat(){this->SetDataScalarType(VTK_FLOAT);}
  void SetDataScalarTypeToInt(){this->SetDataScalarType(VTK_INT);}

  virtual int RequestInformation (vtkInformation*,
                                  vtkInformationVector**,
                                  vtkInformationVector*);

  virtual int RequestData(vtkInformation*,
                          vtkInformationVector**,
                          vtkInformationVector*);

private:
  vtkGRASSRasterImageReader(const vtkGRASSRasterImageReader&);  // Not implemented.
  void operator=(const vtkGRASSRasterImageReader&);  // Not implemented.
};


#endif
