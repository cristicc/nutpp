##
# Locate Log4cplus includes and library.
#
# This module defines the following variables:
#
#    LOG4CPLUS_FOUND        - true if Log4cplus found.
#    LOG4CPLUS_INCLUDE_DIR  - where to find logger.h, etc.
#    LOG4CPLUS_LIBRARIES    - list of libraries when using Log4cplus.
#
# Copyright (C) 2018  Cristian Ciocaltea <cristian.ciocaltea@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
##

# Look for the header file.
find_path(LOG4CPLUS_INCLUDE_DIR NAMES logger.h PATH_PREFIXES log4cplus)

# Look for the library.
find_library(LOG4CPLUS_LIBRARY NAMES log4cplus)

# Handle the QUIETLY and REQUIRED arguments and set LOG4CPLUS_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Log4cplus DEFAULT_MSG
    LOG4CPLUS_LIBRARY LOG4CPLUS_INCLUDE_DIR)

# Copy the results to the output variables.
if(LOG4CPLUS_FOUND)
  set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_LIBRARY})
endif()

mark_as_advanced(LOG4CPLUS_INCLUDE_DIR LOG4CPLUS_LIBRARY)
