PROJECT_DIR ?= /workdir
ZEPHYR_DIR ?= zephyrproject
BOARD ?= qemu_x86

ifeq "$(BOARD)" "qemu_x86"
OVERLAY ?= -DOVERLAY_CONFIG=overlay-e1000.conf
else
OVERLAY ?= 
endif

SHAREUSB ?= --privileged -v /dev/bus/usb/:/dev/bus/usb
SHARETUN ?= --cap-add=NET_ADMIN --device /dev/net/tun --net=host
DOCKER ?= docker run --rm -v $(realpath $(CURDIR)):$(PROJECT_DIR)
REPO ?= zephyrprojectrtos/ci


example:
	true

build:
	cp ../../../mongoose.[c,h] src/
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR)/$(ZEPHYR_DIR)/zephyr && \
	west build -b $(BOARD) -p auto $(PROJECT_DIR) $(OVERLAY) --build-dir $(PROJECT_DIR)/build'

run:
	$(DOCKER) -it $(SHARETUN) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR)/$(ZEPHYR_DIR) && \
	west build -t run --build-dir $(PROJECT_DIR)/build'

clean:
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR)/$(ZEPHYR_DIR) && \
	west build -t clean --build-dir $(PROJECT_DIR)/build'

pristine:
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR)/$(ZEPHYR_DIR) && \
	west build -t pristine --build-dir $(PROJECT_DIR)/build'


flash:
	$(DOCKER) $(SHAREUSB) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR)/$(ZEPHYR_DIR) && \
	west flash --build-dir $(PROJECT_DIR)/build'

debug:
	$(DOCKER) -it $(SHAREUSB) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR)/$(ZEPHYR_DIR) && \
	west debug --build-dir $(PROJECT_DIR)/build'

.PHONY: build flash zephyr clean pristine run

zephyr: 
ifeq ($(wildcard $(ZEPHYR_DIR)/.*),)
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR) && west init ./$(ZEPHYR_DIR)'
endif
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(PROJECT_DIR)/$(ZEPHYR_DIR) && west update'
