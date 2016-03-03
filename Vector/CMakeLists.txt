#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (VECTOR_SRCS
vtkGRASSVectorFeaturePoints.cxx
vtkGRASSVectorFeatureCats.cxx
vtkGRASSVectorMapBase.cxx
vtkGRASSVectorMapWriter.cxx
vtkGRASSVectorMapUpdater.cxx
vtkGRASSVectorMapTopoReader.cxx
vtkGRASSVectorMapNoTopoReader.cxx
vtkGRASSVectorBBox.cxx
)

SET (VECTOR_H
vtkGRASSVectorFeaturePoints.h
vtkGRASSVectorFeatureCats.h
vtkGRASSVectorMapBase.h
vtkGRASSVectorMapWriter.h
vtkGRASSVectorMapUpdater.h
vtkGRASSVectorMapTopoReader.h
vtkGRASSVectorMapNoTopoReader.h
vtkGRASSVectorBBox.h
vtkGRASSBridgeVectorWin32Header.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${VECTOR_SRCS})
SET(MY_H ${VECTOR_H})
# Create the vtkGRASSBridgeVector C++ library.
link_grass_bridge_library(vtkGRASSBridgeVector)

