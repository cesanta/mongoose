PROG ?= example

all: $(PROG)
	$(DEBUGGER) ./$(PROG)

$(PROG):
	$(CC) ../../mongoose.c -I../.. $(CFLAGS) $(EXTRA) -o $(PROG) main.c mjson.c

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb log.txt
