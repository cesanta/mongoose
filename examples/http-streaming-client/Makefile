PROG ?= example
CFLAGS ?= -DMG_ENABLE_LINES $(EXTRA)

ifneq ($(MBEDTLS),)
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLS)/include -I/usr/include
CFLAGS += -L$(MBEDTLS)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif

ifneq ($(OPENSSL),)
CFLAGS += -DMG_ENABLE_OPENSSL=1 -I$(OPENSSL)/include
CFLAGS += -L$(OPENSSL)/lib -lssl -lcrypto
endif

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)


$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. -W -Wall $(CFLAGS) -o $(PROG) main.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
