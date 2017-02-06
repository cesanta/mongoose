PROG = api_server
SOURCES = $(PROG).c sqlite3.c db_plugin_sqlite.c ../../mongoose.c
CFLAGS = -W -Wall $(CFLAGS_EXTRA)


ifeq ($(OS), Windows_NT)
  CFLAGS += -lws2_32
  CC = gcc
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S), Linux)
    CFLAGS += -ldl -lm -pthread
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
