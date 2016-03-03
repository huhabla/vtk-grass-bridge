
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
 * \brief This class provides read-only access to a vector map
 *  with topology information
 *
 * This class provides an interface to read grass vector maps with topology information.
 * Many of the topology functions of the grass vector library are available as member function.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */
#ifndef _vtkGRASSVectorMapTopoReader_h
#define _vtkGRASSVectorMapTopoReader_h

#include "vtkGRASSVectorMapNoTopoReader.h"
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include "vtkGRASSVectorBBox.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class vtkGRASSVectorFeaturePoints;
class vtkGRASSVectorFeatureCats;
class vtkGRASSVectorBBox;
class vtkIntArray;

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapTopoReader : public vtkGRASSVectorMapNoTopoReader {
public:
    static vtkGRASSVectorMapTopoReader *New();
    vtkTypeMacro(vtkGRASSVectorMapTopoReader, vtkGRASSVectorMapNoTopoReader);
    void PrintSelf(ostream& os, vtkIndent indent);

    /*! \brief Read the line at index of the vector map (only level 2)
     *
     *  \return 1 on success,  -1 on error
     * */
    virtual int ReadFeature( int index, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats);

    /*************** AREA FUNCTIONS *********************************/

    int GetAreaFromCentroid(int centroid);

    /*!
       \brief Returns polygon array of points and the categories of given area

       \param area area id
       \param[out] points points array
       \param[out] cats category array

       \return number of points
       \return -1 on error
     */
    int GetArea( int area, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats);

    /*!
       \brief Returns polygon array of points for given isle

       \param isle island id
       \param[out] points points array

       \return number of points
       \return -1 on error
     */
    int GetIslePoints( int isle, vtkGRASSVectorFeaturePoints *points) {
        TRY if (this->Open) return Vect_get_isle_points (&this->map, isle, points->GetPointer());
        else return -1; CATCH_INT
    }
    /*!
       \brief Returns centroid id for given area

       \param area area id

       \return centroid number of area
       \return 0 if no centroid found
     */
    int GetCentroidFromArea(int area ){
        TRY if (this->Open) return Vect_get_area_centroid (&this->map, area);
        else return -1; CATCH_INT
    }

    /*!
       \brief Creates list of boundaries for given area

       \param area area id
       \param[out] List pointer to list of boundaries

       \return number of boundaries
     */
    int GetAreaBoundaries(int area, vtkIntArray *boundaryids);
    /*!
       \brief Creates list of boundaries for given isle

       \param isle island number
       \param[out] List pointer to list where boundaries are stored

       \return number of boundaries
     */
    int GetIsleBoundaries(int area, vtkIntArray *boundaryids);
    /*!
       \brief Returns number of isles for given area

       \param area area id

       \return number of isles for area
       \return 0 if area not found
     */
    int GetNumberOfAreaIsles(int area){
        TRY if (this->Open) return Vect_get_area_num_isles (&this->map, area);
        else return -1; CATCH_INT
    }
    /*!
       \brief Returns isle id for area

       \param area area id
       \param isle isle index (0 .. nisles - 1)

       \return isle id
       \return 0 if no isle found
     */
    int GetAreaIsle(int area, int isle){
        TRY if (this->Open) return Vect_get_area_isle (&this->map, area, isle);
        else return -1; CATCH_INT
    }
    /*!
       \brief Returns area id for isle

       \param isle isle number (0 .. nisles - 1)

       \return area id
       \return 0 area not found
     */
    int GetIsleArea(int isle){
        TRY if (this->Open) return Vect_get_isle_area (&this->map, isle);
        else return -1; CATCH_INT
    }
    /*!
       \brief Check if point is in area

       \param area area id
       \param x,y point coordinates

       \return 1 if point is in area
       \return 0 if not
     */
    int IsPointInArea(int area, double x, double y){
        TRY if (this->Open) {
        struct bound_box box;
        if(!Vect_get_area_box(&this->map, area, &box))
            return 0;
        return Vect_point_in_area (x, y, &this->map, area, &box);
        } else return 0; CATCH_INT
    }
    /*!
       \brief Returns area of area without areas of isles

       \param Map vector map
       \param area area id

       \return area of area without areas of isles
     */
    double GetAreaOfArea(int area) {
        TRY if (this->Open)return Vect_get_area_area(&this->map, area);
        else return -1; CATCH_INT
    }
    /*!
       \brief Calculate area perimeter

       \param Points list of points defining area boundary

       \return area perimeter
     */
    double GetPerimeterOfArea(int area) {
        TRY if (this->Open)return Vect_get_area_perimeter(&this->map, area);
        else return -1; CATCH_INT
    }
        //!\brief Returns area of area without areas of isles.
    double GetAreaBoundingBox(int area, vtkGRASSVectorBBox *box) {
        TRY if (this->Open)return Vect_get_area_box(&this->map, area, box->GetPointer());
        else return -1; CATCH_INT
    }
    /*!
     * \brief Find the nearest node.
     *
     * \param ux,uy,uz point coordinates
     * \param maxdist max distance from the line
     * \param with_z 3D (WITH_Z, WITHOUT_Z)
     *
     * \return number of nearest node
     * \return 0 if not found
     */
    int FindNearestNode(double ux, double uy, double uz, double maxdist, int with_z){
        TRY if (this->Open)return Vect_find_node(&this->map, ux, uy, uz, maxdist, with_z);
        else return -1; CATCH_INT
    }
    /*!
     * \brief Find the nearest line.
     *
     * \param ux,uy,uz points coordinates
     * \param type feature type (GV_LINE, GV_POINT, GV_BOUNDARY or GV_CENTROID)
     * if only want to search certain types of lines or -1 if search all lines
     * \param maxdist max distance from the line
     * \param with_z 3D (WITH_Z, WITHOUT_Z)
     * \param exclude if > 0 number of line which should be excluded from selection.
     * May be useful if we need line nearest to other one.
     *
     * \return number of nearest line
     * \return 0 if not found
     *
     */
    int FindNearestLine(double ux, double uy, double uz, int type, double maxdist, int with_z, int exclude){
        TRY if (this->Open)return Vect_find_line(&this->map, ux, uy, uz, type, maxdist, with_z, exclude);
        else return -1; CATCH_INT
    }

    /*!
     * \brief Find the nearest area

     * \param x,y point coordinates
     *
     * \return area number
     * \return 0 if not found
     */
    int FindNearestArea(double x, double y){
        TRY if (this->Open)return Vect_find_area(&this->map, x, y);
        else return -1; CATCH_INT
    }

    /*!
     * \brief Find the nearest island
     *
     * \param x,y points coordinates
     *
     * \return island number,
     * \return 0 if not found
     */
    int FindNearestIsle(double x, double y){
        TRY if (this->Open)return Vect_find_island(&this->map, x, y);
        else return -1; CATCH_INT
    }

    /***************************** FIND FUNCTIONS *****************************/

    //!\brief Get the bounding box of the map
    void GetBoundingBox(vtkGRASSVectorBBox *box);

    virtual int GetNumberOfFeatures() {
        int nprimitives = 0;

        nprimitives += Vect_get_num_primitives(&this->map, GV_POINT);
        nprimitives += Vect_get_num_primitives(&this->map, GV_LINE);
        nprimitives += Vect_get_num_primitives(&this->map, GV_BOUNDARY);
        nprimitives += Vect_get_num_primitives(&this->map, GV_FACE);
        nprimitives += Vect_get_num_primitives(&this->map, GV_CENTROID);
        nprimitives += Vect_get_num_primitives(&this->map, GV_KERNEL);
        if (this->Open)return (int) nprimitives;
//        if (this->Open)return (int) Vect_get_num_lines(&this->map);
        else return -1;
    }

    virtual int GetNumberOfLines() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_LINE);
        else return -1;
    }

    virtual int GetNumberOfPoints() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_POINT);
        else return -1;
    }

    virtual int GetNumberOfAreas() {
        if (this->Open)return (int) Vect_get_num_areas(&this->map);
        else return -1;
    }

    virtual int GetNumberOfFaces() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_FACE);
        else return -1;
    }

    virtual int GetNumberOfKernels() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_KERNEL);
        else return -1;
    }

    virtual int GetNumberOfIsles() {
        if (this->Open)return (int) Vect_get_num_islands(&this->map);
        else return -1;
    }

    virtual int GetNumberOfNodes() {
        if (this->Open)return (int) Vect_get_num_nodes(&this->map);
        else return -1;
    }

    virtual int GetNumberOfBoundaries() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_BOUNDARY);
        else return -1;
    }

    virtual int GetNumberOfCentroids() {
        if (this->Open)return (int) Vect_get_num_primitives(&this->map, GV_CENTROID);
        else return -1;
    }

    //!\brief Return the number of holes
    virtual int GetNumberOfHoles() {
        if (this->Open)return (int) Vect_get_num_holes(&this->map);
        else return -1;
    }

    /*!
       \brief Select lines by box.

       Select lines whose boxes overlap specified box!!!  It means that
       selected line may or may not overlap the box.

       \param Box bounding box
       \param type line type
       \param[out] list output list, must be initialized

       \return number of lines
     */
    virtual int SelectLinesByBox(vtkGRASSVectorBBox *box, int type, vtkIntArray *ids);

    /*!
       \brief Select areas by box.

       Select areas whose boxes overlap specified box!!!
       It means that selected area may or may not overlap the box.

       \param Box bounding box
       \param[out] output list, must be initialized

       \return number of areas
     */
    virtual int SelectAreasByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids);
    /*!
       \brief Select isles by box.

       Select isles whose boxes overlap specified box!!!
       It means that selected isle may or may not overlap the box.

       \param Box bounding box
       \param[out] list output list, must be initialized

       \return number of isles
     */
    virtual int SelectIslesByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids);
    /*!
       \brief Select nodes by box.

       \param Box bounding box
       \param[out] list output list, must be initialized

       \return number of nodes
     */

    virtual int SelectNodesByBox(vtkGRASSVectorBBox *box, vtkIntArray *ids);

    /*! \brief Copy two vector maps
     *
     * \param src vector map to copy from
     * \return 0 success; -1 failure
     *
     * */
    int DeepCopy(vtkGRASSVectorMapTopoReader *src){
        TRY if(this->Open && src->IsOpen()) return Vect_copy_map_lines(src->GetPointer(), this->GetPointer());
        else return -1; CATCH_INT
    }

protected:
    vtkGRASSVectorMapTopoReader();
    ~vtkGRASSVectorMapTopoReader();

    void SetVectorLevelToTopo() {
        this->SetVectorLevel(2);
    }

private:
    vtkGRASSVectorMapTopoReader(const vtkGRASSVectorMapTopoReader&); // Not implemented.
    void operator=(const vtkGRASSVectorMapTopoReader&); // Not implemented.
};

#endif
