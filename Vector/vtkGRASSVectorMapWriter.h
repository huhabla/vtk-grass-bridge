
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
 * \brief This class provides write access to a new vector map
 *
 * This class provides an interface to write grass vector maps.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */
#ifndef _vtkGRASSVectorMapWriter_h
#define	_vtkGRASSVectorMapWriter_h

#include <vtkGRASSVectorMapTopoReader.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSVectorFeaturePoints.h"
#include "vtkGRASSVectorFeatureCats.h"


class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapWriter : public vtkGRASSVectorMapTopoReader {
public:
    static vtkGRASSVectorMapWriter *New();
    vtkTypeMacro(vtkGRASSVectorMapWriter, vtkGRASSVectorMapTopoReader);


    /*!\brief Open a new vector map to write with 3d z-coordinate support
     *
     * \param name of the vector map
     * \return true or false
     * */
    virtual bool OpenMap(const char *name){return this->OpenMap(name, 1);}

    /*!\brief Open a new vector map to write
     *
     * \param name of the vector map
     * \param with_z open in 2d (0) or 3d (1) mode
     * \return true or false
     * */
    virtual bool OpenMap(const char *name, int with_z);

    /*! \brief close vector map and build topology
     *
     *  \return true for success, false for failure
     *
     * */
    virtual bool CloseMap(){return this->CloseMap(1);}

    /*! \brief close vector map
     *
     *  \param build_topo 1 - build topology, 0 do not build topology
     *  \return true for success, false for failure
     *
     * */
    virtual bool CloseMap(int build_topo);

    /*!
      \brief Writes new feature to the end of file (table)

      \param type feature type
      \param points feature geometry
      \param cats feature categories

      \return new feature id (level 2) or offset into file where the feature starts (level 1)
     */

    virtual int WriteFeature(int type, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats) ;
    /*!
      \brief Writes new feature to the end of file (table)
     *
     * The feature type must be stored in points

      \param points feature geometry with feature type
      \param cats feature categories

      \return new feature id (level 2) or offset into file where the feature starts (level 1)
     */

    virtual int WriteFeature(vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats){return this->WriteFeature(
            points->GetFeatureType(), points, cats);}

    /*!
       \brief Delete feature

       Vector map must be opened on topo level 2.

       \param line feature id

       \return 0 on success
       \return -1 on error
     */
    virtual int DeleteFeature(int feature);


    //!\brief Set the name of the organisation

    virtual int SetOrganisation(const char *org) {
        if (this->Open)return Vect_set_organization(&this->map, org);
        else return -1;
    }
    //!\brief Set date of creation as string

    virtual int SetCreationDate(const char *date) {
        if (this->Open)return Vect_set_date(&this->map, date);
        else return -1;
    }
    //!\brief Set the name of the creator

    virtual int SetPerson(const char* person) {
        if (this->Open)return Vect_set_person(&this->map, person);
        else return -1;
    }
    //!\brief Set the title of the map

    virtual int SetTitle(const char *title) {
        if (this->Open)return Vect_set_map_name(&this->map, title);
        else return -1;
    }
    //!\brief Set the scale

    virtual int SetScale(int scale) {
        if (this->Open)return Vect_set_scale(&this->map, scale);
        else return -1;
    }
    //!\brief Set the map threshold

    virtual int SetThreshold(double thres) {
        if (this->Open)return Vect_set_thresh(&this->map, thres);
        else return -1;
    }


    //! \brief Build the full topology of the vector map

    virtual int BuildAll() {
        TRY if (this->Open) {
            Vect_set_category_index_update(&this->map);
            return Vect_build(&this->map);
        } else return -1; CATCH_INT
    }
    //! \brief Build the base topology of the vector map

    virtual int BuildBase() {
        TRY if (this->Open) return Vect_build_partial(&this->map, GV_BUILD_BASE);
        else return -1; CATCH_INT
    }
    //! \brief Build the area topology of the vector map

    virtual int BuildAreas() {
        TRY if (this->Open) return Vect_build_partial(&this->map, GV_BUILD_AREAS);
        else return -1; CATCH_INT
    }
    //! \brief Attach islands to areas

    virtual int BuildIsles() {
        TRY if (this->Open)return Vect_build_partial(&this->map, GV_BUILD_ATTACH_ISLES);
        else return -1; CATCH_INT
    }

    /*!
     * \brief Snap lines in vector map to existing vertex in threshold.
     *
     *
     * \param[in] type type of lines to snap
     * \param[in] thresh threshold in which snap vertices
     *
     * \return void
     */
    virtual void SnapLines(int featureType, double threshold) {
        TRY if (this->Open) Vect_snap_lines(&this->map, featureType, threshold, NULL); CATCH_VOID
    }

     /*!
       \brief Break lines in vector map at each intersection.

       \param type feature type
       \return void
     */
   virtual void BreakLines(int featureType) {
        TRY if (this->Open) Vect_break_lines(&this->map, featureType, NULL); CATCH_VOID
    }
     /*!
       \brief Remove small areas from the map map.

       Centroid of the area and the longest boundary with adjacent area is
       removed.  Map topology must be built BuildAll().

       \param thresh maximum area size for removed areas

       \return number of removed areas or -1 for failure
     */
    virtual int RemoveSmallAreas(double thres) {
       TRY if (this->Open) return Vect_remove_small_areas(&this->map, thres, NULL, NULL);
        else return -1; CATCH_INT
    }

    /*!
       \brief Remove duplicate features from vector map.

       Remove duplicate lines of given types from vector map. Duplicate
       lines may be optionally written to error map. Input map must be
       opened on level 2 for update. Categories are merged.

       \param type type of line to be delete
       \return void
     */
    virtual void RemoveDuplicates(int type) {
        TRY if (this->Open) Vect_remove_duplicates(&this->map, type, NULL); CATCH_VOID
    }

    //!\brief Set open new vector map without topology support

    virtual void SetVectorLevelToNoTopo() {
        this->SetVectorLevel(1);
    }
    //!\brief Set open new vector map with topology support

    virtual void SetVectorLevelToTopo() {
        this->SetVectorLevel(2);
    }

protected:
    vtkGRASSVectorMapWriter();

    ~vtkGRASSVectorMapWriter() {
    };

    vtkSetMacro(Open3D, int);

    int Open3D;

private:
    vtkGRASSVectorMapWriter(const vtkGRASSVectorMapWriter&); // Not implemented.
    void operator=(const vtkGRASSVectorMapWriter&); // Not implemented.
};

#endif
