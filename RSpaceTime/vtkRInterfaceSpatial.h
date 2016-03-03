/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2011 by Soeren Gebbert, soerengebbert@googlemail.com
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
 * \brief This class allows the conversion of vtkPointData and vtkImageData into R spatial objects 
 * 
 * Only Point coordiantes and point data are currently supported. Use the VTK class vtkCellDataToPointData
 * to convert cell data into point data. Make sure correct and unique array names are used.
 * The spatial objects can be accessed in the VTK R instance by an identifier which must
 * be specified. 
 *
 * \TODO Implement support for all poly data cell types: lines, poly lines, triangles and polygons
 * 
 * */

#ifndef __vtkRInterfaceSpatial_h
#define __vtkRInterfaceSpatial_h

#include <vtkRInterface.h>
#include "vtkGRASSBridgeRSpaceTimeWin32Header.h"

class vtkPointSet;
class vtkImageData;
class vtkStringArray;
class vtkDataSetAttributes;

class VTK_GRASS_BRIDGE_RSPACETIME_EXPORT vtkRInterfaceSpatial : public vtkRInterface
{
public:
  static  vtkRInterfaceSpatial *New();
  vtkTypeMacro(vtkRInterfaceSpatial, vtkRInterface);
  
  //!\brief Assign a vtkPointData object to R as SpatialPointsDataFrame specified in RVariable of the sp package
  //! Only Point coordinates and point data are assigned. Cells and cell data are ignored.
  //! In case no data arrays are present in point data, only a SpatialPoints object will be created in R.
  virtual bool AssignVTKPointSetToRSpatialPointsDataFrame(vtkPointSet *data, const char *RVariable);
  
  //!\brief Assign a vtkImageData object to R as SpatialGridDataFrame specified in RVariable of the sp package
  //! The sptail extent, origin and dimension and the point data of the image are assigned. 
  //! Cells and cell data are ignored.
  //! In case no data arrays are present in point data, only a SpatialGrid object will be created in R.
  virtual bool AssignVTKImageDataToRSpatialGridDataFrame(vtkImageData *data, const char *RVariable);
  
  
  //!\brief Assign a vtkPointData object to R as SpatialPoints specified in RVariable of the sp package
  //! Only Point coordinates are assigned.
  virtual bool AssignVTKPointSetToRSpatialPoints(vtkPointSet *data, const char *RVariable);
  
  //!\brief Assign a vtkImageData object to R as SpatialGrid specified in RVariable of the sp package
  //! The sptail extent, origin and dimension of the image are assigned.
  virtual bool AssignVTKImageDataToRSpatialGrid(vtkImageData *data, const char *RVariable);
  
  
  //!\brief Assign a vtkPointData object to R as SpatialPointsDataFrame specified in RVariable of the sp package
  //! Only Point coordinates and point data are assigned. Cells and cell data are ignored.
  //! A list of point data arrays which should be assigned to R must be provided, so only selected 
  //! arrays will be assigned to R using the array names as R variable names.
  //! In case no data arrays are present in point data, only a SpatialPoints object will be created in R.
  virtual bool AssignVTKPointSetToRSpatialPointsDataFrame(vtkPointSet *data, const char *RVariable, vtkStringArray *arraynames);
  
  //!\brief Assign a vtkImageData object to R as SpatialGridDataFrame with name specified in RVariable of the sp package
  //! The sptail extent, origin and dimension and the point data of the image are assigned. 
  //! Cells and cell data are ignored.
  //! A list of point data arrays which should be assigned to R must be provided, so only selected 
  //! arrays will be assigned to R using the array names as R variable names. 
  //! In case no data arrays are present in point data, only a SpatialGrid object will be created in R.
  virtual bool AssignVTKImageDataToRSpatialGridDataFrame(vtkImageData *data, const char *RVariable, vtkStringArray *arraynames);
  
  //! \brief Assign data arrays present in a dataset attribute to R using the array names as R variable names. Make sure correct and unique array names are used.
  virtual bool AssignVTKDataSetAttributesToRDataFrame(vtkDataSetAttributes *data, const char *dataframe, vtkStringArray *arraynames);
  
  //! \brief Assign data arrays present in a dataset attribute to R using the array names as R variable names. Make sure correct and unique array names are used.
  virtual bool AssignVTKDataSetAttributesToRDataFrame(vtkDataSetAttributes *data, const char *dataframe);
  
  
  //!\brief Set the string which defines the coordiante reference system in proj4 format. This tring will be assigned
  //! to the created spatial objects in R.
  vtkSetStringMacro(Proj4String);
  //!\brief The string which defines the coordiante reference system in proj4 format
  vtkGetStringMacro(Proj4String);

protected:
  vtkRInterfaceSpatial();
  ~vtkRInterfaceSpatial() {};
  
  virtual vtkStringArray *CreateArrayNamesArray(vtkDataSetAttributes *data);
  virtual void AttachProjection(const char *spatial);
  
  char *Proj4String;

private:
  vtkRInterfaceSpatial(const vtkRInterfaceSpatial&);  // Not implemented.
  void operator=(const vtkRInterfaceSpatial&);  // Not implemented.
};


#endif
