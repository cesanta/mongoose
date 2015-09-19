PROG = multithreaded_restful_server
SOURCES = $(PROG).c ../../mongoose.c
APP_FLAGS = -DMG_ENABLE_THREADS $(CFLAGS_EXTRA)

ifeq ($(OS), Windows_NT)
APP_FLAGS += advapi32.lib
endif

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ -W -Wall -I../.. -pthread $(APP_FLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I../.. /MD /Fe$@ $(APP_FLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
