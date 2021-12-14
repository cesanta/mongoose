PROG ?= example

CFLAGS ?= -W -Wall -DMG_ENABLE_LINES=1
ifneq ($(MBEDTLS_DIR),)
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLS_DIR)/include -I/usr/include
CFLAGS += -L$(MBEDTLS_DIR)/lib -lmbedtls -lmbedcrypto -lmbedx509
else ifneq ($(OPENSSL_DIR),)
CFLAGS += -DMG_ENABLE_OPENSSL=1 -I$(OPENSSL_DIR)/include
CFLAGS += -L$(OPENSSL_DIR)/lib -lssl -lcrypto
endif

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. $(CFLAGS) $(EFLAGS) -o $(PROG) main.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
