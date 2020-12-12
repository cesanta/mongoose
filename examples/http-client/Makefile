PROG ?= example
ARGS ?= http://www.ladyada.net/
MBEDTLS_DIR ?=

ifeq "$(MBEDTLS_DIR)" ""
else
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLS_DIR)/include -I/usr/include
CFLAGS += -L$(MBEDTLS_DIR)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)


$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. -W -Wall $(CFLAGS) -o $(PROG) main.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
