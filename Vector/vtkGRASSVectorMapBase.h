
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
 * \brief This is the base class for read/write access to a vector map.
 *
 * This class provides an interface to read grass vector map information
 * without building topology.
 *
 * \TODO Implement database support
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSVectorMapBase_h
#define	_vtkGRASSVectorMapBase_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSDefines.h"

class vtkGRASSVectorFeaturePoints;
class vtkGRASSVectorFeatureCats;
class vtkGRASSVectorBBox;

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapBase : public vtkObjectGRASSErrorHandler {
public:

    static vtkGRASSVectorMapBase *New();
    vtkTypeMacro(vtkGRASSVectorMapBase, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);


    /*! \brief Read the next line of the vector map
     *
     *  \return position in file, -1 on error
     * */
    virtual int ReadNextFeature(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats);

    /*! \brief Rewind the vector map
     *
     *  \return 1 on success,  -1 on error
     * */
    virtual int RewindMap() {
        return Vect_rewind(&this->map);
    }

    /*! \brief Close the map and free any memory
     *
     * */
    virtual bool CloseMap();

    //! \brief Returns thr opening state
    //! \return true for open, false for not open
    virtual bool IsOpen() {
        return this->Open;
    }

    //!\brief Return the name of the organisation
    const char *GetOrganisation() {
        if (this->Open)return Vect_get_organization(&this->map);
        else return NULL;
    }
    //!\brief Return the the date of creation as string of the map
    const char *GetCreationDate() {
        if (this->Open)return Vect_get_date(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the creator
    const char *GetPerson() {
        if (this->Open)return Vect_get_person(&this->map);
        else return NULL;
    }
    //!\brief Return the title of the map
    const char *GetTitle() {
        if (this->Open)return Vect_get_map_name(&this->map);
        else return NULL;
    }
    //!\brief Return the full name of the map 
    const char *GetFullName() {
        if (this->Open)return Vect_get_full_name(&this->map);
        else return NULL;
    }
    //!\brief Return the mapset of the map
    const char *GetMapset() {
        if (this->Open)return Vect_get_mapset(&this->map);
        else return NULL;
    }
    //!\brief Return the date of the map
    const char *GetMapDate() {
        if (this->Open)return Vect_get_map_date(&this->map);
        else return NULL;
    }
    //!\brief Return the scale
    int GetScale() {
        if (this->Open)return Vect_get_scale(&this->map);
        else return 0;
    }
    //!\brief Return the map comment

    const char *GetComment() {
        if (this->Open)return Vect_get_comment(&this->map);
        else return NULL;
    }
    //!\brief Return the name of the map zone

    int GetZone() {
        if (this->Open)return Vect_get_zone(&this->map);
        else return -1;
    }
    //!\brief Return the map threshold

    double GetThreshold() {
        if (this->Open)return Vect_get_thresh(&this->map);
        else return -1;
    }
    //!\brief Return the projection number of the map

    int GetProjection() {
        if (this->Open)return Vect_get_proj(&this->map);
        else return -1;
    }
    //!\brief Return the projection name of the map

    const char *GetProjectionName() {
        if (this->Open)return Vect_get_proj_name(&this->map);
        else return NULL;
    }

    //!\brief Return the number of database links
    virtual int GetNumberOfDBLinks() {
        if (this->Open)return (int) Vect_get_num_dblinks(&this->map);
        else return -1;
    }
    
    /*!
       \brief Check if vector map is 3D (with z)

       \return 1 map is 3D
       \return 0 map is not 3D
     * \return -1 map not open
     */

    virtual int Is3d() {
        if (this->Open)return Vect_is_3d(&this->map);
        else return -1;
    }
    /*!
       \brief Check if feature of vector map is alive
     *
     * \param feature id

       \return 1 is alive
       \return 0 is not alive
     * \return -1 map not open
     */
    virtual int IsFeatureAlive(int feature) {
        TRY if (this->Open)return Vect_line_alive(&this->map, feature);
        else return -1; CATCH_INT
    }
    /*!
       \brief Check if node of vector map is alive

     *
     * \param node id
     *
       \return 1 is alive
       \return 0 is not alive
     * \return -1 map not open
     */
    virtual int IsNodeAlive(int node) {
        TRY if (this->Open)return Vect_node_alive(&this->map, node);
        else return -1; CATCH_INT
    }
    /*!
       \brief Check if area of vector map is alive
     *
     * \param area id
     *

       \return 1 is alive
       \return 0 is not alive
     * \return -1 map not open
     */
    virtual int IsAreaAlive(int area) {
        TRY if (this->Open)return Vect_line_alive(&this->map, area);
        else return -1; CATCH_INT
    }
    /*!
       \brief Check if isle of vector map is alive
     *
     * \param isle id
     *

       \return 1 is alive
       \return 0 is not alive
     * \return -1 map not open
     */
    virtual int IsIsleAlive(int isle) {
        TRY if (this->Open)return Vect_line_alive(&this->map, isle);
        else return -1; CATCH_INT
    }
    /*!
      \brief Get feature offset

      Can be used for RestoreLine().

      \param line feature id

      \return feature offset
      \return -1 on error
    */
    virtual int GetLineOffset(int line){
        TRY if (this->Open)return Vect_get_line_offset(&this->map, line);
        else return -1; CATCH_INT
    }

    //!\brief compute the total number of points used by this map. This may be very time consuming.
    virtual int GetTotalNumberOfPoints();

    //!\brief Return the number of the feature type line
    int GetFeatureTypeLine(){return GV_LINE;}
    //!\brief Return the number of the feature type point
    int GetFeatureTypePoint(){return GV_POINT;}
    //!\brief Return the number of the feature type boundary
    int GetFeatureTypeBoundary(){return GV_BOUNDARY;}
    //!\brief Return the number of the feature type area
    int GetFeatureTypeArea(){return GV_AREA;}
    //!\brief Return the number of the feature type face
    int GetFeatureTypeCentroid(){return GV_CENTROID;}
    //!\brief Return the number of the feature type kernel
    int GetFeatureTypeKernel(){return GV_KERNEL;}
    //!\brief Return the number of the feature type face
    int GetFeatureTypeFace(){return GV_FACE;}
    //!\brief Return the number of the feature type face
    int GetFeatureTypeVolume(){return GV_VOLUME;}
    
    //BTX
    virtual struct Map_info *GetPointer() {
        return &this->map;
    }
    //ETX

protected:
    vtkGRASSVectorMapBase();
    ~vtkGRASSVectorMapBase();

    int VectorLevel;
    long TotalNumberOfPoints;
    bool Initiated;
    bool Open;
    char *VectorName;

    //BTX
    struct Map_info map;
    //ETX

    //!\brief Set the level hte vector map should be opened, default is 2
    //TODO: describe the levels
    vtkSetMacro(VectorLevel, int);
    vtkSetStringMacro(VectorName);
    int CheckBeforOpen(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats);
    
    //!\brief Set open vector map without topology support

    void SetVectorLevelToNoTopo() {
        this->SetVectorLevel(1);
    }
    //!\brief Set open vector map without topology support

    void SetVectorLevelToTopo() {
        this->SetVectorLevel(2);
    }

private:
    vtkGRASSVectorMapBase(const vtkGRASSVectorMapBase&); // Not implemented.
    void operator=(const vtkGRASSVectorMapBase&); // Not implemented.
};

#endif
