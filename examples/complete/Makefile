PROG ?= example
SOURCES ?= ../../mongoose.c main.c mjson.c packed_fs.c
CFLAGS ?= -I../.. -DMG_ENABLE_PACKED_FS=1 $(EXTRA)
FILES_TO_EMBED ?= $(wildcard web_root/*.js) $(wildcard web_root/*.css) $(wildcard web_root/*.html) $(wildcard web_root/images/*.png) $(wildcard images/*.jpg)
ROOT ?= $(realpath $(CURDIR)/../../..)
DOCKER ?= docker run --rm -e Tmp=. -e WINEDEBUG=-all -v $(ROOT):$(ROOT) -w $(CURDIR)
VC98 ?= $(DOCKER) mdashnet/vc98 wine
MINGW ?= $(DOCKER) mdashnet/mingw

all: $(PROG)
	$(RUN) ./$(PROG)

$(PROG):
	$(CC) ../../test/pack.c -o pack
	./pack $(FILES_TO_EMBED) > packed_fs.c
	$(CC) -W -Wall -Wextra -Os -g3 $(CFLAGS) -o $(PROG) $(SOURCES)


mingw:
	$(MINGW) i686-w64-mingw32-gcc $(CFLAGS) ../../test/pack.c -o pack.exe
	$(MINGW) wine cmd /c 'pack.exe $(FILES_TO_EMBED) > packed_fs.c'
	$(MINGW) i686-w64-mingw32-gcc $(CFLAGS) $(SOURCES) -lws2_32 -o $(PROG).exe
	$(MINGW) wine $(PROG).exe

vc98:
	$(VC98) cl.exe /nologo $(CFLAGS) ../../test/pack.c /Fepack.exe
	$(VC98) cmd /c 'pack.exe $(FILES_TO_EMBED) > packed_fs.c'
	$(VC98) cl.exe /nologo /O2 $(CFLAGS) $(SOURCES) /Fe$(PROG).exe
	$(VC98) $(PROG).exe

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb log.txt packed_fs.c pack
