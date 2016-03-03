#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (DBMI_SRCS
vtkGRASSDbmiCatValueArray.cxx
vtkGRASSDbmiColumn.cxx
vtkGRASSDbmiInterface.cxx
vtkGRASSDbmiTable.cxx
vtkGRASSDbmiValue.cxx
)

SET (DBMI_H
vtkGRASSDbmiCatValueArray.h
vtkGRASSDbmiColumn.h
vtkGRASSDbmiInterface.h
vtkGRASSDbmiTable.h
vtkGRASSDbmiValue.h
vtkGRASSBridgeDbmiWin32Header.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${DBMI_SRCS})
SET(MY_H ${DBMI_H})
# Create the vtkGRASSBridgeDbmi C++ library.
link_grass_bridge_library(vtkGRASSBridgeDbmi)

