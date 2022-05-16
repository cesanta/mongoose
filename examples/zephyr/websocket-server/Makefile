CWD = $(realpath $(CURDIR))
ZEPHYR_DIR ?= $(realpath ../../../../../../zephyrproject)
BOARD ?= nucleo_f746zg

example:
	true

build:
	make -C ../ -f docker.mk CWD=$(CWD)
	
localbuild:
	cp ../../../mongoose.c ../../../mongoose.h src/
	cd $(ZEPHYR_DIR) && west build -b $(BOARD) -p auto $(CWD)

flash:
	cd $(ZEPHYR_DIR) && west flash

clean:
	rm -rf */*/mongoose.*
