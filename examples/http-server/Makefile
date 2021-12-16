PROG ?= example
ROOT ?= $(realpath $(CURDIR)/../..)
DEFS ?= -DMG_ENABLE_LINES=1
CFLAGS ?= -I../.. -W -Wall -DMG_ENABLE_IPV6=1 $(DEFS) $(EXTRA)
VCFLAGS = /nologo /W3 /O2 /I../.. $(DEFS) $(EXTRA) /link /incremental:no /machine:IX86
VC98 = docker run -it --rm -e Tmp=. -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/vc98
LIN = docker run -it --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/cc2

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)

$(PROG): main.c Makefile
	$(CC) ../../mongoose.c main.c -I../.. $(CFLAGS) -o $@

mongoose.exe: main.c
	$(VC98) wine cl ../../mongoose.c main.c $(VCFLAGS) ws2_32.lib /out:$@

mingw:
	gcc ../../mongoose.c main.c -I../.. -W -Wall -DMG_ENABLE_IPV6=1 -DMG_ENABLE_LINES=1 -D_POSIX_C_SOURCE=200000L -lws2_32 -o mongoose.exe

linux: all
linux: CFLAGS += -O2 -fsanitize=address,undefined,shift,null,return,bounds,alignment,object-size,bool,enum -static-libasan
#linux: CC = $(LIN) cc
#linux: DEBUGGER = $(LIN)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb mongoose mongoose_* mongoose.*
