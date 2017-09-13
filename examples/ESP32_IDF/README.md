This is a Mongoose "Hello, world" that can be compiled under Espressif IoT Development Framework for ESP32

It connects to WiFi network and serves a "hello world" page.

Most of the the boilerplate comes from [project_template](https://github.com/espressif/esp-idf-template) with minimal changes.

For building the example, you need to have [Docker](https://www.docker.com/products/docker) and use our pre-built SDK container.
To build just run in the example directory
```
$ make
```

Note: before building, change `WIFI_SSID` and `WIFI_PASS` macros in main/main.c file

Once built, use [esptool](https://github.com/espressif/esptool) for flashing
```
$ python esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 --before default_reset --after hard_reset write_flash -u --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 build/bootloader/bootloader.bin 0x10000 build/esp32_idf.bin 0x8000 build/partitions_singleapp.bin
```
