#
# Find the native GRASS includes and library
#
# This module defines
# GRASS_INCLUDE_DIR, where to find gis.h, raster.h, vector.h ...
# GRASS_LIBRARY_DIR, where to find lib_gis, lib_raster  ...
# GRASS_LIBRARIES, the libraries to link against to use GRASS.
# GRASS_FOUND, If false, do not try to use GRASS.

FIND_PATH(GRASS_INCLUDE_DIR grass/gis.h
  /home/soeren/grass/grass7_trunk/dist.x86_64-pc-linux-gnu/include
  /usr/local/include
  /usr/include
)

FIND_PATH(GRASS_LIBRARY_DIR grass_gis.7.1.svn
  /home/soeren/grass/grass7_trunk/dist.x86_64-pc-linux-gnu/lib
  /usr/local/lib
  /usr/lib
)

IF(GRASS_LIBRARY_DIR)
    FIND_LIBRARY(GRASS_GIS_LIBRARY grass_gis.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_RASTER_LIBRARY grass_raster.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_VECTOR_LIBRARY grass_vector.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_RASTER3D_LIBRARY grass_g3d.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_GMATH_LIBRARY grass_gmath.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_GPDE_LIBRARY grass_gpde.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_DBMIBASE_LIBRARY grass_dbmibase.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_DBMIDRIVER_LIBRARY grass_dbmidriver.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_DBMICLIENT_LIBRARY grass_dbmiclient.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
    FIND_LIBRARY(GRASS_DATETIME_LIBRARY grass_datetime.7.1.svn
        ${GRASS_LIBRARY_DIR}
    )
ENDIF(GRASS_LIBRARY_DIR)

IF(GRASS_INCLUDE_DIR)
  IF(GRASS_GIS_LIBRARY)
    IF(GRASS_RASTER3D_LIBRARY)
      IF(GRASS_VECTOR_LIBRARY)
        SET( GRASS_FOUND "YES" )
        SET( GRASS_BASIC_LIBRARIES 
          ${GRASS_GIS_LIBRARY}
          ${GRASS_DATETIME_LIBRARY} 
          ${GRASS_RASTER_LIBRARY}
          ${GRASS_RASTER3D_LIBRARY}
          ${GRASS_VECTOR_LIBRARY}
          )
        SET( GRASS_LIBRARIES 
          ${GRASS_BASIC_LIBRARIES}
          ${GRASS_DBMIBASE_LIBRARY}
          ${GRASS_DBMICLIENT_LIBRARY}
          ${GRASS_DBMIDRIVER_LIBRARY}
          ${GRASS_GMATH_LIBRARY}
          ${GRASS_GPDE_LIBRARY})
      ENDIF(GRASS_VECTOR_LIBRARY)
    ENDIF(GRASS_RASTER3D_LIBRARY)
  ENDIF(GRASS_GIS_LIBRARY)
ENDIF(GRASS_INCLUDE_DIR)

MARK_AS_ADVANCED(
  GRASS_GIS_LIBRARY
  GRASS_DATETIME_LIBRARY
  GRASS_RASTER3D_LIBRARY
  GRASS_RASTER_LIBRARY
  GRASS_VECTOR_LIBRARY
  GRASS_GMATH_LIBRARY
  GRASS_GPDE_LIBRARY
  GRASS_DBMICLIENT_LIBRARY
  GRASS_DBMIDRIVER_LIBRARY
  GRASS_DBMIBASE_LIBRARY
  )
