#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (COMMON_SRCS
vtkGRASSDatabaseInfo.cxx
vtkGRASSFileLister.cxx
vtkGRASSRasterMapLister.cxx
vtkGRASSRaster3dMapLister.cxx
vtkGRASSVectorMapLister.cxx
vtkGRASSRegion.cxx
vtkObjectGRASSErrorHandler.cxx
vtkGRASSInit.cxx
vtkCELL.cxx
vtkFCELL.cxx
vtkDCELL.cxx
vtkGRASSModule.cxx
vtkGRASSFlag.cxx
vtkGRASSOption.cxx
vtkGRASSOptionFactory.cxx
vtkGRASSMessagingInterface.cxx
)

SET (COMMON_H
vtkGRASSDatabaseInfo.h
vtkGRASSFileLister.h
vtkGRASSRasterMapLister.h
vtkGRASSRaster3dMapLister.h
vtkGRASSVectorMapLister.h
vtkGRASSRegion.h
vtkGRASSInit.h
vtkCELL.h
vtkFCELL.h
vtkDCELL.h
vtkGRASSModule.h
vtkGRASSFlag.h
vtkGRASSOption.h
vtkGRASSOptionFactory.h
vtkObjectGRASSErrorHandler.h
vtkGRASSMessagingInterface.h
vtkGRASSBridgeCommonWin32Header.h
)

# --------------------------------------------------------------------------
# Set the MY_SRCS and MY_H variables that are used in the
# link_grass_bridge_library function

SET(MY_SRCS ${COMMON_SRCS})
SET(MY_H ${COMMON_H})
# Create the vtkGRASSBridgeCommon C++ library.
link_grass_bridge_library(vtkGRASSBridgeCommon)
