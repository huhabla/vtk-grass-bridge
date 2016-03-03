/*
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2012 by Soeren Gebbert, soerengebbert@googlemail.com
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

#include <vtkDataSetAttributes.h>
#include "vtkGRASSMultiRasterPolyDataLineReader.h"

#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkIdList.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkShortArray.h>
#include "vtkDCELL.h"
#include "vtkGRASSRasterRow.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSMessagingInterface.h"

vtkStandardNewMacro(vtkGRASSMultiRasterPolyDataLineReader);

//----------------------------------------------------------------------------

vtkGRASSMultiRasterPolyDataLineReader::vtkGRASSMultiRasterPolyDataLineReader() {
	this->DataName = NULL;
	this->LineLengths = NULL;
	this->RasterNames = NULL;
	this->RasterMap = vtkGRASSRasterMapReader::New();
	this->SetNumberOfInputPorts(0);
}

//----------------------------------------------------------------------------

vtkGRASSMultiRasterPolyDataLineReader::~vtkGRASSMultiRasterPolyDataLineReader() {
	if (this->DataName)
		delete[] this->DataName;
	if (this->LineLengths)
		this->LineLengths->Delete();

	this->RasterMap->Delete();
}

//----------------------------------------------------------------------------

void vtkGRASSMultiRasterPolyDataLineReader::PrintSelf(ostream& os,
		vtkIndent indent) {
	this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------

int vtkGRASSMultiRasterPolyDataLineReader::RequestData(vtkInformation*,
		vtkInformationVector**, vtkInformationVector* outputVector) {

	int n;
	int rows, row, cols, col;
	int cellCount = 0;
	double length, x, y;
	vtkIdType id;
	VGB_CREATE(vtkDCELL, dvalue);
	char message[1024];
	VGB_CREATE(vtkGRASSMessagingInterface, talk);

	if (this->RasterNames == NULL) {
		vtkErrorMacro( << "Raster names not set.");
		return -1;
	}

	if (this->DataName == NULL) {
		vtkErrorMacro( << "The DataName must be specified");
		return -1;
	}

	if (this->LineLengths == NULL) {
		vtkErrorMacro( << "Line lengths not set.");
		return -1;
	}

	if (this->RasterNames->GetNumberOfTuples()
			!= this->LineLengths->GetNumberOfTuples()) {
		vtkErrorMacro( << "The number of line length entries is not equal "
		"the number of raster names");
		return -1;
	}

	vtkPolyData* output = vtkPolyData::GetData(outputVector);
	output->Allocate(1);

	VGB_CREATE(vtkGRASSRasterRow, buff);
	VGB_CREATE(vtkIdList, ids);
	VGB_CREATE(vtkPoints, points);

	VGB_CREATE(vtkDoubleArray, values);
	values->SetNumberOfComponents(1);
	values->SetName(this->DataName);

	VGB_CREATE(vtkShortArray, layer);
	layer->SetNumberOfComponents(1);
	layer->SetName("Layer");

	length = 0.0;

	for (n = 0; n < this->RasterNames->GetNumberOfValues(); n++) {
		const char *name = this->RasterNames->GetValue(n);
	    int cellCount2 = 0;

		length += this->LineLengths->GetValue(n);

		if (!this->RasterMap->OpenMap(name)) {
			vtkErrorMacro(
					<< "Unable to open raster map " << this->RasterNames->GetValue(n));
			return -1;
		}

		/*
		G_snprintf(message, 1024, "vtkGRASSMultiRasterPolyDataLineReader: "
				"Processing layer %i of %i", n + 1,
				(int)this->RasterNames->GetNumberOfValues());
		talk->Message(message);
		*/

		rows = this->RasterMap->GetNumberOfRows();
		cols = this->RasterMap->GetNumberOfCols();

		if(!buff->IsAllocated())
			buff->Allocate(cols, this->RasterMap->GetMapType());

		for (row = 0; row < rows; row++) {

			talk->Percent(row, rows, 1);
			this->RasterMap->GetRow(row, buff);

			for (col = 0; col < cols; col++) {

				/* Only non-Null values */
				if (buff->IsNull(col))
					continue;

				buff->GetDCELLValue(col, dvalue);

				values->InsertNextValue((double) dvalue->Value);
				layer->InsertNextValue(n + 1);

				/* Convert rows and columns into coordinates */
				x = this->RasterMap->ColumnToEasting((double) col + 0.5);
				y = this->RasterMap->RowToNorthing((double) row + 0.5);

				ids->Initialize();

				// We create two points for the first layer
				if (n == 0) {
					// Fist point
					id = points->InsertNextPoint(x, y, 0);
					ids->InsertNextId(id);
					// second point
					id = points->InsertNextPoint(x, y, length);
					ids->InsertNextId(id);
					cellCount++;
					cellCount2++;
				} else {
					// New lower point with accumulated line length
					id = points->InsertNextPoint(x, y, length);
					// The id of the lower points of the layer before this
					// This assures connected lines
					if (n == 1) {
						/* Numbering is based on:
						 *
						 *  Top layer
						 *
						 *  0 o   2 o   4 o   6 o   8 o
						 *    |     |     |     |     |
						 *  1 o   3 o   5 o   7 o   9 o
						 *    |     |     |     |     |
						 * 10 o  11 o  12 o  13 o  14 o
						 *    |     |     |     |     |
						 * 15 o  16 o  17 o  18 o  19 o
						 *
						 * Bottom layer
						 *
						 */
						ids->InsertNextId(
								id - (2 * cellCount - cellCount2 - 1));
						//std::cout << "First point id: " << id
						//		<< "  Second point id: "
						//		<< id - (2 * cellCount - cellCount2 - 1)
						//		<< std::endl;
					} else {
						ids->InsertNextId(id - cellCount);
						//std::cout << "First point id: " << id
						//		<< "  Second point id: " << id - cellCount
						//		<< std::endl;
					}
					// Second point
					ids->InsertNextId(id);
					cellCount2++;
				}
				output->InsertNextCell(VTK_LINE, ids);
			}
		}
		if (cellCount != cellCount2) {
			vtkErrorMacro(
					<< "Number of non-null cells are different in input vector maps");
			return -1;
		}
		this->RasterMap->CloseMap();
	}

	// Store the points in the output data object.
	output->GetCellData()->SetScalars(values);
	output->GetCellData()->AddArray(layer);
	output->SetPoints(points);

	return 1;
}
