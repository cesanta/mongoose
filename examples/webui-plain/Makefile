PROG ?= example
CFLAGS ?= -W -Wall -Wextra -O2
SSL = ?

ifeq "$(SSL)" "MBEDTLS"
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -lmbedtls -lmbedcrypto -lmbedx509 -L$(MBEDTLS)/lib -I$(MBEDTLS)/include
endif

ifeq "$(SSL)" "OPENSSL"
CFLAGS += -DMG_ENABLE_OPENSSL=1 -lssl -lcrypto -L$(OPENSSL)/lib -I$(OPENSSL)/include
endif

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)


$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. $(CFLAGS) $(EXTRA_CFLAGS) -o $(PROG) main.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
