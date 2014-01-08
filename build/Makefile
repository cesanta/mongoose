# This Makefile is part of Mongoose web server project,
# https://github.com/valenok/mongoose
#
# This Makefile is GNU make compatible. You can get GNU Make from
# http://gnuwin32.sourceforge.net/packages/make.htm

PROG = mongoose
EXE_SUFFIX =
CFLAGS = -std=c99 -O2 -W -Wall -pedantic -pthread -pipe -I. -I.. $(CFLAGS_EXTRA)
VERSION = $(shell perl -lne \
            'print $$1 if /define\s+MONGOOSE_VERSION\s+"(\S+)"/' ../mongoose.h)
VDIR = mongoose-$(VERSION)

# Stock windows binary builds with Lua and YASSL library.
YASSL       = ../../cyassl-2.4.6
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

TINY_SOURCES = ../mongoose.c main.c
LUA_SOURCES = $(TINY_SOURCES) lua_5.2.1.c
LUA_SQLITE_SOURCES = $(LUA_SOURCES) sqlite3.c lsqlite3.c
BIG_SOURCES = $(TINY_SOURCES) lua_5.2.1.c sqlite3.c lsqlite3.c $(YASSL_SOURCES)

SQLITE_FLAGS = -DTHREADSAFE=1 -DSQLITE_ENABLE_FTS3 \
               -DSQLITE_ENABLE_FTS3_PARENTHESIS
LUA_FLAGS = -DUSE_LUA
LUA_SQLITE_FLAGS = $(SQLITE_FLAGS) $(LUA_FLAGS) -DUSE_LUA_SQLITE3
BIG_FLAGS = -DTHREADSAFE=1 -DSQLITE_ENABLE_FTS3 \
	    -DSQLITE_ENABLE_FTS3_PARENTHESIS \
	    -DUSE_LUA -DUSE_LUA_SQLITE3 -DUSE_SSL -DUSE_CYASSL $(YASSL_FLAGS)

# Using Visual Studio 6.0. To build Mongoose:
#  Set MSVC variable below to where VS 6.0 is installed on your system
#  Run "PATH_TO_VC6\bin\nmake windows"
MSVC = ../../vc6
#DBG = /Zi /Od
DBG  = /DNDEBUG /O1 $(CFLAGS_EXTRA)
CL   = $(MSVC)/bin/cl /MD /TC /nologo $(DBG) /W3 /I$(MSVC)/include /I.. /I.
LINK_FLAGS = /incremental:no /libpath:$(MSVC)/lib /machine:IX86 \
            user32.lib shell32.lib comdlg32.lib ws2_32.lib advapi32.lib
LINK = $(MSVC)/bin/link $(LINK_FLAGS)

ifeq ($(OS), Windows_NT)
  EXE_SUFFIX = .exe
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S), Linux)
    CFLAGS += -ldl -lm
  endif
endif

all: mongoose$(EXE_SUFFIX)

# Make sure that the compiler flags come last in the compilation string.
# If not so, this can break some on some Linux distros which use
# "-Wl,--as-needed" turned on by default  in cc command.
# Also, this is turned in many other distros in static linkage builds.
$(PROG): $(TINY_SOURCES)
	$(CC) $(TINY_SOURCES) -o $@ $(CFLAGS)

$(PROG)-lua: $(TINY_SOURCES) $(LUA_SOURCES)
	$(CC) $(LUA_SOURCES) -o $@ $(LUA_FLAGS) $(CFLAGS)

$(PROG)-lua-sqlite: $(LUA_SQLITE_SOURCES)
	$(CC) $(LUA_SQLITE_SOURCES) -o $@ $(LUA_SQLITE_FLAGS) $(CFLAGS)

$(PROG)-lua-sqlite-ssl: $(BIG_SOURCES)
	$(CC) $(BIG_SOURCES) -o $@ $(BIG_FLAGS) $(CFLAGS)

$(PROG)-big: $(BIG_SOURCES)
	$(CC) $(BIG_SOURCES) -o $@ $(CFLAGS) $(BIG_FLAGS)

