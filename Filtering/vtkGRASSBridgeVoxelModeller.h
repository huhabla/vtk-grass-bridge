/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkGRASSBridgeVoxelModeller.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
     

  Modified for better polydata support by Soeren Gebbert
  soerengebbert (at) googlemail (dot) com

=========================================================================*/
//!\class vtkGRASSBridgeVoxelModeller
//!\brief  convert an arbitrary dataset to a voxel representation
//
//! vtkGRASSBridgeVoxelModeller is a filter that converts an arbitrary data set to a
//! structured point (i.e., voxel) representation.


#ifndef __vtkGRASSBridgeVoxelModeller_h
#define __vtkGRASSBridgeVoxelModeller_h

#include <vtkImageAlgorithm.h>
#include "vtkGRASSBridgeFilteringWin32Header.h"

class VTK_GRASS_BRIDGE_FILTERING_EXPORT vtkGRASSBridgeVoxelModeller : public vtkImageAlgorithm
{
public:
  vtkTypeMacro(vtkGRASSBridgeVoxelModeller,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //!\brief  Construct an instance of vtkGRASSBridgeVoxelModeller with its sample dimensions
  //! set to (50,50,50), and so that the model bounds are
  //! automatically computed from its input. The maximum distance is set to 
  //! examine the whole grid. This could be made much faster, and probably
  //! will be in the future.
  static vtkGRASSBridgeVoxelModeller *New();

  //!\brief  Compute the ModelBounds based on the input geometry.
  double ComputeModelBounds(double origin[3], double ar[3]);

  //!\brief  Set the i-j-k dimensions on which to sample the distance function.
  void SetSampleDimensions(int i, int j, int k);

  //!\brief  Set the i-j-k dimensions on which to sample the distance function.
  void SetSampleDimensions(int dim[3]);
  vtkGetVectorMacro(SampleDimensions,int,3);

  //!\brief  Specify distance away from surface of input geometry to sample. Smaller
  //! values make large increases in performance.
  vtkSetClampMacro(MaximumDistance,double,0.0,1.0);
  vtkGetMacro(MaximumDistance,double);

  //!\brief  Specify the position in space to perform the voxelization.
  void SetModelBounds(double bounds[6]);
  void SetModelBounds(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
  vtkGetVectorMacro(ModelBounds,double,6);

protected:
  vtkGRASSBridgeVoxelModeller();
  ~vtkGRASSBridgeVoxelModeller() {};

  
  virtual int RequestInformation (vtkInformation *, 
                                  vtkInformationVector **, 
                                  vtkInformationVector *);

  // see vtkAlgorithm for details
  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector** inputVector,
                          vtkInformationVector* outputVector);

  // see algorithm for more info
  virtual int FillInputPortInformation(int port, vtkInformation* info);

  int SampleDimensions[3];
  double MaximumDistance;
  double ModelBounds[6];
private:
  vtkGRASSBridgeVoxelModeller(const vtkGRASSBridgeVoxelModeller&);  // Not implemented.
  void operator=(const vtkGRASSBridgeVoxelModeller&);  // Not implemented.
};

#endif
