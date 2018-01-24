#
# Try to find Log4cplus.
#
# Once done this will define:
#  LOG4CPLUS_FOUND - System has Log4cplus
#  LOG4CPLUS_INCLUDE_DIRS - The Log4cplus include directories
#  LOG4CPLUS_LIBRARIES - The libraries needed to use Log4cplus

find_path(LOG4CPLUS_INCLUDE_DIRS
  NAMES
    log4cplus/config.hxx
  PATHS
    /usr/local/include
    /usr/include
    $ENV{LOG4CPLUS_ROOT}/include
    ${LOG4CPLUS_ROOT}/include
)

find_library(LOG4CPLUS_LIBRARIES
  NAMES
    log4cplus log4cplusSD log4cplusD log4cplusS
  PATHS
    /usr/local
    /usr
    $ENV{LOG4CPLUS_ROOT}/lib
    ${LOG4CPLUS_ROOT}/lib
)

include(FindPackageHandleStandardArgs)

# Handle the QUIETLY and REQUIRED arguments and set LOG4CPLUS_FOUND to TRUE
# if all listed variables are TRUE.
find_package_handle_standard_args(
    Log4cplus
    "Could NOT find Log4cplus, try to set the path to Log4cplus root folder in the system variable LOG4CPLUS_ROOT"
    LOG4CPLUS_INCLUDE_DIRS LOG4CPLUS_LIBRARIES
)

mark_as_advanced(LOG4CPLUS_INCLUDE_DIRS LOG4CPLUS_LIBRARIES)
