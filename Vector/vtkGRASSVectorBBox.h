
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
 * \brief This class provides access to the grass bounding box
 *
 * Most of the grass vector bounding box functions are supported.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Sep. 2009
 * \author soerengebbert@googlemail.com
 * */
#ifndef _vtkGRASSVectorBBox_h
#define	_vtkGRASSVectorBBox_h

#include <vtkObjectGRASSErrorHandler.h>
#include "vtkGRASSBridgeVectorWin32Header.h"
#include "vtkGRASSRegion.h"

extern "C" {
#include <grass/gis.h>
#include <grass/vector.h>
}

class VTK_GRASS_BRIDGE_VECTOR_EXPORT vtkGRASSVectorBBox : public vtkObjectGRASSErrorHandler {
public:
    static vtkGRASSVectorBBox *New();
    vtkTypeMacro(vtkGRASSVectorBBox, vtkObjectGRASSErrorHandler);
    void PrintSelf(ostream& os, vtkIndent indent);

    /*!
       \brief Tests for point in box

       \param x,y,z coordinates

       \return 1 point is in box
       \return 0 point is not in box
     */
    virtual int PointInBox(double x, double y, double z) {
        return Vect_point_in_box(x, y, z, &this->box);
    }

    /*!
       \brief Tests for overlap of two boxes

       \param Box to test with

       \return 1 boxes overlap
       \return 0 boxes do not overlap
     */
    virtual int Overlap(vtkGRASSVectorBBox *Box) {
        return Vect_box_overlap(&this->box, Box->GetPointer());
    }

    /*!
       \brief Deep copy of bounding box

       \param Box the boundary box

       \return 1
     */
    virtual int DeepCopy(vtkGRASSVectorBBox *Box) {
        return Vect_box_copy(Box->GetPointer(), &this->box);
    }

    /*!
       \brief Extend this box with Box

       \param Box boundary box

       \return 1
     */
    virtual int Extent(vtkGRASSVectorBBox *Box) {
        return Vect_box_extend(&this->box, Box->GetPointer());
    }

    /*!
       \brief Copy region  to box

       \param region vtkGRASSRegion

       \return 1 on success, 0 on error
     */
    virtual int CopyBoxFromRegion(vtkGRASSRegion *region) {return Vect_region_box(region->GetPointer(), &this->box);}

    /*!
     * \brief Set the box values
     *
     * */
    virtual void SetBox(double north, double south, double east, double west, double top, double bottom) {
        this->box.N = north;
        this->box.S = south;
        this->box.E = east;
        this->box.W = west;
        this->box.T = top;
        this->box.B = bottom;
    };

    virtual double GetNorth()  {return this->box.N;}
    virtual double GetSouth()  {return this->box.S;}
    virtual double GetEast()   {return this->box.E;}
    virtual double GetWest()   {return this->box.W;}
    virtual double GetTop()    {return this->box.T;}
    virtual double GetBottom() {return this->box.B;}

    /*!
     * \brief return the box content as double array with 6 entries
     *
     * Pos 0: north
     * Pos 1: south
     * Pos 2: east
     * Pos 3: west
     * Pos 4: top
     * Pos 5: bottom
     * \return array
     *
     */
    virtual double* GetBoxAsArray();

    //BTX
    virtual struct bound_box *GetPointer() {
        return &this->box;
    }
    //ETX

protected:
    vtkGRASSVectorBBox(){};
    ~vtkGRASSVectorBBox(){};

    //BTX
    struct bound_box box;
    //ETX

private:
    vtkGRASSVectorBBox(const vtkGRASSVectorBBox&); // Not implemented.
    void operator=(const vtkGRASSVectorBBox&); // Not implemented.
};

#endif	

