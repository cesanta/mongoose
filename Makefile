# This file is part of Mongoose project, http://code.google.com/p/mongoose
# $Id: Makefile 473 2009-09-02 11:20:06Z valenok $

PROG=	mongoose

all:
	@echo "make (linux|bsd|solaris|mac|windows|mingw)"

# Possible COPT values: (in brackets are rough numbers for 'gcc -O2' on i386)
# -DHAVE_MD5		- use system md5 library (-2kb)
# -DNDEBUG		- strip off all debug code (-5kb)
# -DDEBUG		- build debug version (very noisy) (+7kb)
# -DNO_CGI		- disable CGI support (-5kb)
# -DNO_SSL		- disable SSL functionality (-2kb)
# -DCONFIG_FILE=\"file\" - use `file' as the default config file
# -DNO_SSI		- disable SSI support (-4kb)
# -DHAVE_STRTOUI64	- use system strtoui64() function for strtoull()


##########################################################################
###                 UNIX build: linux, bsd, mac, rtems
##########################################################################

CFLAGS=		-W -Wall -std=c99 -pedantic -Os -fomit-frame-pointer $(COPT)
MAC_SHARED=	-flat_namespace -bundle -undefined suppress
SSLFLAGS=	-lssl -lcrypto
LINFLAGS=	-ldl -pthread $(SSLFLAGS) $(CFLAGS)
LIB=		_$(PROG).so

linux:
	$(CC) $(LINFLAGS) mongoose.c -shared -fPIC -fpic -s -o $(LIB)
	$(CC) $(LINFLAGS) mongoose.c main.c -s -o $(PROG)

bsd:
	$(CC) $(CFLAGS) mongoose.c -shared -pthread -s -fpic -fPIC -o $(LIB)
	$(CC) $(CFLAGS) mongoose.c main.c -pthread -s $(SSLFLAGS) -o $(PROG)

mac:
	$(CC) $(CFLAGS) $(MAC_SHARED) mongoose.c -pthread $(SSLFLAGS) -o $(LIB)
	$(CC) $(CFLAGS) mongoose.c main.c -pthread $(SSLFLAGS) -o $(PROG)

solaris:
	gcc $(CFLAGS) mongoose.c -pthread -lnsl \
		-lsocket $(SSLFLAGS) -s -fpic -fPIC -shared -o $(LIB)
	gcc $(CFLAGS) mongoose.c main.c -pthread -lnsl -lsocket $(SSLFLAGS) \
		-s -o $(PROG)


##########################################################################
###            WINDOWS build: Using Visual Studio or Mingw
##########################################################################

# Using Visual Studio 6.0
# Assuming that studio is installed in d:\vc6, change VC variable below to
# the correct path on your system. Run "d:\vc6\bin\nmake windows"

VC=		d:\vc2010\vc
#VC=		d:\vc6
SDK=		d:\sdk\v7.1
#WINDBG=	/Zi /DDEBUG /Od /DDEBUG
WINDBG=		/DNDEBUG /Os
WINFLAGS=	/MT /TC /nologo /W4 $(WINDBG) /I $(VC)/include \
		/I $(SDK)\include /link /incremental:no /libpath:$(VC)\lib \
		/libpath:$(SDK)/lib ws2_32.lib

windows:
	$(VC)\bin\cl.exe mongoose.c $(WINFLAGS) \
		/DLL /DEF:win32\dll.def /out:_$(PROG).dll
	$(VC)\bin\cl.exe mongoose.c main.c $(WINFLAGS) /out:$(PROG).exe

# Build for Windows under MinGW
#MINGWDBG= -DDEBUG -O0
MINGWDBG= -DNDEBUG -Os
MINGWOPT= -W -Wall -mthreads -Wl,--subsystem,console $(MINGWDBG) -DHAVE_STDINT
mingw:
	gcc $(MINGWOPT) mongoose.c -lws2_32 \
		-shared -Wl,--out-implib=$(PROG).lib -o _$(PROG).dll
	gcc $(MINGWOPT) mongoose.c main.c -lws2_32 -ladvapi32 -o $(PROG).exe


##########################################################################
###            Manuals, cleanup, test, release
##########################################################################

man:
	cat mongoose.1 | tbl | groff -man -Tascii | col -b > mongoose.1.txt
	cat mongoose.1 | tbl | groff -man -Tascii | less

# "TEST=unit make test" - perform unit test only
# "TEST=embedded" - test embedded API by building and testing test/embed.c
# "TEST=basic_tests" - perform basic tests only (no CGI, SSI..)
test: do_test
do_test:
	perl test/test.pl $(TEST)

release: clean
	F=mongoose-`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`.tgz ; cd .. && tar --exclude \*.svn --exclude \*.swp --exclude \*.nfs\* --exclude win32 -czf x mongoose && mv x mongoose/$$F

clean:
	rm -rf *.o *.core $(PROG) *.obj $(PROG).1.txt *.dSYM *.tgz
