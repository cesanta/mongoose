# This file is part of Mongoose project, http://code.google.com/p/mongoose
# $Id: Makefile 473 2009-09-02 11:20:06Z valenok $

PROG=	mongoose

all:
	@echo "make (linux|bsd|solaris|mac|windows|mingw|cygwin)"

# Possible COPT values: (in brackets are rough numbers for 'gcc -O2' on i386)
# -DHAVE_MD5              - use system md5 library (-2kb)
# -DNDEBUG                - strip off all debug code (-5kb)
# -DDEBUG                 - build debug version (very noisy) (+7kb)
# -DNO_CGI                - disable CGI support (-5kb)
# -DNO_SSL                - disable SSL functionality (-2kb)
# -DNO_SSL_DL             - link against system libssl library (-1kb)
# -DCONFIG_FILE=\"file\"  - use `file' as the default config file
# -DSSL_LIB=\"libssl.so.<version>\"   - use system versioned SSL shared object
# -DCRYPTO_LIB=\"libcrypto.so.<version>\" - use system versioned CRYPTO so
# -DUSE_LUA               - embed Lua in Mongoose (+100kb)


##########################################################################
###                 UNIX build: linux, bsd, mac, rtems
##########################################################################

# To build with Lua, download and unzip Lua 5.2.1 source code into the
# mongoose directory, and then add $(LUA_FLAGS) to CFLAGS below
LUA         = lua-5.2.1/src
LUA_FLAGS   = -DUSE_LUA -I$(LUA) -L$(LUA) -llua -lm

CFLAGS      = -std=c99 -O2 -W -Wall -pedantic $(COPT)
LIB         = lib$(PROG).so$(MONGOOSE_LIB_SUFFIX)

# Make sure that the compiler flags come last in the compilation string.
# If not so, this can break some on some Linux distros which use
# "-Wl,--as-needed" turned on by default  in cc command.
# Also, this is turned in many other distros in static linkage builds.
linux:
	$(CC) mongoose.c main.c -o $(PROG) -ldl -pthread $(CFLAGS)

bsd:
	$(CC) mongoose.c main.c -pthread -o $(PROG) $(CFLAGS)

mac:
	$(CC) mongoose.c main.c -pthread $(CFLAGS) -o $(PROG)

cocoa:
	$(CC) mongoose.c main.c -DUSE_COCOA -pthread $(CFLAGS) -framework Cocoa -ObjC -arch i386 -arch x86_64 -o Mongoose
	V=`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`; DIR=dmg/Mongoose.app && rm -rf $$DIR && mkdir -p $$DIR/Contents/{MacOS,Resources} && install -m 644 build/mongoose_*.png $$DIR/Contents/Resources/ && install -m 644 build/Info.plist $$DIR/Contents/ && install -m 755 Mongoose $$DIR/Contents/MacOS/ && ln -fs /Applications dmg/ ; hdiutil create Mongoose_$$V.dmg -volname "Mongoose $$V" -srcfolder dmg -ov #; rm -rf dmg

solaris:
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
        /I$(MSVC)/include /DUSE_LUA /I$(LUA)
GUILIB= user32.lib shell32.lib comdlg32.lib
LINK  = /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86 \
        /subsystem:windows ws2_32.lib advapi32.lib cyassl.lib lua.lib
CYAFL = /c /I $(CYA)/include -I $(CYA)/include/openssl /I$(MSVC)/INCLUDE \
        /I $(CYA)/ctaocrypt/include /D _LIB /D OPENSSL_EXTRA

LUA_SOURCES = $(LUA)/lapi.c $(LUA)/lcode.c $(LUA)/lctype.c \
              $(LUA)/ldebug.c $(LUA)/ldo.c $(LUA)/ldump.c \
              $(LUA)/lfunc.c $(LUA)/lgc.c $(LUA)/llex.c \
              $(LUA)/lmem.c $(LUA)/lobject.c $(LUA)/lopcodes.c \
              $(LUA)/lparser.c $(LUA)/lstate.c $(LUA)/lstring.c \
              $(LUA)/ltable.c $(LUA)/ltm.c $(LUA)/lundump.c \
              $(LUA)/lvm.c $(LUA)/lzio.c $(LUA)/lauxlib.c \
              $(LUA)/lbaselib.c $(LUA)/lbitlib.c $(LUA)/lcorolib.c \
              $(LUA)/ldblib.c $(LUA)/liolib.c $(LUA)/lmathlib.c \
              $(LUA)/loslib.c $(LUA)/lstrlib.c $(LUA)/ltablib.c \
              $(LUA)/loadlib.c $(LUA)/linit.c
