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

#include "vtkGRASSRasterToImageReader.h"

#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>

extern "C" {
#include <grass/gis.h>
#include <grass/raster.h>
#include <math.h>
}

vtkStandardNewMacro(vtkGRASSRasterToImageReader);

//----------------------------------------------------------------------------

vtkGRASSRasterToImageReader::vtkGRASSRasterToImageReader() {
    this->DataExtent[0] = 0;
    this->DataExtent[1] = 255;
    this->DataExtent[2] = 0;
    this->DataExtent[3] = 255;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = 0;

    this->DataScalarType = VTK_DOUBLE;

    this->DataOrigin[0] = this->DataOrigin[1] = this->DataOrigin[2] = 0.0;
    this->DataSpacing[0] = this->DataSpacing[1] = this->DataSpacing[2] = 1.0;

    this->RasterName = NULL;
    this->Mapset = NULL;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);

    this->RegionUsage = VTK_GRASS_REGION_CURRENT;

    this->Region = NULL;

    //G_gisinit("vtkGRASSRasterToImageReader");
}

//----------------------------------------------------------------------------

vtkGRASSRasterToImageReader::~vtkGRASSRasterToImageReader() {
    if(this->RasterName)
        delete [] this->RasterName;
    if(this->Mapset)
        delete [] this->Mapset;
}

//----------------------------------------------------------------------------

int vtkGRASSRasterToImageReader::RequestInformation(
        vtkInformation * vtkNotUsed(request),
        vtkInformationVector ** vtkNotUsed(inputVector),
        vtkInformationVector *outputVector) {

    struct Cell_head head;
    const char *mapset;

    // Open the ratser map
    mapset = G_find_raster2(this->GetRasterName(), "");
    if (mapset == NULL) {
        fprintf(stderr, "Rasterfile %s not found", this->GetRasterName());
        return -1;
    }

    this->SetMapset(mapset);

    if(this->RegionUsage == VTK_GRASS_REGION_CURRENT)
    {
        cout << "VTK_GRASS_REGION_CURRENT" << endl;
        G_get_set_window(&head);
    }
    else if(this->RegionUsage == VTK_GRASS_REGION_DEFAULT)
    {
        cout << "VTK_GRASS_REGION_DEFAULT" << endl;
        G_get_default_window(&head);
        G_set_window(&head);
    }
    else if(this->RegionUsage == VTK_GRASS_REGION_RASTER)
    {
        cout << "VTK_GRASS_REGION_RASTER" << endl;
        Rast_get_cellhd(this->GetRasterName(), this->GetMapset(), &head);
        G_set_window(&head);
    }
    else if(this->RegionUsage == VTK_GRASS_REGION_USER && this->Region != NULL)
    {
        this->Region->CopyRegionTo(&head);
        G_set_window(&head);
    }
    else {
        // Use current region as default
        G_get_set_window(&head);
    }

    // Init the image extent with the grass region settings
    this->DataExtent[0] = 0;
    this->DataExtent[1] = head.cols - 1;
    this->DataExtent[2] = 0;
    this->DataExtent[3] = head.rows - 1;
    this->DataExtent[4] = 0;
    this->DataExtent[5] = 0;

    this->DataSpacing[0] = head.ew_res;
    this->DataSpacing[1] = head.ns_res;

    this->DataOrigin[0] = head.west + head.ew_res/2.0;
    this->DataOrigin[1] = head.south + head.ns_res/2.0;
    this->DataOrigin[2] = 0;

    // get the info objects
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    outInfo->Set(vtkDataObject::SPACING(), this->DataSpacing, 3);
    outInfo->Set(vtkDataObject::ORIGIN(), this->DataOrigin, 3);
    outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
            this->DataExtent, 6);
    vtkDataObject::SetPointDataActiveScalarInfo(outInfo, this->DataScalarType, 1);
    return 1;
}

//----------------------------------------------------------------------------

