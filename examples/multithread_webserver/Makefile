# Copyright (c) 2014 Cesanta Software
# All rights reserved

PROG = web_server
CFLAGS = -W -Wall -I../.. -pthread -g -O0 -DMONGOOSE_ENABLE_THREADS $(CFLAGS_EXTRA)
SOURCES = $(PROG).c ../../mongoose.c
OPENSSL_FLAGS = -DNS_ENABLE_SSL -lssl
CC=g++
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


linkinfo= -pthread -g -O0
predef= -DMONGOOSE_ENABLE_THREADS $(CFLAGS_EXTRA) -I../..
tar=web_server.o ../../mongoose.o
test:$(tar)
	$(CC) $(tar) $(linkinfo) -o main

web_server.o:web_server.c
	$(CC) web_server.c -c $(predef)


mongoose.o:mongoose.c
	$(CC) mongoose.c -c $(predef)
	
