VERSION = 7.21.2
REPO_ROOT = ../..
FILES = Mongoose App Cesanta.I-CUBE-Mongoose.pdsc License.txt Release_Notes.html

all: Cesanta.I-CUBE-Mongoose.$(VERSION).pack

Cesanta.I-CUBE-Mongoose.$(VERSION).pack: $(FILES) Mongoose/mongoose.h Mongoose/mongoose.c App/mongoose_app.c App/dashboard.html.c
	zip -r $@ $^

# Get packchk from https://github.com/Open-CMSIS-Pack/cmsis-toolbox
check: Cesanta.I-CUBE-Mongoose.$(VERSION).pack
	grep $(VERSION) Cesanta.I-CUBE-Mongoose.pdsc || exit 1
	packchk Cesanta.I-CUBE-Mongoose.pdsc

License.txt:
	cp $(REPO_ROOT)/LICENSE $@
Mongoose:
	mkdir -p Mongoose
App/mongoose_app.c:
	cp $(REPO_ROOT)/tutorials/device-dashboard/mongoose_app.c $@
App/dashboard.html.c:
	make -C $(REPO_ROOT)/tutorials/device-dashboard dashboard.html.c
	cp $(REPO_ROOT)/tutorials/device-dashboard/dashboard.html.c $@
Mongoose/mongoose.h: Mongoose
	cp $(REPO_ROOT)/mongoose.h $@
Mongoose/mongoose.c: Mongoose
	cp $(REPO_ROOT)/mongoose.c $@

clean:
	rm -rf *.pack License.txt Mongoose App/*.{html,js,c}
