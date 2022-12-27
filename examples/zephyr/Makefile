DOCKER_PROJECT_DIR ?= /workdir
ZEPHYR_DIR ?= zephyrproject
PROJECT_PATH = $(realpath $(CURDIR))
ZEPHYR_PATH = $(realpath $(CURDIR))/$(ZEPHYR_DIR)
DOCKER_PROJECT_PATH = $(DOCKER_PROJECT_DIR)
DOCKER_ZEPHYR_PATH = $(DOCKER_PROJECT_DIR)/$(ZEPHYR_DIR)

DOCKER ?= docker run --rm -v $(PROJECT_PATH):$(DOCKER_PROJECT_PATH) -v $(ZEPHYR_PATH):$(DOCKER_ZEPHYR_PATH)
REPO ?= zephyrprojectrtos/ci

YQ ?= yq

example:
	true
clean:
	true

init:
ifeq ($(wildcard $(ZEPHYR_PATH)/.*),)
	mkdir $(ZEPHYR_PATH)
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(DOCKER_PROJECT_DIR) && west init ./$(ZEPHYR_DIR)'
endif

update:
	$(DOCKER) $(REPO) /bin/sh -c 'cd $(DOCKER_ZEPHYR_PATH) && west update'

minify:
	$(YQ) -i eval '(.manifest.defaults, .manifest.remotes, .manifest.projects[] | select(.name == "cmsis" or .name == "hal_stm32" or .name == "mbedtls" or .name == "mcuboot" or .name == "picolibc" | del(.null) ), .manifest.self) as $$i ireduce({};setpath($$i | path; $$i)) | del(.manifest.projects.[].null) | del(..|select(length==0))' zephyrproject/zephyr/west.yml

zephyr: init minify update
