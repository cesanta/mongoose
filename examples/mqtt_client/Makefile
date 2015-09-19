PROG = mqtt_client
SOURCES = $(PROG).c ../../mongoose.c
CFLAGS = -W -Wall -I../.. -pthread -DMG_ENABLE_SSL -lssl -lcrypto $(CFLAGS_EXTRA)

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /DMG_ENABLE_SSL /MD /Fe$@

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
