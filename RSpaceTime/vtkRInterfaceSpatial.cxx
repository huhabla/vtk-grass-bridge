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

#include "vtkRInterfaceSpatial.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"
#include "vtkGRASSVectorPolyDataWriter.h"
#include "vtkGRASSVectorPolyDataReader.h"
#include <vtkStdString.h>
#include <vtkPointSet.h>
#include <vtkPointData.h>
#include <vtkCellDataToPointData.h>
#include <sstream>
#include <vtkFieldData.h>
#include <vtkImageData.h>
#include <vtkStringArray.h>
#include <vtkDataSetAttributes.h>

vtkStandardNewMacro(vtkRInterfaceSpatial);

//----------------------------------------------------------------------------

vtkRInterfaceSpatial::vtkRInterfaceSpatial()
{
  // The sp package is needed
  this->EvalRscript("require(\"sp\")", true);
  this->Proj4String = NULL;
}

//----------------------------------------------------------------------------

vtkStringArray *vtkRInterfaceSpatial::CreateArrayNamesArray(vtkDataSetAttributes *data)
{
  vtkStringArray *arrayNames = vtkStringArray::New();
  int i;

  arrayNames->Initialize();
  arrayNames->SetNumberOfValues(data->GetNumberOfArrays());

  for(i = 0; i < data->GetNumberOfArrays(); i++)
  {
    arrayNames->InsertValue(i, data->GetArray(i)->GetName());
  }

  return arrayNames; // Must be deleted in the class which calls this method
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKDataSetAttributesToRDataFrame(vtkDataSetAttributes *data, const char *dataframe, vtkStringArray *arrays)
{
  std::ostringstream arrayNames;
  std::ostringstream script;
  int i;

  // Assign the point data arrays as R variables
  for(i = 0; i < arrays->GetNumberOfValues(); i++)
  {
    // Check if array is present in input
    if(data->HasArray(arrays->GetValue(i)) == 0) {
      vtkErrorMacro(<< "Unable to find array " << arrays->GetValue(i) << " in data");
      return false;
    }

    vtkDataArray *array = data->GetArray(arrays->GetValue(i));

    if(i == 0)
      arrayNames << array->GetName();
    else
      arrayNames << "," << array->GetName();

    this->AssignVTKDataArrayToRVariable(array, array->GetName());
  }

  // We need to create a data frame from all the attached arrays
  script << dataframe << " = data.frame(" << arrayNames.str().c_str() << ")";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);


  // Remove unneeded data
  script.str(""); // Clear the script
  script << "remove(" << arrayNames.str().c_str() << ")";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  return true;
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKDataSetAttributesToRDataFrame(vtkDataSetAttributes *data, const char *dataframe)
{
  vtkStringArray *arrayNames = this->CreateArrayNamesArray(data);

  return this->AssignVTKDataSetAttributesToRDataFrame(data, dataframe, arrayNames);
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKPointSetToRSpatialPointsDataFrame(vtkPointSet *data, const char *name)
{
  vtkStringArray *arrayNames = this->CreateArrayNamesArray(data->GetPointData());

  return this->AssignVTKPointSetToRSpatialPointsDataFrame(data, name, arrayNames);
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKPointSetToRSpatialPoints(vtkPointSet *data, const char *name)
{
  return this->AssignVTKPointSetToRSpatialPointsDataFrame(data, name, NULL);
}


//----------------------------------------------------------------------------
// We convert points and assigned point data into a R
// SpatialPointsDataFrame from the sp package
// Use the vtlCellDataToPointData to convert cell data into point data
//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKPointSetToRSpatialPointsDataFrame(vtkPointSet *data, const char *name, vtkStringArray *arrayNames)
{
  std::ostringstream script;

  vtkDataArray *orig_coords = data->GetPoints()->GetData();

  // Attach the point coordiantes
  this->AssignVTKDataArrayToRVariable(orig_coords, orig_coords->GetName());

  // Create a dataframe if point data arrays are available
  if(data->GetPointData()->GetNumberOfArrays() > 0 && arrayNames != NULL && arrayNames->GetNumberOfValues() > 0)
  {
    if(this->AssignVTKDataSetAttributesToRDataFrame(data->GetPointData(), "MyTempDataFrame", arrayNames) != true)
      return false;

    // Create the SpatialPointsDataFrame
    script << name << " = SpatialPointsDataFrame(" <<orig_coords->GetName()  //<< "[,-3]" // For 2d data only
           << ", MyTempDataFrame)";
  } else {

    // Create the SpatialPoints
    script << name << " = SpatialPoints(" << orig_coords->GetName() << ")";
  }

  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  this->AttachProjection(name);

  if(arrayNames)
    arrayNames->Delete();

  // Remove unneeded data
  script.str(""); // Clear the script
  if(arrayNames)
    script << "remove(" << orig_coords->GetName() <<  ",MyTempDataFrame)";
  else
    script << "remove(" << orig_coords->GetName() <<  ")";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  return true;
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKImageDataToRSpatialGridDataFrame(vtkImageData *data, const char *name)
{
  vtkStringArray *arrayNames = this->CreateArrayNamesArray(data->GetPointData());

  return this->AssignVTKImageDataToRSpatialGridDataFrame(data, name, arrayNames);
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKImageDataToRSpatialGrid(vtkImageData *data, const char *name)
{
  return this->AssignVTKImageDataToRSpatialGridDataFrame(data, name, NULL);
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpatial::AssignVTKImageDataToRSpatialGridDataFrame(vtkImageData *data, const char *name, vtkStringArray *arrayNames)
{
  std::ostringstream script;
  int dim[3] = {0.0, 0.0, 0.0};
  double origin[3] = {0.0, 0.0, 0.0};
  double spacing[3] = {0.0, 0.0, 0.0};

  // Get the size of the image
  data->GetDimensions(dim);
  data->GetOrigin(origin);
  data->GetSpacing(spacing);

  // Create the GridTopology
  script << "MyTempGrid = GridTopology(cellcentre.offset = c(" << origin[0] + spacing[0]/2.0 << "," << origin[1] + spacing[1]/ 2.0  << ")";
  script << ", cellsize = c(" << spacing[0] << "," << spacing[1] << ")";
  script << ", cells.dim = c(" << dim[0] << "," << dim[1] << "))";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  // Clear the script for new input
  script.str("");

  // Create a dataframe if point data arrays are available
  if(data->GetPointData()->GetNumberOfArrays() > 0 && arrayNames != NULL && arrayNames->GetNumberOfValues() > 0)
  {
    if(this->AssignVTKDataSetAttributesToRDataFrame(data->GetPointData(), "MyTempDataFrame", arrayNames) != true)
      return false;

    // Create the SpatialGridDataFrame
    script << name << " = SpatialGridDataFrame(MyTempGrid"  << ", MyTempDataFrame)";
  } else {

    // Create the SpatialGrid
    script << name << " = SpatialGrid(MyTempGrid)";
  }

  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  this->AttachProjection(name);

  if(arrayNames)
    arrayNames->Delete();

  // Remove unneeded data
  script.str(""); // Clear the script
  if(arrayNames)
    script << "remove(MyTempDataFrame,MyTempGrid)";
  else
    script << "remove(MyTempGrid)";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  return true;
}

//----------------------------------------------------------------------------

void vtkRInterfaceSpatial::AttachProjection(const char* spatial)
{
  std::ostringstream script;

  // Set the projection if available
  if(this->Proj4String)
  {
    // Remove the new line at the end of the string
    int len = strlen(this->Proj4String);
    if( this->Proj4String[len-1] == '\n' )
      this->Proj4String[len-1] = 0;

    // Attach the projection to the spatial R object
    script << "proj4string(" << spatial << ") = \"" << this->Proj4String << "\"";
    cout << script.str() << endl;
    this->EvalRscript(script.str().c_str(), true);
  }

  return;
}
