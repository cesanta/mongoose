VERSION = 7.23.1
REPO_ROOT = ../..
MX ?= /Applications/STMicroelectronics/STM32CubeMX.app/Contents/MacOs/STM32CubeMX
FILES = Cesanta.I-CUBE-Mongoose.pdsc Release_Notes.html mongoose_config.h README.md
GENERATED = License.txt mongoose.h mongoose.c

PACK = Cesanta.I-CUBE-Mongoose.$(VERSION).pack
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

clean:
	rm -rf *.pack STM32CubeMX/*.$(VERSION)_*.xml $(GENERATED)
