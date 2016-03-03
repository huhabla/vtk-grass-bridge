
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
 * \brief This class provides read/write access to an existing vector map
 *
 * This class provides an interface to update grass vector maps.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */
#ifndef _vtkGRASSVectorMapUpdater_h
#define	_vtkGRASSVectorMapUpdater_h

#include <vtkGRASSVectorMapWriter.h>


class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorMapUpdater : public vtkGRASSVectorMapWriter {
public:
    static vtkGRASSVectorMapUpdater *New();
    vtkTypeMacro(vtkGRASSVectorMapUpdater, vtkGRASSVectorMapWriter);


    /*!\brief Open a new vector map to write
     *
     * \param name of the vector map
     * \param with_z open in 2d or 3d mode
     * \return true or false
     * */
    virtual bool OpenMap(const char *name, int with_z = 0);

    /*!
       \brief Rewrites feature info at the given offset.

       The number of points or cats or type may change. If necessary, the
       old feature is deleted and new is written.

       \param feature feature id
       \param type feature type
       \param points feature geometry
       \param cats feature categories

       \return new feature id
       \return -1 on error
     */

    virtual int RewriteFeature(int feature, int type, vtkGRASSVectorFeaturePoints *points, vtkGRASSVectorFeatureCats *cats) ;
    
    /*!
       \brief Restore previously deleted feature

       Vector map must be opened on topo level 2.

       \param feature feature id to be deleted

       \return 0 on success
       \return -1 on error
     */
    virtual int RestoreFeature(int feature, int offset);


protected:
    vtkGRASSVectorMapUpdater();

    ~vtkGRASSVectorMapUpdater() {
    };

private:
    vtkGRASSVectorMapUpdater(const vtkGRASSVectorMapUpdater&); // Not implemented.
    void operator=(const vtkGRASSVectorMapUpdater&); // Not implemented.
};

#endif
