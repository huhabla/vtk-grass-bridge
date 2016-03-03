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
 * \brief This module is designed to export PolyData build upon vertical lines
 * that was created by vtkGRASSMulti(Raster|Vector)PolyDataLineReader as
 * GRASS raster maps using the current region settings.
 *
 * As input a PolyData set is expected. The name of a single
 * array, the layer number and the resulting raster map name must be provided.
 *
 * Only a single array and a single layer can be exported as raster map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2012
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSMultiRasterPolyDataLineWriter_h
#define __vtkGRASSMultiRasterPolyDataLineWriter_h

#ifndef NULL
#define NULL 0
#endif

#include <vtkStringArray.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataAlgorithm.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSRasterMapWriter.h"
#include "vtkGRASSRegion.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSMultiRasterPolyDataLineWriter : public vtkPolyDataAlgorithm
{
public:
  static vtkGRASSMultiRasterPolyDataLineWriter* New();
  vtkTypeMacro(vtkGRASSMultiRasterPolyDataLineWriter,vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //!\brief Get the name of the generated raster maps
  vtkGetStringMacro(RasterMapName);
  //!\brief Set the name of the generated raster maps
  vtkSetStringMacro(RasterMapName);

  //!\brief Get the name of the array that should be written as raster map,
  //! default is the active array in case this variable was not set
  vtkGetStringMacro(ArrayName);
  //!\brief Set the name of the array that should be written as raster map
  //! default is the active array in case this variable was not set
  vtkSetStringMacro(ArrayName);

  //!\brief Get the number of the layer that should be written as raster map,
  //! default is layer 1
  vtkGetMacro(Layer, int);
  //!\brief Set the number of the layer that should be written as raster map,
  //! default is layer 1
  vtkSetMacro(Layer, int);

protected:
  vtkGRASSMultiRasterPolyDataLineWriter();
  ~vtkGRASSMultiRasterPolyDataLineWriter();

  char *RasterMapName;
  char *ArrayName;
  int Layer;

  virtual int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);

private:
  vtkGRASSMultiRasterPolyDataLineWriter(const vtkGRASSMultiRasterPolyDataLineWriter&);  // Not implemented.
  void operator=(const vtkGRASSMultiRasterPolyDataLineWriter&);  // Not implemented.
};

#endif
