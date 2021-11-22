PROG ?= example

all: $(PROG)
	$(DEBUGGER) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) main.c mjson.c ../../mongoose.c -I../.. -W -Wall -DMG_ENABLE_LINES=1 $(CFLAGS) -o $(PROG)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
