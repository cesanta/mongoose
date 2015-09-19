PROG = http_client
SOURCES = $(PROG).c ../../mongoose.c
CFLAGS = -W -Wall -I../.. -DMG_ENABLE_SSL -lssl -lcrypto -pthread $(CFLAGS_EXTRA)

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /MD /Fe$@

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
