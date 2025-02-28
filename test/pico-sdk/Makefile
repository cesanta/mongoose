PROJECTS ?= $(wildcard ../../tutorials/pico-sdk/*-picosdk-*)
SDK_PATH ?= $(realpath $(PWD))/pico-sdk
SDK_VERSION ?= 2.1.0

all: $(PROJECTS)
	echo 

$(PROJECTS): pico-sdk FORCE
	(make -C $@ build SDK_PATH=$(SDK_PATH) && make -C $@ clean) || ( \
	ln -s $(SDK_PATH) $@/pico-sdk && \
	cd $@ && rm -rf build && mkdir -p build && \
	cd build && cmake -DPICO_BOARD=$(SDKBNAME) -G "Unix Makefiles" .. && make && \
	cd .. && rm -rf build pico-sdk )

FORCE:

pico-sdk:
	test -d $(SDK_PATH) || ( git clone --depth 1 -b $(SDK_VERSION) https://github.com/raspberrypi/pico-sdk $@ && \
	cd $@ && git submodule update --init )

clean:
	rm -rf $(SDK_PATH)

# Wizard-style board name --> Pico-SDK board name; SDKBNAME = table[BOARD]
SDK_evb-pico  := pico
SDK_evb-pico2 := pico2
SDK_evb-pico2-w5100 := pico2
SDK_pico-w   := pico_w
SDK_pico2-w  := pico2_w
SDKBNAME = $(SDK_$(BOARD))
