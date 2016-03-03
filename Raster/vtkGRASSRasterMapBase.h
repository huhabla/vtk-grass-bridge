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
 * \brief This is the base class for grass raster map reading and writing.
 *
 * It provides function to allocate the raster buffer, to set and get the
 * vtkGRASSRegion and the vtkGRASSHistory.
 * 
 * \TODO Use the grass raster row instead of the vtkDoubleArray for row processing
 * \TODO Implement better NULL support
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 *
 * */

#ifndef __vtkGRASSRasterMapBase_h
#define __vtkGRASSRasterMapBase_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeRasterWin32Header.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSHistory.h"
#include "vtkDCELL.h"
#include "vtkFCELL.h"
#include "vtkCELL.h"

extern "C" {
#include <grass/gis.h>
#include <grass/raster.h>
}

class vtkCharArray;

class VTK_GRASS_BRIDGE_RASTER_EXPORT vtkGRASSRasterMapBase : public vtkObjectGRASSErrorHandler {
public:
    static vtkGRASSRasterMapBase *New();
    vtkTypeMacro(vtkGRASSRasterMapBase, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkGetStringMacro(RasterName);
    vtkGetStringMacro(Mapset);

    //! \brief Use the default region to open/read the raster map
    //! Region settings should not change while reading 
    virtual void UseDefaultRegion() {
        this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);
    }

    //! \brief Use the current region to open/read the raster map (default)
    //! Region settings should not change while reading 
    virtual void UseCurrentRegion() {
        this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);
    }

    //! \brief Use the raster specific region to open/read the raster map
    //! Region settings should not change while reading 
    virtual void UseRasterRegion() {
        this->SetRegionUsage(VTK_GRASS_REGION_RASTER);
    }

    //! \brief Use the user defined region to open/read the raster map
    //! The region must be set using SetRegion(vtkGRASSRegion *region)
    //! Region settings should not change while reading 
    virtual void UseUserDefinedRegion() {
        this->SetRegionUsage(VTK_GRASS_REGION_USER);
    }
    vtkGetMacro(RegionUsage, int);

    //!\brief Return the maptype as string (CELL_TYPE; FCELL_TYPE and DCELL_TYPE or NULL if unknown)
    virtual const char* GetMapTypeAsString();

    virtual bool IsMapTypeCELL() {
        return (this->MapType == CELL_TYPE ? true : false);
    }

    virtual bool IsMapTypeFCELL() {
        return (this->MapType == FCELL_TYPE ? true : false);
    }

    virtual bool IsMapTypeDCELL() {
        return (this->MapType == DCELL_TYPE ? true : false);
    }

    //! \brief Set the Region which should be used to open and read the grass raster map
    vtkSetObjectMacro(Region, vtkGRASSRegion);
    //! \brief Get the Region which should be used to open and read the grass raster map
    vtkGetObjectMacro(Region, vtkGRASSRegion);

    //! \brief Get the History of the grass raster map
    vtkGetObjectMacro(History, vtkGRASSHistory);

    //! \brief open the map for reading or writing. This function must be implemented by the subclasses.
    virtual bool OpenMap(const char *name) {
        return false;
    }

    //! \brief Close the raster map
    virtual bool CloseMap();

    /*!
     *  \brief Extract a cell value from raster map (neighbor interpolation)
     *
     *  Extract a cell value from raster map at given northing and easting
     *  with a sampled 3x3 window using a neighbor interpolation.
     *
     *  \param north northing position
     *  \param east easting position
     *  \param value pointer to a vtkDCELL value to store the result
     *
     *  \return true if found 
     */
    virtual bool GetNearestSampleValue(double north, double east, vtkDCELL *value);
    /*!
     *  \brief Extract a cell value from raster map (bilinear interpolation)
     *
     *  Extract a cell value from raster map at given northing and easting
     *  with a sampled 3x3 window using a bilinear interpolation.
     *
     *  \param north northing position
     *  \param east easting position
     *  \param value pointer to a vtkDCELL value to store the result
     *
     *  \return true if found 
     */
    virtual bool GetBilinearSampleValue(double north, double east, vtkDCELL *value);
    /*!
     *  \brief Extract a cell value from raster map (bicubic interpolation)
     *
     *  Extract a cell value from raster map at given northing and easting
     *  with a sampled 3x3 window using a bicubic interpolation.
     *
     *  \param north northing position
     *  \param east easting position
     *  \param value pointer to a vtkDCELL value to store the result
     *
     *  \return true if found 
     */
    virtual bool GetBicubicSampleValue(double north, double east, vtkDCELL *value);

    vtkGetMacro(MapType, int);
    vtkGetMacro(NumberOfRows, int);
    vtkGetMacro(NumberOfCols, int);

    //! \brief Returns true if the map is open
    //! \return true for open, false for not open

    virtual bool IsOpen() {
        return this->Open;
    }

    //! \brief Null value which should replace the default grass null value for CELL, FCELL andDCELL maps
    //! to enable the NullValue, set the this->UseNullValueOn()
    vtkSetMacro(NullValue, double);
    //! \brief Null value which should replace the default grass null value for CELL, FCELL andDCELL maps
    //! to enable the NullValue, set the this->UseNullValueOff()
    vtkGetMacro(NullValue, double);

    //! \brief Use a user defined NULL value to be stored in the vtkDataArray, otherwise a grass NULL value is used
    vtkGetMacro(UseNullValue, int);
    //! \brief Use a user defined  NULL value to be stored in the vtkDataArray, otherwise a grass NULL value is used
    vtkBooleanMacro(UseNullValue, int);

    /*!
     * \brief Column to easting.
     *
     * Converts a <i>col</i> relative to a <i>window</i> to an easting.
     *
     * <b>Note:</b> <i>col</i> is a <i>double</i>:
     *  - col+0.0 will return the easting for the western edge of the column.
     *  - col+0.5 will return the easting for the center of the column.
     *  - col+1.0 will return the easting for the eastern edge of the column.
     *
     * \param col column number
     *
     * \return east coordinate
     */
    double ColumnToEasting(double col) {
        return Rast_col_to_easting(col, this->Region->GetPointer());
    }

    /*!
     * \brief Row to northing.
     *
     * Converts a <i>row</i> relative to a <i>window</i> to a
     * northing.

     * <b>Note:</b> row is a double:
     *  - row+0.0 will return the northing for the northern edge of the row.
     *  - row+0.5 will return the northing for the center of the row.
     *  - row+1.0 will return the northing for the southern edge of the row.
     *
     * <b>Note:</b> The result is a <i>double</i>. Casting it to an
     * <i>int</i> will give the column number.
     *
     * \param row row number
     *
     * \return north coordinate
     */
    double RowToNorthing(double row) {
        return Rast_row_to_northing(row, this->Region->GetPointer());
    }

    /*!
     * \brief Easting to column.
     *
     * Converts <i>east</i> relative to a <i>window</i> to a column.

     * <b>Note:</b> The result is a <i>double</i>. Casting it to an
     * <i>int</i> will give the column number.
     *
     * \param east east coordinate
     *
     * \return column number
     */
    double EastingToColumn(double east) {
        return Rast_easting_to_col(east, this->Region->GetPointer());
    }

    /*!
     * \brief Northing to row.
     *
     * Converts a <i>north</i>ing relative to a <i>window</i> to a row.

     * <b>Note:</b> The result is a double. Casting it to an integer will
     * give the row number.
     *
     * \param north northing value
     * \param window pointer to Cell_head
     *
     * \return row number
     */
    double NorthingToRow(double north) {
        return Rast_northing_to_row(north, this->Region->GetPointer());
    }

