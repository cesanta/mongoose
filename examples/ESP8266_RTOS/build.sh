#!/bin/bash
REPO=$(cd ../.. && pwd)

docker run \
  --rm -i -v $REPO:/src \
  --entrypoint=/bin/bash $(cat sdk.version) -l -c -x '
    export SDK_PATH=/opt/Espressif/ESP8266_RTOS_SDK;
    export BIN_PATH=./bin;
    cd /src/examples/ESP8266_RTOS &&
    make clean &&
    make BOOT=none APP=0 SPI_SPEED=40 SPI_MODE=dio SPI_SIZE_MAP=0 &&
    make fw.zip'
