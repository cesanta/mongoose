VERSION = 7.23.3
REPO_ROOT = ../..
MX ?= /Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX
FILES = Cesanta.I-CUBE-Mongoose.pdsc Release_Notes.html mongoose_config.h README.md .config/mongoose_parameters.json
GENERATED = License.txt mongoose.h mongoose.c
MX2 ?= $(HOME)/STMicroelectronics/STM32CubeMX2_1.1.2/.bin/cube
PACKSDIR ?= $(HOME)/Library/Application Support/stm32cube/packs
MX2CPN ?= STM32C5A3ZGT6
MX2PROJ = $(CURDIR)/mx2test/test.ioc2

NAME = Cesanta.I-CUBE-Mongoose.$(VERSION)
PACK = $(NAME).pack

all: $(PACK)

Cesanta.I-CUBE-Mongoose.$(VERSION).pack: $(FILES) $(GENERATED)
	zip -q -r $@ $^
	cp $@ Cesanta.I-CUBE-Mongoose.pack

# Get packchk from https://github.com/Open-CMSIS-Pack/cmsis-toolbox
check: Cesanta.I-CUBE-Mongoose.$(VERSION).pack
	grep $(VERSION) Cesanta.I-CUBE-Mongoose.pdsc > /dev/null || exit 1
	packchk -w 1 Cesanta.I-CUBE-Mongoose.pdsc

License.txt:
	cp $(REPO_ROOT)/LICENSE $@

Mongoose:
	mkdir -p Mongoose

dashboard.c:
	cp $(REPO_ROOT)/tutorials/device-dashboard/full/$@ $@

dashboard.html:
	cp $(REPO_ROOT)/tutorials/device-dashboard/full/$@ $@

file_data.c:
	make -C $(REPO_ROOT)/tutorials/device-dashboard/full $@
	cp $(REPO_ROOT)/tutorials/device-dashboard/full/$@ $@

html2c.js:
	cp $(REPO_ROOT)/resources/$@ $@

mongoose_mqtt.c:
	cp $(REPO_ROOT)/tutorials/mqtt/mqtt-client/mongoose_mqtt.c $@

mongoose.h: Mongoose
	cp $(REPO_ROOT)/mongoose.h $@

mongoose.c: Mongoose
	cp $(REPO_ROOT)/mongoose.c $@

test_mx2: $(PACK)
	-$(MX2) pack remove --cmsis-pack-root "$(PACKSDIR)" $(NAME)
	rm -f "$(PACKSDIR)/.Download/$(PACK)"
	$(MX2) pack install --cmsis-pack-root "$(PACKSDIR)" "$(CURDIR)/$(PACK)"
	export PATH="$(dir $(MX2)):$$PATH"; \
	PORT=$$(cube lifecycle-mx start-only | sed -n 's/^BACKEND_PORT *//p'); \
	cube mx project close --port $$PORT -p "$(MX2PROJ)" --discard > /dev/null 2>&1; \
	rm -rf mx2test && mkdir mx2test && \
	cube mx project create-from-mcu --port $$PORT --cpn $(MX2CPN) -l "$(CURDIR)/mx2test" -n test --empty-project && \
	cube mx middleware enable-middleware --port $$PORT -p "$(MX2PROJ)" -n Mongoose | tee mx2test/enable.json && \
	grep -q '"status": "success"' mx2test/enable.json && \
	cube mx project save --port $$PORT -p "$(MX2PROJ)" && \
	cube mx project close --port $$PORT -p "$(MX2PROJ)" --discard

clean:
	rm -rf *.pack STM32CubeMX/*.$(VERSION)_*.xml $(GENERATED) mx2test
