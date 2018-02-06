# This CMake package creates a static library target.
#
# The following variables are used as input for the script:
#
#    LIBRARY_NAME   - name of the library without the "lib" prefix (mandatory)
#    LIBRARY_SRCS   - listing of the sources to be build (mandatory)
#    LIBRARY_LIBS   - listing of the libraries to be linked with (optional)
#
# The target name to build the library will be "${PROJECT_NAME}-${LIBRARY_NAME}"
# and can be referenced using ${LIBRARY_TARGET} variable.
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

# Check mandatory settings.
if("${LIBRARY_NAME}" STREQUAL "")
    message(FATAL_ERROR
        "Please set 'LIBRARY_NAME' before using 'StaticLibraryTargets'!")
endif()
if("${LIBRARY_SRCS}" STREQUAL "")
    message(FATAL_ERROR
        "Please set 'LIBRARY_SRCS' before using 'StaticLibraryTargets'!")
endif()

# Set full library name and unit tests dir.
set(LIBRARY_TARGET ${PROJECT_NAME}-${LIBRARY_NAME})
set(LIBRARY_UT_DIR ut)

# Create build target for static library.
add_library(
    ${LIBRARY_TARGET} STATIC ${LIBRARY_SRCS})
set_target_properties(
    ${LIBRARY_TARGET} PROPERTIES OUTPUT_NAME ${LIBRARY_TARGET})
if(NOT "${LIBRARY_LIBS}" STREQUAL "")
    target_link_libraries(${LIBRARY_TARGET} ${LIBRARY_LIBS})
endif()

# Enable unit tests if available.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_UT_DIR}/CMakeLists.txt")
    add_subdirectory("${LIBRARY_UT_DIR}")
endif()
