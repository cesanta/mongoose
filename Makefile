# This Makefile is part of Mongoose web server project,
# https://github.com/valenok/mongoose
#
# Example custom build:
# COPT="-g -O0 -DNO_SSL_DL -DUSE_LUA -llua -lcrypto -lssl" make linux
#
# Flags are:
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

PROG        = mongoose
CFLAGS      = -std=c99 -O2 -W -Wall -pedantic -pthread -pipe $(COPT)

# To build with Lua, download and unzip Lua 5.2.1 source code into the
# mongoose directory, and then add $(LUA_SOURCES) to CFLAGS
LUA         = lua-5.2.1/src
LUA_FLAGS   = -I$(LUA) -DLUA_COMPAT_ALL
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
LUA_WINOBJS = $(LUA_SOURCES:%.c=%.obj)

ifneq ($(OS), Windows_NT)
  LUA_FLAGS += -DLUA_USE_DLOPEN
endif

# Stock windows binary builds with Lua and YASSL library.
YASSL       = ../cyassl-2.4.6
YASSL_FLAGS = -I $(YASSL) -I $(YASSL)/cyassl \
              -D _LIB -D OPENSSL_EXTRA -D HAVE_ERRNO_H \
              -D HAVE_GETHOSTBYNAME -D HAVE_INET_NTOA -D HAVE_LIMITS_H \
              -D HAVE_MEMSET -D HAVE_SOCKET -D HAVE_STDDEF_H -D HAVE_STDLIB_H \
              -D HAVE_STRING_H -D HAVE_SYS_STAT_H -D HAVE_SYS_TYPES_H
YASSL_SOURCES = \
  $(YASSL)/src/internal.c $(YASSL)/src/io.c $(YASSL)/src/keys.c \
  $(YASSL)/src/ssl.c $(YASSL)/src/tls.c $(YASSL)/ctaocrypt/src/hmac.c \
  $(YASSL)/ctaocrypt/src/random.c $(YASSL)/ctaocrypt/src/sha.c \
  $(YASSL)/ctaocrypt/src/sha256.c $(YASSL)/ctaocrypt/src/logging.c \
  $(YASSL)/ctaocrypt/src/error.c $(YASSL)/ctaocrypt/src/rsa.c \
  $(YASSL)/ctaocrypt/src/des3.c $(YASSL)/ctaocrypt/src/asn.c \
  $(YASSL)/ctaocrypt/src/coding.c $(YASSL)/ctaocrypt/src/arc4.c \
  $(YASSL)/ctaocrypt/src/md4.c $(YASSL)/ctaocrypt/src/md5.c \
  $(YASSL)/ctaocrypt/src/dh.c $(YASSL)/ctaocrypt/src/dsa.c \
  $(YASSL)/ctaocrypt/src/pwdbased.c $(YASSL)/ctaocrypt/src/aes.c \
  $(YASSL)/ctaocrypt/src/md2.c $(YASSL)/ctaocrypt/src/ripemd.c \
  $(YASSL)/ctaocrypt/src/sha512.c $(YASSL)/src/sniffer.c \
  $(YASSL)/ctaocrypt/src/rabbit.c $(YASSL)/ctaocrypt/src/misc.c \
  $(YASSL)/ctaocrypt/src/tfm.c $(YASSL)/ctaocrypt/src/integer.c \
  $(YASSL)/ctaocrypt/src/ecc.c $(YASSL)/src/ocsp.c $(YASSL)/src/crl.c \
  $(YASSL)/ctaocrypt/src/hc128.c $(YASSL)/ctaocrypt/src/memory.c

ALL_SOURCES = main.c mongoose.c build/sqlite3.c build/lsqlite3.c \
              $(LUA_SOURCES) $(YASSL_SOURCES)
ALL_OBJECTS = $(ALL_SOURCES:%.c=%.o)
ALL_WINOBJS = $(ALL_SOURCES:%.c=%.obj)

SQLITE_FLAGS = -DTHREADSAFE=1 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS
MONGOOSE_FLAGS = -DNO_SSL_DL -DUSE_LUA -DUSE_LUA_SQLITE3 $(COPT)

FLAGS = $(MONGOOSE_FLAGS) $(SQLITE_FLAGS) $(YASSL_FLAGS) $(LUA_FLAGS)

# Using Visual Studio 6.0. To build Mongoose:
#  Set MSVC variable below to where VS 6.0 is installed on your system
#  Run "PATH_TO_VC6\bin\nmake windows"
MSVC = ../vc6
#DBG = /Zi /Od
DBG  = /DNDEBUG /O1
CL   = $(MSVC)/bin/cl /MD /TC /nologo $(DBG) /W3 /GA /I$(MSVC)/include
LINK = $(MSVC)/bin/link /incremental:no /libpath:$(MSVC)/lib /machine:IX86 \
       user32.lib shell32.lib comdlg32.lib ws2_32.lib advapi32.lib

all:
	@echo "make (linux|bsd|solaris|mac|windows|mingw|cygwin)"

%.obj: %.c
	$(CL) /c $(FLAGS) /Fo$@ $<

