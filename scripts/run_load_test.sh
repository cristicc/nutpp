#!/bin/sh

# Retrieve script path.
RUN_PATH=$(dirname "$(readlink -fn "${0}")")

WEBCFG_URL="http://127.0.0.1:8080/UserInterfaces"
MAX_PROC=110

i=0
while [ $((i=i+1)) -le ${MAX_PROC} ]; do
    printf "############# Test $i ################\n"
    {
        sessionID=$(curl -s -X GET "${WEBCFG_URL}" \
            | grep "Load basic HTML" | grep -o -E 'wtd=[^&]+')
        curl -s -X GET "${WEBCFG_URL}?${sessionID}&js=no" >/dev/null
    } &
    pidlist="$pidlist $!"
done

FAIL=0
for job in ${pidlist}; do
    wait ${job} || FAIL=$((FAIL + 1))
done

[ "${FAIL}" = "0" ] && printf "SUCCESS!\n" || printf "FAIL! ($FAIL)\n"
