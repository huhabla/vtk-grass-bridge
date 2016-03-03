#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (Filtering_SRCS
vtkGRASSBridgeVoxelModeller.cxx
)

SET (Filtering_H
vtkGRASSBridgeVoxelModeller.h
vtkGRASSBridgeFilteringWin32Header.h
)
# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${Filtering_SRCS})
SET(MY_H ${Filtering_H})
# Create the vtkGRASSBridgeFiltering C++ library.
link_grass_bridge_library(vtkGRASSBridgeFiltering)

