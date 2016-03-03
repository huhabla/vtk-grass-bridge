#
# Set the project name.
#

PROJECT (VTK_GRASS_BRIDGE)

# Minimale Version von cmake
cmake_minimum_required(VERSION 2.8.7)
find_package(VTK 7.0 REQUIRED)

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

  SUBDIRS (
     Common
     DBMI
     Filtering
     Graphics
     IO
     Modules
     Raster
     Raster3d
     Vector
  )

  if (USE_VTK_R_SUPPORT)
     SUBDIRS (
         RSpaceTime
      )
     INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/RSpaceTime)
  ENDIF(USE_VTK_R_SUPPORT)

  if (USE_VTK_TEMPORAL_SUPPORT)
     SUBDIRS (
         Temporal
      )
     INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Temporal)
  ENDIF(USE_VTK_TEMPORAL_SUPPORT)


  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Common)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/DBMI)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Filtering)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Graphics)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/IO)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Raster)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Raster3d)
  INCLUDE_DIRECTORIES (${VTK_GRASS_BRIDGE_SOURCE_DIR}/Vector)
  INCLUDE_DIRECTORIES (${GRASS_INCLUDE_DIR})
  IF (GDAL_FOUND)
    INCLUDE_DIRECTORIES (${GDAL_INCLUDE_DIRS})
  ENDIF (GDAL_FOUND)
  IF (GEOS_FOUND)
    INCLUDE_DIRECTORIES (${GEOS_INCLUDE_DIRS})
  ENDIF (GEOS_FOUND)
  IF (VTK_GRASS_BRIDGE_WRAP_PYTHON)
    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
  ENDIF (VTK_GRASS_BRIDGE_WRAP_PYTHON)

  CONFIGURE_FILE(
    ${VTK_GRASS_BRIDGE_SOURCE_DIR}/vtkGRASSBridgeConfigure.h.in
    ${VTK_GRASS_BRIDGE_BINARY_DIR}/vtkGRASSBridgeConfigure.h
  )

  INCLUDE_DIRECTORIES(${VTK_GRASS_BRIDGE_BINARY_DIR})

  # Install the configure file
  INSTALL_FILES(/include .h ${VTK_GRASS_BRIDGE_BINARY_DIR}/vtkGRASSBridgeConfigure.h)

ENDIF (VTK_GRASS_BRIDGE_CAN_BUILD)