unix_unit_test: $(LUA_SOURCES) Makefile ./test/unit_test.c
	$(CC) ./test/unit_test.c lua_5.2.1.c $(CFLAGS) -g -O0 -o t && ./t

# Windows build
$(PROG)-lua-sqlite.exe: $(LUA_SQLITE_SOURCES)
	$(MSVC)/bin/rc res.rc
	$(CL) $(LUA_SQLITE_SOURCES) $(LUA_SQLITE_FLAGS) /link $(LINK_FLAGS) res.res /out:$@

$(PROG)-lua-sqlite-ssl.exe: $(BIG_SOURCES)
	$(MSVC)/bin/rc res.rc
	$(CL) $(BIG_SOURCES) $(BIG_FLAGS) /link $(LINK_FLAGS) res.res /out:$@

$(PROG)-lua.exe: $(LUA_SOURCES)
	$(MSVC)/bin/rc res.rc
	$(CL) $(LUA_SOURCES) $(LUA_FLAGS) /link $(LINK_FLAGS) res.res /out:$@

$(PROG).exe: $(TINY_SOURCES)
	$(MSVC)/bin/rc res.rc
	$(CL) $(TINY_SOURCES) /link $(LINK_FLAGS) res.res /out:$@

$(PROG).dll: $(TINY_SOURCES) Makefile
	$(CL) ../mongoose.c /Gz /link $(LINK_FLAGS) /DLL /DEF:dll.def /out:$@

# This is broken now due to SSL exclusion
windows_unit_test.exe: ../mongoose.c Makefile
	$(CL) ./test/unit_test.c lua_5.2.1.c  \
          /link /libpath:$(MSVC)/lib advapi32.lib /out:$@
	./$@

# MacOS build with Cocoa GUI
# For codesign to work in non-interactive mode, unlock login keychain:
# security unlock ~/Library/Keychains/login.keychain
# See e.g. http://lists.apple.com/archives/apple-cdsa/2008/Jan/msg00027.html
macos: $(BIG_SOURCES)
	DIR=dmg/Mongoose.app
	rm -rf dmg
	mkdir -p dmg/Mongoose.app/Contents/{MacOS,Resources}
	install -m 644 *.icns *.png dmg/Mongoose.app/Contents/Resources/
	install -m 644 Info.plist dmg/Mongoose.app/Contents/
	$(CC) $(BIG_SOURCES) \
  -DUSE_COCOA $(CFLAGS) $(BIG_FLAGS) -mmacosx-version-min=10.4 \
  -framework Cocoa -ObjC -arch i386 -arch x86_64 \
  -o dmg/Mongoose.app/Contents/MacOS/Mongoose
	ln -fs /Applications dmg/
	hdiutil create Mongoose-$(VERSION).dmg -volname "Mongoose $(VERSION)" \
  -srcfolder dmg -ov
	#rm -rf dmg

tests:
	perl ./test/test.pl $(TEST)

tarball: clean
	rm -rf $(VDIR)
	install -d $(VDIR) $(VDIR)/docs $(VDIR)/examples
	install -m 644 ../{LICENSE,README.md,mongoose.[ch]} $(VDIR)
	install -m 644 ../docs/*.md $(VDIR)/docs
	install -m 644 ../examples/{Makefile,*.c} $(VDIR)/examples
	tar -czf $(VDIR).tgz $(VDIR)

release: tarball macos
	wine make mongoose.exe mongoose-lua-sqlite-ssl.exe
	upx mongoose.exe
	upx mongoose-lua-sqlite-ssl.exe
	cp mongoose.exe mongoose-$(VERSION).exe
	cp mongoose-lua-sqlite-ssl.exe mongoose-lua-sqlite-ssl-$(VERSION).exe
	cp mongoose-lua-sqlite-ssl.exe mongoose_php_bundle/mongoose.exe
	zip -r mongoose-php-$(VERSION).zip mongoose_php_bundle/

clean:
	cd ../examples && $(MAKE) clean
	rm -rf *.o *.core $(PROG) *.obj *.so $(PROG).txt *.dSYM *.tgz \
	$(PROG).exe *.dll *.lib res.o res.RES *.dSYM *.zip *.pdb \
	*.exe *dmg* $(PROG)-* unix_unit_test
