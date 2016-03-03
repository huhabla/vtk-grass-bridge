
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
 * \brief This class provides access to vector featrue categories
 * information
 *
 * Most of the grass vector lib functions are supported. You can add, set or remove a category.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef _vtkGRASSVectorLineCats_h
#define	_vtkGRASSVectorLineCats_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSRegion.h"

class vtkIntArray;

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorFeatureCats : public vtkObjectGRASSErrorHandler {
public:
    static vtkGRASSVectorFeatureCats *New();
    vtkTypeMacro(vtkGRASSVectorFeatureCats, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    /*!
       \brief Add new field/cat to category structure if doesn't exist yet.

       \param[in] field layer number
       \param[in] cat category number

       \return number of categories
       \return 0 if no space for new category in structure, n_cats would be > GV_NCATS_MAX
       \return -1 on out of memory
       \return -2 if field out of range: 1 - GV_FIELD_MAX or cat out of range:  1 - GV_CAT_MAX
     */
    virtual int AddCat(int field, int cat) {return Vect_cat_set(this->cats, field, cat);}

    /*!
       \brief Get first found category of given field.

       'cat' is set to first category found or -1 if field was not found

       \param[in] field layer number

       \return cat
     */
    virtual int GetCat(int field);

    /*!
       \brief Get list of categories of given field.

       \param field layer number
       \param[out] pointer to a vtkIntArray where cats will be written

       \return number of found categories
       \return -1 on invalid field
       \return -2 unable to allocate internal memory
     */
    virtual int GetCats(int field, vtkIntArray *categories);

    /*!
       \brief Delete all categories of given layer

       \param[in] field layer number

       \return 1 deleted
       \return 0 layer does not exist
     */
    virtual int DeleteCats(int field){return Vect_cat_del(this->cats, field);}

    /*!
       \brief Delete field/cat 

       \param[in] field layer number
       \param[in] cat category to be deleted or -1 to delete all cats of given field

       \return 1 deleted
       \return 0 field/category number does not exist
     */
    virtual int DeleteCat(int field, int cat){return Vect_field_cat_del(this->cats, field, cat);}

    /*!
       \brief Reset category structure to make sure cats structure is clean to be re-used.

       I.e. it has no cats associated with it. Cats must have
       previously been created with Vect_new_cats_struct()

       \param[out] Cats line_cats structure

       \return 0
     */
    virtual int Reset() {return Vect_reset_cats(this->cats);}
    
    //!\brief Return the number of categroies
    virtual int GetNumberOfCats(){return this->cats->n_cats;}

    //BTX
    virtual struct line_cats *GetPointer() {
        return this->cats;
    }
    //ETX

    //!\brief Deep copy the feature categories
    virtual void DeepCopy(vtkGRASSVectorFeatureCats *cats);
    
protected:
    vtkGRASSVectorFeatureCats();
    ~vtkGRASSVectorFeatureCats();

    //BTX
    struct line_cats *cats;
    //ETX

private:
    vtkGRASSVectorFeatureCats(const vtkGRASSVectorFeatureCats&); // Not implemented.
    void operator=(const vtkGRASSVectorFeatureCats&); // Not implemented.
};

#endif
