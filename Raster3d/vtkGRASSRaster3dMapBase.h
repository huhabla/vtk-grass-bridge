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
 * \brief This is the base class for grass raster3d map reading and writing.
 *
 * It provides function to set and get the
 * vtkGRASSRegion and the vtkGRASSHistory.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRaster3dMapBase_h
#define __vtkGRASSRaster3dMapBase_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeRaster3dWin32Header.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSHistory.h"
#include "vtkDCELL.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster3d.h>
#include <grass/raster.h>
}

class VTK_GRASS_BRIDGE_RASTER3D_EXPORT vtkGRASSRaster3dMapBase : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSRaster3dMapBase *New();
  vtkTypeMacro(vtkGRASSRaster3dMapBase,vtkObjectGRASSErrorHandler);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkGetStringMacro(Raster3dName);
  vtkGetStringMacro(Mapset);

  virtual void UseDefaultRegion(){this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);}
  virtual void UseCurrentRegion(){this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);}
  virtual void UseRasterRegion(){this->SetRegionUsage(VTK_GRASS_REGION_RASTER);}
  virtual void UseUserDefinedRegion(){this->SetRegionUsage(VTK_GRASS_REGION_USER);}
  vtkGetMacro(RegionUsage, int);

  //!\brief Return the maptype as string (FCELL_TYPE and DCELL_TYPE or NULL if unknown)
  virtual const char* GetMapTypeAsString();

  //! \brief Set the Region which should be used to open the grass raster3d map
  vtkSetObjectMacro(Region, vtkGRASSRegion);
  vtkGetObjectMacro(Region, vtkGRASSRegion);

    //! \brief Get the History of the grass raster3d map
  vtkGetObjectMacro(History, vtkGRASSHistory);
  
  //! \brief open the map for reading or writing. This function must be implemented by the subclasses.
  virtual bool OpenMap(char *name){return false;}

  //! \brief Close the raster 3d map
  virtual bool CloseMap();

  //! \brief Read a single value on index position x, y and z in right hand 
  //! side cube coordinate system
  //! Double is always returned, independent from the map type
  virtual double GetValue(int x, int y, int z);

  //! \brief Read a single value on index position x, y and z in right hand 
  //! side cube coordinate system
  virtual bool GetValue(int x, int y, int z, vtkDCELL *value);

  //! \brief Read a single value as DCELL value at window coordinates north, east and top
  virtual bool GetValue(double north, double east, double top, vtkDCELL *value);
  
  vtkGetMacro(MapType, int);
  vtkGetMacro(NumberOfRows, int);
  vtkGetMacro(NumberOfCols, int);
  vtkGetMacro(NumberOfDepths, int);

  //! \brief Returns true if the map is open
  //! \return true for open, false for not open
  virtual bool IsOpen() {return this->Open;}
  

protected:
  vtkGRASSRaster3dMapBase();
  ~vtkGRASSRaster3dMapBase();
  
  int RegionUsage;
  bool Open; // True if the raster3d map was opened
  int MapType; // FCELL or DCELL
  int NumberOfRows;
  int NumberOfCols;
  int NumberOfDepths;
  char *Raster3dName;
  char *Mapset;

  //BTX
  RASTER3D_Map *Map;  // Pointer to the raster3d map
  //ETX

  vtkGRASSRegion *Region;
  vtkGRASSHistory *History;

  //! \brief Set the region
  bool SetRegion();

  //! \brief Name of the raster3d map to be readed or created
  vtkSetStringMacro(Raster3dName);
  vtkSetStringMacro(Mapset);
  vtkSetMacro(MapType, int);
  vtkSetMacro(RegionUsage, int);

private:
  vtkGRASSRaster3dMapBase(const vtkGRASSRaster3dMapBase&);  // Not implemented.
  void operator=(const vtkGRASSRaster3dMapBase&);  // Not implemented.
};


#endif
