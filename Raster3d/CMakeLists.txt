#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (RASTER3D_SRCS
vtkGRASSRaster3dMapBase.cxx
vtkGRASSRaster3dMapReader.cxx
vtkGRASSRaster3dMapWriter.cxx
)

SET (RASTER3D_H
vtkGRASSRaster3dMapBase.h
vtkGRASSRaster3dMapReader.h
vtkGRASSRaster3dMapWriter.h
vtkGRASSBridgeRaster3dWin32Header.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${RASTER3D_SRCS})
SET(MY_H ${RASTER3D_H})
# Create the vtkGRASSBridgeRaster3d C++ library.
link_grass_bridge_library(vtkGRASSBridgeRaster3d)
