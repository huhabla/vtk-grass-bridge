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
from libvtkGRASSBridgeCommonPython import *


class GRASSModuleTest(unittest.TestCase):
    def testSmoke(self):

        init = vtkGRASSInit()
        init.Init("GRASSRegionTest")
        init.ExitOnErrorOn()

	module = vtkGRASSModule()
	module.SetDescription("Test my module description")
	module.SetLabel("label")
	module.AddKeyword("Test")
	module.AddKeyword("raster")

	flag = vtkGRASSFlag()
	flag.SetDescription("This is a flag")
	flag.SetKey('f')
	flag.SetGuiSection("Flags")

	option1 = vtkGRASSOption()
	option1.SetKey("option1")
	option1.SetDescription("This is option one")
	option1.SetGuiSection("Options")
	option1.SetGisprompt("old,raster,cell")
	option1.RequiredOff()
	option1.MultipleOff()
	option1.SetTypeToString()

	option2 = vtkGRASSOption()
	option2.SetKey("option2")
	option2.SetDescription("This is option two")
	option2.SetGuiSection("Options")
	option2.SetGisprompt("old,raster,cell")
	option2.RequiredOn()
	option2.MultipleOn()
	option2.SetTypeToString()

        # Using default options is implemented in the option factory
        option3 =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputType())
        option4 =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputType(), "elev")
        option5 =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetRasterInputType(), "topo", "The topographic index")
        option6 =  vtkGRASSOptionFactory().CreateInstance(vtkGRASSOptionFactory.GetDataBaseWhereType())

        argv = vtkStringArray()
        argv.InsertNextValue("test")
        argv.InsertNextValue("-f")
        argv.InsertNextValue("option1=test1")
        argv.InsertNextValue("option2=aa,bb,cc,dd")
        argv.InsertNextValue("input=test2")
        argv.InsertNextValue("elev=test3")
        argv.InsertNextValue("topo=test4")
        argv.InsertNextValue("where=income < 1000 and inhab >= 10000")

	init.Parser(argv)

        answers = vtkStringArray()
        option2.GetAnswers(answers)

        self.assertEqual(flag.GetAnswer(), True, "Flag is not functional")
        self.assertEqual(option1.GetAnswer(), "test1", "Options is not functional")
        self.assertEqual(option3.GetAnswer(), "test2", "Options is not functional")
        self.assertEqual(option4.GetAnswer(), "test3", "Options is not functional")
        self.assertEqual(option5.GetAnswer(), "test4", "Options is not functional")
        self.assertEqual(answers.GetValue(0), "aa", "Options is not functional")
        self.assertEqual(answers.GetValue(1), "bb", "Options is not functional")
        self.assertEqual(answers.GetValue(2), "cc", "Options is not functional")
        self.assertEqual(answers.GetValue(3), "dd", "Options is not functional")
        self.assertEqual(option6.GetAnswer(), "income < 1000 and inhab >= 10000", "Options is not functional")

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GRASSModuleTest)
    unittest.TextTestRunner(verbosity=2).run(suite)

