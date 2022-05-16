PROJECT_DIR ?= /mnt
ZEPHYR_DIR ?= /zephyrproject
BOARD ?= nucleo_f746zg
DOCKER ?= docker run -v $(realpath $(CURDIR)):/mnt mdashnet/zephyr

example:
	true

build:
	cp ../../../mongoose.c ../../../mongoose.h src/
	$(DOCKER) /bin/sh -c 'cd $(ZEPHYR_DIR) && west build -b $(BOARD) -p auto $(PROJECT_DIR)'

flash:
	cd $(ZEPHYR_DIR) && west flash

clean:
	rm -rf */*/mongoose.*
