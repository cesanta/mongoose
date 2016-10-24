SDK ?= $(shell cat sdk.version)
SRC_DIR ?= $(realpath ../../..)

.PHONY: all clean flash gdbserver gdb jlinkexe rtt

MAKEFLAGS += w

ifeq ("$(VERBOSE)","1")
V := 
else
V := @
endif

ADD_DOCKER_ARGS =

# For communication with the device, we need docker container to be able
# to access it.
flash gdbserver gdb jlinkexe rtt: \
  ADD_DOCKER_ARGS = --privileged -v /dev/bus/usb:/dev/bus/usb --net=host

all clean flash gdbserver gdb jlinkexe rtt:
	$(V)docker run --rm -it $(ADD_DOCKER_ARGS) -v $(SRC_DIR):/src $(SDK) \
		/bin/bash -c "\
			make -C /src/mongoose mongoose.c mongoose.h && \
			make -C /src/mongoose/examples/nRF52 -f Makefile.build $@ -$(MAKEFLAGS) \
		"
