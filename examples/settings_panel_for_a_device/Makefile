PROG = settings_panel
SOURCES = $(PROG).c ../../mongoose.c
CFLAGS = -W -Wall -I../.. -DMG_ENABLE_SSL -lssl -lcrypto $(CFLAGS_EXTRA)

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
