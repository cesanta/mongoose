PROG ?= example
CFLAGS ?= -W -Wall -Wextra -O0 -g

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. -W -Wall -DMG_ENABLE_LINES=1 $(CFLAGS) -o $(PROG) main.c mjson.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
