CWD = $(realpath $(CURDIR))
ZEPHYR_DIR ?= $(realpath ../../../zephyrproject)

example:
	true

build:
	cp ../../mongoose.c ../../mongoose.h http-server/src/
	cd $(ZEPHYR_DIR) && west build -b nucleo_f746zg -p auto $(CWD)/http-server

flash:
	cd $(ZEPHYR_DIR) && west flash
