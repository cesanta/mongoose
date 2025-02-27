TOP_DIR ?= $(realpath $(CURDIR)/../../..)
ZEPHYR_DIR ?= $(realpath $(CURDIR)/..)/zephyrproject
DOCKER ?= docker run --rm \
          -v $(TOP_DIR):$(TOP_DIR) \
          -w $(realpath $(CURDIR)) \
          -e ZEPHYR_BASE=$(ZEPHYR_DIR)/zephyr \
          $(DOCKER_ARGS) zephyrprojectrtos/ci
BUILD_ARGS ?= -b nucleo_f746zg
.PHONY: build

example:
	true

zephyr:
	test -d $(ZEPHYR_DIR) || ( mkdir -p $(ZEPHYR_DIR) ; $(DOCKER) west init $(ZEPHYR_DIR))
	$(DOCKER) sh -c 'cd $(ZEPHYR_DIR) && west update'

build:
	cp $(TOP_DIR)/mongoose.[ch] src/
	cp $(TOP_DIR)/examples/device-dashboard/net.[ch] src/
	cp $(TOP_DIR)/examples/device-dashboard/packed_fs.c src/
	$(DOCKER) west build $(BUILD_ARGS) -p auto $(realpath $(CURDIR)) $(OVERLAY)

run: DOCKER_ARGS ?= --cap-add=NET_ADMIN --device /dev/net/tun --net=host -it
run:
	$(DOCKER) west build -t run

clean:
	-$(DOCKER) west build -t clean

pristine:
	-$(DOCKER) west build -t pristine

purge: clean pristine
	rm -rf $(TOP_DIR)/.west $(ZEPHYR_DIR) build

flash debug: DOCKER_ARGS ?= --privileged -v /dev/bus/usb/:/dev/bus/usb -it
flash:
	$(DOCKER) west flash --build-dir $(realpath $(CURDIR))/build

debug:
	$(DOCKER) west debug --build-dir $(realpath $(CURDIR))/build

test: BUILD_ARGS = -b qemu_x86
test: OVERLAY = -DOVERLAY_CONFIG=overlay-e1000.conf
test: build run
