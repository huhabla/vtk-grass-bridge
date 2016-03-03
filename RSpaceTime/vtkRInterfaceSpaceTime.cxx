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

#include "vtkRInterfaceSpaceTime.h"
#include <vtkStringArray.h>
#include <vtkObjectFactory.h>
#include "vtkGRASSDefines.h"
#include <vtkStdString.h>
#include <vtkPointSet.h>
#include <vtkPointData.h>
#include <sstream>
#include <vtkFieldData.h>
#include <vtkImageData.h>
#include <vtkStringArray.h>
#include <vtkDataSetAttributes.h>
#include <vtkTemporalDataSet.h>

vtkStandardNewMacro(vtkRInterfaceSpaceTime);

//----------------------------------------------------------------------------

vtkRInterfaceSpaceTime::vtkRInterfaceSpaceTime()
{
  // The spacetime package is needed
  this->EvalRscript("require(\"spacetime\")", true);
}

//----------------------------------------------------------------------------

bool vtkRInterfaceSpaceTime::AssignVTKTemporalDataSetToRSTFDF(vtkTemporalDataSet *temporalData, const char *RVariable, vtkDataArray *timesteps, const char *startDate)
{
  unsigned int timestep;
  bool topoCreated = false;
  unsigned int imageData = 0;
  unsigned int polyData = 0;
  unsigned int numberOfPoints = 0, numberOfCells = 0;
  int numberOfArrays = 0;
  std::ostringstream dataFrameNames;
  std::ostringstream dataFrameName;
  std::ostringstream script;

  // Create the spatial object at the first time step
  // and attach point data for each time step
  // It is assumed that the number of points and cells as
  // well as the number and names of the arrays in each time step
  // do not change.
  for (timestep = 0; timestep < temporalData->GetNumberOfTimeSteps(); timestep++) {
    vtkDataSet *data = vtkDataSet::SafeDownCast(temporalData->GetTimeStep(timestep));

    // Check if the data changes over the time
    if (timestep == 0) {
      numberOfPoints = data->GetNumberOfPoints();
      numberOfCells = data->GetNumberOfCells();
      numberOfArrays = data->GetPointData()->GetNumberOfArrays();
    } else {
      if (numberOfPoints != data->GetNumberOfPoints() ||
        numberOfCells != data->GetNumberOfCells() ||
        numberOfArrays != data->GetPointData()->GetNumberOfArrays()) {
        vtkErrorMacro( << "Data sets in temporal data set have different number of arrays, points or cells at timestep " << timestep);
        return false;
      }
    }

    // Check for specific supported VTK data sets
    if (data->IsA("vtkImageData") != 1 && data->IsA("vtkPointSet") != 1 && data->IsA("vtkPolyData")) {
      vtkWarningMacro( << "Time step " << timestep << " has an unsupported data set type, skipping");
      continue;
    }

    if (data->IsA("vtkImageData")) {
      imageData++;
      if (topoCreated == false) {
        vtkImageData *imageData = vtkImageData::SafeDownCast(data);
        this->AssignVTKImageDataToRSpatialGrid(imageData, "DataSet");
        topoCreated = true;
      }
    }

    if (data->IsA("vtkPolyData")) {
      polyData++;
      if (topoCreated == false) {
        vtkPointSet *polyData = vtkPointSet::SafeDownCast(data);
        this->AssignVTKPointSetToRSpatialPoints(polyData, "DataSet");
        topoCreated = true;
      }
    }

    // Data set types should not mixed
    if (imageData > 0 && polyData > 0) {
      vtkErrorMacro( << "Mixed data set types are unsupported");
      return false;
    }

    // Only point data is supported, use vtkCellDataToPointData
    // to for conversion of cell data
    vtkDataSetAttributes *pointData = data->GetPointData();

    dataFrameName.str("");
    dataFrameName << "dataFrame_" << timestep;

    // Create a R data frame with data for a single time step
    this->AssignVTKDataSetAttributesToRDataFrame(pointData, dataFrameName.str().c_str());

    // Safe the names for each created data frame
    if (timestep == 0)
      dataFrameNames << dataFrameName.str();
    else
      dataFrameNames << "," << dataFrameName.str();
  }

  if(topoCreated == false) {
    vtkErrorMacro("Unsupported data set type");
    return false;
  }

  // Create the time steps
  this->AssignVTKDataArrayToRVariable(timesteps, "timeSteps");

  // Create the time value vector for spacetime object
  script << "time = xts(1:" << timesteps->GetNumberOfTuples() << ", as.POSIXct(\""
         << startDate << "\") + "<< "timeSteps)";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  // Connect the data frames of each time step
  script.str("");
  script << "bind_data = rbind(" << dataFrameNames.str().c_str() << ")";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  // Create the space time full data frame
  script.str(""); // Clear the script
  script << RVariable << " = STFDF(DataSet, time, bind_data)";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  // Remove obsolete data
  script.str(""); // Clear the script
  script << "remove(DataSet, time, timeSteps, bind_data, "
         << dataFrameNames.str().c_str() << ")";
  cout << script.str() << endl;
  this->EvalRscript(script.str().c_str(), true);

  return true;
}
