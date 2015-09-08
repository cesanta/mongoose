PROG = nc
SOURCES = $(PROG).c ../../mongoose.c
CFLAGS = -W -Wall -I../.. -pthread -DNS_ENABLE_SSL -DNS_ENABLE_THREADS -lssl -lcrypto $(CFLAGS_EXTRA)

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /DNS_ENABLE_SSL /MD /Fe$@

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
