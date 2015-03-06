# Copyright (c) 2014 Cesanta Software
# All rights reserved

PROG = ws_ssl
CFLAGS = -W -Wall -I../.. -I. -pthread -g -O0 -DMONGOOSE_ENABLE_THREADS -DNS_ENABLE_SSL -DSSL_WRAPPER_USE_AS_LIBRARY $(CFLAGS_EXTRA)
LDFLAGS = -lssl
SOURCES = ws_ssl.c ../../mongoose.c ssl_wrapper.c

# PolarSSL paths and flags
POLARSSL_PATH = /usr/local
POLARSSLCOMPAT_PATH = ./../../../polar
SOURCES_POLAR = $(SOURCES) $(POLARSSLCOMPAT_PATH)/polarssl_compat.c
INCDIR_POLAR = -I$(POLARSSLCOMPAT_PATH) -I$(POLARSSL_PATH)/include
LDFLAGS_POLAR = -L$(POLARSSL_PATH)/lib -lmbedtls
CFLAGS_POLAR = $(CFLAGS) $(INCDIR_POLAR)
#

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(LDFLAGS) $(CFLAGS)

polarssl: $(SOURCES_POLAR)
	$(CC) -o $(PROG) $(SOURCES_POLAR) $(LDFLAGS_POLAR) $(CFLAGS_POLAR)

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib
