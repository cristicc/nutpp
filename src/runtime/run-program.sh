#!/bin/sh
#
# Helper script to run a bundled/distributable application.
#

SCRIPT_DIR=$(dirname $(readlink -mn "$0"))
LIB_DIR=${SCRIPT_DIR}/lib

[ $# -eq 0 ] && {
    echo "Usage: $0 PROGRAM [ARG...]"
    exit 1
}

"${SCRIPT_DIR}/ld-linux.so.2" --inhibit-cache --library-path "${LIB_DIR}" "$@"
