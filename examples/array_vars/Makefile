# Copyright (c) 2014 Cesanta Software
# All rights reserved

PROG = array_vars
CFLAGS = -W -Wall -I../.. -pthread -g -O0 $(CFLAGS_EXTRA)
SOURCES = $(PROG).c ../../mongoose.c

all: $(PROG)

run: $(PROG)
	./$(PROG)

$(PROG): $(SOURCES) Makefile
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

win:
	wine cl $(SOURCES) /MD /nologo /DNDEBUG /O1 /I../.. /Fe$(PROG).exe
	wine $(PROG).exe

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib *.gc*