LUA_OBJECTS = $(LUA_SOURCES:%.c=%.o)

CYA_SOURCES = $(CYA)/src/cyassl_int.c $(CYA)/src/cyassl_io.c \
              $(CYA)/src/keys.c $(CYA)/src/tls.c $(CYA)/src/ssl.c \
              $(CYA)/ctaocrypt/src/aes.c $(CYA)/ctaocrypt/src/arc4.c \
              $(CYA)/ctaocrypt/src/asn.c $(CYA)/ctaocrypt/src/coding.c \
              $(CYA)/ctaocrypt/src/ctc_asm.c $(CYA)/ctaocrypt/src/ctc_misc.c \
              $(CYA)/ctaocrypt/src/cyassl_memory.c $(CYA)/ctaocrypt/src/des3.c \
              $(CYA)/ctaocrypt/src/dh.c $(CYA)/ctaocrypt/src/dsa.c \
              $(CYA)/ctaocrypt/src/ecc.c $(CYA)/ctaocrypt/src/hc128.c \
              $(CYA)/ctaocrypt/src/hmac.c $(CYA)/ctaocrypt/src/integer.c \
              $(CYA)/ctaocrypt/src/md4.c $(CYA)/ctaocrypt/src/md5.c \
              $(CYA)/ctaocrypt/src/pwdbased.c $(CYA)/ctaocrypt/src/rabbit.c \
              $(CYA)/ctaocrypt/src/random.c $(CYA)/ctaocrypt/src/ripemd.c \
              $(CYA)/ctaocrypt/src/rsa.c $(CYA)/ctaocrypt/src/sha.c \
              $(CYA)/ctaocrypt/src/sha256.c $(CYA)/ctaocrypt/src/sha512.c \
              $(CYA)/ctaocrypt/src/tfm.c

cyassl.lib:
	$(CL) /Fo$(CYA)/ $(CYA_SOURCES) $(CYAFL) $(DEF)
	$(MSVC)/bin/lib $(CYA)/*.obj /out:$@

lua.lib:
	$(CL) /c /Fo$(LUA)/ $(LUA_SOURCES)
	$(MSVC)/bin/lib $(LUA_SOURCES:%.c=%.obj) /out:$@

windows: cyassl.lib lua.lib
	$(MSVC)/bin/rc build\res.rc
	$(CL) /Ibuild main.c mongoose.c /GA $(LINK) build\res.res \
		$(GUILIB) /out:$(PROG).exe
#	$(CL) mongoose.c /GD $(LINK) /DLL /DEF:build\dll.def /out:$(PROG).dll

# Build for Windows under MinGW
#MINGWDBG= -DDEBUG -O0 -ggdb
MINGWDBG= -DNDEBUG -Os
MINGWOPT=  -W -Wall -mthreads -Wl,--subsystem,console $(MINGWDBG) -DHAVE_STDINT $(GCC_WARNINGS) $(COPT)
#MINGWOPT= -W -Wall -mthreads -Wl,--subsystem,windows $(MINGWDBG) -DHAVE_STDINT $(GCC_WARNINGS) $(COPT)
mingw:
	windres build\res.rc build\res.o
	$(CC) $(MINGWOPT) mongoose.c -lws2_32 \
		-shared -Wl,--out-implib=$(PROG).lib -o $(PROG).dll
	$(CC) $(MINGWOPT) -build mongoose.c main.c build\res.o \
	-lws2_32 -ladvapi32 -o $(PROG).exe

# Build for Windows under Cygwin
#CYGWINDBG= -DDEBUG -O0 -ggdb
CYGWINDBG= -DNDEBUG -Os
CYGWINOPT=  -W -Wall -mthreads -Wl,--subsystem,console $(CYGWINDBG) -DHAVE_STDINT $(GCC_WARNINGS) $(COPT)
cygwin:
	windres ./build/res.rc ./build/res.o
	$(CC) $(CYGWINOPT) mongoose.c -lws2_32 \
		-shared -Wl,--out-implib=$(PROG).lib -o $(PROG).dll
	$(CC) $(CYGWINOPT) -Ibuild mongoose.c main.c ./build/res.o \
	-lws2_32 -ladvapi32 -o $(PROG).exe

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
	F=mongoose-`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`.tgz ; cd .. && tar -czf x mongoose/{LICENSE,Makefile,examples,test,build,*.[ch],*.md} && mv x mongoose/$$F

clean:
	cd examples && $(MAKE) clean
	rm -rf *.o *.core $(PROG) *.obj *.so $(PROG).txt *.dSYM *.tgz \
	$(PROG).exe *.dll *.lib build/res.o build/res.RES *.dSYM
