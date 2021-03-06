#
# Try to find VTK and include its settings (otherwise complain)
#
INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
# We need grass, so no option is defined
INCLUDE (CMake/FindGRASS.cmake)
INCLUDE (CMake/FindGEOS.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGDAL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)

IF (USE_VTK_FILE)
  INCLUDE (${USE_VTK_FILE})
ELSE (USE_VTK_FILE)
  SET (VTK_GRASS_BRIDGE_CAN_BUILD 0)
ENDIF (USE_VTK_FILE)


# Make the R support optional
OPTION(USE_VTK_R_SUPPORT
       "Use VTK's R support classes."
       OFF)
MARK_AS_ADVANCED (USE_VTK_R_SUPPORT)

# Make the temporal dependency (VTK 5.9) optional
OPTION(USE_VTK_TEMPORAL_SUPPORT
       "Use VTK's R support classes."
       OFF)
MARK_AS_ADVANCED (USE_VTK_TEMPORAL_SUPPORT)

#
# Defaults to the same VTK setting.
#

IF (USE_VTK_FILE)
  # Standard CMake option for building libraries shared or static by default.
  OPTION(BUILD_SHARED_LIBS 
         "Build with shared libraries." 
         ${VTK_BUILD_SHARED_LIBS})
  # Copy the CMake option to a setting with VTK_GRASS_BRIDGE_ prefix for use in
  # our project.  This name is used in vtkGRASSBridgeLibraryConfigure.h.in.
  SET(VTK_GRASS_BRIDGE_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

  #
  # Output path(s)
  #
  SET (LIBRARY_OUTPUT_PATH ${VTK_GRASS_BRIDGE_BINARY_DIR}/bin CACHE PATH
       "Single output directory for building all libraries.")

  SET (EXECUTABLE_OUTPUT_PATH ${VTK_GRASS_BRIDGE_BINARY_DIR}/bin CACHE PATH
       "Single output directory for building all executables.")

  MARK_AS_ADVANCED (
    LIBRARY_OUTPUT_PATH 
    EXECUTABLE_OUTPUT_PATH
  )

  # If this is a build tree, provide an option for putting
  # this project's executables and libraries in with VTK's.
  IF (EXISTS ${VTK_DIR}/bin)
    OPTION(USE_VTK_OUTPUT_PATHS
           "Use VTK's output directory for this project's executables and libraries."
           OFF)
    MARK_AS_ADVANCED (USE_VTK_OUTPUT_PATHS)
    IF (USE_VTK_OUTPUT_PATHS)
      SET (LIBRARY_OUTPUT_PATH ${VTK_DIR}/lib)
      SET (EXECUTABLE_OUTPUT_PATH ${VTK_DIR}/bin)
    ENDIF (USE_VTK_OUTPUT_PATHS)
  ENDIF (EXISTS ${VTK_DIR}/bin)

#
# Python Wrapping
#

IF (VTK_WRAP_PYTHON)

  OPTION(VTK_GRASS_BRIDGE_WRAP_PYTHON
         "Wrap classes into the Python interpreted language."
         ON)

  IF (VTK_GRASS_BRIDGE_WRAP_PYTHON)
    INCLUDE(${VTK_CMAKE_DIR}/vtkWrapPython.cmake)
    IF (WIN32)
      IF (NOT BUILD_SHARED_LIBS)
        MESSAGE(FATAL_ERROR "Python support requires BUILD_SHARED_LIBS to be ON.")
        SET (VTK_GRASS_BRIDGE_CAN_BUILD 0)
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (WIN32)
  ENDIF (VTK_GRASS_BRIDGE_WRAP_PYTHON)

ELSE (VTK_WRAP_PYTHON)

  IF (VTK_GRASS_BRIDGE_WRAP_PYTHON)
    MESSAGE("Warning. VTK_GRASS_BRIDGE_WRAP_PYTHON is ON but the VTK version you have "
            "chosen has not support for Python (VTK_WRAP_PYTHON is OFF).  "
            "Please set VTK_GRASS_BRIDGE_WRAP_PYTHON to OFF.")
    SET (VTK_GRASS_BRIDGE_WRAP_PYTHON OFF)
  ENDIF (VTK_GRASS_BRIDGE_WRAP_PYTHON)

ENDIF (VTK_WRAP_PYTHON)

#
# Java Wrapping
#

IF (VTK_WRAP_JAVA)

  OPTION(VTK_GRASS_BRIDGE_WRAP_JAVA
         "Wrap classes into the Java interpreted language."
         OFF)

  IF (VTK_GRASS_BRIDGE_WRAP_JAVA)
    SET(VTK_WRAP_JAVA3_INIT_DIR "${VTK_GRASS_BRIDGE_SOURCE_DIR}/Wrapping")
    INCLUDE(${VTK_CMAKE_DIR}/vtkWrapJava.cmake)
    IF (WIN32)
      IF (NOT BUILD_SHARED_LIBS)
        MESSAGE(FATAL_ERROR "Java support requires BUILD_SHARED_LIBS to be ON.")
        SET (VTK_GRASS_BRIDGE_CAN_BUILD 0)
      ENDIF (NOT BUILD_SHARED_LIBS)
    ENDIF (WIN32)

    # Tell the java wrappers where to go.
    SET(VTK_JAVA_HOME ${VTK_GRASS_BRIDGE_BINARY_DIR}/java/vtk)
    MAKE_DIRECTORY(${VTK_JAVA_HOME})
  ENDIF (VTK_GRASS_BRIDGE_WRAP_JAVA)

ELSE (VTK_WRAP_JAVA)

  IF (VTK_GRASS_BRIDGE_WRAP_JAVA)
    MESSAGE("Warning. VTK_GRASS_BRIDGE_WRAP_JAVA is ON but the VTK version you have "
            "chosen has not support for Java (VTK_WRAP_JAVA is OFF).  "
            "Please set VTK_GRASS_BRIDGE_WRAP_JAVA to OFF.")
    SET (VTK_GRASS_BRIDGE_WRAP_JAVA OFF)
  ENDIF (VTK_GRASS_BRIDGE_WRAP_JAVA)
ENDIF (VTK_WRAP_JAVA)

SET(VTK_WRAP_HINTS ${VTK_GRASS_BRIDGE_SOURCE_DIR}/Wrapping/hints)

ENDIF (USE_VTK_FILE)

