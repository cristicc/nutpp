##
# Locate Wt includes and library.
#
# This module defines the following variables:
#
#    WT_FOUND        - true if Wt found.
#    WT_INCLUDE_DIR  - where to find WApplication.h, etc.
#    WT_LIBRARIES    - list of libraries when using Wt.
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
find_path(WT_INCLUDE_DIR NAMES WApplication.h PATH_PREFIXES Wt)

# Look for the libraries.
find_library(WT_LIBRARY NAMES wt)
find_library(WT_HTTP_LIBRARY NAMES wthttp)

# Handle the QUIETLY and REQUIRED arguments and set WT_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Wt DEFAULT_MSG WT_LIBRARY WT_INCLUDE_DIR)

# Copy the results to the output variables.
if(WT_FOUND)
  set(WT_LIBRARIES ${WT_LIBRARY} ${WT_HTTP_LIBRARY})
endif()

mark_as_advanced(WT_INCLUDE_DIR WT_LIBRARY WT_HTTP_LIBRARY)
