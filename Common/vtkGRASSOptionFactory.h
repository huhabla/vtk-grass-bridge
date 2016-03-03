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
 * \brief This class should be used to create vtkGRASSOption objects of 
 * standard types. 
 * 
 * Many grass standard options are available:
 * vector, raster, raster3d, file inputs and outputs as well as database
 * column names and where statements. The creation methods are static and can be
 * used without creating an object. Python example of standard input option using different
 * key and description:
 * \verbatim
 * option =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputType(), "topo", "The topographic index")
 * \endverbatim
 * 
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2010
 * \author soerengebbert@googlemail.com
 *
 *
 * \TODO
 *
 * */

#ifndef __vtkGRASSOptionFactory_h
#define __vtkGRASSOptionFactory_h

#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSBridgeCommonWin32Header.h"

extern "C" {
#include <grass/gis.h>
}

class vtkGRASSOption;

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSOptionFactory : public vtkObjectGRASSErrorHandler
{
public:
  static  vtkGRASSOptionFactory *New();
  vtkTypeMacro(vtkGRASSOptionFactory,vtkObjectGRASSErrorHandler);

  static vtkGRASSOption* CreateInstance(int OptionType);
  static vtkGRASSOption* CreateInstance(int OptionType, const char* key);
  static vtkGRASSOption* CreateInstance(int OptionType, const char *key, const char *description);

  //!\brief Standard option for one raster input map
  static int GetRasterInputType(){return G_OPT_R_INPUT;}
  //!\brief Standard option for several raster input maps
  static int GetRasterInputsType(){return G_OPT_R_INPUTS;}
  //!\brief Standard option for a raster output map
  static int GetRasterOutputType(){return G_OPT_R_OUTPUT;}
  //!\brief Standard option for one vector input map
  static int GetVectorInputType(){return G_OPT_V_INPUT;}
  //!\brief Standard option for several vector input maps
  static int GetVectorInputsType(){return G_OPT_V_INPUTS;}
  //!\brief Standard option for a vector output map
  static int GetVectorOutputType(){return G_OPT_V_OUTPUT;}
  //!\brief Standard option for feature type of a vector map
  static int GetVectorFeatureType(){return G_OPT_V_TYPE;}
  //!\brief Standard option for the field index of a vector map
  static int GetVectorFieldType(){return G_OPT_V_FIELD;}
  //!\brief Standard option for one raster3d input map
  static int GetRaster3dInputType(){return G_OPT_R3_INPUT;}
  //!\brief Standard option for several raster3d input maps
  static int GetRaster3dInputsType(){return G_OPT_R3_INPUTS;}
  //!\brief Standard option for a raster3d output map
  static int GetRaster3dOutputType(){return G_OPT_R3_OUTPUT;}
  //!\brief Standard option for a databse where condition
  static int GetDataBaseWhereType(){return G_OPT_DB_WHERE;}
  //!\brief Standard option for the column name of a database table
  static int GetDataBaseColumnType(){return G_OPT_DB_COLUMN;}
  //!\brief Standard option for several column names of a database table
  static int GetDataBaseColumnsType(){return G_OPT_DB_COLUMNS;}
  //!\brief Standard option for one input file
  static int GetFileInputType(){return G_OPT_F_INPUT;}
  //!\brief Standard option for a file output
  static int GetFileOutputType(){return G_OPT_F_OUTPUT;}
  //!\brief Standard option for temporal sample type
  static int GetTemporalSampleType(){return G_OPT_T_SAMPLE;}
  //!\brief Standard option for the temporal type
  static int GetTemporalTypeType(){return G_OPT_T_TYPE;}
  //!\brief Standard option for the temporal where statement
  static int GetTemporalWhereype(){return G_OPT_T_WHERE;}
  //!\brief Standard option for space time dataset input
  static int GetSTDSInputType(){return G_OPT_STDS_INPUT;}
  //!\brief Standard option for space time dataset inputs
  static int GetSTDSInputsType(){return G_OPT_STDS_INPUTS;}
  //!\brief Standard option for space time raster dataset input
  static int GetSTRDSInputType(){return G_OPT_STRDS_INPUT;}
  //!\brief Standard option for space time raster dataset inputs
  static int GetSTRDSInputsType(){return G_OPT_STRDS_INPUTS;}
  //!\brief Standard option for space time raster dataset output
  static int GetSTRDSOutputType(){return G_OPT_STRDS_OUTPUT;}
  //!\brief Standard option for space time vector dataset input
  static int GetSTVDSInputType(){return G_OPT_STVDS_INPUT;}
  //!\brief Standard option for space time vector dataset inputs
  static int GetSTVDSInputsType(){return G_OPT_STVDS_INPUTS;}
  //!\brief Standard option for space time vector dataset output
  static int GetSTVDSOutputType(){return G_OPT_STVDS_OUTPUT;}

  protected:
  vtkGRASSOptionFactory();
  ~vtkGRASSOptionFactory() {};
  
private:
  vtkGRASSOptionFactory(const vtkGRASSOptionFactory&);  // Not implemented.
  void operator=(const vtkGRASSOptionFactory&);  // Not implemented.
};

#endif

