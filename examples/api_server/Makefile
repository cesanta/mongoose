PROG = api_server
SOURCES = $(PROG).c sqlite3.c db_plugin_sqlite.c ../../mongoose.c
CFLAGS = -W -Wall -pthread $(CFLAGS_EXTRA)

ifeq ($(OS), Windows_NT)
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S), Linux)
    CFLAGS += -ldl -lm
  endif
endif

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl $(SOURCES) /I.. /MD /Fe$@

test: $(PROG)
	sh unit_test.sh $$(pwd)/$(PROG)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)
