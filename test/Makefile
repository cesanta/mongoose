# Copyright (c) 2014 Cesanta Software
# All rights reserved

PROG = unit_test
PROF_FLAGS = -fprofile-arcs -ftest-coverage -g -O0 -DGUI
CFLAGS = -W -Wall -pthread -I.. $(PROF_FLAGS) $(CFLAGS_EXTRA)
SOURCES = $(PROG).c

all: $(PROG)
	./$(PROG)
	gcov -b $(PROG).c | egrep '^(File|Lines)'

$(PROG): $(SOURCES) Makefile ../mongoose.c clean
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS) -ldl -lssl

win:
	wine cl $(SOURCES) /MD /nologo /DNDEBUG /O1 /Fe$(PROG).exe
	wine $(PROG).exe

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib *.gc*
