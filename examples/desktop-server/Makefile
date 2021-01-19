PROG ?= example
ROOT ?= $(realpath $(CURDIR)/../..)
DEFS ?= -DMG_ENABLE_LINES=1 -DMG_ENABLE_DIRECTORY_LISTING=1 -DMG_ENABLE_SSI=1
CFLAGS ?= -I../.. -W -Wall -DMG_ENABLE_IPV6=1 $(DEFS) $(EXTRA)
VCFLAGS = /nologo /W3 /O2 /I../.. $(DEFS) $(EXTRA) /link /incremental:no /machine:IX86
VC98 = docker run --rm -e Tmp=. -v $(ROOT):$(ROOT) -w $(CURDIR) docker.io/mdashnet/vc98

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)

$(PROG): main.c Makefile
	$(CC) ../../mongoose.c main.c -I../.. $(CFLAGS) -o $@

mongoose.exe: main.c
	$(VC98) wine cl ../../mongoose.c main.c $(VCFLAGS) ws2_32.lib /out:$@

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb mongoose mongoose_* mongoose.*
