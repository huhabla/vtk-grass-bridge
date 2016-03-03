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
 * \brief This class reads multiple vector maps with feature types point or
 * centroid and creates a vtkPolyData dataset.
 *
 * The vector maps may represent the soil properties at different depths. Hence
 * they must be identical with the exception of the attribute table values.
 * The number of feature,the feature types, their location,
 * the categories and the column names must be identical.
 *
 * The z-coordinates of the points/centroids are ignored.
 *
 * The point/centroids of the input vector
 * maps will be  connected via lines in vertical direction.
 * The distance between the points must be provided as double array
 * and must be equal to the number of vector maps that are defined.
 *
 * The categorical data of the vector maps are assigned to the created lines,
 * hence each vector feature is represented as a vertical line and all vector
 * attributes are assigned to the line.
 *
 * In addition to the vector attribute column names that must be specified
 * alternative names can be provided. The vector column names are replaced
 * by the alternative names in the cell data arrays.
 * That is needed in case filter expect a static name for arrays to process.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2012
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSMultiVectorPolyDataLineReader_h
#define __vtkGRASSMultiVectorPolyDataLineReader_h

#include "vtkGRASSVectorPolyDataReader.h"
#include "vtkGRASSBridgeIOWin32Header.h"
#include <vtkStringArray.h>
#include <vtkDoubleArray.h>

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSMultiVectorPolyDataLineReader : public vtkGRASSVectorPolyDataReader
{
public:
  static vtkGRASSMultiVectorPolyDataLineReader* New();
  vtkTypeMacro(vtkGRASSMultiVectorPolyDataLineReader,vtkGRASSVectorPolyDataReader);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //!\brief The output will only contain vertices
  virtual void SetFeatureTypeToPoint(){this->SetFeatureType(GV_POINT);}
  //!\brief The output will only contain vertices
  virtual void SetFeatureTypeToCentroid(){this->SetFeatureType(GV_CENTROID);}

  //!\brief Get the array names that replace the column names in the cell data
  vtkGetObjectMacro(NewArrayNames, vtkStringArray);
  //!\brief Set the array names that replace the column names in the cell data
  vtkSetObjectMacro(NewArrayNames, vtkStringArray);

  //!\brief Get names of the vector maps as string array
  vtkGetObjectMacro(VectorNames, vtkStringArray);
  //!\brief Set names of the vector maps as string array
  vtkSetObjectMacro(VectorNames, vtkStringArray);

  //!\brief Get vertical line lengths for each vector map as double array
  vtkGetObjectMacro(LineLengths, vtkDoubleArray);
  //!\brief Set vertical line lengths for each vector map as double array
  vtkSetObjectMacro(LineLengths, vtkDoubleArray);

protected:
  vtkGRASSMultiVectorPolyDataLineReader();
  ~vtkGRASSMultiVectorPolyDataLineReader();

  vtkSetMacro(FeatureType, int);
  int FeatureType;

  vtkStringArray *NewArrayNames;
  vtkStringArray *VectorNames;
  vtkDoubleArray *LineLengths;

  virtual int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);

private:
  vtkGRASSMultiVectorPolyDataLineReader(const vtkGRASSMultiVectorPolyDataLineReader&);  // Not implemented.
  void operator=(const vtkGRASSMultiVectorPolyDataLineReader&);  // Not implemented.
};

#endif
