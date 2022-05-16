CWD = $(realpath $(CURDIR))
ZEPHYR_DIR ?= /zephyrproject
BOARD ?= nucleo_f746zg
DOCKER ?= docker run -v  $(CWD):/mnt mdashnet/zephyr
PROJECT_PATH ?= /mnt

build:
	cp ../../mongoose.c ../../mongoose.h $(CWD)/src/
	$(DOCKER) /bin/sh -c 'cd $(ZEPHYR_DIR); west build -b $(BOARD) -p auto $(PROJECT_PATH)'
