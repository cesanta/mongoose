WIZARD_URL ?= http://mongoose.ws/wizard

all build example: firmware.bin

ifeq "$(IDE)" "GCC+make"
BUILD = $(MAKE) -f ../gcc+make/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "CubeIDE"
BUILD = mkdir -p workspace && $(MAKE) -k -f ../cube/Makefile PROJECTS=wizard && rmdir workspace
endif
ifeq "$(IDE)" "MCUXpresso"
BUILD = mkdir -p workspace && $(MAKE) -k -f ../xpresso/Makefile PROJECTS=wizard && rmdir workspace
endif
ifeq "$(IDE)" "ModusIDE"
BUILD = true
endif
ifeq "$(IDE)" "e2studio"
BUILD = true
endif
ifeq "$(IDE)" "Pico-SDK"
BUILD = $(MAKE) -f ../pico-sdk/Makefile PROJECTS=wizard BOARD=$(BOARD)
endif
ifeq "$(IDE)" "ESP-IDF"
BUILD = $(MAKE) -f ../esp-idf/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "Keil"
BUILD = true
endif
ifeq "$(IDE)" "Arduino"
BUILD = true
endif
ifeq "$(IDE)" "CGT+make"
BUILD = $(MAKE) -f ../cgt+make/Makefile PROJECTS=wizard
endif
ifeq "$(IDE)" "Zephyr"
BUILD = $(MAKE) -f ../zephyr/Makefile PROJECTS=wizard BOARD=$(BOARD)
endif


firmware.bin: wizard
	(test "$(BUILD)" == "true" && echo "NOT TESTING $(BOARD) $(IDE) $(RTOS): don't know how to test $(IDE) projects" >> test.log) || true
	$(BUILD) && rm -rf wizard*

wizard: FORCE
	hash=$$(curl -s -X POST -H "Content-Type: application/json" -d '{"build":{"board":"$(BOARD)","ide":"$(IDE)","rtos":"$(RTOS)"}}' $(WIZARD_URL)/api/hash | jq -r '.hash') \
	&& curl -s $(WIZARD_URL)/api/zip/$(BOARD)/$(IDE)/$(RTOS)/$$hash -o wizard.zip
	unzip wizard.zip

FORCE:
