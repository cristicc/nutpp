# Locates Log4cplus includes and library.
#
# This module defines the following variables:
#
#    LOG4CPLUS_FOUND            - true if Log4cplus found.
#    LOG4CPLUS_INCLUDE_DIRS     - where to find logger.h, etc.
#    LOG4CPLUS_LIBRARIES        - list of libraries when using Log4cplus.
#    LOG4CPLUS_VERSION_STRING   - version of Log4cplus found.
#
# Copyright (C) 2018, Cristian Ciocaltea <cristian.ciocaltea@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Look for the header file.
find_path(LOG4CPLUS_INCLUDE_DIR NAMES log4cplus/logger.h)

# Look for the library.
find_library(LOG4CPLUS_LIBRARY NAMES log4cplus)

if(LOG4CPLUS_INCLUDE_DIR
        AND EXISTS "${LOG4CPLUS_INCLUDE_DIR}/log4cplus/version.h")
    file(STRINGS "${LOG4CPLUS_INCLUDE_DIR}/log4cplus/version.h"
        log4cplus_version_str
        REGEX "^#define[\t ]+LOG4CPLUS_VERSION[\t ]+.*\(.*\)")

    string(REGEX REPLACE
        ".*[(][\t ]*([0-9]+)[\t ]*,[\t ]*([0-9]+)[\t ]*,[\t ]*([0-9]+)[\t ]*[)].*"
        "\\1.\\2.\\3"
        LOG4CPLUS_VERSION_STRING "${log4cplus_version_str}")
    unset(log4cplus_version_str)
endif()

# Handle the QUIETLY and REQUIRED arguments and set LOG4CPLUS_FOUND to TRUE
# if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Log4cplus
    REQUIRED_VARS LOG4CPLUS_LIBRARY LOG4CPLUS_INCLUDE_DIR
    VERSION_VAR LOG4CPLUS_VERSION_STRING)

mark_as_advanced(LOG4CPLUS_INCLUDE_DIR LOG4CPLUS_LIBRARY)

if(LOG4CPLUS_FOUND)
    set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIR})
    set(LOG4CPLUS_LIBRARIES ${LOG4CPLUS_LIBRARY})
endif()