%.o: %.c
	$(CC) -o $@ $< -c $(FLAGS) $(CFLAGS)

# Lua library for Windows
lua.lib: $(LUA_WINOBJS)
	$(MSVC)/bin/lib /out:$@ $(LUA_WINOBJS)

# To build with lua, make sure you have Lua unpacked into lua-5.2.1 directory
linux_lua: $(ALL_OBJECTS)
	$(CC) $(ALL_OBJECTS) -o $(PROG) -ldl

mongoose.o: mod_lua.c

# Make sure that the compiler flags come last in the compilation string.
# If not so, this can break some on some Linux distros which use
# "-Wl,--as-needed" turned on by default  in cc command.
# Also, this is turned in many other distros in static linkage builds.
linux:
	$(CC) mongoose.c main.c -o $(PROG) -ldl $(CFLAGS)

mac: bsd
bsd:
	$(CC) mongoose.c main.c -o $(PROG) $(CFLAGS)

bsd_lua: $(ALL_OBJECTS)
	$(CC) $(ALL_OBJECTS) -o $@

solaris:
	$(CC) mongoose.c main.c -lnsl -lsocket -o $(PROG) $(CFLAGS)

lib$(PROG).a: $(ALL_OBJECTS)
	ar cr $@ $(ALL_OBJECTS)

$(PROG).lib: $(ALL_WINOBJS)
	$(MSVC)/bin/lib /out:$@ $(ALL_WINOBJS)

# For codesign to work in non-interactive mode, unlock login keychain:
# security unlock ~/Library/Keychains/login.keychain
# See e.g. http://lists.apple.com/archives/apple-cdsa/2008/Jan/msg00027.html
Mongoose: mongoose.c main.c
	$(CC) mongoose.c main.c build/lsqlite3.c build/sqlite3.c \
          -DUSE_COCOA $(CFLAGS) $(FLAGS) -mmacosx-version-min=10.4 \
          $(YASSL_SOURCES) $(LUA_SOURCES) \
          -framework Cocoa -ObjC -arch i386 -arch x86_64 -o Mongoose

cocoa: Mongoose
	V=`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`; DIR=dmg/Mongoose.app && rm -rf $$DIR && mkdir -p $$DIR/Contents/{MacOS,Resources} && install -m 644 build/mongoose_*.png $$DIR/Contents/Resources/ && install -m 644 build/Info.plist $$DIR/Contents/ && install -m 755 Mongoose $$DIR/Contents/MacOS/ && ln -fs /Applications dmg/ ; hdiutil create Mongoose_$$V.dmg -volname "Mongoose $$V" -srcfolder dmg -ov #; rm -rf dmg

un:
	$(CC) test/unit_test.c -o unit_test -I. -I$(LUA) $(LUA_SOURCES) \
          $(CFLAGS) -g -O0
	./unit_test

wi:
	$(CL) test/unit_test.c $(LUA_SOURCES) $(LUA_FLAGS) \
          $(YASSL_SOURCES) $(YASSL_FLAGS) /I. /DNO_SSL_DL \
          /link /libpath:$(MSVC)/lib advapi32.lib /out:unit_test.exe
	./unit_test.exe

windows: $(ALL_WINOBJS)
	$(MSVC)/bin/rc build/res.rc
	$(LINK) /nologo $(ALL_WINOBJS) build/res.res /out:$(PROG).exe

# Build for Windows under MinGW
#MINGWDBG= -DDEBUG -O0 -ggdb
MINGWDBG= -DNDEBUG -Os
MINGWOPT=  -W -Wall -mthreads -Wl,--subsystem,console $(MINGWDBG) -DHAVE_STDINT $(GCC_WARNINGS) $(COPT)
mingw:
	windres build\res.rc build\res.o
	$(CC) $(MINGWOPT) mongoose.c -lws2_32 \
		-shared -Wl,--out-implib=$(PROG).lib -o $(PROG).dll
	$(CC) $(MINGWOPT) mongoose.c main.c build\res.o \
	-lws2_32 -ladvapi32 -lcomdlg32 -o $(PROG).exe

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

tests:
	perl test/test.pl $(TEST)

tarball: clean
	F=mongoose-`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`.tgz ; cd .. && tar -czf x mongoose/{LICENSE,Makefile,examples,test,build,*.[ch],*.md} && mv x mongoose/$$F

release: tarball cocoa
	wine make windows
	V=`perl -lne '/define\s+MONGOOSE_VERSION\s+"(\S+)"/ and print $$1' mongoose.c`; upx mongoose.exe; cp mongoose.exe mongoose-$$V.exe; cp mongoose.exe mongoose_php_bundle/; zip -r mongoose_php_bundle_$$V.zip mongoose_php_bundle/

clean:
	cd examples && $(MAKE) clean
	rm -rf *.o *.core $(PROG) *.obj *.so $(PROG).txt *.dSYM *.tgz \
	$(PROG).exe *.dll *.lib build/res.o build/res.RES *.dSYM *.zip *.pdb \
	*.exe *dmg* $(ALL_OBJECTS) $(ALL_WINOBJS)
