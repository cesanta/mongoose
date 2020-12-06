SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)
DEFS ?= -DMG_MAX_HTTP_HEADERS=5 -DMG_ENABLE_LINES -DMG_ENABLE_HTTP_DEBUG_ENDPOINT=1
CFLAGS ?= -W -Wall -Werror -Isrc -I. -O0 -g $(DEFS) $(TFLAGS) $(EXTRA)
SSL ?= MBEDTLS
CDIR ?= $(realpath $(CURDIR))
VC98 = docker run --rm -e WINEDEBUG=-all -v $(CDIR):$(CDIR) -w $(CDIR) docker.io/mdashnet/vc98
VC2017 = docker run --rm -e WINEDEBUG=-all -v $(CDIR):$(CDIR) -w $(CDIR) docker.io/mdashnet/vc2017
GCC = docker run --rm -v $(CDIR):$(CDIR) -w $(CDIR) mdashnet/cc2
VCFLAGS = /nologo /W3 /O2 /I. $(DEFS) $(TFLAGS)
CLANG ?= clang # /usr/local/opt/llvm\@9/bin/clang
ASAN_OPTIONS ?=
EXAMPLES := $(wildcard examples/*)
EXAMPLE_TARGET ?= example
.PHONY: $(EXAMPLES)

ifeq "$(SSL)" "MBEDTLS"
MBEDTLSDIR ?= /usr/local/Cellar/mbedtls/2.23.0
CFLAGS += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLSDIR)/include
LDFLAGS ?= -L$(MBEDTLSDIR)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif
ifeq "$(SSL)" "OPENSSL"
OPENSSLDIR ?= /usr/local/Cellar/openssl@1.1/1.1.1g
CFLAGS += -DMG_ENABLE_OPENSSL=1 -I$(OPENSSLDIR)/include
LDFLAGS ?= -L$(OPENSSLDIR)/lib -lssl -lcrypto
endif

all: mg_prefix cpp test ex vc98 vc2017 linux infer fuzz

ex: $(EXAMPLES)
$(EXAMPLES):
	@$(MAKE) -C $@ $(EXAMPLE_TARGET)

# Check that all external (exported) symbols have "mg_" prefix
mg_prefix: mongoose.c mongoose.h
	$(CLANG) mongoose.c $(CFLAGS) -c -o /tmp/x.o && nm /tmp/x.o | grep ' T' | grep -v 'mg_' ; test $$? = 1

# C++ build
cpp: CLANG = g++ -Wno-deprecated
cpp: unamalgamated

# Make sure we can build from an unamalgamated sources
unamalgamated: $(SRCS) $(HDRS) Makefile
	$(CLANG) src/*.c test/unit_test.c $(CFLAGS) $(LDFLAGS) -g -o unit_test

fuzz: mongoose.c mongoose.h Makefile test/fuzz.c
	$(CLANG) mongoose.c test/fuzz.c $(CFLAGS) -DMG_ENABLE_LOG=0 -fsanitize=fuzzer,signed-integer-overflow,address $(LDFLAGS) -g -o fuzzer
	$(DEBUGGER) ./fuzzer

# make CLANG=/usr/local/opt/llvm\@8/bin/clang ASAN_OPTIONS=detect_leaks=1
test: CFLAGS += -DMG_ENABLE_IPV6=1 -fsanitize=address#,undefined
test: mongoose.c mongoose.h  clean Makefile test/unit_test.c
	$(CLANG) mongoose.c test/unit_test.c $(CFLAGS) -coverage $(LDFLAGS) -g -o unit_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(DEBUGGER) ./unit_test

coverage: test
	gcov -l -n *.gcno | sed '/^$$/d' | sed 'N;s/\n/ /'

infer:
	infer run -- cc test/unit_test.c -c -W -Wall -Werror -Isrc -I. -O2 -DMG_ENABLE_MBEDTLS=1 -DMG_ENABLE_LINES -I/usr/local/Cellar/mbedtls/2.23.0/include  -DMG_ENABLE_IPV6=1 -g -o /dev/null

#vc98: VCFLAGS += -DMG_ENABLE_IPV6=1
vc98: clean Makefile mongoose.c mongoose.h test/unit_test.c
	$(VC98) wine cl mongoose.c test/unit_test.c $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(VC98) wine $@.exe

vc2017: CFLAGS += -DMG_ENABLE_IPV6=1
vc2017: clean Makefile mongoose.c mongoose.h test/unit_test.c
	$(VC2017) wine64 cl mongoose.c test/unit_test.c $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(VC2017) wine64 $@.exe

linux: CFLAGS += -DMG_ENABLE_IPV6=1 -fsanitize=address,undefined
linux: clean Makefile mongoose.c mongoose.h test/unit_test.c
	$(GCC) gcc mongoose.c test/unit_test.c $(CFLAGS) $(LDFLAGS) -o unit_test_gcc
	$(GCC) ./unit_test_gcc

mongoose.c: $(SRCS) Makefile
	(cat src/license.h; echo; echo '#include "mongoose.h"' ; (for F in src/private.h src/*.c ; do echo; echo '#ifdef MG_ENABLE_LINES'; echo "#line 1 \"$$F\""; echo '#endif'; cat $$F | sed -e 's,#include ".*,,'; done))> $@

mongoose.h: $(HDRS) Makefile
	(cat src/license.h src/version.h ; cat src/arch.h src/arch_*.h src/config.h src/str.h src/log.h src/timer.h src/util.h src/url.h src/iobuf.h src/base64.h src/md5.h src/sha1.h src/event.h src/net.h src/http.h src/tls.h src/ws.h src/sntp.h src/mqtt.h src/dns.h | sed -e 's,#include ".*,,' -e 's,^#pragma once,,')> $@

clean: EXAMPLE_TARGET = clean
clean: $(EXAMPLES)
	rm -rf $(PROG) *.o *.dSYM unit_test* ut fuzzer *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb slow-unit* _CL_* infer-out

