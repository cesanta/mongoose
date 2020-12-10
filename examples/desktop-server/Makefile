PROG ?= example
ROOT ?= $(realpath $(CURDIR)/../..)
DEFS ?= -DMG_ENABLE_LINES=1 -DMG_ENABLE_DIRECTORY_LISTING=1
CFLAGS ?= -I../.. -W -Wall -DMG_ENABLE_IPV6=1 $(DEFS) $(EXTRA)
VCFLAGS = /nologo /W3 /O2 /I../.. $(DEFS) $(EXTRA)
VC98 = docker run --rm -e WINEDEBUG=-all -v $(ROOT):$(ROOT) -w $(CURDIR) docker.io/mdashnet/vc98
GCC = docker run --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/cc2

all: $(PROG)
	$(DEBUGGER) ./$(PROG)

$(PROG):
	$(CC) ../../mongoose.c main.c -I../.. $(CFLAGS) $(EXTRA) -o $@

mongoose_mac: $(PROG)
	cp $(PROG) $@

mongoose_linux: main.c
	$(GCC) $(CC) ../../mongoose.c main.c $(CFLAGS) -o $@

mongoose.exe: main.c
	$(VC98) wine cl ../../mongoose.c main.c $(VCFLAGS) ws2_32.lib /Fe$@

zip: mongoose.exe mongoose_linux mongoose_mac
	rm -rf mongoose; mkdir mongoose
	cp mongoose.exe mongoose_mac mongoose_linux mongoose
	zip -r mongoose.zip mongoose

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb mongoose mongoose_* mongoose.*
