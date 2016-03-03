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
 * \brief This class provides an interface to the raster map io library to
 * read a raster map row by row.
 *
 * The user can choose the current region, the default region, the raster map region or
 * a user defined region to open the raster map.
 *
 * This class does not use the vtk update mechanism.
 * Before reading the raster map rows you need to open the raster map.
 * After reading you must close the raster map. The
 * destructor will not close the map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRasterMapReader_h
#define __vtkGRASSRasterMapReader_h

#include <vtkGRASSRasterMapBase.h>
#include "vtkGRASSBridgeRasterWin32Header.h"

class vtkGRASSRasterRow;
class vtkDataArray;
class vtkCharArray;

class VTK_GRASS_BRIDGE_RASTER_EXPORT vtkGRASSRasterMapReader : public vtkGRASSRasterMapBase
{
public:
  static  vtkGRASSRasterMapReader *New();
  vtkTypeMacro(vtkGRASSRasterMapReader,vtkGRASSRasterMapBase);

  //! \brief Open an existing raster map for reading.
  //! \param name the name of an existing raster map
  //! \return true for success, false fo error
  virtual bool OpenMap(const char *name);
  
  //! \brief Return the min and max range as double for all map types.
  //! Call only after the raster map was opened.
  //! \return true for success, false for error
  virtual bool GetRange(double range[2]);

  //! \brief Read a row of the map at position index
  //!
  //! This method reads the raster data directly into the vtkGRASSRasterRow and
  //! is much faster than the vtkDataArray approach. The NullValue setting is ignored
  //! using this method.
  //!
  //! \param idx the index of the raster row
  //! \param row the raster row used to read the row internally
  //! \return true on success, false otherwise
  virtual bool GetRow(int idx, vtkGRASSRasterRow *row);

  //! \brief Read a row of the map at position idx and return a vtkDataArray
  //! the returned array is of type vtkIntArray in case of map type CELL,
  //! of type vtkFloatArray in case of map type FCELL and of type vtkDoubleArray
  //! in case of map type DCELL. If the NullValue is set and activated it will
  //! be used to convert GRASS null values into the NullValue in the vtkDataArray.
  virtual vtkDataArray *GetRow(int idx);

  //! \brief Read the null row of the map at position idx
  //! 1 == null value, 0 != null value
  virtual vtkCharArray *GetNullRow(int idx);

protected:
  vtkGRASSRasterMapReader(){};
  ~vtkGRASSRasterMapReader(){};

private:

    vtkGRASSRasterMapReader(const vtkGRASSRasterMapReader&);  // Not implemented.
  void operator=(const vtkGRASSRasterMapReader&);  // Not implemented.
};


#endif
