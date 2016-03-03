#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (IO_SRCS
vtkGRASSRasterImageReader.cxx
vtkGRASSRaster3dImageReader.cxx
vtkGRASSRasterImageWriter.cxx
vtkGRASSRaster3dImageWriter.cxx
vtkGRASSRasterToImageReader.cxx
vtkGRASSVectorPolyDataReader.cxx
vtkGRASSVectorPolyDataWriter.cxx
vtkGRASSVectorPolyDataAreaWriter.cxx
vtkGRASSVectorTopoPolyDataReader.cxx
vtkGRASSMultiVectorPolyDataLineReader.cxx
vtkGRASSMultiRasterPolyDataLineReader.cxx
vtkGRASSMultiRasterPolyDataLineWriter.cxx
)

SET (IO_H
vtkGRASSRasterImageReader.h
vtkGRASSRaster3dImageReader.h
vtkGRASSRasterImageWriter.h
vtkGRASSRaster3dImageWriter.h
vtkGRASSRasterToImageReader.h
vtkGRASSVectorPolyDataReader.h
vtkGRASSVectorPolyDataWriter.h
vtkGRASSVectorPolyDataAreaWriter.h
vtkGRASSVectorTopoPolyDataReader.h
vtkGRASSBridgeIOWin32Header.h
vtkGRASSMultiVectorPolyDataLineReader.h
vtkGRASSMultiRasterPolyDataLineReader.h
vtkGRASSMultiRasterPolyDataLineWriter.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${IO_SRCS})
SET(MY_H ${IO_H})
# Create the vtkGRASSBridgeIO C++ library.
link_grass_bridge_library(vtkGRASSBridgeIO)


