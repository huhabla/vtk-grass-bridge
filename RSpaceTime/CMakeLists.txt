#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (RSpaceTime_SRCS
  vtkRInterfaceSpatial.cxx
  vtkRInterfaceSpaceTime.cxx
)

SET (RSpaceTime_H
  vtkGRASSBridgeRSpaceTimeWin32Header.h
  vtkRInterfaceSpatial.h
  vtkRInterfaceSpaceTime.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${RSpaceTime_SRCS})
SET(MY_H ${RSpaceTime_H})
# Create the vtkGRASSBridgeRSpaceTime C++ library.
link_grass_bridge_library(vtkGRASSBridgeRSpaceTime)
