#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (RASTER_SRCS
vtkGRASSRasterRow.cxx
vtkGRASSRasterMemoryMap.cxx
vtkGRASSRasterMapBase.cxx
vtkGRASSRasterMapReader.cxx
vtkGRASSRasterMapWriter.cxx
vtkGRASSHistory.cxx
)

SET (RASTER_H
vtkGRASSRasterRow.h
vtkGRASSRasterMemoryMap.h
vtkGRASSRasterMapBase.h
vtkGRASSRasterMapReader.h
vtkGRASSRasterMapWriter.h
vtkGRASSHistory.h
vtkGRASSBridgeRasterWin32Header.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${RASTER_SRCS})
SET(MY_H ${RASTER_H})
# Create the vtkGRASSBridgeRaster C++ library.
link_grass_bridge_library(vtkGRASSBridgeRaster)

