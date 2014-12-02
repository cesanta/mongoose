# Copyright (c) 2014 Cesanta Software
# All rights reserved

PROG = ws_ssl
CFLAGS = -W -Wall -I../.. -I. -pthread -g -O0 $(CFLAGS_EXTRA)
SOURCES = ws_ssl.c ../../mongoose.c ssl_wrapper.c

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) \
		-DNS_ENABLE_SSL -DSSL_WRAPPER_USE_AS_LIBRARY -lssl $(CFLAGS)

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib
