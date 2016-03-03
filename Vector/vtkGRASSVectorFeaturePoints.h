
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

/*!
 * \brief This class provides access to a vector feature
 *
 * This class wrapps the functionality of the grass vector struct line_pnts
 * and sets additionally the type of the feature within this class.
 *
 * Methods for adding, setting and removing of points are available. Most
 * of the grass vector library functions are supported.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSVectorLinePoints_h
#define	__vtkGRASSVectorLinePoints_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}
class vtkGRASSVectorBBox;

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorFeaturePoints : public vtkObjectGRASSErrorHandler {
public:
    static vtkGRASSVectorFeaturePoints *New();
    vtkTypeMacro(vtkGRASSVectorFeaturePoints, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    /*!
       \brief Appends one point to the end of a feature.

       Returns new number of points or -1 on out of memory Note, this will
       append to whatever is in vtkGRASSVectorFeaturePoints.  If you are re-using
       vtkGRASSVectorFeaturePoints, be sure to clear out old data first by calling
       Reset().

       \param x,y,z point coordinates to be added

       \return number of points
     */

    virtual int AppendPoint(double x, double y, double z) {
        return Vect_append_point(this->points, x, y, z);
    }

    /*!
   \brief Insert new point at index position and move all old points at that position and above up


   \param index (from 0 to NumberOfPoints - 1)
   \param x,y,z point coordinates

   \return number of points
   \return -1 on error (alocation)
     */
    virtual int InsertPoint(int index, double x, double y, double z) {
        return Vect_line_insert_point(this->points, index, x, y, z);
    }

    /*!
   \brief Set the value of a point

   \param index (from 0 to NumberOfPoints - 1)
   \param x,y,z point coordinates

   \return false or true
     */
    virtual bool SetPoint(int index, double x, double y, double z);

    /*!
       \brief Delete point at given index and move all points above down

       \param index (from 0 to NumberOfPoints - 1)

       \return number of points
     */
    virtual int DeletePoint(int index) {
        return Vect_line_delete_point(this->points, index);
    }

    /*!
       \brief Remove duplicate points, i.e. zero length segments

       \return number of points
     */
    virtual int Prune() {
        return Vect_line_prune(this->points);
    }

    /*!
       \brief Remove points in threshold

       \param threshold threshold value

       \return number of points in result
     */
    virtual int PruneThreshold(double threshold) {
        return Vect_line_prune_thresh(this->points, threshold);
    }

    /*!
       \brief Appends points to the end of a feature.

       Note, this will append to whatever is.  If you are
       re-using the object, be sure to clear out old data first by
       calling Reset().

       \param APoints points to be included
       \param direction direction (1 == FORWARD, -1 == BACKWARD)

       \return new number of points
       \return -1 on out of memory
     */
    virtual int AppendPoints(vtkGRASSVectorFeaturePoints *Points, int direction);

    /*!
      \brief  Find point on line in the specified distance.

      From the begining, measured along line.

      If the distance is greater than line length or negative, error is returned.

      \param distance distance value

      \return number of segment the point is on (first is 1),
      \return 0 error when point is outside the line

     */
    virtual int PointOnLine(double distance) {
        return Vect_point_on_line(this->points, distance, NULL, NULL, NULL, NULL, NULL);
    }

    /*!
      \brief Create line segment.

      Creates segment from start to end measured along the
      line and write it to OutPoints.

      If the distance is greater than line length or negative, error is
      returned.

      \param start segment number
      \param end segment number
      \param OutPoints output line

      \return 1 success
      \return 0 error when start > length or end < 0 or start < 0 or end > length
     */
    virtual int CreateLineSegment(double start, double end, vtkGRASSVectorFeaturePoints *OutPoints) {
        return Vect_line_segment(this->points, start, end, OutPoints->GetPointer());
    }

    /*!
       \brief Calculate line 3D-length

       For Lat-Long use GetGeodesicLength() instead.

       \return line length
     */
    virtual double GetLineLength() {
        return Vect_line_length(this->points);
    }

    /*!
       \brief Calculate line length.

       If projection is LL, the length is measured along the geodesic.

       \return line length
     */
    virtual double GetGeodesicLength() {
        return Vect_line_geodesic_length(this->points);
    }

    /*!
       \brief calculate point ot line distance.

       \param x,y,z point coordinates

       \return distance
     */
    virtual double GetPointDistanceFromLine(double x, double y, double z);

    /*!
       \brief Calculate distance of 2 points

       Simply uses Pythagoras.

       \param x1,y1,z1 first point
       \param x2,y2,z2 second point

       \return distance
     */
    virtual double GetPointsDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
        return Vect_points_distance(x1, y1, z1, x2, y2, z2, 1);
    }

    /*!
       \brief Get bounding box of line
     *
       \return NULL on error
     */
    virtual vtkGRASSVectorBBox *GetBoundingBox();

    /*!
       \brief Reverse the order of vertices

       \return void
     */
    virtual void Reverse() {
        Vect_line_reverse(this->points);
    }

    /*!\brief Set the Number of points to zero
     \return 0
     */
    virtual void Reset() {
        Vect_reset_line(this->points);
    }

    /*! \brief Return the number of points
     * */
    virtual int GetNumberOfPoints() {
        return this->points->n_points;
    }

    //!\brief return an array with x, y, and z values from index
    virtual double* GetPoint(int index);


    //!\brief Get the type of the line
    //TODO: describe the vector line types
    vtkGetMacro(FeatureType, int);
    //!\brief Set the Type of the line
    vtkSetMacro(FeatureType, int);

    virtual void SetFeatureTypeToPoint() {
        this->SetFeatureType(GV_POINT);
    }

    virtual void SetFeatureTypeToLine() {
        this->SetFeatureType(GV_LINE);
    }

    virtual void SetFeatureTypeToBoundary() {
        this->SetFeatureType(GV_BOUNDARY);
    }

    virtual void SetFeatureTypeToCentroid() {
        this->SetFeatureType(GV_CENTROID);
    }

    virtual void SetFeatureTypeToArea() {
        this->SetFeatureType(GV_AREA);
    }

    virtual void SetFeatureTypeToFace() {
        this->SetFeatureType(GV_FACE);
    }

    virtual void SetFeatureTypeToKernel() {
        this->SetFeatureType(GV_KERNEL);
    }

    virtual bool IsFeatureTypePoint() {
        if (this->FeatureType == GV_POINT) return true;
        else return false;
    }

    virtual bool IsFeatureTypeLine() {
        if (this->FeatureType == GV_LINE) return true;
        else return false;
    }

    virtual bool IsFeatureTypeBoundary() {
        if (this->FeatureType == GV_BOUNDARY) return true;
        else return false;
    }

    virtual bool IsFeatureTypeCentroid() {
        if (this->FeatureType == GV_CENTROID) return true;
        else return false;
    }

    virtual bool IsFeatureTypeArea() {
        if (this->FeatureType == GV_AREA) return true;
        else return false;
    }

    virtual bool IsFeatureTypeFace() {
        if (this->FeatureType == GV_FACE) return true;
        else return false;
    }

    virtual bool IsFeatureTypeKernel() {
        if (this->FeatureType == GV_KERNEL) return true;
        else return false;
    }

    //!\brief Return the feature type as string
    char *GetFeatureTypeAsString();

    //BTX
    /*! \brief Get the pointer of internal struct line_pnts
     * */
    virtual struct line_pnts *GetPointer() {
        return this->points;
    }
    //ETX

    //GRASS vector to VTK support functions
    //!\brief Convert the grass feature type into a VTK cell id
    int GetVTKCellId();

    //!\brief Deep copy the feature points
    virtual void DeepCopy(vtkGRASSVectorFeaturePoints *points);


protected:
    vtkGRASSVectorFeaturePoints();
    ~vtkGRASSVectorFeaturePoints();

    int FeatureType;

    //BTX
    struct line_pnts *points;
    //ETX

private:
    vtkGRASSVectorFeaturePoints(const vtkGRASSVectorFeaturePoints&); // Not implemented.
    void operator=(const vtkGRASSVectorFeaturePoints&); // Not implemented.
};


#endif	/* _VTKGRASSVECTORMAPREADER_H */

