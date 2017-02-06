SDK ?= $(shell cat sdk.version)
SRC_DIR ?= $(realpath ../../..)
PORT ?= auto

.PHONY: all clean

MAKEFLAGS += w

all clean:
	docker run --rm -i -v $(SRC_DIR):/src $(SDK) \
		/bin/bash -c "\
			make -C /src/mongoose mongoose.c mongoose.h && \
			make -C /src/mongoose/examples/CC3200 -f Makefile.build $@ -$(MAKEFLAGS) \
		"

ifeq ("$(PORT)", "auto")
PORT = $(shell ls -1 /dev/ttyUSB* | tail -n 1)
endif
flash:
	docker run --rm -it --privileged -v $(SRC_DIR):/src $(SDK) /bin/bash -c "\
		cd /usr/local/bin; \
		./cc3200prog $(PORT) /src/fw/platforms/cc3200/firmware/fw.bin \
	"

debug:
	docker run --rm -it --privileged -v $(SRC_DIR):/src $(SDK) \
		/bin/bash -c "cd /src/fw/platforms/cc3200 && tools/gdb.sh"
