PROG = restful_server
SOURCES = $(PROG).c ../../mongoose.c
CFLAGS = -W -Wall -I../.. $(CFLAGS_EXTRA)

ifeq ($(SSL), openssl)
	CFLAGS += -DMG_ENABLE_SSL -lssl -lcrypto -lcrypto
else ifeq ($(SSL), krypton)
	CFLAGS += -DMG_ENABLE_SSL ../../../krypton/krypton.c
endif

ifeq ($(JS), yes)
	V7_PATH = ../../deps/v7
	CFLAGS_EXTRA += -DMG_ENABLE_JAVASCRIPT -I $(V7_PATH) $(V7_PATH)/v7.c
endif

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /MD /DMG_ENABLE_THREADS /Fe$@ advapi32.lib

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
