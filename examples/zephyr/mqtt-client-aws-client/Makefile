CWD = $(realpath $(CURDIR))
ZEPHYR_DIR ?= $(realpath ../../../../zephyrproject)
BOARD ?= nucleo_h743zi

example:
	true

build:
	cp ../../../mongoose.c ../../../mongoose.h src/
	cd $(ZEPHYR_DIR) && west build -b $(BOARD) -p auto $(CWD)

flash:
	cd $(ZEPHYR_DIR) && west flash

clean:
	rm -rf */*/mongoose.*
