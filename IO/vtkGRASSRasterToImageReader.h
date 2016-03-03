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
 * \brief This class uses the native grass io library functions to read a
 * raster map into a vtkImageData object. This class has been replaced by
 * vtkGRASSRasterImageReader.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */
#ifndef __vtkGRASSRasterToImageReader_h
#define __vtkGRASSRasterToImageReader_h

#include <vtkImageAlgorithm.h>
#include "vtkGRASSBridgeIOWin32Header.h"

#include "vtkGRASSDefines.h"
#include <vtkGRASSRegion.h>

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSRasterToImageReader : public vtkImageAlgorithm
{
public:
  static vtkGRASSRasterToImageReader *New();
  vtkTypeMacro(vtkGRASSRasterToImageReader,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get the data type of pixels in the imported data.
  // As a convenience, the OutputScalarType is set to the same value.
  vtkGetMacro(DataScalarType, int);
  const char *GetDataScalarTypeAsString() {
    return vtkImageScalarTypeNameMacro(this->DataScalarType); }

  vtkSetStringMacro(RasterName);
  vtkGetStringMacro(RasterName);
  vtkGetStringMacro(Mapset);

  void UseDefaultRegion(){this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);}
  void UseCurrentRegion(){this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);}
  void UseRasterRegion(){this->SetRegionUsage(VTK_GRASS_REGION_RASTER);}
  void UseUserDefinedRegion(){this->SetRegionUsage(VTK_GRASS_REGION_USER);}
  vtkGetMacro(RegionUsage, int);

  //! \brief Set the Region which should be used to open the grass raster map
  vtkSetObjectMacro(Region, vtkGRASSRegion);
  vtkGetObjectMacro(Region, vtkGRASSRegion);

  double GetRasterValueAsDouble(int MapType, void *ptr, double nullval);

protected:
  vtkGRASSRasterToImageReader();
  ~vtkGRASSRasterToImageReader();

  vtkSetStringMacro(Mapset);
  vtkSetMacro(RegionUsage, int);

  char *RasterName;
  char *Mapset;
  int DataScalarType;
  int RegionUsage;

  int DataExtent[6];
  double DataSpacing[3];
  double DataOrigin[3];

  vtkGRASSRegion *Region;

  vtkSetMacro(DataScalarType,int);
  void SetDataScalarTypeToDouble(){this->SetDataScalarType(VTK_DOUBLE);}
  void SetDataScalarTypeToInt(){this->SetDataScalarType(VTK_INT);}
  void SetDataScalarTypeToShort(){this->SetDataScalarType(VTK_SHORT);}
  void SetDataScalarTypeToUnsignedShort() {this->SetDataScalarType(VTK_UNSIGNED_SHORT);}
  void SetDataScalarTypeToUnsignedChar() {this->SetDataScalarType(VTK_UNSIGNED_CHAR);}
  virtual int RequestInformation (vtkInformation*,
                                  vtkInformationVector**,
                                  vtkInformationVector*);

  virtual int RequestData(vtkInformation*,
                          vtkInformationVector**,
                          vtkInformationVector*);

private:
  vtkGRASSRasterToImageReader(const vtkGRASSRasterToImageReader&);  // Not implemented.
  void operator=(const vtkGRASSRasterToImageReader&);  // Not implemented.
};


#endif
