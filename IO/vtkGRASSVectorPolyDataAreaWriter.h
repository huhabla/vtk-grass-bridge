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
 * \brief This class writes vtkPolyData as grass vector map into a valid location/mapset.
 * It is specially designed to write nested, overlapping and non-convex areas as grass
 * vector data, so topology is consistent.
 *
 * Two inputs are needed. The first input is vtkPolyData with vtkPolyLine or vtkLine,
 * which are written as grass vector boundaries. The
 * centroids must be specified as the second input. They will be attached to the
 * boundaries and reference the cell data.
 *
 * This class uses the vtkGRASSVectorMapWriter to write the vector map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSVectorPolyDataAreaWriter_h
#define __vtkGRASSVectorPolyDataAreaWriter_h

#include "vtkGRASSVectorPolyDataWriter.h"
#include "vtkGRASSBridgeIOWin32Header.h"

class vtkGRASSVectorMapWriter;
class vtkCellArray;
class vtkDataArray;

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSVectorPolyDataAreaWriter : public vtkGRASSVectorPolyDataWriter
{
public:
  static vtkGRASSVectorPolyDataAreaWriter* New();
  vtkTypeMacro(vtkGRASSVectorPolyDataAreaWriter,vtkGRASSVectorPolyDataWriter);
  void PrintSelf(ostream& os, vtkIndent indent);

    //!\brief Set the boundaries
  virtual void SetBoundaries(vtkDataObject *in) { this->SetInputData(0,in); }

    //!\brief Set the centroids
  virtual void SetCentroids(vtkDataObject *in) { this->SetInputData(1,in); }

protected:
  vtkGRASSVectorPolyDataAreaWriter();
  ~vtkGRASSVectorPolyDataAreaWriter();

  int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);
private:
  vtkGRASSVectorPolyDataAreaWriter(const vtkGRASSVectorPolyDataAreaWriter&);  // Not implemented.
  void operator=(const vtkGRASSVectorPolyDataAreaWriter&);  // Not implemented.
};

#endif
