#!/bin/bash

docker run \
  --rm -i -v $(realpath ${PWD}/../..):/src \
  --entrypoint=/bin/bash $(cat sdk.version) -l -c -x '
    export SDK_PATH=/opt/Espressif/ESP8266_SDK;
    export BIN_PATH=./bin;
    cd /src/examples/ESP8266_RTOS &&
    mkdir -p ./bin && make clean &&
    make BOOT=none APP=0 SPI_SPEED=40 SPI_MODE=qio SPI_SIZE_MAP=0'
