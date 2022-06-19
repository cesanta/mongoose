PROG ?= example
SOURCES ?= ../../mongoose.c main.c net.c packed_fs.c
CFLAGS ?= -I../.. -DMG_ENABLE_PACKED_FS=1 -DMG_ENABLE_LINES=1 $(EXTRA_CFLAGS)
FILES_TO_EMBED ?= $(wildcard web_root/*)
ROOT ?= $(realpath $(CURDIR)/../../..)
DOCKER ?= docker run --rm -e Tmp=. -e WINEDEBUG=-all -v $(ROOT):$(ROOT) -w $(CURDIR)
VC98 ?= $(DOCKER) mdashnet/vc98 wine
MINGW ?= $(DOCKER) mdashnet/mingw

all: $(PROG)
	$(RUN) ./$(PROG)

$(PROG): $(SOURCES) $(FILES_TO_EMBED)
	$(CC) ../../test/pack.c -o pack
	./pack $(FILES_TO_EMBED) > packed_fs.c
	$(CC) -W -Wall -Wextra -O0 -g3 $(CFLAGS) -o $(PROG) $(SOURCES)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb log.txt pack config.json
