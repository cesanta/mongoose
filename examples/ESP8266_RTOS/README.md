This is a Mongoose "Hello, world" that can be compiled under ESP8266 RTOS SDK.

It sets up an AP (SSID `Mongoose`) and serves a "hello world" page on http://192.168.4.1/

Most of the the boilerplate comes from [project_template](https://github.com/espressif/ESP8266_RTOS_SDK/tree/master/examples/project_template) (@ [3ca6af5](https://github.com/espressif/ESP8266_RTOS_SDK/tree/3ca6af5da68678d809b34c7cd98bee71e0235039/examples/project_template)) with minimal changes.

To build with no changes to the SDK, you will need a module with 1MB (8Mb) flash or more.

Compile (for NodeMCU 1.0):

```
$ export SDK_PATH=/path/to/ESP8266_RTOS_SDK
$ export BIN_PATH=./bin
$ make clean; make BOOT=none APP=0 SPI_SPEED=40 SPI_MODE=dio SPI_SIZE_MAP=0
```

Flash (using [esptool](https://github.com/themadinventor/esptool)):

```
  $ esptool.py --port /dev/ttyUSB0 --baud 230400 \
      write_flash --flash_mode=qio --flash_size=4m \
      0x00000 ${BIN_PATH}/eagle.flash.bin \
      0x20000 ${BIN_PATH}/eagle.irom0text.bin \
      0x7e000 ${SDK_PATH}/bin/esp_init_data_default.bin
```
