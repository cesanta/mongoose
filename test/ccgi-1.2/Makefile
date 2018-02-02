# Set OPENSSL_INCLUDE to the directory that contains the
# "openssl" include directory.

OPENSSL_INCLUDE = /usr/include

# To build without openssl comment out the "CRYPT =" line

CRYPT = crypt.o

CFLAGS = -I . -I $(OPENSSL_INCLUDE)

libccgi.a: ccgi.o prefork.o $(CRYPT)
	ar r libccgi.a ccgi.o prefork.o $(CRYPT)
	ranlib libccgi.a

ccgi.o: ccgi.c ccgi.h

prefork.o: prefork.c

crypt.o: crypt.c ccgi.h

test: libccgi.a
	cd t; make

clean:
	rm -f *.o *.a */*.o
	rm -f t/test examples/dump.cgi