protected:
    vtkGRASSRasterMapBase();
    ~vtkGRASSRasterMapBase();

    vtkSetMacro(UseNullValue, int);

    virtual bool GetSampleValue(double x, double y, vtkDCELL *value, int type);

    int RegionUsage;
    bool Open; // True if the raster file was opened
    int Map; // File descriptor
    int MapType; //CELL; FCELL or DCELL
    int NumberOfRows;
    int NumberOfCols;
    char *RasterName;
    char *Mapset;
    double NullValue;
    int UseNullValue;

    vtkGRASSRegion *Region;
    vtkGRASSHistory *History;
    //BTX
    //  struct Range crange;
    //  struct FPRange range;
    //  struct Categories cats;
    //  struct TimeStamp ts;
    vtkSmartPointer<vtkDataArray> Row;
    vtkSmartPointer<vtkCharArray> NullRow;
    void *RasterBuff;
    char *NullBuff;
    //ETX
    // Colortable, history and cats, timestamp are needed

    //! \brief Set the region
    bool SetRegion();
    //! \brief Allocate the grass raster buffer and the vtkDädataArray. 
    //! The SetRegion() must be called befor this method. And the Map must be opened.
    bool SetUpRasterBuffer();

    //! \brief Name of the raster map to be readed or created
    vtkSetStringMacro(RasterName);
    vtkSetStringMacro(Mapset);
    vtkSetMacro(MapType, int);
    vtkSetMacro(RegionUsage, int);

private:
    vtkGRASSRasterMapBase(const vtkGRASSRasterMapBase&); // Not implemented.
    void operator=(const vtkGRASSRasterMapBase&); // Not implemented.
};


#endif
