# Locate geos
#
# This module accepts the following environment variables:
#
#    GEOS_DIR or GEOS_ROOT - Specify the location of GEOS
#
# This module defines the following CMake variables:
#
#    GEOS_FOUND - True if libgeos is found
#    GEOS_LIBRARY - A variable pointing to the GEOS library
#    GEOS_INCLUDE_DIR - Where to find the headers
#
#include "geos.h"

FIND_PATH(GEOS_INCLUDE_DIR geos.h
  HINTS
    $ENV{GEOS_DIR}
    $ENV{GEOS_ROOT}
  PATH_SUFFIXES
     include/geos
     include/GEOS
     include
  PATHS
      /usr/
)

IF(UNIX)
    # Use geos-config to obtain the library version
    FIND_PROGRAM(GEOS_CONFIG geos-config
        HINTS
          $ENV{GEOS_DIR}
          $ENV{GEOS_ROOT}
        PATH_SUFFIXES bin
        PATHS
            /usr/
    )

    if(GEOS_CONFIG)
        exec_program(${GEOS_CONFIG} ARGS --libs OUTPUT_VARIABLE GEOS_CONFIG_LIBS)
    endif()
endif()

FIND_LIBRARY(GEOS_LIBRARY
  NAMES ${_geos_lib} geos
  HINTS
     $ENV{GEOS_DIR}
     $ENV{GEOS_ROOT}
     ${_geos_libpath}
  PATH_SUFFIXES lib64 lib
  PATHS
    /usr/
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GEOS DEFAULT_MSG GEOS_LIBRARY GEOS_INCLUDE_DIR)

set(GEOS_LIBRARIES ${GEOS_LIBRARY})
set(GEOS_INCLUDE_DIRS ${GEOS_INCLUDE_DIR})
