VERSION = 7.21.3
REPO_ROOT = ../..
FILES = Cesanta.I-CUBE-Mongoose.pdsc Release_Notes.html mongoose_config.h README.md
GENERATED = License.txt \
            mongoose.h mongoose.c \
            mongoose_dashboard.c dashboard.html.c
        # STM32CubeMX/Cesanta.I-CUBE-Mongoose.$(VERSION)_Configs.xml \
        # STM32CubeMX/Cesanta.I-CUBE-Mongoose.$(VERSION)_Modes.xml

all: Cesanta.I-CUBE-Mongoose.$(VERSION).pack

Cesanta.I-CUBE-Mongoose.$(VERSION).pack: $(FILES) $(GENERATED)
	zip -r $@ $^
	cp $@ Cesanta.I-CUBE-Mongoose.pack

# Get packchk from https://github.com/Open-CMSIS-Pack/cmsis-toolbox
check: Cesanta.I-CUBE-Mongoose.$(VERSION).pack
	grep $(VERSION) Cesanta.I-CUBE-Mongoose.pdsc || exit 1
	packchk Cesanta.I-CUBE-Mongoose.pdsc

License.txt:
	cp $(REPO_ROOT)/LICENSE $@

Mongoose:
	mkdir -p Mongoose

mongoose_dashboard.c:
	cp $(REPO_ROOT)/tutorials/device-dashboard/mongoose_dashboard.c $@

dashboard.html.c:
	make -C $(REPO_ROOT)/tutorials/device-dashboard dashboard.html.c
	cp $(REPO_ROOT)/tutorials/device-dashboard/dashboard.html.c $@

mongoose.h: Mongoose
	cp $(REPO_ROOT)/mongoose.h $@

mongoose.c: Mongoose
	cp $(REPO_ROOT)/mongoose.c $@

STM32CubeMX/Cesanta.I-CUBE-Mongoose.${VERSION}_Modes.xml:	STM32CubeMX/m.xml
	sed s,__VERSION__,$(VERSION),g $^ > $@

STM32CubeMX/Cesanta.I-CUBE-Mongoose.${VERSION}_Configs.xml:	STM32CubeMX/c.xml
	sed s,__VERSION__,$(VERSION),g $^ > $@

clean:
	rm -rf *.pack STM32CubeMX/*.$(VERSION)_*.xml $(GENERATED)
