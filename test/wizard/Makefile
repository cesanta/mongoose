WIZARD_URL ?= http://mongoose.ws/wizard

all build example: firmware.bin

ifeq "$(IDE)" "GCC+make"
BUILD = $(MAKE) -f ../gcc+make/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "CubeIDE"
BUILD = $(MAKE) -f ../cube/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "MCUXpresso"
BUILD = $(MAKE) -f ../xpresso/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "Zephyr"
BUILD = true
endif
ifeq "$(IDE)" "Keil"
BUILD = true
endif
ifeq "$(IDE)" "Pico-SDK"
BUILD = $(MAKE) -f ../pico-sdk/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "ESP-IDF"
BUILD = $(MAKE) -f ../esp-idf/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "Arduino"
BUILD = true
endif


firmware.bin: wizard
	$(BUILD) && rm -rf wizard*

wizard: FORCE
	hash=$$(curl -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -s $(WIZARD_URL)/api/zip/$(BOARD)/$(IDE)/$(RTOS)/$$hash -o wizard.zip
	unzip wizard.zip

FORCE:
