PROG ?= example
CFLAGS += -DMG_ENABLE_SOCKETPAIR=1
CDIR ?= $(realpath $(CURDIR))
ROOT ?= $(realpath $(CURDIR)/../..)
VC2017 = docker run --rm -e WINEDEBUG=-all -v $(ROOT):$(ROOT) -w $(CDIR) docker.io/mdashnet/vc2017

all: $(PROG)
	$(DEBUGGER) ./$(PROG)

$(PROG):
	$(CC) ../../mongoose.c -I../.. -pthread $(CFLAGS) -o $(PROG) main.c

vc2017:
	$(VC2017) wine64 cl ../../mongoose.c main.c -I../.. $(CFLAGS) ws2_32.lib /Fe$@.exe
	$(VC2017) wine64 $@.exe

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb log.txt
