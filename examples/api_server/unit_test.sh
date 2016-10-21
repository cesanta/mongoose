#!/bin/sh

PROG=$1
PORT=${2:-8000}  # If second param is given, this is load balancer port
DB_FILE=/tmp/_$$.db
URL=http://127.0.0.1:$PORT/api/v1

cleanup() {
  rm -rf $DB_FILE
  kill -9 $PID >/dev/null 2>&1
}

#set -x
trap cleanup EXIT

cleanup
$PROG -f $DB_FILE &
PID=$!

#sleep 1
curl -s -X PUT -d 'value=123' $URL/foo
curl -s -X PUT -d 'value=success' $URL/bar/baz

# Fetch existing key
RESULT=$(curl -s $URL/bar/baz)
test "$RESULT" = "success" || exit 1

# Delete it
curl -s -X DELETE $URL/bar/baz

# Make sure it's deleted - GET must result in 404
RESULT=$(curl -s -i $URL/bar/baz | head -1 | tr -d '\r')
test "$RESULT" = "HTTP/1.1 404 Not Found" || exit 1

exit 0
