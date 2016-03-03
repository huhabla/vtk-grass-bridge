#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (SRCS
v.sample.rast.cxx
)


# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the executable C++ library.
ADD_EXECUTABLE (v.sample.rast ${SRCS})
TARGET_LINK_LIBRARIES(v.sample.rast vtkGRASSBridgeCommon vtkGRASSBridgeDbmi vtkGRASSBridgeRaster vtkGRASSBridgeVector ${VTK_LIBRARIES}  ${GRASS_LIBRARIES})

INSTALL_TARGETS(/bin v.sample.rast)

