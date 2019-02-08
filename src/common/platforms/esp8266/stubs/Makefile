#
# Copyright (c) 2015 Cesanta Software Limited
# All rights reserved
#

STUB = stub_hello.c
LIBS =
PARAMS =
CFLAGS =
PORT = /dev/ttyUSB0

BUILD_DIR = .build
COMMON_STUB_DIR = ../../esp
STUB_ELF = $(BUILD_DIR)/$(patsubst %.c,%.elf,$(notdir $(STUB)))
STUB_JSON ?= $(BUILD_DIR)/$(patsubst %.c,%.json,$(notdir $(STUB)))
SDK = $(shell cat ../../../../fw/platforms/esp8266/sdk.version)
XT_CC = xtensa-lx106-elf-gcc

.PHONY: all clean run wrap

all: $(STUB_ELF)

$(STUB_ELF): $(STUB) $(LIBS)
	@echo "  CC   $^ -> $@"
	@[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	docker run --rm -i -v $(CURDIR)/../../../..:/src $(SDK) //bin/bash -c \
    "cd /src/common/platforms/esp8266/stubs && \
     $(XT_CC) -std=c99 -Wall -Werror -Os -DESP8266 \
         -mtext-section-literals -mlongcalls -nostdlib -fno-builtin \
         -I. -I/src/common/platforms/esp -I/src/common/platforms/esp8266 \
         -I/opt/Espressif/ESP8266_SDK \
         -Wl,-static -ffunction-sections -lgcc -Wl,--gc-sections \
         -Tstub.ld $(CFLAGS) -o $@ $^"

wrap: $(STUB_JSON)

$(STUB_JSON): $(STUB_ELF) $(COMMON_STUB_DIR)/esptool.py
	@echo "  WRAP $< -> $@"
	@docker run --rm -i -v $(CURDIR)/../../../..:/src $(SDK) //bin/bash -c \
    "cd /src/common/platforms/esp8266/stubs && \
     $(COMMON_STUB_DIR)/esptool.py wrap_stub $<" > $@

run: $(STUB_JSON)
	@echo "  RUN  $< $(PARAMS) -> $(PORT)"
	@docker run --rm -i --privileged -v $(CURDIR)/../../../..:/src $(SDK) //bin/bash -c \
    "cd /src/common/platforms/esp8266/stubs && \
     $(COMMON_STUB_DIR)/esptool.py --port $(PORT) run_stub $< $(PARAMS)"

clean:
	@rm -rf $(BUILD_DIR)
