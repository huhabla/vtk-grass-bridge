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
 * \brief This class represents a single raster row of GRASS GIS raster maps
 *
 * Methods for allocation, deallocation and value access are provided.
 * 
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRasterMemoryMap_h
#define __vtkGRASSRasterMemoryMap_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C"{
#include <grass/gis.h>
#include <grass/raster.h>
}

class vtkDCELL;
class vtkFCELL;
class vtkCELL;
class vtkGRASSRasterRow;

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSRasterMemoryMap : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSRasterMemoryMap *New();
  vtkTypeMacro(vtkGRASSRasterMemoryMap, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  //!\brief Allocate a raster memory map of type DCELL, FCELL or CELL,
  //!removes previously allocated data
  //!\param rows The number of rows to allocate
  //!\param cols The number of cols to allocate
  bool Allocate(int rows, int cols, int rowtype);
  //!\brief Allocate a raster memory map of type DCELL,
  //!removes previously allocated data
  //!\param rows The number of rows to allocate
  //!\param cols The number of cols to allocate
  bool AllocateDCELL(int rows, int cols);
  //!\brief Allocate a raster memory map of type FCELL,
  //!removes previously allocated data
  //!\param rows The number of rows to allocate
  //!\param cols The number of cols to allocate
  bool AllocateFCELL(int rows, int cols);
  //!\brief Allocate a raster memory map of type CELL,
  //!removes previously allocated data
  //!\param rows The number of rows to allocate
  //!\param cols The number of cols to allocate
  bool AllocateCELL(int rows, int cols);
  //!\brief Check if the memory map was allocated
  bool IsAllocated(){return this->Allocated;}

  //!\brief Get the DCELL value of the memory map at position (row,col)
  //!\param row: The row index
  //!\param col: The column index
  //!\\param value: The value
  bool GetDCELLValue(int row, int col, vtkDCELL *value);
  //!\brief Get the FCELL value of the memory map at position (row,col)
  //!\param row: The row index
  //!\param col: The column index
  //!\\param value: The value
  bool GetFCELLValue(int row, int col, vtkFCELL *value);
  //!\brief Get the CELL value of the memory map at position (row,col)
  //!\param row: The row index
  //!\param col: The column index
  //!\\param value: The value
  bool GetCELLValue(int row, int col, vtkCELL *value);

  //!\brief Set the DCELL value of the memory map at position (row,col)
  //!\param row: The row index
  //!\param col: The column index
  //!\\param value: The value
  bool SetDCELLValue(int row, int col, vtkDCELL *value);
  //!\brief Set the FCELL value of the memory map at position (row,col)
  //!\param row: The row index
  //!\param col: The column index
  //!\\param value: The value
  bool SetFCELLValue(int row, int col, vtkFCELL *value);
  //!\brief Set the CELL value of the memory map at position (row,col)
  //!\param row: The row index
  //!\param col: The column index
  //!\\param value: The value
  bool SetCELLValue(int row, int col, vtkCELL *value);

  //!\brief Get a specific row buffer
  //!\param row: The row index
  //!\param buff: The raster row buffer to use
  bool GetRow(int row, vtkGRASSRasterRow *buff);

  //!\brief Check if column is of type Null
  //!\param row: The row index
  //!\param col: The column index
  bool IsNull(int row, int col);
  //!\brief Set the memory map at position (row,col) to Null
  //!\param row: The row index
  //!\param col: The column index
  bool SetToNull(int row, int col);
  //!\brief Set the raster memory map to Null
  bool SetToNull();

  //!\brief Reset the raster memory map and free internal memory
  void Reset();
  
  vtkGetMacro(NumberOfCols, int);
  vtkGetMacro(NumberOfRows, int);
  vtkGetMacro(MapType, int);

  //BTX
  //!\brief Return the active buffer or NULL if no buffer was allocated
  void * GetBuffer();
  //ETX

protected:
  vtkGRASSRasterMemoryMap();
  ~vtkGRASSRasterMemoryMap();

  bool Allocated;
  int MapType;
  int NumberOfCols;
  int NumberOfRows;
  CELL *CELLBuff;
  FCELL *FCELLBuff;
  DCELL *DCELLBuff;

private:
  vtkGRASSRasterMemoryMap(const vtkGRASSRasterMemoryMap&);  // Not implemented.
  void operator=(const vtkGRASSRasterMemoryMap&);  // Not implemented.
};

#endif
