################################################################################
# Author:	Soeren Gebbert
#               Parts of this code are from the great pyWPS from Jachym Cepicky:
#               http://pywps.wald.intevation.org/
#
# Copyright (C) 2009 Soeren Gebbert
#               mail to: soerengebbert <at> googlemail <dot> com
#
# License:
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

from subprocess import Popen
from optparse import OptionParser
import os
import os.path
import tempfile
from ParameterParser import *
from GrassSettings import *
from ProcessLogging import *

GRASS_LOCATION_NAME = "startLocation"
GRASS_WORK_LOCATION = "workLocation"
GRASS_MAPSET_NAME = "PERMANENT"

# This module needs an input file for processing. All input and output parameter
# are defined within this file. The file parser expects an input file exactly as
# defined below. All key names must be specified. New-lines between the key names are forbidden.
#
# The format of the input file is defined as:
#
# [System]
#  WorkDir= temporal created locations and mapsets are put in this directory
#  OutputDir= The output of the grass module is put in tis directory
#
# [GRASS]
#  GISBASE= the gisbase directory of grass
#  GRASS_ADDON_PATH= path to addon modules
#  GRASS_VERSION= the version of grass
#  Module= the name of the module which should be executed
#  LOCATION=Name of an existing location with an existing mapset PERMANENT, which should be used for processing, the mapsets are generated temporaly
#  LinkInput=TRUE/FALSE Try to link the input into the generated/existing location, default is TRUE
#  IgnoreProjection=TRUE/FALSE Ignore the projection settings when trying to import the input data (ignored if LinkInput is true), default is FALSE
#  UseXYLocation=TRUE/FALSE create only a XY location/mapset and import all data ignoring the projection information. The resolution will be set based on LiteralData or based on the first input raster resolution, default is FALSE
#
# [ComplexData]
#  Identifier=input
#  PathToFile=/tmp/srtm90.tiff
#  MimeType=text/xml
#  Encoding=UTF-8
#  Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
#
# [LiteralData]
#  Identifier=-p
#  DataType=boolean/integer/double/string
#  Value=true
#
# [ComplexOutput]
#  Identifier=output
#  PathToFile=/tmp/srtm90contour.gml
#  MimeType=text/xml
#  Encoding=UTF-8
#  Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
#
#
# Example with multiple LiteralData

"""
[System]
 WorkDir=/tmp
 OutputDir=/tmp
 
[GRASS]
 GISBASE=/home/soeren/src/grass7.0/grass_trunk/dist.i686-pc-linux-gnu
 GRASS_ADDON_PATH=
 GRASS_VERSION=7.0.svn
 Module=r.contour
 LOCATION=
 LinkInput=TRUE
 IgnoreProjection=FALSE
 UseXYLocation=FALSE
 
[ComplexData]
 Identifier=input
 PathToFile=/tmp/srtm90.tiff
 MimeType=image/tiff
 Encoding=
 Schema=

[LiteralData]
 Identifier=ns_resolution
 DataType=double
 Value=10

[LiteralData]
 Identifier=sw_resolution
 DataType=double
 Value=10

[LiteralData]
 Identifier=levels
 DataType=double
 Value=50

[LiteralData]
 Identifier=levels
 DataType=double
 Value=100

[LiteralData]
 Identifier=levels
 DataType=double
 Value=200

[LiteralData]
 Identifier=levels
 DataType=double
 Value=300

[ComplexOutput]
 Identifier=output
 PathToFile=/tmp/srtm90contour.gml
 MimeType=text/xml
 Encoding=UTF-8
 Schema=http://schemas.opengis.net/gml/3.1.0/polygon.xsd
"""

###############################################################################
###############################################################################
###############################################################################

