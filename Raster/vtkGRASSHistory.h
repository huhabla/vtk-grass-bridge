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
 * \brief This class creates, reads and writes the History file of a raster map
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSHistory_h
#define __vtkGRASSHistory_h

#include "vtkGRASSBridgeRasterWin32Header.h"
#include "vtkObjectGRASSErrorHandler.h"
#include "vtkGRASSDefines.h"

extern "C" {
#include <grass/gis.h>
#include <grass/raster3d.h>
#include <grass/raster.h>
}

class VTK_GRASS_BRIDGE_RASTER_EXPORT vtkGRASSHistory : public vtkObjectGRASSErrorHandler
{
public:
  static vtkGRASSHistory *New();
  vtkTypeMacro(vtkGRASSHistory,vtkObjectGRASSErrorHandler);
  void PrintSelf(ostream& os, vtkIndent indent);

  //! \brief Read the history of a raster map
  virtual bool ReadHistory(const char *rastername);
  //! \brief Write the history of a raster map
  virtual bool WriteHistory(const char *rastername);

  //! \brief Read the history of a 3d raster map
  virtual bool Read3dHistory(const char *rastername);
  //! \brief Write the history of a 3d raster map
  virtual bool Write3dHistory(const char *rastername);

  vtkGetStringMacro(RasterName);
  virtual const char *GetMapId(){return Rast_get_history(this->hist, HIST_MAPID);}
  virtual const char *GetTitle(){return Rast_get_history(this->hist, HIST_TITLE);}
  virtual const char *GetMapset(){return Rast_get_history(this->hist, HIST_MAPSET);}
  virtual const char *GetCreator(){return Rast_get_history(this->hist, HIST_CREATOR);}
  virtual const char *GetMaptype(){return Rast_get_history(this->hist, HIST_MAPTYPE);}
  virtual const char *GetDataSource1(){return Rast_get_history(this->hist, HIST_DATSRC_1);}
  virtual const char *GetDataSource2(){return Rast_get_history(this->hist, HIST_DATSRC_2);}
  virtual const char *GetKeywords(){return Rast_get_history(this->hist, HIST_KEYWRD);}

  virtual void SetTitle(const char *title){TRY Rast_set_history(this->hist, HIST_TITLE, title); CATCH_VOID}
  virtual void SetCreator(const char *creator){TRY Rast_set_history(this->hist, HIST_CREATOR, creator); CATCH_VOID}
  virtual void SetMaptype(const char *maptype){TRY Rast_set_history(this->hist, HIST_MAPTYPE, maptype); CATCH_VOID}
  virtual void SetDataSource1(const char *datsrc_1){TRY Rast_set_history(this->hist, HIST_DATSRC_1, datsrc_1); CATCH_VOID}
  virtual void SetDataSource2(const char *datsrc_2){TRY Rast_set_history(this->hist, HIST_DATSRC_2, datsrc_2); CATCH_VOID}
  virtual void SetKeywords(const char *keywrd){TRY Rast_set_history(this->hist, HIST_KEYWRD, keywrd); CATCH_VOID}
  
  //! \brief Append a comment to the raster map history file.
  virtual void  AppendComment(char *comment) {TRY Rast_append_history(this->hist, comment); CATCH_VOID}

  //! \brief Return true if the history is of an 3d raster map
  virtual bool Check3d(){return this->Is3d;}

  //BTX
  virtual struct History* GetPointer(){return this->hist;}
  //ETX

protected:
  vtkGRASSHistory();
  ~vtkGRASSHistory();

  vtkSetStringMacro(RasterName);
  vtkSetStringMacro(Mapset);

  char *RasterName;
  char *Mapset;
  bool Is3d;

  //BTX
  struct History *hist;
  //ETX

private:
  vtkGRASSHistory(const vtkGRASSHistory&);  // Not implemented.
  void operator=(const vtkGRASSHistory&);  // Not implemented.
};


#endif
