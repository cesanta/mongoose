# Copyright (c) 2014 Cesanta Software
# All rights reserved

PROG = proxy_server
FLAGS = -I../..  -DNS_ENABLE_SSL
CFLAGS = -W -Wall -g -O0 -pthread  -lssl -DMONGOOSE_ENABLE_THREADS $(FLAGS) $(CFLAGS_EXTRA)
SOURCES = $(PROG).c ../../mongoose.c

unix: $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib
