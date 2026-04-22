VERSION = 7.21.1
PACKPATH = Mongoose
FILES = $(PACKPATH) App Cesanta.I-CUBE-Mongoose.pdsc License.txt Release_Notes.html

all: Cesanta.I-CUBE-Mongoose.$(VERSION).pack

Cesanta.I-CUBE-Mongoose.$(VERSION).pack: $(FILES) $(PACKPATH)/mongoose.h $(PACKPATH)/mongoose.c
	zip -r $@ $^

# Get packchk from https://github.com/Open-CMSIS-Pack/cmsis-toolbox
check: Cesanta.I-CUBE-Mongoose.$(VERSION).pack
	grep $(VERSION) Cesanta.I-CUBE-Mongoose.pdsc || exit 1
	packchk Cesanta.I-CUBE-Mongoose.pdsc

License.txt:
	cp ../../LICENSE $@
$(PACKPATH):
	mkdir -p $(PACKPATH)
$(PACKPATH)/mongoose.h: $(PACKPATH)
	cp ../../mongoose.h $@
$(PACKPATH)/mongoose.c: $(PACKPATH)
	cp ../../mongoose.c $@

clean:
	rm -rf *.pack License.txt $(PACKPATH)
