PROG ?= example
DEFS ?= -DMG_ENABLE_LINES=1 -DMG_ENABLE_MIP=1 -DMG_ENABLE_SOCKET=0 -DMG_ENABLE_PACKED_FS=1 -I../..
CFLAGS ?= -W -Wall $(EXTRA_CFLAGS)
LIBS ?= -lpcap
SOURCES = main.c ../../mongoose.c ../device-dashboard/net.c ../device-dashboard/packed_fs.c

ifeq "$(SSL)" "MBEDTLS"
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -lmbedtls -lmbedcrypto -lmbedx509 -I$(MBEDTLS)/include -L$(MBEDTLS)/lib
endif

ifeq "$(SSL)" "OPENSSL"
CFLAGS += -DMG_ENABLE_OPENSSL=1 -lssl -lcrypto -I$(OPENSSL)/include -L$(OPENSSL)/lib
endif

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) $(SOURCES) $(CFLAGS) $(DEFS) -o $(PROG) $(LIBS)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
