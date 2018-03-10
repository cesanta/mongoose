# set OPENSSL_LIB to the directory containing openssl libcrypto

OPENSSL_LIB = /usr/lib

CFLAGS = -I ..
LDFLAGS = -L .. -L $(OPENSSL_LIB)

all: test.sh test
	./test.sh

test: test.o ../libccgi.a 
	$(CC) -o test $(LDFLAGS) test.o -lccgi -lcrypto

test.o: test.c ../ccgi.h
