PROG ?= example
ROOT ?= $(realpath $(CURDIR)/../..)
CWD ?= $(realpath $(CURDIR))
DOCKER ?= docker run --rm -it -e Tmp=. -e WINEDEBUG=-all -v $(ROOT):$(ROOT) -w $(CWD)

all: $(PROG)
	$(RUN) ./$(PROG)

$(PROG):
	$(CC) ../../mongoose.c -I../.. -pthread $(CFLAGS) -o $(PROG) main.c

$(PROG).exe:
	$(DOCKER) mdashnet/vc98 wine cl ../../mongoose.c main.c -I../.. /MD ws2_32.lib /Fe$@
	$(DOCKER) mdashnet/vc98 wine $@

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb log.txt
