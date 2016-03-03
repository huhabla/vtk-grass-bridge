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
 * \brief This class reads and writes region information from
 * and to the grass database
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRegion_h
#define __vtkGRASSRegion_h

#include "vtkGRASSBridgeCommonWin32Header.h"
#include "vtkObjectGRASSErrorHandler.h"

extern "C" {
#include <grass/gis.h>
}

class VTK_GRASS_BRIDGE_COMMON_EXPORT vtkGRASSRegion : public vtkObjectGRASSErrorHandler
{
public:
  static vtkGRASSRegion *New();
  vtkTypeMacro(vtkGRASSRegion,vtkObjectGRASSErrorHandler);
  void PrintSelf(ostream& os, vtkIndent indent);
  void DeepCopy(vtkGRASSRegion *region);
  
  vtkGetMacro(BytesPerCell, int);
  vtkGetMacro(CompressionFlag, int);
  vtkGetMacro(Rows, int);
  vtkGetMacro(Cols, int);
  vtkGetMacro(Rows3d, int);
  vtkGetMacro(Cols3d, int);
  vtkGetMacro(Depths, int);
  vtkGetMacro(Top, int);
  vtkGetMacro(Bottom, int);
  vtkGetMacro(Projection, int);
  vtkGetMacro(Zone, int);
  vtkGetMacro(EastWestResolution, double);
  vtkGetMacro(NorthSouthResolution, double);
  vtkGetMacro(EastWestResolution3d, double);
  vtkGetMacro(NorthSouthResolution3d, double);
  vtkGetMacro(TopBottomResolution, double);
  vtkGetMacro(NorthernEdge, double);
  vtkGetMacro(SouthernEdge, double);
  vtkGetMacro(EasternEdge, double);
  vtkGetMacro(WesternEdge, double);

  vtkSetMacro(BytesPerCell, int);
  vtkSetMacro(CompressionFlag, int);
  vtkSetMacro(Rows, int);
  vtkSetMacro(Cols, int);
  vtkSetMacro(Rows3d, int);
  vtkSetMacro(Cols3d, int);
  vtkSetMacro(Depths, int);
  vtkSetMacro(Top, int);
  vtkSetMacro(Bottom, int);
  vtkSetMacro(Projection, int);
  vtkSetMacro(Zone, int);
  vtkSetMacro(EastWestResolution, double);
  vtkSetMacro(NorthSouthResolution, double);
  vtkSetMacro(EastWestResolution3d, double);
  vtkSetMacro(NorthSouthResolution3d, double);
  vtkSetMacro(TopBottomResolution, double);
  vtkSetMacro(NorthernEdge, double);
  vtkSetMacro(SouthernEdge, double);
  vtkSetMacro(EasternEdge, double);
  vtkSetMacro(WesternEdge, double);

  //! \brief Sets the current grass region with the internal data
  //! \return ture for succsess, false for error
  bool SetCurrentRegion();
  //! \brief Reads the current grass region
  //! \return ture for succsess, false for error
  bool ReadCurrentRegion();
  //! \brief Reads the Default grass region
  //! \return true for success, false for error
  bool ReadDefaultRegion();
  //! \brief Reads a grass region from file
  //! \return true for success, false for error
  bool ReadRegion(char *regionName);
  //! \brief Save the vtkGRASSRegion as a grass region file
  //! \param regionName the name of the region
  //! \return true for success, false for error
  bool SaveRegion(char *regionName);
  //! \brief Save the vtkGRASSRegion as a default grass region
  //! \return true for success, false for error
  bool SaveRegionAsDefault();
  //! \brief Adjust the region resolution using the rows and cols settings
  //! \return true for success, false for error
  bool AdjustRegionResolution();
  //! \brief Adjust the rows and cols the from region resolution
  //! \return true for success, false for error
  bool AdjustRegionFromResolution();
  //! \brief Adjust the region 3d resolution using the depth, rows and cols
  //! settings
  //! \return true for success, false for error
  bool AdjustRegion3dResolution();
  //! \brief Adjust the rows, cols and depths the from region resolution
  //! \return true for success, false for error
  bool AdjustRegion3dFromResolution();

  vtkGetStringMacro(Name);

  //BTX
  //! \brief Copy grass struct Cell_head information into the member variables.
  bool CopyRegionFrom(struct Cell_head *head);
  //! \brief Copy member variables into a grass Cell_head struct.
  bool CopyRegionTo(struct Cell_head *head);
  //! \brief Return the pointer to the internal struct
  struct Cell_head *GetPointer(){return &this->head;}
  //ETX

protected:
  vtkGRASSRegion();
  ~vtkGRASSRegion();

  vtkSetStringMacro(Name);

  char *Name;

  int BytesPerCell;
  int CompressionFlag;
  int Rows;
  int Cols;
  int Rows3d;
  int Cols3d;
  int Top;
  int Bottom;
  int Depths;
  int Projection;
  int Zone;
  double EastWestResolution3d;
  double NorthSouthResolution3d;
  double TopBottomResolution;
  double EastWestResolution;
  double NorthSouthResolution;
  double NorthernEdge;
  double SouthernEdge;
  double EasternEdge;
  double WesternEdge;

  //BTX
  struct Cell_head head;
  //ETX


private:
  vtkGRASSRegion(const vtkGRASSRegion&);  // Not implemented.
  void operator=(const vtkGRASSRegion&);  // Not implemented.
};


#endif
