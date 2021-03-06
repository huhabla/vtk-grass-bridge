#SUBDIRS(Testing)

#
# Source files
#
# Here is where you can add the name of your local unsorted classes.
#

SET (IO_SRCS
vtkGRASSRaster3dMapBase.cxx
vtkGRASSRaster3dMapReader.cxx
vtkGRASSRaster3dMapWriter.cxx
)

SET (IO_H
vtkGRASSRaster3dMapBase.h
vtkGRASSRaster3dMapReader.h
vtkGRASSRaster3dMapWriter.h
vtkGRASSBridgeRaster3dWin32Header.h
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

# Create the vtkGRASSBridgeRaster C++ library.
ADD_LIBRARY (vtkGRASSBridgeRaster3d ${IO_SRCS}  ${IO_H})
TARGET_LINK_LIBRARIES(vtkGRASSBridgeRaster3d vtkGRASSBridgeRaster vtkGRASSBridgeCommon vtkCommon  ${GRASS_LIBRARIES})

# Create the vtkGRASSBridgeRaster Java library.
IF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)
  VTK_WRAP_JAVA3 (vtkGRASSBridgeRaster3dJava IOJava_SRCS "${IO_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeRaster3dJava SHARED ${IOJava_SRCS})
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster3dJava vtkGRASSBridgeRaster3d vtkGRASSBridgeRaster vtkGRASSBridgeCommonJava vtkCommonJava)

  INSTALL_TARGETS(/lib vtkGRASSBridgeRaster3dJava)
ENDIF (VTK_WRAP_JAVA AND VTK_GRASS_BRIDGE_WRAP_JAVA)

# Create the vtkGRASSBridgeLibraryFilter Python library.
IF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)
  VTK_WRAP_PYTHON3 (vtkGRASSBridgeRaster3dPython IOPython_SRCS "${IO_SRCS}")
  ADD_LIBRARY (vtkGRASSBridgeRaster3dPythonD ${IOPython_SRCS})
  ADD_LIBRARY (vtkGRASSBridgeRaster3dPython MODULE vtkGRASSBridgeRaster3dPythonInit.cxx)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeRaster3dPythonD vtkGRASSBridgeRaster3d vtkGRASSBridgeCommonPythonD vtkGRASSBridgeRasterPythonD)
  TARGET_LINK_LIBRARIES(vtkGRASSBridgeRaster3dPython vtkGRASSBridgeRaster3dPythonD)

  INSTALL_TARGETS(/lib vtkGRASSBridgeRaster3dPythonD vtkGRASSBridgeRaster3dPython)
ENDIF (VTK_WRAP_PYTHON AND VTK_GRASS_BRIDGE_WRAP_PYTHON)

IF (VTK_USE_HYBRID)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster3d vtkHybrid)
ENDIF (VTK_USE_HYBRID)

IF (VTK_USE_PARALLEL)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster3d vtkParallel)
ENDIF (VTK_USE_PARALLEL)

IF (VTK_USE_PATENTED)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster3d vtkPatented)
ENDIF (VTK_USE_PATENTED)

IF (VTK_USE_RENDERING)
  TARGET_LINK_LIBRARIES (vtkGRASSBridgeRaster3d vtkRendering)
ENDIF (VTK_USE_RENDERING)

INSTALL_FILES(/include .h ${IO_H})
INSTALL_TARGETS(/lib vtkGRASSBridgeRaster3d)

