MONGOOSE_ROOT ?= $(realpath $(CURDIR)/../../..)
DOCKER_PROJECT_DIR ?= /workdir
ZEPHYR_DIR ?= zephyrproject
PROJECT_NAME = $(notdir $(CURDIR))
PROJECT_PATH = $(realpath $(CURDIR))
ZEPHYR_PATH = $(realpath $(CURDIR)/..)/$(ZEPHYR_DIR)
DOCKER_PROJECT_PATH = $(DOCKER_PROJECT_DIR)/$(PROJECT_NAME)
DOCKER_ZEPHYR_PATH = $(DOCKER_PROJECT_DIR)/$(ZEPHYR_DIR)

BOARD ?= qemu_x86
ifeq "$(BOARD)" "qemu_x86"
OVERLAY ?= -DOVERLAY_CONFIG=overlay-e1000.conf
else
OVERLAY ?= 
endif

SHAREUSB ?= --privileged -v /dev/bus/usb/:/dev/bus/usb
SHARETUN ?= --cap-add=NET_ADMIN --device /dev/net/tun --net=host
DOCKER ?= docker run --rm -v $(PROJECT_PATH):$(DOCKER_PROJECT_PATH) -v $(ZEPHYR_PATH):$(DOCKER_ZEPHYR_PATH)
REPO ?= zephyrprojectrtos/ci


example:
	true

build:
	cp $(MONGOOSE_ROOT)/mongoose.[ch] src/
	cp $(MONGOOSE_ROOT)/examples/device-dashboard/net.c src/
	cp $(MONGOOSE_ROOT)/examples/device-dashboard/packed_fs.c src/
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH)/zephyr && \
	west build -b $(BOARD) -p auto $(DOCKER_PROJECT_PATH) $(OVERLAY) --build-dir $(DOCKER_PROJECT_PATH)/build'

run:
	$(DOCKER) -it $(SHARETUN) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH) && \
	west build -t run --build-dir $(DOCKER_PROJECT_PATH)/build'

clean:
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH) && \
	west build -t clean --build-dir $(DOCKER_PROJECT_PATH)/build'

pristine:
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH) && \
	west build -t pristine --build-dir $(DOCKER_PROJECT_PATH)/build'


flash:
	$(DOCKER) $(SHAREUSB) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH) && \
	west flash --build-dir $(DOCKER_PROJECT_PATH)/build'

debug:
	$(DOCKER) -it $(SHAREUSB) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH) && \
	west debug --build-dir $(DOCKER_PROJECT_PATH)/build'

.PHONY: build flash zephyr clean pristine run

zephyr:
ifeq ($(wildcard $(ZEPHYR_PATH)/.*),)
	mkdir $(ZEPHYR_PATH)
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(DOCKER_PROJECT_DIR) && west init ./$(ZEPHYR_DIR)'
endif
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH) && west update'
