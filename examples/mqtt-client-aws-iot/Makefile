PROG ?= example
CFLAGS ?= -W -Wall -O2 -DMG_ENABLE_MBEDTLS=1 -lmbedtls -lmbedcrypto -lmbedx509

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) main.c ../../mongoose.c -I../.. $(CFLAGS) -o $(PROG)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
