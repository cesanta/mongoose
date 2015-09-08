#!/bin/sh

PROG=$1
PORT=8002

cleanup() {
  kill -9 $PID >/dev/null 2>&1
}

#set -x
trap cleanup EXIT

cleanup
$PROG -p $PORT -b /api/ 127.0.0.1:8000 &
PID=$!

# Perform api_server unit test through the load balancer by passing
# load balancer port to the unit test script
(cd ../api_server && make && sh unit_test.sh ./api_server $PORT)

exit $?
