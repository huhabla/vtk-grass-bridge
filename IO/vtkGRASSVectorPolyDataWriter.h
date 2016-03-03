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
 * \brief This class writes vtkPolyData as grass vector map into a valid location/mapset
 *
 * Points (vtkVertex), lines (vtkLine, vtkPolyLine), convex areas (vtlPolygon, vtkQuad) 
 * and faces (vtkTriangle) are supported. Nested and overlapping areas are not supported.
 * Centroids are computed for each area as center point.
 *
 * This class uses the vtkGRASSVecctorMapWriter to write the vector map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSVectorPolyDataWriter_h
#define __vtkGRASSVectorPolyDataWriter_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkGRASSBridgeIOWin32Header.h"

class vtkGRASSVectorMapWriter;
class vtkCellArray;
class vtkDataArray;

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSVectorPolyDataWriter : public vtkPolyDataAlgorithm
{
public:
  static vtkGRASSVectorPolyDataWriter* New();
  vtkTypeMacro(vtkGRASSVectorPolyDataWriter,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //! \brief Set the name of vector map
  vtkSetStringMacro(VectorName);
  //! \brief Set the name of vector map
  vtkGetStringMacro(VectorName);
  //! \brief Get the name of the mapset the vector map was written to
  vtkGetStringMacro(Mapset);
  //! \brief Set the name of the category array
  vtkSetStringMacro(CategoryArrayName);
  //! \brief Get the name of the category array
  vtkGetStringMacro(CategoryArrayName);
  //! \brief Build topology of grass vector map
  vtkSetMacro(BuildTopo, int);
  //! \brief Build topology of grass vector map
  vtkGetMacro(BuildTopo, int);
  //! \brief Build topology of grass vector map
  vtkBooleanMacro(BuildTopo, int);

protected:
  vtkGRASSVectorPolyDataWriter();
  ~vtkGRASSVectorPolyDataWriter();

  char* VectorName;
  char* Mapset;
  char *CategoryArrayName;
  int BuildTopo;

  vtkSetStringMacro(Mapset);

  void AddCellDataToVectorMap(vtkCellData *celldata, vtkDataArray *categories, vtkGRASSVectorMapWriter *writer);

  int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);
private:
  vtkGRASSVectorPolyDataWriter(const vtkGRASSVectorPolyDataWriter&);  // Not implemented.
  void operator=(const vtkGRASSVectorPolyDataWriter&);  // Not implemented.
};

#endif
