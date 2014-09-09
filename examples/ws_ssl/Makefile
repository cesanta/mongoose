# Copyright (c) 2014 Cesanta Software
# All rights reserved

CFLAGS = -W -Wall -I../.. -g -O0 $(CFLAGS_EXTRA)
NS = ../../../net_skeleton
SW = ../../../ssl_wrapper

SOURCES = ws_ssl.c ../../mongoose.c $(NS)/net_skeleton.c $(SW)/ssl_wrapper.c
PROG = ws_ssl

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) \
		-I$(NS) -DNS_ENABLE_SSL -DNOEMBED_NET_SKELETON \
		-I$(SW) -DSSL_WRAPPER_USE_AS_LIBRARY -lssl $(CFLAGS)

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib
