SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)
DEFS ?= -DMG_MAX_HTTP_HEADERS=5 -DMG_ENABLE_LINES -DMG_ENABLE_HTTP_DEBUG_ENDPOINT=1 -DMG_ENABLE_DIRECTORY_LISTING=1 -DMG_ENABLE_SSI=1
CFLAGS ?= -W -Wall -Werror -Isrc -I. -O0 -g $(DEFS) $(TFLAGS) $(EXTRA)
SSL ?= MBEDTLS
CDIR ?= $(realpath $(CURDIR))
VC98 = docker run --rm -e WINEDEBUG=-all -v $(CDIR):$(CDIR) -w $(CDIR) docker.io/mdashnet/vc98
VC2017 = docker run --rm -e WINEDEBUG=-all -v $(CDIR):$(CDIR) -w $(CDIR) docker.io/mdashnet/vc2017
MINGW = docker run --rm -v $(CDIR):$(CDIR) -w $(CDIR) docker.io/mdashnet/mingw
GCC = docker run --rm -v $(CDIR):$(CDIR) -w $(CDIR) mdashnet/cc2
VCFLAGS = /nologo /W3 /O2 /I. $(DEFS) $(TFLAGS)
CLANG ?= clang # /usr/local/opt/llvm\@9/bin/clang
IPV6 ?= 1
ASAN_OPTIONS ?=
EXAMPLES := $(wildcard examples/*)
EXAMPLE_TARGET ?= example
.PHONY: ex test

ifeq "$(SSL)" "MBEDTLS"
MBEDTLS_DIR ?= $(shell brew --cellar mbedtls)
MBEDTLS_VER ?= $(shell brew info mbedtls --json | jq -j .[0].installed[0].version)
MBEDTLS ?= $(MBEDTLS_DIR)/$(MBEDTLS_VER)
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLS)/include -I/usr/include
LDFLAGS ?= -L$(MBEDTLS)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif
ifeq "$(SSL)" "OPENSSL"
OPENSSL_DIR ?= $(shell brew --cellar openssl)
OPENSSL_VER ?= $(shell brew info openssl --json | jq -j .[0].installed[0].version)
OPENSSL ?= $(OPENSSL_DIR)/$(OPENSSL_VER)
CFLAGS += -DMG_ENABLE_OPENSSL=1 -I$(OPENSSL)/include
LDFLAGS ?= -L$(OPENSSL)/lib -lssl -lcrypto
endif

all: mg_prefix test test++ ex vc98 vc2017 mingw mingw++ linux linux++ infer fuzz

ex:
	@for X in $(EXAMPLES); do $(MAKE) -C $$X $(EXAMPLE_TARGET); done

# Check that all external (exported) symbols have "mg_" prefix
mg_prefix: mongoose.c mongoose.h
	$(CLANG) mongoose.c $(CFLAGS) -c -o /tmp/x.o && nm /tmp/x.o | grep ' T' | grep -v 'mg_' ; test $$? = 1

# C++ build
test++: CLANG = g++ -Wno-deprecated -Wno-missing-field-initializers
test++: unamalgamated

# Make sure we can build from an unamalgamated sources
unamalgamated: $(SRCS) $(HDRS) Makefile
	$(CLANG) src/*.c test/unit_test.c $(CFLAGS) $(LDFLAGS) -g -o unit_test

fuzz: mongoose.c mongoose.h Makefile test/fuzz.c
	$(CLANG) mongoose.c test/fuzz.c $(CFLAGS) -DMG_ENABLE_LINES -DMG_ENABLE_LOG=0 -fsanitize=fuzzer,signed-integer-overflow,address $(LDFLAGS) -g -o fuzzer
	$(DEBUGGER) ./fuzzer

# make CLANG=/usr/local/opt/llvm\@8/bin/clang ASAN_OPTIONS=detect_leaks=1
test: CFLAGS += -DMG_ENABLE_IPV6=$(IPV6) -fsanitize=address#,undefined
test: mongoose.c mongoose.h  Makefile test/unit_test.c
	$(CLANG) mongoose.c test/unit_test.c $(CFLAGS) -coverage $(LDFLAGS) -g -o unit_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(DEBUGGER) ./unit_test

coverage: test
	gcov -l -n *.gcno | sed '/^$$/d' | sed 'N;s/\n/ /'
	gcov mongoose.gcno >/dev/null

upload-coverage: coverage
	curl -s https://codecov.io/bash | /bin/bash

infer:
	infer run -- cc test/unit_test.c -c -W -Wall -Werror -Isrc -I. -O2 -DMG_ENABLE_MBEDTLS=1 -DMG_ENABLE_LINES -I/usr/local/Cellar/mbedtls/2.23.0/include  -DMG_ENABLE_IPV6=1 -g -o /dev/null

#vc98: VCFLAGS += -DMG_ENABLE_IPV6=1
vc98: Makefile mongoose.c mongoose.h test/unit_test.c
	$(VC98) wine cl mongoose.c test/unit_test.c $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(VC98) wine $@.exe

#vc2017: VCFLAGS += -DMG_ENABLE_IPV6=1
vc2017: Makefile mongoose.c mongoose.h test/unit_test.c
	$(VC2017) wine64 cl mongoose.c test/unit_test.c $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(VC2017) wine64 $@.exe

mingw: Makefile mongoose.c mongoose.h test/unit_test.c
	$(MINGW) i686-w64-mingw32-gcc mongoose.c test/unit_test.c -W -Wall -Werror -I. $(DEFS) -lwsock32 -o test.exe
	$(VC98) wine test.exe

mingw++: Makefile mongoose.c mongoose.h test/unit_test.c
	$(MINGW) i686-w64-mingw32-g++ mongoose.c test/unit_test.c -W -Wall -Werror -I. $(DEFS) -lwsock32 -o test.exe
  # Note: for some reason, a binary built with mingw g++, fails to run

#linux: CFLAGS += -DMG_ENABLE_IPV6=$(IPV6)
linux: CFLAGS += -fsanitize=address,undefined
linux: Makefile mongoose.c mongoose.h test/unit_test.c
	$(GCC) $(CC) mongoose.c test/unit_test.c $(CFLAGS) $(LDFLAGS) -o unit_test_gcc
	$(GCC) ./unit_test_gcc

linux++: CC = g++ -Wno-missing-field-initializers
linux++: linux

mongoose.c: $(SRCS) Makefile
	(cat src/license.h; echo; echo '#include "mongoose.h"' ; (for F in src/private.h src/*.c ; do echo; echo '#ifdef MG_ENABLE_LINES'; echo "#line 1 \"$$F\""; echo '#endif'; cat $$F | sed -e 's,#include ".*,,'; done))> $@

mongoose.h: $(HDRS) Makefile
	(cat src/license.h src/version.h ; cat src/arch.h src/arch_*.h src/config.h src/str.h src/log.h src/timer.h src/util.h src/url.h src/iobuf.h src/base64.h src/md5.h src/sha1.h src/event.h src/net.h src/http.h src/ssi.h src/tls.h src/ws.h src/sntp.h src/mqtt.h src/dns.h | sed -e 's,#include ".*,,' -e 's,^#pragma once,,')> $@

clean: EXAMPLE_TARGET = clean
clean: ex
	rm -rf $(PROG) *.o *.dSYM unit_test* ut fuzzer *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb slow-unit* _CL_* infer-out data.txt crash-*
