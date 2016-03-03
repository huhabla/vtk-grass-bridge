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
 * \brief This class reads a grass vector map without topology support from a valid location/mapset
 * and creates a vtkPolyData as output.
 *
 * Areas and isles are not supported. The only data types which are supported are points and lines.
 * The category is saved as an integer cell data array and set to active. Points may be redundant.
 * Use a point merger to remove redundant points from the dataset.
 *
 * To use this class make sure you are in a grass session and vtkGRASSInit was called first.
 *
 * This class uses the vtkGRASSVectorMapNoTopoReader to read the vector map.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSVectorPolyDataReader_h
#define __vtkGRASSVectorPolyDataReader_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkStringArray.h"

class vtkStringArray;
class vtkDataSetAttributes;
class vtkIntArray;
class vtkGRASSVectorMapNoTopoReader;

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSVectorPolyDataReader : public vtkPolyDataAlgorithm
{
public:
  static vtkGRASSVectorPolyDataReader* New();
  vtkTypeMacro(vtkGRASSVectorPolyDataReader,vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //! \brief Set the name of vector map
  vtkSetStringMacro(VectorName);
  //! \brief Set the name of vector map
  vtkGetStringMacro(VectorName);
  //! \brief Get the name of the mapset the vector map was read from
  vtkGetStringMacro(Mapset);
  //! \brief Set the name of the category array
  vtkSetStringMacro(CategoryArrayName);
  //! \brief Get the name of the category array
  vtkGetStringMacro(CategoryArrayName);

  //!\brief Get the array of column names which should be read as celldata
  vtkGetObjectMacro(ColumnNames, vtkStringArray);
  //!\brief Set the array of column names which should be read as celldata
  vtkSetObjectMacro(ColumnNames, vtkStringArray);

  //!\brief Set the layer number (1 .. n) of the vector database connection. Default is 1
  vtkSetMacro(Layer, int);
  //!\brief Get the layer number (1 .. n) of the vector database connection. Default is 1
  vtkGetMacro(Layer, int);

  //!\brief Switch database table reading on/off
  vtkSetMacro(ReadDBTable, int);
  //!\brief Switch database table reading on/off
  vtkGetMacro(ReadDBTable, int);
  //!\briefvSwitch database table reading on/off
  vtkBooleanMacro(ReadDBTable, int);

  //!\brief Set the number which should be used if there is no valid database
  //!table entry for the readed vector feature, default is -999999
  vtkSetMacro(NoDataValue, int);
  //!\brief Get the number which should be used if there is no valid database
  //!table entry for the readed vector feature, efault is -999999
  vtkGetMacro(NoDataValue, int);

protected:
  vtkGRASSVectorPolyDataReader();
  ~vtkGRASSVectorPolyDataReader();

  char* VectorName;
  char* Mapset;
  char *CategoryArrayName;
  int Layer;
  int NoDataValue;
  int ReadDBTable;

  vtkStringArray *ColumnNames;

  vtkSetStringMacro(Mapset);

  virtual int RequestData(vtkInformation*,
                          vtkInformationVector**,
                          vtkInformationVector*);

  //!\brief Read selected double and integer database columns as celldata arrays
  //! The selection is based on column names located in the internal vtkStringArray ColumnNames
  //! In case this array is empty, all available double and integer columns are readed.
  //! The cell data arrays are named like the column names of the database table.
  virtual void ReadDatabaseData(vtkGRASSVectorMapNoTopoReader *map,
                                vtkIntArray *cats, vtkDataSetAttributes *cdata);

private:
  vtkGRASSVectorPolyDataReader(const vtkGRASSVectorPolyDataReader&);  // Not implemented.
  void operator=(const vtkGRASSVectorPolyDataReader&);  // Not implemented.
};

#endif
