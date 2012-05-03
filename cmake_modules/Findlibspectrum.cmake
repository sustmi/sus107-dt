# - Try to find libspectrum
# Once done this will define
#  LIBSPECTRUM_FOUND - System has libspectrum
#  LIBSPECTRUM_INCLUDE_DIRS - The libspectrum include directories
#  LIBSPECTRUM_LIBRARIES - The libraries needed to use libspectrum
#  LIBSPECTRUM_DEFINITIONS - Compiler switches required for using libspectrum

find_package(PkgConfig)
pkg_check_modules(PC_LIBSPECTRUM QUIET libspectrum)
set(LIBSPECTRUM_DEFINITIONS ${PC_LIBSPECTRUM_CFLAGS_OTHER})

find_path(LIBSPECTRUM_INCLUDE_DIR libspectrum.h
          HINTS ${PC_LIBSPECTRUM_INCLUDEDIR} ${PC_LIBSPECTRUM_INCLUDE_DIRS} )

find_library(LIBSPECTRUM_LIBRARY NAMES spectrum libspectrum
             HINTS ${PC_LIBSPECTRUM_LIBDIR} ${PC_LIBSPECTRUM_LIBRARY_DIRS} )

set(LIBSPECTRUM_LIBRARIES ${LIBSPECTRUM_LIBRARY} )
set(LIBSPECTRUM_INCLUDE_DIRS ${LIBSPECTRUM_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBSPECTRUM_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libspectrum  DEFAULT_MSG
                                  LIBSPECTRUM_LIBRARY LIBSPECTRUM_INCLUDE_DIR)

mark_as_advanced(LIBSPECTRUM_INCLUDE_DIR LIBSPECTRUM_LIBRARY )
