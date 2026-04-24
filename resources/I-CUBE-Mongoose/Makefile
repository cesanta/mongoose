VERSION = 7.21.4
REPO_ROOT = ../..
FILES = Cesanta.I-CUBE-Mongoose.pdsc Release_Notes.html mongoose_config.h README.md
GENERATED = License.txt \
            mongoose.h mongoose.c \
            mongoose_dashboard.c dashboard.html.c \
            mongoose_mqtt.c dashboard.html.c

all: Cesanta.I-CUBE-Mongoose.$(VERSION).pack

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

mongoose_dashboard.c:
	cp $(REPO_ROOT)/tutorials/device-dashboard/mongoose_dashboard.c $@

mongoose_mqtt.c:
	cp $(REPO_ROOT)/tutorials/mqtt/mqtt-client/mongoose_mqtt.c $@

dashboard.html.c:
	make -C $(REPO_ROOT)/tutorials/device-dashboard dashboard.html.c
	cp $(REPO_ROOT)/tutorials/device-dashboard/dashboard.html.c $@

mongoose.h: Mongoose
	cp $(REPO_ROOT)/mongoose.h $@

mongoose.c: Mongoose
	cp $(REPO_ROOT)/mongoose.c $@

clean:
	rm -rf *.pack STM32CubeMX/*.$(VERSION)_*.xml $(GENERATED)
