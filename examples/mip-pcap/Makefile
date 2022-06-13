PROG ?= example
DEFS ?= -DMG_ENABLE_LINES=1 -DMG_ENABLE_MIP=1 -DMG_ENABLE_SOCKET=0 -DMG_ENABLE_WINSOCK=0 -I../..
CFLAGS ?= -W -Wall
LIBS ?= -lpcap

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) main.c ../../mongoose.c $(CFLAGS) $(DEFS) -o $(PROG) $(LIBS)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
