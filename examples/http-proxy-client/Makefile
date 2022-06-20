PROG ?= example
ARGS ?= 167.235.63.238:3128 http://info.cern.ch/
SSL = ?

ifeq "$(SSL)" "MBEDTLS"
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -lmbedtls -lmbedcrypto -lmbedx509
endif

ifeq "$(SSL)" "OPENSSL"
CFLAGS += -DMG_ENABLE_OPENSSL=1 -lssl -lcrypto
endif

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)


$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. -W -Wall $(CFLAGS) $(EXTRA_CFLAGS) -o $(PROG) main.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
