# This Makefile is part of Mongoose web server project,
# https://github.com/valenok/mongoose
#
# This Makefile is GNU make compatible. You can get GNU Make from
# http://gnuwin32.sourceforge.net/packages/make.htm
#
# Example custom build:
# CFLAGS_EXTRA="-g -O0 -DNO_SSL_DL -DUSE_LUA -llua -lcrypto -lssl" make linux
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

PROG = mongoose
EXE_SUFFIX =
CFLAGS = -std=c99 -O2 -W -Wall -pedantic -pthread -pipe -I.. $(CFLAGS_EXTRA)
VERSION = $(shell perl -lne \
            'print $$1 if /define\s+MONGOOSE_VERSION\s+"(\S+)"/' ../mongoose.c)

TINY_SOURCES = ../mongoose.c main.c
LUA_SOURCES = $(TINY_SOURCES) sqlite3.c lsqlite3.c lua_5.2.1.c

SQLITE_FLAGS = -DTHREADSAFE=1 -DSQLITE_ENABLE_FTS3 \
               -DSQLITE_ENABLE_FTS3_PARENTHESIS
LUA_FLAGS = -DUSE_LUA -DUSE_LUA_SQLITE3 -DLUA_COMPAT_ALL
LUA_SQLITE_FLAGS = $(SQLITE_FLAGS) $(LUA_FLAGS)

# Using Visual Studio 6.0. To build Mongoose:
#  Set MSVC variable below to where VS 6.0 is installed on your system
#  Run "PATH_TO_VC6\bin\nmake windows"
MSVC = ../../vc6
#DBG = /Zi /Od
DBG  = /DNDEBUG /O1
CL   = $(MSVC)/bin/cl /MD /TC /nologo $(DBG) /W3 /GA /I$(MSVC)/include /I..
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

all:
	@echo "make (unix|windows|macos)"

mongoose.c: mod_lua.c mongoose.h Makefile

unix_unit_test: $(LUA_SOURCES) Makefile
	$(CC) ../test/unit_test.c lua_5.2.1.c -o $@ $(CFLAGS) -g -O0
	./$@

# Make sure that the compiler flags come last in the compilation string.
# If not so, this can break some on some Linux distros which use
# "-Wl,--as-needed" turned on by default  in cc command.
# Also, this is turned in many other distros in static linkage builds.
$(PROG): $(TINY_SOURCES)
	$(CC) $(TINY_SOURCES) -o $@ $(CFLAGS)

$(PROG)-lua: $(LUA_SOURCES)
	$(CC) $(LUA_SOURCES) -o $@ $(LUA_SQLITE_FLAGS) $(CFLAGS)

unix: unix_unit_test $(PROG) $(PROG)-lua

# Windows build
$(PROG).exe: $(LUA_SOURCES)
	$(MSVC)/bin/rc res.rc
	$(CL) $(LUA_SOURCES) $(LUA_SQLITE_FLAGS) /link $(LINK_FLAGS) res.res /out:$@

$(PROG)-tiny.exe: $(TINY_SOURCES)
	$(MSVC)/bin/rc res.rc
	$(CL) $(TINY_SOURCES) /link $(LINK_FLAGS) res.res /out:$@

# This is broken now due to SSL exclusion
windows_unit_test.exe: $(LUA_SOURCES) Makefile
	$(CL) ../test/unit_test.c lua_5.2.1.c -DLUA_COMPAT_ALL \
          /link /libpath:$(MSVC)/lib advapi32.lib /out:$@
	./$@

windows: $(PROG).exe $(PROG)-tiny.exe

# MacOS build with Cocoa GUI
# For codesign to work in non-interactive mode, unlock login keychain:
# security unlock ~/Library/Keychains/login.keychain
# See e.g. http://lists.apple.com/archives/apple-cdsa/2008/Jan/msg00027.html
macos: $(LUA_SOURCES)
	DIR=dmg/Mongoose.app
	rm -rf dmg
	mkdir -p dmg/Mongoose.app/Contents/{MacOS,Resources}
	install -m 644 *.icns *.png dmg/Mongoose.app/Contents/Resources/
	install -m 644 Info.plist dmg/Mongoose.app/Contents/
	$(CC) $(LUA_SOURCES) \
  -DUSE_COCOA $(CFLAGS) $(LUA_SQLITE_FLAGS) -mmacosx-version-min=10.4 \
  -framework Cocoa -ObjC -arch i386 -arch x86_64 \
  -o dmg/Mongoose.app/Contents/MacOS/Mongoose
	ln -fs /Applications dmg/
	hdiutil create Mongoose-$(VERSION).dmg -volname "Mongoose $(VERSION)" \
  -srcfolder dmg -ov
	#rm -rf dmg

tests:
	perl ../test/test.pl $(TEST)

tarball: clean
	rm -rf mongoose-$(VERSION)
	mkdir mongoose-$(VERSION)
	install -m 644 ../{LICENSE,*.[ch]} mongoose-$(VERSION)
	tar -czf mongoose-$(VERSION).tgz mongoose-$(VERSION)

release: tarball macos
	wine make windows
	upx mongoose.exe
	upx mongoose-tiny.exe
	cp mongoose.exe mongoose-$(VERSION).exe
	cp mongoose-tiny.exe mongoose-tiny-$(VERSION).exe
	cp mongoose.exe mongoose_php_bundle/
	zip -r mongoose-php-$(VERSION).zip mongoose_php_bundle/

clean:
	cd ../examples && $(MAKE) clean
	rm -rf *.o *.core $(PROG) *.obj *.so $(PROG).txt *.dSYM *.tgz \
	$(PROG).exe *.dll *.lib res.o res.RES *.dSYM *.zip *.pdb \
	*.exe *dmg* $(PROG)-* unix_unit_test
