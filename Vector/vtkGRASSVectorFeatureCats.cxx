
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

#include "vtkGRASSVectorFeatureCats.h"
#include <vtkObjectFactory.h>
#include <vtkIntArray.h>

vtkStandardNewMacro(vtkGRASSVectorFeatureCats);

//----------------------------------------------------------------------------

vtkGRASSVectorFeatureCats::vtkGRASSVectorFeatureCats()
{
    this->cats = Vect_new_cats_struct();
    if (this->cats == NULL)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate the internal structure.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
    }
}

//----------------------------------------------------------------------------

vtkGRASSVectorFeatureCats::~vtkGRASSVectorFeatureCats()
{

    if (this->cats)
        Vect_destroy_cats_struct(this->cats);
}

//----------------------------------------------------------------------------

void vtkGRASSVectorFeatureCats::DeepCopy(vtkGRASSVectorFeatureCats *cats)
{
    int i, field, cat;
    this->Reset();
    for(i = 0; i < cats->GetNumberOfCats(); i++)
    {
        field = cats->GetPointer()->field[i];
        cat = cats->GetPointer()->cat[i];
        this->AddCat(field, cat);
    }
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorFeatureCats::GetCat(int field)
{
    int cat;
    Vect_cat_get(this->cats, field, &cat);

    return cat;
}

//----------------------------------------------------------------------------

int
vtkGRASSVectorFeatureCats::GetCats(int field, vtkIntArray *categories)
{
    int ret;
    int i;
    struct ilist *list = Vect_new_list();

    if (list == NULL)
    {
        char buff[1024];
        G_snprintf(buff, 1024, "class: %s line: %i Unable to allocate internal structure.",
                   this->GetClassName(), __LINE__);
        this->InsertNextError(buff);
        return -2;
    }


    categories->Initialize();
    categories->SetNumberOfComponents(1);

    ret = Vect_field_cat_get(this->cats, field, list);
    if (ret > 0)
    {
        categories->SetNumberOfValues(list->n_values);
        for (i = 0; i < list->n_values; i++)
            categories->SetValue(i, list->value[i]);
    }

    Vect_destroy_list(list);

    return ret;

}

//----------------------------------------------------------------------------

void
vtkGRASSVectorFeatureCats::PrintSelf(ostream& os, vtkIndent indent)
{

    this->Superclass::PrintSelf(os, indent);

    os << indent << "Number of Cats: " << this->cats->n_cats << "\n";
    int i;
    for (i = 0; i < this->cats->n_cats; i++)
    {
        os << indent << this->cats->cat[i] << endl;
    }
}
