#
#  Program: vtkGRASSBridge
#  COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; version 2 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

import unittest
from vtk import *
from libvtkGRASSBridgeRasterPython import *
from libvtkGRASSBridgeCommonPython import *

class GRASSRasterMapReaderWriterTest4(unittest.TestCase):
    def setUp(self):
        init = vtkGRASSInit()
        init.Init("GRASSRasterMapReaderWriterTest3")
        init.ExitOnErrorOn()

    def testSimpleReadWriteCycleWithMemoryMap(self):

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToCELL()
        writer.UseDefaultRegion()
        writer.OpenMap("test_cell")
        print writer
        
        mmap = vtkGRASSRasterMemoryMap()
        mmap.Allocate(writer.GetNumberOfRows(), writer.GetNumberOfCols(),
                      writer.GetMapType())
        # Init the array with no-data
        mmap.SetToNull()
        print mmap

        cell = vtkCELL()
        
        data = vtkIntArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        min = 100000000
        max = 0
        
        nrows = writer.GetNumberOfRows()
        ncols = writer.GetNumberOfCols()
        
        for i in range(nrows):
            for j in range(ncols):
                val = i + j + 100
                cell.SetIntValue(val)
                if min > val:
                    min = val
                if max < val:
                    max = val
                data.SetValue(j, val)
                mmap.SetCELLValue(i, j, cell)

        writer.WriteMemoryMap(mmap)
        print writer
        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_cell")

        newMin = 100000000
        newMax = 0
        for i in range(reader.GetNumberOfRows()):
            data = reader.GetRow(i)
            for j in range(reader.GetNumberOfCols()):
                val =  int(data.GetTuple1(j))
                if newMin > val:
                    newMin = val
                if newMax < val:
                    newMax = val

        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader

        self.assertEqual(val[0], min, "Error range is incorrect")
        self.assertEqual(val[1], max, "Error range is incorrect")

        reader.CloseMap()

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRasterMapReaderWriterTest4)
    unittest.TextTestRunner(verbosity=2).run(suite)
