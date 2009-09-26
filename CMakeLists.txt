#
# Set the project name.
#

PROJECT (VTK_GRASS_BRIDGE)

# Minimale Version von cmake
cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#
# Assume everything is set up correctly for build.
#
# If some requirements are not met, set it to 0.
# In that case, the build won't be done.
#

SET (VTK_GRASS_BRIDGE_CAN_BUILD 1)

#
# Load CMake commands that you probably should not modify.
#

INCLUDE (${VTK_GRASS_BRIDGE_SOURCE_DIR}/CMakeOptions.cmake)

#
# If everything is OK, then go into the sub directories and build.
#

IF (VTK_GRASS_BRIDGE_CAN_BUILD)

  #
  # Here is where you can list the sub-directories holding your local 
  # classes. Sorting classes by 'package' type like VTK does (Common, 
  # Rendering, Filtering, Imaging, IO, etc.) is a good thing and prevents 
  # numerous dependencies problems.
  #

  #Common, Filtering und IO sollte immer gebaut werden
  SUBDIRS (
     Common
#     Filtering
     IO
     Raster
     Vector
  )
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Common)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Filtering)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/IO)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Raster)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Vector)
  INCLUDE_DIRECTORIES (${GRASS_INCLUDE_DIR})

  # Die Visualisierungsbibliothek ist optional, da nur von FePview verwendet
#  IF (BUILD_GRASS_VISUALIZATION)
    # Diese Bibliotheken dienen zur Darstellung
#      SUBDIRS (
#         Graphics     
#      )
#      INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Graphics)
#  ENDIF (BUILD_FP_VISUALIZATION)

  CONFIGURE_FILE(
    ${VTK_GRASS_BRIDGE_SOURCE_DIR}/vtkGRASSBridgeConfigure.h.in 
    ${VTK_GRASS_BRIDGE_BINARY_DIR}/vtkGRASSBridgeConfigure.h
  )

  INCLUDE_DIRECTORIES(${VTK_GRASS_BRIDGE_BINARY_DIR})

ENDIF (VTK_GRASS_BRIDGE_CAN_BUILD)