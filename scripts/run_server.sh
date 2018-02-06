#!/bin/sh

# Retrieve script path.
RUN_PATH=$(dirname "$(readlink -fn "${0}")")

# Set local env.
BUILD_PATH=${RUN_PATH}/../../build
APP_PATH=${BUILD_PATH}/nutpp
DIST_PATH=${BUILD_PATH}/dist
SRV_PATH=${APP_PATH}/run

[ -d ${SRV_PATH} ] || {
    # Install default configuration.
    mkdir -p ${SRV_PATH}/web
    ln -s ${RUN_PATH}/../web/css ${SRV_PATH}/web/
    ln -s ${RUN_PATH}/../web/images ${SRV_PATH}/web/
    ln -s ${DIST_PATH}/share/Wt/resources ${SRV_PATH}/web/

    mkdir -p ${SRV_PATH}/conf
    for xmlfile in ${RUN_PATH}/../conf/*.xml; do
        ln -s ${xmlfile} ${SRV_PATH}/conf/
    done

    cp -f ${RUN_PATH}/../conf/*.properties ${SRV_PATH}/conf/
}

[ "$1" = "debug" ] && {
    # GLib has a few features that confuse Valgrind.
    # 1. memory pools (g_slice in newer glib, "mem chunks" in older).
    #    These are specialized allocators used for small objects such as list nodes.
    #    Use G_SLICE=always-malloc to disable the slice allocator.
    # 2. Sometimes GLib would avoid initializing new memory or keep dead pointers in
    #    freed slices/chunks. Fix with G_DEBUG=gc-friendly
    START_CMD="G_SLICE=always-malloc G_DEBUG=gc-friendly"

    # Valgrind configuration
    START_CMD+=" valgrind "
    START_CMD+=" --tool=memcheck --leak-check=full --leak-resolution=high"
    START_CMD+=" --vgdb=no --num-callers=40"
    for supf in $(ls ${RUN_PATH}/valgrind/); do
        START_CMD+=" --suppressions=${RUN_PATH}/valgrind/${supf}"
    done
    START_CMD+=" --log-file=${SRV_PATH}/valgrind.log"
    #START_CMD+=" --trace-children=yes"
    #START_CMD+=" --read-var-info=yes"
    START_CMD+=" --track-origins=yes"
    #START_CMD+=" --show-reachable=yes"
}

# Enable webui local configuration
export RSG_LOCAL_DEPLOYMENT=1
#export LD_LIBRARY_PATH=${DIST_PATH}/lib

SRV_BIN=${APP_PATH}/src/webserver/nutpp-webserver

# Start app server
eval ${START_CMD} ${SRV_BIN} \
    --no-compression \
    --accesslog ${SRV_PATH}/access.log \
    --pid-file ${SRV_PATH}/webcfg.pid \
    --docroot ${SRV_PATH}/web \
    --approot ${SRV_PATH}/conf \
    --http-address 0.0.0.0 --http-port 8080 \
    --threads 1 \
    "$@"
