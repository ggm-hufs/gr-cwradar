INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_CWRADAR cwradar)

FIND_PATH(
    CWRADAR_INCLUDE_DIRS
    NAMES cwradar/api.h
    HINTS $ENV{CWRADAR_DIR}/include
        ${PC_CWRADAR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    CWRADAR_LIBRARIES
    NAMES gnuradio-cwradar
    HINTS $ENV{CWRADAR_DIR}/lib
        ${PC_CWRADAR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CWRADAR DEFAULT_MSG CWRADAR_LIBRARIES CWRADAR_INCLUDE_DIRS)
MARK_AS_ADVANCED(CWRADAR_LIBRARIES CWRADAR_INCLUDE_DIRS)

