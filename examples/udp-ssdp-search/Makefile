PROG ?= example

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) ../../mongoose.c -I../.. -W -Wall $(CFLAGS) -o $(PROG) main.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
