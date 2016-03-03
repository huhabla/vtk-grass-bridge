/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2012 by Soeren Gebbert, soerengebbert@googlemail.com
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
 * \brief This class reads multiple raster maps and creates a vtkPolyData
 * dataset that represents the non-Null cells of the raster maps as vertical
 * lines with a user defined length.
 *
 * The raster maps may represent the soil properties at different depths. Hence
 * maps must be provided in increasing or decreasing order.
 *
 * The cells of the input raster maps
 * will be  connected via lines in vertical direction.
 * The distance between the points must be provided as double array
 * and must be equal to the number of raster maps that are defined.
 *
 * The cell value data of the raster maps are assigned to the created lines,
 * hence each raster cell is represented as a vertical line and the cell
 * value is assigned to the line.
 *
 * The raster names will be replaced in the resulting datasets
 * by an alternative user defined name.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2012
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSMultiRasterPolyDataLineReader_h
#define __vtkGRASSMultiRasterPolyDataLineReader_h

#ifndef NULL
#define NULL 0
#endif

#include <vtkStringArray.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataAlgorithm.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSRasterMapReader.h"
#include "vtkGRASSRegion.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSMultiRasterPolyDataLineReader : public vtkPolyDataAlgorithm
{
public:
  static vtkGRASSMultiRasterPolyDataLineReader* New();
  vtkTypeMacro(vtkGRASSMultiRasterPolyDataLineReader,vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //!\brief Get the array name that replace the raster names in the cell data
  vtkGetStringMacro(DataName);
  //!\brief Set the array name that replace the raster names in the cell data
  vtkSetStringMacro(DataName);

  //!\brief Get names of the vector maps as string array
  vtkGetObjectMacro(RasterNames, vtkStringArray);
  //!\brief Set names of the vector maps as string array
  vtkSetObjectMacro(RasterNames, vtkStringArray);

  //!\brief Get vertical line lengths for each vector map as double array
  vtkGetObjectMacro(LineLengths, vtkDoubleArray);
  //!\brief Set vertical line lengths for each vector map as double array
  vtkSetObjectMacro(LineLengths, vtkDoubleArray);

  /*! \brief Set the region which should be used to open the raster map
   *
   * */
  void SetRegion(vtkGRASSRegion *region) {this->RasterMap->SetRegion(region);}
  vtkGRASSRegion *GetRegion() {return this->RasterMap->GetRegion();}

  /*! \brief Return the RasterMap object
   *
   * */
  vtkGetObjectMacro(RasterMap, vtkGRASSRasterMapReader);

protected:
  vtkGRASSMultiRasterPolyDataLineReader();
  ~vtkGRASSMultiRasterPolyDataLineReader();

  char *DataName;
  vtkStringArray *RasterNames;
  vtkDoubleArray *LineLengths;
  vtkGRASSRasterMapReader *RasterMap;

  virtual int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);

private:
  vtkGRASSMultiRasterPolyDataLineReader(const vtkGRASSMultiRasterPolyDataLineReader&);  // Not implemented.
  void operator=(const vtkGRASSMultiRasterPolyDataLineReader&);  // Not implemented.
};

#endif
