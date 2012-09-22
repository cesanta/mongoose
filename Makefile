# This file is part of Mongoose project, http://code.google.com/p/mongoose
# $Id: Makefile 473 2009-09-02 11:20:06Z valenok $

PROG=	mongoose

all:
	@echo "make (linux|bsd|solaris|mac|windows|mingw)"

# Possible COPT values: (in brackets are rough numbers for 'gcc -O2' on i386)
# -DHAVE_MD5              - use system md5 library (-2kb)
# -DNDEBUG                - strip off all debug code (-5kb)
# -DDEBUG                 - build debug version (very noisy) (+7kb)
# -DNO_CGI                - disable CGI support (-5kb)
# -DNO_SSL                - disable SSL functionality (-2kb)
# -DCONFIG_FILE=\"file\"  - use `file' as the default config file
# -DHAVE_STRTOUI64        - use system strtoui64() function for strtoull()
# -DSSL_LIB=\"libssl.so.<version>\"   - use system versioned SSL shared object
# -DCRYPTO_LIB=\"libcrypto.so.<version>\" - use system versioned CRYPTO so


##########################################################################
###                 UNIX build: linux, bsd, mac, rtems
##########################################################################

GCC_WARNS   = -W -Wall -pedantic
#  -Wno-missing-field-initializers  -Wno-unused-parameter -Wno-format-zero-length -Wno-missing-braces
CFLAGS      = -std=c99 -O2 $(GCC_WARNS) $(COPT)
MAC_SHARED  = -flat_namespace -bundle -undefined suppress
LINFLAGS    = -ldl -pthread $(CFLAGS)
LIB         = lib$(PROG).so$(MONGOOSE_LIB_SUFFIX)

# Make sure that the compiler flags come last in the compilation string.
# If not so, this can break some on some Linux distros which use
# "-Wl,--as-needed" turned on by default  in cc command.
# Also, this is turned in many other distros in static linkage builds.
linux:
	$(CC) mongoose.c -shared -fPIC -fpic -o $(LIB) -Wl,-soname,$(LIB) $(LINFLAGS)
	$(CC) mongoose.c main.c -o $(PROG) $(LINFLAGS)

bsd:
	$(CC) mongoose.c -shared -pthread -fpic -fPIC -o $(LIB) $(CFLAGS)
	$(CC) mongoose.c main.c -pthread -o $(PROG) $(CFLAGS)

mac:
	$(CC) mongoose.c -pthread -o $(LIB) $(MAC_SHARED) $(CFLAGS)
	$(CC) mongoose.c main.c -pthread -o $(PROG) $(CFLAGS)

solaris:
	$(CC) mongoose.c -pthread -lnsl \
		-lsocket -fpic -fPIC -shared -o $(LIB) $(CFLAGS)
	$(CC) mongoose.c main.c -pthread -lnsl -lsocket -o $(PROG) $(CFLAGS)


##########################################################################
###            WINDOWS build: Using Visual Studio or Mingw
##########################################################################

# Using Visual Studio 6.0. To build Mongoose:
#  o  Set MSVC variable below to where VS 6.0 is installed on your system
#  o  Run "PATH_TO_VC6\bin\nmake windows"

MSVC  = e:/vc6
CYA   = e:/cyassl-2.0.0rc2
#DBG  = /Zi /DDEBUG /Od
DBG   = /DNDEBUG /O1
CL    = $(MSVC)/bin/cl /MD /TC /nologo $(DBG) /Gz /W3 /DNO_SSL_DL \
        /I$(MSVC)/include
GUILIB= user32.lib shell32.lib
LINK  = /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86 \
        /subsystem:windows ws2_32.lib advapi32.lib cyassl.lib
CYAFL = /c /I $(CYA)/include -I $(CYA)/include/openssl /I$(MSVC)/INCLUDE \
        /I $(CYA)/ctaocrypt/include /D _LIB /D OPENSSL_EXTRA

