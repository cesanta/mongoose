PROG ?= example
DEFS ?= -DMG_ENABLE_LINES=1 -DMG_ENABLE_MIP=1 -DMG_ENABLE_SOCKET=0 -DMG_ENABLE_WINSOCK=0 -DMG_ENABLE_PACKED_FS=1 -I../..
CFLAGS ?= -W -Wall
LIBS ?= -lpcap
SOURCES = main.c ../../mongoose.c ../device-dashboard/net.c ../device-dashboard/packed_fs.c

all: $(PROG)
	$(RUN) ./$(PROG) $(ARGS)

$(PROG): main.c
	$(CC) $(SOURCES) $(CFLAGS) $(DEFS) -o $(PROG) $(LIBS)

clean:
	rm -rf $(PROG) *.o *.dSYM *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb
