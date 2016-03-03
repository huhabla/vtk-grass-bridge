#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (Temporal_SRCS
vtkTemporalDataSetSource.cxx
)

SET (Temporal_H
vtkGRASSBridgeTemporalWin32Header.h
vtkTemporalDataSetSource.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${Temporal_SRCS})
SET(MY_H ${Temporal_H})
# Create the vtkGRASSBridgeTemporal C++ library.
link_grass_bridge_library(vtkGRASSBridgeTemporal)