CYASRC= \
	$(CYA)/src/cyassl_int.c \
	$(CYA)/src/cyassl_io.c \
	$(CYA)/src/keys.c \
	$(CYA)/src/tls.c \
	$(CYA)/src/ssl.c \
	$(CYA)/ctaocrypt/src/aes.c \
	$(CYA)/ctaocrypt/src/arc4.c \
	$(CYA)/ctaocrypt/src/asn.c \
	$(CYA)/ctaocrypt/src/coding.c \
	$(CYA)/ctaocrypt/src/ctc_asm.c \
	$(CYA)/ctaocrypt/src/ctc_misc.c \
	$(CYA)/ctaocrypt/src/cyassl_memory.c \
	$(CYA)/ctaocrypt/src/des3.c \
	$(CYA)/ctaocrypt/src/dh.c \
	$(CYA)/ctaocrypt/src/dsa.c \
	$(CYA)/ctaocrypt/src/ecc.c \
	$(CYA)/ctaocrypt/src/hc128.c \
	$(CYA)/ctaocrypt/src/hmac.c \
	$(CYA)/ctaocrypt/src/integer.c \
	$(CYA)/ctaocrypt/src/md4.c \
	$(CYA)/ctaocrypt/src/md5.c \
	$(CYA)/ctaocrypt/src/pwdbased.c \
	$(CYA)/ctaocrypt/src/rabbit.c \
	$(CYA)/ctaocrypt/src/random.c \
	$(CYA)/ctaocrypt/src/ripemd.c \
	$(CYA)/ctaocrypt/src/rsa.c \
	$(CYA)/ctaocrypt/src/sha.c \
	$(CYA)/ctaocrypt/src/sha256.c \
	$(CYA)/ctaocrypt/src/sha512.c \
	$(CYA)/ctaocrypt/src/tfm.c

cyassl.lib:
	$(CL) $(CYASRC) $(CYAFL) $(DEF)
	$(MSVC)/bin/lib *.obj /out:$@

windows: cyassl.lib
	$(MSVC)/bin/rc win32\res.rc
	$(CL) /I win32 main.c mongoose.c /GA $(LINK) win32\res.res \
		$(GUILIB) /out:$(PROG).exe
	$(CL) mongoose.c /GD $(LINK) /DLL /DEF:win32\dll.def /out:$(PROG).dll

# Build for Windows under MinGW
#MINGWDBG= -DDEBUG -O0 -ggdb
MINGWDBG= -DNDEBUG -Os
MINGWOPT=  -W -Wall -mthreads -Wl,--subsystem,console $(MINGWDBG) -DHAVE_STDINT $(GCC_WARNINGS) $(COPT)
#MINGWOPT= -W -Wall -mthreads -Wl,--subsystem,windows $(MINGWDBG) -DHAVE_STDINT $(GCC_WARNINGS) $(COPT)
mingw:
	windres win32\res.rc win32\res.o
	$(CC) $(MINGWOPT) mongoose.c -lws2_32 \
		-shared -Wl,--out-implib=$(PROG).lib -o $(PROG).dll
	$(CC) $(MINGWOPT) -Iwin32 mongoose.c main.c win32\res.o -lws2_32 -ladvapi32 \
		-o $(PROG).exe


##########################################################################
###            Manuals, cleanup, test, release
##########################################################################

man:
	groff -man -T ascii mongoose.1 | col -b > mongoose.txt
	groff -man -T ascii mongoose.1 | less

# "TEST=unit make test" - perform unit test only
# "TEST=embedded" - test embedded API by building and testing test/embed.c
# "TEST=basic_tests" - perform basic tests only (no CGI, SSI..)
tests:
	perl test/test.pl $(TEST)

release: clean
	F=mongoose-`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`.tgz ; cd .. && tar -czf x mongoose/{LICENSE,Makefile,bindings,examples,test,win32,mongoose.c,mongoose.h,mongoose.1,main.c} && mv x mongoose/$$F

clean:
	rm -rf *.o *.core $(PROG) *.obj *.so $(PROG).txt *.dSYM *.tgz $(PROG).exe *.dll *.lib
