PROG ?= example
ROOT ?= $(realpath $(CURDIR)/../..)
CFLAGS ?= -DMG_ENABLE_LINES $(EXTRA)
LIN = docker run -it --rm -v $(ROOT):$(ROOT) -w $(CURDIR) mdashnet/cc2

ifneq ($(MBEDTLS),)
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLS)/include -I/usr/include
CFLAGS += -L$(MBEDTLS)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif

ifneq ($(OPENSSL),)
CFLAGS += -DMG_ENABLE_OPENSSL=1 -I$(OPENSSL)/include
CFLAGS += -L$(OPENSSL)/lib -lssl -lcrypto
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. -W -Wall $(CFLAGS) -o $(PROG) main.c

linux: all
linux: CFLAGS += -O2 -g -fsanitize=address,undefined,shift,null,return,bounds,alignment,object-size,bool,enum -static-libasan
linux: CC = $(LIN) cc
linux: RUN = $(LIN)


ROOT ?= $(realpath $(CURDIR)/../..)
DOCKER = docker run -it --rm -e Tmp=. -e WINEDEBUG=-all -v $(ROOT):$(ROOT) -w $(CURDIR)
WDEFS = -DMG_ENABLE_MBEDTLS -DMG_ENABLE_LINES -Imbedtls/include -Imbedtls/library $(EXTRA)
WFLAGS = /nologo -O2 -I. -I../.. ws2_32.lib advapi32.lib /link /out:$(PROG).exe
WSRCS = ../../mongoose.c main.c mbedtls/library/*.c
$(PROG).exe: main.c mbedtls
	$(DOCKER) mdashnet/vc2017 wine cl $(WSRCS) $(WDEFS) $(WFLAGS)
vc2017: $(PROG).exe
	$(DOCKER) mdashnet/vc2017 wine $(PROG).exe $(ARGS)
mbedtls:
	git clone --depth 1 -b v3.1.0 https://github.com/ARMmbed/mbedtls

clean:
	rm -rf $(PROG) _CL* *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