template<class T>
void vtkGRASSRasterToImageReaderExecute(vtkGRASSRasterToImageReader *self,
    vtkImageData *data, T *outPtr,
    int outExt[6]) {

    int idxX, idxY, idxZ;
    vtkIdType outIncX, outIncY, outIncZ;
    int fd;
    int out_type;
    void *ptr, *raster;
    int rowcount = outExt[3];

    /* open raster map */
    fd = Rast_open_old(self->GetRasterName(), self->GetMapset());
    if (fd < 0)
        fprintf(stderr, "Unable to open raster map <%s>", self->GetRasterName());

    out_type = Rast_get_map_type(fd);
    raster = Rast_allocate_buf(out_type);

    // Get increments to march through data
    data->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);


    // Loop through output pixel
    for (idxZ = outExt[4]; idxZ <= outExt[5]; idxZ++) {
        for (idxY = outExt[2]; !self->GetAbortExecute() && idxY <= outExt[3]; idxY++) {

            Rast_get_row(fd, raster, rowcount, out_type);
            rowcount--;

            for (idxX = outExt[0], ptr = raster; idxX <= outExt[1]; idxX++, ptr = G_incr_void_ptr(ptr, Rast_cell_size(out_type))) {
                *outPtr++ = self->GetRasterValueAsDouble(out_type, ptr, 0);
            }
            outPtr += outIncY;
        }
        outPtr += outIncZ;
    }

    Rast_close(fd);

}

//----------------------------------------------------------------------------

int
vtkGRASSRasterToImageReader::RequestData(vtkInformation*,
                                       vtkInformationVector**,
                                       vtkInformationVector* outputVector)
{
    // get the info objects
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    vtkImageData *data = this->AllocateOutputData(outInfo->Get(vtkDataObject::DATA_OBJECT()),
                                                  outInfo);

    int *outExt = data->GetExtent();
    void *outPtr = data->GetScalarPointerForExtent(outExt);

    // Call the correct templated function for the output
    switch (this->GetDataScalarType()) {
            vtkTemplateMacro(vtkGRASSRasterToImageReaderExecute(this, data,
                    static_cast<VTK_TT *> (outPtr),
                    outExt));
        default:
            vtkErrorMacro( << "Execute: Unknown data type");
    }
}

//----------------------------------------------------------------------------

void vtkGRASSRasterToImageReader::PrintSelf(ostream& os, vtkIndent indent) {
    this->Superclass::PrintSelf(os, indent);

    os << indent << "DataScalarType: " <<
            vtkImageScalarTypeNameMacro(this->DataScalarType) << "\n";
    os << indent << "DataExtent: (" << this->DataExtent[0] << ", "
            << this->DataExtent[1] << ", "
            << this->DataExtent[2] << ", "
            << this->DataExtent[3] << ", "
            << this->DataExtent[4] << ", "
            << this->DataExtent[5] << ")\n";
    os << indent << "DataSpacing: (" << this->DataSpacing[0] << ", "
            << this->DataSpacing[1] << ", "
            << this->DataSpacing[2] << ")\n";
    os << indent << "DataOrigin: (" << this->DataOrigin[0] << ", "
            << this->DataOrigin[1] << ", "
            << this->DataOrigin[2] << ")\n";
}

//----------------------------------------------------------------------------

double vtkGRASSRasterToImageReader::GetRasterValueAsDouble(int MapType, void *ptr, double nullval)
{
    double val = nullval;

    if (MapType == CELL_TYPE) {
    if (Rast_is_null_value(ptr, MapType)) {
        val = nullval;
    }
    else {
        val = *(CELL *) ptr;
    }
    }
    if (MapType == FCELL_TYPE) {
    if (Rast_is_null_value(ptr, MapType)) {
        val = nullval;
    }
    else {
        val = *(FCELL *) ptr;
    }
    }
    if (MapType == DCELL_TYPE) {
    if (Rast_is_null_value(ptr, MapType)) {
        val = nullval;
    }
    else {
        val = *(DCELL *) ptr;
    }
    }

    return val;
}
