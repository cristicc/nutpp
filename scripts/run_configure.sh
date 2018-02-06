#!/bin/sh

# Retrieve script path.
RUN_PATH=$(dirname "$(readlink -fn "${0}")")

# Set build path.
BUILD_PATH=${RUN_PATH}/../../build/nutpp

mkdir -p "${BUILD_PATH}" \
&& cd ${BUILD_PATH} \
&& rm -rf CMakeCache.txt
&& cmake -G "Eclipse CDT4 - Unix Makefiles" \
    -DCMAKE_PREFIX_PATH="${BUILD_PATH}/../dist" \
    -DCMAKE_INSTALL_PREFIX="${BUILD_PATH}/../dist" \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_ECLIPSE_VERSION=4.7.2 \
    -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j4 \
    ../../nutpp/ \
&& printf \
    "%s\n" \
    "" \
    "################################################################" \
    "To build application, run:" \
    "$ cd '${BUILD_PATH}'; make" \
    "To test local install, run:" \
    "$ make install" \
    "################################################################"
