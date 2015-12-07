PROG = simplest_web_server
SOURCES = $(PROG).c ../../mongoose.c
CFLAGS = -W -Wall -I../.. $(CFLAGS_EXTRA) -DMG_DISABLE_DAV_AUTH

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /MD /Fe$@

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
