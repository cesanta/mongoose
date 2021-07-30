PROG ?= example
SOURCES ?= ../../mongoose.c main.c mjson.c packed_fs.c
CFLAGS ?= -I../.. -DMG_ENABLE_PACKED_FS=1 $(EXTRA)
FILES_TO_EMBED ?= $(wildcard web_root/*.js) $(wildcard web_root/*.css) $(wildcard web_root/*.html) $(wildcard web_root/images/*.png) $(wildcard images/*.jpg)

all: $(PROG)
	$(RUN) ./$(PROG)

$(PROG):
	$(CC) ../../test/pack.c -o pack
	./pack $(FILES_TO_EMBED) > packed_fs.c
	$(CC) -W -Wall -Wextra -Os -g3 $(CFLAGS) -o $(PROG) $(SOURCES)

ROOT ?= $(realpath $(CURDIR)/../..)
DOCKER ?= docker run -it --rm -e Tmp=. -e WINEDEBUG=-all -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/vc98 wine
windows:
	$(DOCKER) cl.exe /nologo $(CFLAGS) ../../test/pack.c /Fepack.exe
	$(DOCKER) cmd /c 'pack.exe $(FILES_TO_EMBED) > packed_fs.c'
	$(DOCKER) cl.exe /nologo /O2 $(CFLAGS) $(SOURCES) ws2_32.lib /Fe$(PROG).exe
	$(DOCKER) $(PROG).exe

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb log.txt packed_fs.c pack
