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
 *\class vtkTemporalDataSetSource 
 * \brief This class is designed to act as a source for vtkTemporalDataSet's.
 *
 * This class creates a vtkTemporalDataSet based on vtkDataSets which must be added
 * using the SetInput or SetInputConnection method and a time steps array of type 
 * vtkDoubleArray containing the time steps for each input. The number of inputs ports 
 * is computed based on the number of entries in the time steps array. The port id of
 * an input data set is used as id of the corresponding time step in the time steps array.
 * Additionally a time range must be provided which can be different from the range
 * of the time steps array. The time values of the time steps array must be within this range.
 *
 * The range and time steps array is used to generate the TIME_RANGE and TIME_STEPS information
 * of the generated output vtkTemporalDataSet. 
 *
 * This source only provides data at diskrete time steps.
 * 
 */  


#ifndef __vtkTemporalDataSetSource_h
#define __vtkTemporalDataSetSource_h

#include <vtkTemporalDataSetAlgorithm.h>
#include <vtkDoubleArray.h>
#include "vtkGRASSBridgeTemporalWin32Header.h"

class VTK_GRASS_BRIDGE_TEMPORAL_EXPORT vtkTemporalDataSetSource : public vtkTemporalDataSetAlgorithm
{
public:
  vtkTypeMacro(vtkTemporalDataSetSource,vtkTemporalDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);
  static vtkTemporalDataSetSource *New();

  //!\brief Set the time range and the time steps for each input connection. The id of
  //! the time step in the double array is related to the input port of the input data set
  //! IMPORTANT! This Method must be set before any SetInput method is called, because
  //! the number of input ports is computed based on these values.
  virtual bool SetTimeRange(double min, double max, vtkDoubleArray *timeSteps);
  //!\brief Get the time range [min, max]
  vtkGetVectorMacro(TimeRange, double, 2);
  //!\brief Get the time steps double array
  vtkGetObjectMacro(TimeSteps, vtkDoubleArray);
  //!\brief Get the number of diskrete time steps
  vtkGetMacro(DiscreteTimeSteps, int);
  
protected:
  vtkTemporalDataSetSource();
  ~vtkTemporalDataSetSource();
  
  vtkSetMacro(DiscreteTimeSteps, int);
  
  //!\brief Populate the time range and time steps in pipeline 
  virtual int RequestInformation (vtkInformation *, 
                                  vtkInformationVector **, 
                                  vtkInformationVector *);

  //!\brief Combine the inputs and the time steps to a vtkTemporalDataSet
  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector** inputVector,
                          vtkInformationVector* outputVector);

  //!\brief We accept only vtkDataSets as inputs
  virtual int FillInputPortInformation(int port, vtkInformation* info);

  double TimeRange[2];
  vtkDoubleArray *TimeSteps;
  int DiscreteTimeSteps;
  
private:
  vtkTemporalDataSetSource(const vtkTemporalDataSetSource&);  // Not implemented.
  void operator=(const vtkTemporalDataSetSource&);  // Not implemented.
};

#endif
