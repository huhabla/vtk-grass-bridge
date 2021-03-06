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

class GRASSRasterMapReaderWriterTest(unittest.TestCase):
    def setUp(self):
        init = vtkGRASSInit()
        init.Init("GRASSRasterMapReaderWriterTest")
        init.ExitOnErrorOff()

    def testSimpleReadWriteCycleDCELL(self):

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(10)
        region.SetRows(10)
        region.AdjustRegionResolution()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToDCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_dcell")

        value = vtkDCELL()
        row = vtkGRASSRasterRow()
        row.AllocateDCELL(region.GetCols())

        min = 100000000.0
        max = 0.0

        for i in range(region.GetRows()):
            for j in range(region.GetCols()):
                val = i + j + 100.5
                if min > val:
                    min = val
                if max < val:
                    max = val
                value.SetDoubleValue(val)
                row.SetDCELLValue(j, value)
            writer.PutNextRow(row)

        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_dcell")

        newMin = 100000000.0
        newMax = 0.0
        for i in range(region.GetRows()):
            reader.GetRow(i, row)
            for j in range(region.GetCols()):
                row.GetDCELLValue(j, value)
                val = value.GetValueAsDouble()
                if newMin > val:
                    newMin = val
                if newMax < val:
                    newMax = val
                print val,
            print " "
        print " "

        print "Min ", newMin, min
        print "Max ", newMax, max
        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader

        self.assertEqual(val[0], min, "Error range is incorrect")
        self.assertEqual(val[1], max, "Error range is incorrect")
        self.assertEqual(reader.GetNumberOfRows(), region.GetRows(), "Error rows differ")
        self.assertEqual(reader.GetNumberOfCols(), region.GetCols(), "Error cols differ")

        reader.CloseMap()

    def testSimpleReadWriteCycleFCELL(self):

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(10)
        region.SetRows(10)
        region.AdjustRegionResolution()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToFCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_fcell")

        value = vtkFCELL()
        row = vtkGRASSRasterRow()
        row.AllocateFCELL(region.GetCols())

        min = 100000000.0
        max = 0.0

        for i in range(region.GetRows()):
            for j in range(region.GetCols()):
                val = i + j + 100.5
                if min > val:
                    min = val
                if max < val:
                    max = val
                value.SetFloatValue(val)
                row.SetFCELLValue(j, value)
            writer.PutNextRow(row)

        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_fcell")

        newMin = 100000000.0
        newMax = 0.0
        for i in range(region.GetRows()):
            reader.GetRow(i, row)
            for j in range(region.GetCols()):
                row.GetFCELLValue(j, value)
                val = value.GetValueAsFloat()
                if newMin > val:
                    newMin = val
                if newMax < val:
                    newMax = val
                print val,
            print " "
        print " "

        print "Min ", newMin, min
        print "Max ", newMax, max
        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader

        self.assertEqual(val[0], min, "Error range is incorrect")
        self.assertEqual(val[1], max, "Error range is incorrect")
        self.assertEqual(reader.GetNumberOfRows(), region.GetRows(), "Error rows differ")
        self.assertEqual(reader.GetNumberOfCols(), region.GetCols(), "Error cols differ")

        reader.CloseMap()

    def testSimpleReadWriteCycleCELL(self):

        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(10)
        region.SetRows(10)
        region.AdjustRegionResolution()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_cell")

        value = vtkCELL()
        row = vtkGRASSRasterRow()
        row.AllocateCELL(region.GetCols())

        min = 100000000.0
        max = 0.0

        for i in range(region.GetRows()):
            for j in range(region.GetCols()):
                val = i + j + 100
                if min > val:
                    min = val
                if max < val:
                    max = val
                value.SetIntValue(val)
                row.SetCELLValue(j, value)
            writer.PutNextRow(row)

        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.OpenMap("test_cell")

        newMin = 100000000.0
        newMax = 0.0
        for i in range(region.GetRows()):
            reader.GetRow(i, row)
            for j in range(region.GetCols()):
                row.GetCELLValue(j, value)
                val = value.GetValueAsInt()
                if newMin > val:
                    newMin = val
                if newMax < val:
                    newMax = val
                print val,
            print " "
        print " "

        print "Min ", newMin, min
        print "Max ", newMax, max
        self.assertEqual(newMin, min, "Error while reading map")
        self.assertEqual(newMax, max, "Error while reading map")

        val = [0,0]
        reader.GetRange(val);
        print "Range ", val
        print reader

        self.assertEqual(val[0], min, "Error range is incorrect")
        self.assertEqual(val[1], max, "Error range is incorrect")
        self.assertEqual(reader.GetNumberOfRows(), region.GetRows(), "Error rows differ")
        self.assertEqual(reader.GetNumberOfCols(), region.GetCols(), "Error cols differ")

        reader.CloseMap()
        
    def testRasterSampling(self):
            
        region = vtkGRASSRegion()
        region.ReadCurrentRegion()
        region.SetCols(10)
        region.SetRows(10)
        region.SetNorthernEdge(10)
        region.SetSouthernEdge(0)
        region.SetEasternEdge(10)
        region.SetWesternEdge(0)
        region.AdjustRegionResolution()

        writer = vtkGRASSRasterMapWriter()
        writer.SetMapTypeToDCELL()
        writer.SetRegion(region)
        writer.UseUserDefinedRegion()
        writer.OpenMap("test_sample_dcell")

        data = vtkDoubleArray()
        data.SetNumberOfTuples(writer.GetNumberOfCols())

        for i in range(writer.GetNumberOfRows()):
            for j in range(writer.GetNumberOfCols()):
                val = i * (writer.GetNumberOfCols())  +  j
                data.SetTuple1(j, val)
                print val,
            writer.PutNextRow(data)
            print " "
        print " "

        writer.CloseMap()

        reader = vtkGRASSRasterMapReader()
        reader.UseRasterRegion()
        reader.OpenMap("test_sample_dcell")
                
        for i in range(reader.GetNumberOfRows()):
            for j in range(reader.GetNumberOfCols()): 
                
                row = reader.GetNumberOfRows() - 1 - i
                value = vtkDCELL()
                check = i * (reader.GetNumberOfCols())  +  j
                if reader.GetNearestSampleValue(row + 0.5, j + 0.5, value):
                    print "Nearest ", value.GetValueAsDouble(), "Check", check
                    self.assertEqual(value.GetValueAsDouble(), check, "Error in nearest neighbour sampling")
                if reader.GetBilinearSampleValue(row + 0.5, j + 0.5, value):
                    print "Bilinear ", value.GetValueAsDouble(), "Check", check
                    self.assertEqual(value.GetValueAsDouble(), check, "Error in bilinear sampling")
                if reader.GetBicubicSampleValue(row + 0.5, j + 0.5, value):
                    print "Bicubic ", value.GetValueAsDouble(), "Check", check
                    self.assertEqual(value.GetValueAsDouble(), check, "Error in bicubic sampling")

        reader.CloseMap()
        
if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSRasterMapReaderWriterTest)
    unittest.TextTestRunner(verbosity=2).run(suite)
