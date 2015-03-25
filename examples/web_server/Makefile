# Copyright (c) 2014 Cesanta Software
# All rights reserved

PROG = web_server
CFLAGS = -W -Wall -I../.. -g -O0  $(CFLAGS_EXTRA)
SOURCES = $(PROG).c ../../mongoose.c
OPENSSL_FLAGS = -DNS_ENABLE_SSL -lssl

# PolarSSL paths and flags
POLARSSL_PATH = /usr/local
POLARSSLCOMPAT_PATH = ./../../../polar
SOURCES_POLAR = $(SOURCES) $(POLARSSLCOMPAT_PATH)/polarssl_compat.c
INCDIR_POLAR = -I$(POLARSSLCOMPAT_PATH) -I$(POLARSSL_PATH)/include
LDFLAGS_POLAR = -L$(POLARSSL_PATH)/lib -lmbedtls
CFLAGS_POLAR = $(CFLAGS) $(INCDIR_POLAR) -DNS_ENABLE_SSL

$(PROG): $(SOURCES)
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS)

$(PROG).exe: $(SOURCES)
	cl -Fo $(PROG) $(SOURCES) -nologo -MD -I../..

openssl:
	$(CC) -o $(PROG) $(SOURCES) $(CFLAGS) $(OPENSSL_FLAGS)

polarssl:
	$(CC) -o $(PROG) $(SOURCES_POLAR) $(LDFLAGS_POLAR) $(CFLAGS_POLAR)

clean:
	rm -rf $(PROG) *.exe *.dSYM *.obj *.exp .*o *.lib