class GrassModuleStarter(ModuleLogging):
    """This class does the following:

     The goal is to process the input data within grass in its own coordinate system without import
       and export

     Main steps are:

     1.) Parse the input file (may be KVP from a WPS execution request)
     2.) Create the new grass location with r.in.gdal or v.in.ogr with the input coordinate system
     3.) Use r.in.gdal/v.in.ogr and v/r.external to import the data into a newly created grass location
     4.) Start the grass module within this location
     5.) Use r.external.out/r.out.gdal and v.out.ogr to export the result data
     6.) Cleanup

     The steps in more detail:
     
     1.) Parse the input parameter and create the parameter map (GISBASE; work dir, ...)
     2.) Create a temporal directory in the work-dir based on temporal directoy creation of python
     3.) Create a temporal location and PERMANENT mapset to execute the ogr and gdal import modules
         * Create the environment for grass (GIS_LOCK, GISRC, GISBASE ...)
         * Write the gisrc file in the PERMANENT directory
         * create the WIND and DEFAULT_WIND file in the PERMANENT directory of the new location
     4.) Now create a new location/mapset with the coordinate system of the first complex input
         * Use r.in.gdal or v.in.ogr to create the new location without actually importing the map,
           log stdout and stderr of the import modules
         * Rewrite the gisrc with new location name (we work in PERMANENT mapset)
     5.) Link all other maps via r/v.external(TODO) into the new location, log stdout and stderr
         or import with r.in.gdal or v.in.org. This can be specified in the input file
     6.) Start the grass module, log stdout and stderr, provide the stdout as file
     7.) In case raster output should be created use r.out.gdal or use r.external.out(TODO) to force the direct creation
         of images output files, otherwise export the output with v.out.ogr, log stdout and stderr
     8.) Remove the temporal directory and exit properly

     In case an error occured, return an error code and write the error protocoll to stderr
     Create meaningful logfiles, so the user will be informed properly what was going wrong
       in case of an error (TODO)

    This python script is based on the latest grass7 svn version
    """
    ############################################################################
    def __init__(self, inputfile, logfile, module_output, module_error):

        ModuleLogging.__init__(self, logfile, module_output, module_error)

        self.inputCounter = 0
        self.outputCounter = 0

        # These maps are used to create the parameter for the grass command
        self.inputMap = {}
        self.outputMap = {}

        # the pid of the process which is currently executed, to be used for suspending
        self.runPID = -1

        self.inputParameter = InputParameter(self.logfile)
        try:
            self.inputParameter.parseFile(inputfile)
        except:
            self.LogError("Error parsing the input file")

        # Create the input parameter of literal data
        self.__createLiteralInputMap()
        # Create the output map for data export
        self.__createOutputMap()
        
        # Create a temporal directory for the location and mapset creation
        if self.inputParameter.workDir != "":
            try:
                self.gisdbase = tempfile.mkdtemp(dir=self.inputParameter.workDir)
            except:
                raise
        else:
            try:
                self.gisdbase = tempfile.mkdtemp()
            except:
                raise

        try:
            os.mkdir(os.path.join(self.gisdbase, GRASS_LOCATION_NAME))
            os.mkdir(os.path.join(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME))
        except:
            raise

        self.fullmapsetpath = os.path.join(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME)

        # set the evironment variables for grass (Unix system only)
        try:
            self.__setEnvironment()
        except:
            raise
        
        # gisrc and wind file creation
        try:
            self.gisrc = GrassGisRC(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME, self.logfile)
            self.gisrc.writeFile(tempdir = self.gisdbase)
            self.gisrcfile = self.gisrc.getFileName()
            print self.gisrcfile
        except:
            raise

        try:
            self.wind = GrassWindFile(self.gisdbase, GRASS_LOCATION_NAME, GRASS_MAPSET_NAME, self.logfile)
            self.windfile = self.wind.getFileName()
            print self.windfile
        except:
            raise

        # Create the new location based on the first input  and import all maps
        try:
            self.__importData()
        except:
            raise

        # start the grass module
        try:
            self.__startGrassModule()
        except:
            raise

        # now export the results
        try:
            self.__exportOutput()
        except:
            raise
        
        # remove the created directory
        try:
            print "Remove ", self.gisdbase
            os.rmdir(self.gisdbase)
        except:
            pass

    ############################################################################
    def __setEnvironment(self):
        # set the grass environment
        self.genv = GrassEnvironment(self.logfile)
        self.genv.env["GIS_LOCK"] = str(os.getpid())
        self.genv.env["GISBASE"] = self.inputParameter.grassGisBase
        self.genv.env["GISRC"] = os.path.join(self.gisdbase, "gisrc")
        self.genv.env["LD_LIBRARY_PATH"] = str(os.path.join(self.genv.env["GISBASE"], "lib"))
        self.genv.env["GRASS_VERSION"] = "7.0.svn"
        self.genv.env["GRASS_ADDON_PATH"] = self.inputParameter.grassAddonPath
        self.genv.env["PATH"] = str(os.path.join(self.genv.env["GISBASE"], "bin") + ":" + os.path.join(self.genv.env["GISBASE"], "scripts"))
        self.genv.setEnvVariables()
        self.genv.getEnvVariables()

    ############################################################################
    def __importData(self):
        print "Import data"

        list = self.inputParameter.complexDataList

        # The list may be empty
        if len(list) == 0:
            return

        #Debug
        #proc = Popen(["g.region", '-p'])
        #proc.communicate()

        # Create a new location based on the first input map
        self.__createInputLocation(list[0])

        # Rewrite the gisrc file to enable the new created location
        self.gisrc.locationName = GRASS_WORK_LOCATION
        self.gisrc.rewriteFile()

        # Set the region resolution in case resolution values are provided as literal data
        self.__setRegionResolution()
        
        #Debug
        #proc = Popen(["g.region", '-p'])
        #proc.communicate()

        if self.inputParameter.linkInput == "FALSE":
            for i in list:
                self.__importInput(i)
        else:
            # Link the inputs into the location
            for i in list:
                self.__linkInput(i)

            #Debug
            #proc = Popen(["r.info", str(self.inputMap[i.identifier])])
            #proc.communicate()

    ############################################################################
    def __isRaster(self, input):
        """Check for raster input"""
        if input.mimeType.upper() == "IMAGE/TIFF":
            print "Raster is TIFF"
            return "GTiff"
        elif input.mimeType.upper() == "IMAGE/PNG":
            print "Raster is PNG"
            return "PNG"
        else:
            return ""

    ############################################################################
    def __isVector(self, input):
        """Check for vector input"""
        if input.mimeType.upper() == "TEXT/XML" and input.schema.upper().find("GML") != -1:
            print "Vector is gml"
            return "GML"
        else:
            return ""

    ############################################################################
    def __isTextFile(self, input):
        """Check for vector input"""
        if input.mimeType.upper() == "TEXT/PLAIN":
            print "Text file"
            return "TXT"
        else:
            return ""

    ############################################################################
    def __createInputLocation(self, input):
        """Creat a new work location based on an input dataset"""
        print "Create new location"
        # TODO: implement correct and meaningful error handling and error messages
        
        if self.__isRaster(input) != "":
            proc = Popen(["r.in.gdal", "input=" + input.pathToFile, "location=" + GRASS_WORK_LOCATION , "-ce", "output=undefined"])
            self.runPID = proc.pid
            print self.runPID
            proc.communicate()

            if proc.returncode != 0:
                print "Unable to create new grass location based on input map"
                raise IOError

        elif self.__isVector(input) != "":
            proc = Popen(["v.in.ogr", "dsn=" + input.pathToFile, "location=" + GRASS_WORK_LOCATION , "-ce", "output=undefined"])
            self.runPID = proc.pid
            print self.runPID
            proc.communicate()

            if proc.returncode != 0:
                print "Unable to create new grass location based on input map"
                raise IOError
            
        elif self.__isTextFile(input) != "":
            return
        else:
            raise IOError

    ############################################################################
    def __setRegionResolution(self):
        # Set the region resolution accordingly to the literal input parameters
        print "Set the region resolution"
        values = 0
        ns = 10.0
        ew = 10.0
        for i in self.inputMap:
            if i == "ns_resolution" and self.inputMap[i] != "":
                print "Noticed ns_resolution"
                values += 1
            if i == "ew_resolution" and self.inputMap[i] != "":
                print "Notices ew_resolution"
                values += 1

            if values == 2:
                proc = Popen(["g.region", "ewres=" + str(ew), "nsres=" + str(ns)])
                proc.communicate()
                if proc.returncode != 0:
                    print "Unable to set the region resolution"
                    raise IOError

    ############################################################################
    def __linkInput(self, input):
        """Link the input data into a grass work location"""
        print "Link input"
        # TODO: implement correct and meaningful error handling and error messages

        inputName = "input_" + str(self.inputCounter)

        if self.__isRaster(input) != "":
            if self.inputParameter.ignoreProjection == "FALSE":
                proc = Popen(["r.external", "input=" + input.pathToFile, "output=" + inputName])
            else:
                proc = Popen(["r.external", "-o", "input=" + input.pathToFile, "output=" + inputName])

            self.runPID = proc.pid
            print self.runPID
            proc.communicate()

            # If the linking fails, import the data with r.in.gdal
            if proc.returncode != 0:
                print "Unable to link the raster map, try to import."
                try:
                    self.__importInput(input)
                except:
                    print "Unable to link or import the raster map into the grass mapset"
                    raise IOError
                return
            
            proc = Popen(["r.info", inputName ])
            proc.communicate()

        elif self.__isVector(input) != "":
            # Linking does not work properly right now for GML -> no random access, so we import the vector data
            self.__importInput(input)
            return

#            print "Linking of vector data is currently not supported"
#            raise IOError
#            # We need to check at first the layer within the vector dataset
#            proc = Popen(["v.external", "dsn=" + input.pathToFile, "layer=tmp", "output=" + inputName])
#            self.runPID = proc.pid
#            print self.runPID
#            proc.communicate()
#
#            if proc.returncode != 0:
#                print "Unable to link the vector map into the grass mapset"
#                raise IOError
#
#            proc = Popen(["v.info", inputName ])
#            proc.communicate()

        elif self.__isTextFile(input) != "":
            self.__updateInputMap(input, input.pathToFile)
            return
        else:
            raise IOError

        self.__updateInputMap(input, inputName)

    ############################################################################
    def __importInput(self, input):
        # TODO: implement correct and meaningful error handling and error messages

        inputName = "input_" + str(self.inputCounter)

        # import the raster data via gdal
        if self.__isRaster(input) != "":
            if self.inputParameter.ignoreProjection == "FALSE":
                proc = Popen(["r.in.gdal", "input=" + input.pathToFile, "output=" + inputName])
            else:
                proc = Popen(["r.in.gdal", "-o", "input=" + input.pathToFile, "output=" + inputName])
                
            self.runPID = proc.pid
            print self.runPID
            proc.communicate()

            if proc.returncode != 0:
                raise IOError

            proc = Popen(["r.info", inputName ])
            proc.communicate()
            
        # import the vector data via ogr
        elif self.__isVector(input) != "":
            proc = Popen(["v.in.ogr", "dsn=" + input.pathToFile, "output=" + inputName])
            self.runPID = proc.pid
            print self.runPID
            proc.communicate()
        
            if proc.returncode != 0:
                raise IOError

            proc = Popen(["v.info", inputName ])
            proc.communicate()

        # Text input file, no need to create a new name or for import, use the path as input
        elif self.__isTextFile(input) != "":
            self.__updateInputMap(input, input.pathToFile)
            return
        else:
            raise IOError

        self.__updateInputMap(input, inputName)

    ############################################################################
    def __updateInputMap(self, input, inputName):
        # Connect the values if multiple defined
        if self.inputMap.has_key(input.identifier):
            self.inputMap[input.identifier] += "," + inputName
        else:
            self.inputMap[input.identifier] = inputName
        self.inputCounter += 1

    ############################################################################
    def __createLiteralInputMap(self):
        """Create the entries of the input map for literal data"""
        list = self.inputParameter.literalDataList

        # The list may be empty
        if len(list) == 0:
            return

        for i in list:
            # Boolean values are unique and have no values eg -p or --o
            if i.type.upper() == "BOOLEAN":
                self.inputMap[i.identifier] = ""
            # Connect the values if multiple defined
            elif self.inputMap.has_key(i.identifier):
                self.inputMap[i.identifier] += "," + i.value
            else:
                self.inputMap[i.identifier] = i.value

    ############################################################################
    def __createOutputMap(self):
        """Create the entries of the output map for literal data"""
        list = self.inputParameter.complexOutputList

        # The list may be empty
        if len(list) == 0:
            return

        for i in list:
            outputName = "output_" + str(self.outputCounter)
            # Ignore if multiple defined
            if self.outputMap.has_key(i.identifier):
                pass
            else:
                self.outputMap[i.identifier] = outputName

            self.outputCounter += 1

    ############################################################################
    def __exportOutput(self):
        """Export the output"""
        # TODO: implement correct and meaningful error handling and error messages

        for output in self.inputParameter.complexOutputList:
            outputName = self.outputMap[output.identifier]

            # export the data via gdal
            if self.__isRaster(output) != "":
                proc = Popen(["r.out.gdal", "-c", "input=" + outputName, "format=" + self.__isRaster(output), "output=" + output.pathToFile])
                self.runPID = proc.pid
                print self.runPID
                proc.communicate()

                if proc.returncode != 0:
                    raise IOError

            # export the data via ogr
            elif self.__isVector(output) != "":
                proc = Popen(["v.out.ogr", "input=" + outputName, "format=" + self.__isVector(output),"dsn=" + output.pathToFile])
                self.runPID = proc.pid
                print self.runPID
                proc.communicate()

                if proc.returncode != 0:
                    raise IOError
            else:
                raise IOError

    ############################################################################
    def __startGrassModule(self):
        """Create the parameter list and start the grass module"""
        print "Start GRASS module ", self.inputParameter.grassModule
        parameterMap = []

        parameterMap.append(self.inputParameter.grassModule)

        for i in self.inputMap:
            # filter the resolution adjustment and the stdout output from the parameter list!
            if i != "ns_resolution" and i != "ew_resolution":
                if self.inputMap[i] != "":
                    parameterMap.append(i + "=" + self.inputMap[i])
                else:
                    parameterMap.append(i)

        for i in self.outputMap:
            parameterMap.append(i + "=" + self.outputMap[i])

        print parameterMap
        proc = Popen(parameterMap)
        self.runPID = proc.pid
        print self.runPID
        proc.communicate()

        if proc.returncode != 0:
            print "Error while executing the grass module"
            raise IOError

###############################################################################
###############################################################################
###############################################################################

def main():
    """The main function which will be called if the script is executed directly"""

    usage = "usage: %prog [-help,--help] --file inputfile.txt [--logfile log.txt] [--module_output mout.txt] [--module_error merror.txt]"
    description = "Use %prog to process geo-data with grass without the need to explicitely " +\
                  "generate a grass location and the import/export of the input and output geo-data. " +\
                  "This may helpful for WPS server or other web services providing grass geo-processing."
    parser = OptionParser(usage=usage, description=description)
    parser.add_option("-f", "--file", dest="filename",
                      help="The path to the input file", metavar="FILE")
    parser.add_option("-l", "--logfile", dest="logfile", default="logfile.txt", \
                      help="The name to the logfile. This file logs everything "\
                      "which happens in this module (import, export, location creation ...).", metavar="FILE")
    parser.add_option("-m", "--module_output", dest="module_output", default="logfile_module_output.txt",
                      help="The name to the file logging the messages to stdout "\
                      "of the called grass processing module (textual module output).", metavar="FILE")
    parser.add_option("-e", "--module_error", dest="module_error", default="logfile_module_error.txt",\
                      help="The name to the file logging the messages to stderr"\
                      " of the called grass processing module (warnings and errors).", metavar="FILE")

    (options, args) = parser.parse_args()


    if options.filename == None:
        parser.print_help()
        parser.error("A file name must be provided")

    starter = GrassModuleStarter(options.filename, options.logfile, options.module_output, options.module_error)
    exit(0)

###############################################################################
if __name__ == "__main__":
    main()
