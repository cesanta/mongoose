SDK ?= $(shell cat sdk.version)
SRC_DIR ?= $(realpath ../..)

.PHONY: all clean

MAKEFLAGS += w

all clean:
	docker run --rm -i -v $(SRC_DIR):/src $(SDK) \
		/bin/bash -c "\
			make -C /src/examples/STM32F4_CC3100 -f Makefile.build $@ -$(MAKEFLAGS) \
		"
