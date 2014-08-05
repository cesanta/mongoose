# Copyright (c) 2014 Cesanta Software
# All rights reserved

CFLAGS = -W -Wall -I../.. -g -O0 $(CFLAGS_EXTRA)
SOURCES = http_client.c ../../mongoose.c
PROG = http_client

unix: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib
