SRCS = mongoose.c test/unit_test.c test/packed_fs.c
HDRS = $(wildcard src/*.h)
DEFS ?= -DMG_MAX_HTTP_HEADERS=7 -DMG_ENABLE_LINES -DMG_ENABLE_PACKED_FS=1
WARN ?= -W -Wall -Werror -Wshadow -Wdouble-promotion -fno-common -Wconversion -Wundef
OPTS ?= -O3 -g3
INCS ?= -Isrc -I.
CFLAGS ?= $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) $(EXTRA)
SSL ?= MBEDTLS
CWD ?= $(realpath $(CURDIR))
DOCKER ?= docker run --rm -e Tmp=. -e WINEDEBUG=-all -v $(CWD):$(CWD) -w $(CWD)
VCFLAGS = /nologo /W3 /O2 /I. $(DEFS) $(TFLAGS)
IPV6 ?= 1
ASAN_OPTIONS ?=
EXAMPLES := $(wildcard examples/*)
PREFIX ?= /usr/local
VERSION ?= $(shell cut -d'"' -f2 src/version.h)
.PHONY: examples test

ifeq "$(SSL)" "MBEDTLS"
MBEDTLS ?= /usr
CFLAGS  += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLS)/include -I/usr/include
LDFLAGS ?= -L$(MBEDTLS)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif
ifeq "$(SSL)" "OPENSSL"
OPENSSL ?= /usr
CFLAGS  += -DMG_ENABLE_OPENSSL=1 -I$(OPENSSL)/include
LDFLAGS ?= -L$(OPENSSL)/lib -lssl -lcrypto
endif

all: mg_prefix unpacked test test++ arm examples vc98 vc2017 mingw mingw++ linux linux++ fuzz

examples:
	@for X in $(EXAMPLES); do $(MAKE) -C $$X example || break; done

test/packed_fs.c: Makefile src/fs.h src/ssi.h test/fuzz.c test/data/a.txt
	$(CC) $(CFLAGS) test/pack.c -o pack
	./pack $? > $@

# Check that all external (exported) symbols have "mg_" prefix
mg_prefix: mongoose.c mongoose.h
	$(CC) mongoose.c $(CFLAGS) -c -o /tmp/x.o && nm /tmp/x.o | grep ' T' | grep -v 'mg_' ; test $$? = 1

# C++ build
test++: CC = g++
test++: WARN += -Wno-shadow -Wno-missing-field-initializers -Wno-deprecated
test++: test

# Make sure we can build from an unamalgamated sources
unamalgamated: $(HDRS) Makefile test/packed_fs.c
	$(CC) src/*.c test/packed_fs.c test/unit_test.c $(CFLAGS) $(LDFLAGS) -g -o unit_test

unpacked:
	$(CC) -I. mongoose.c test/unit_test.c -o unit_test

fuzzer: mongoose.c mongoose.h Makefile test/fuzz.c
	clang mongoose.c test/fuzz.c $(WARN) $(INCS) -DMG_ENABLE_LINES -DMG_ENABLE_LOG=0 -fsanitize=fuzzer,signed-integer-overflow,address -g -o $@

fuzz: fuzzer
	$(RUN) ./fuzzer

# make CC=/usr/local/opt/llvm\@8/bin/clang ASAN_OPTIONS=detect_leaks=1
test: CFLAGS += -DMG_ENABLE_IPV6=$(IPV6) -fsanitize=address#,undefined
test: mongoose.h  Makefile $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -coverage $(LDFLAGS) -g -o unit_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./unit_test

coverage: test
	gcov -l -n *.gcno | sed '/^$$/d' | sed 'N;s/\n/ /'
	gcov mongoose.gcno >/dev/null

upload-coverage: coverage
	curl -s https://codecov.io/bash | /bin/bash

infer:
	infer run -- cc test/unit_test.c -c -W -Wall -Werror -Isrc -I. -O2 -DMG_ENABLE_MBEDTLS=1 -DMG_ENABLE_LINES -I/usr/local/Cellar/mbedtls/2.23.0/include  -DMG_ENABLE_IPV6=1 -g -o /dev/null

arm: mongoose.h $(SRCS)
	$(DOCKER) mdashnet/armgcc arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb $(SRCS) test/mongoose_custom.c -Itest -DMG_ARCH=MG_ARCH_CUSTOM $(OPTS) $(WARN) $(INCS) $(DEFS) -o unit_test -nostartfiles --specs nosys.specs -e 0

riscv: mongoose.h $(SRCS)
	$(DOCKER) mdashnet/riscv riscv-none-elf-gcc -march=rv32imc -mabi=ilp32 $(SRCS) test/mongoose_custom.c -Itest -DMG_ARCH=MG_ARCH_CUSTOM $(OPTS) $(WARN) $(INCS) $(DEFS) -o unit_test

#vc98: VCFLAGS += -DMG_ENABLE_IPV6=1
vc98: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc98 wine cl $(SRCS) $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(DOCKER) mdashnet/vc98 wine $@.exe

#vc2017: VCFLAGS += -DMG_ENABLE_IPV6=1
vc2017: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc2017 wine64 cl $(SRCS) $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(DOCKER) mdashnet/vc2017 wine64 $@.exe

mingw: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw i686-w64-mingw32-gcc $(SRCS) -W -Wall -Werror -I. $(DEFS) -lwsock32 -o test.exe
	$(DOCKER) mdashnet/vc98 wine test.exe

mingw++: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw i686-w64-mingw32-g++ $(SRCS) -W -Wall -Werror -I. $(DEFS) -lwsock32 -o test.exe
  # Note: for some reason, a binary built with mingw g++, fails to run

#linux: CFLAGS += -DMG_ENABLE_IPV6=$(IPV6)
linux: CFLAGS += -fsanitize=address,undefined
linux: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/cc2 gcc $(SRCS) $(CFLAGS) $(LDFLAGS) -o unit_test_gcc
	$(DOCKER) mdashnet/cc2 ./unit_test_gcc

linux++: CC = g++
linux++: WARN += -Wno-missing-field-initializers
linux++: linux

linux-libs: CFLAGS += -fPIC
linux-libs: mongoose.o
	$(CC) mongoose.o $(LDFLAGS) -shared -o libmongoose.so.$(VERSION)
	$(AR) rcs libmongoose.a mongoose.o

install: linux-libs
	install -Dm644 libmongoose.a libmongoose.so.$(VERSION) $(DESTDIR)$(PREFIX)/lib
	ln -s libmongoose.so.$(VERSION) $(DESTDIR)$(PREFIX)/lib/libmongoose.so
	install -Dm644 mongoose.h $(DESTDIR)$(PREFIX)/include/mongoose.h

uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/lib/libmongoose.a $(DESTDIR)$(PREFIX)/lib/libmongoose.so.$(VERSION) $(DESTDIR)$(PREFIX)/include/mongoose.h $(DESTDIR)$(PREFIX)/lib/libmongoose.so

mongoose.c: Makefile $(wildcard src/*)
	(cat src/license.h; echo; echo '#include "mongoose.h"' ; (for F in src/*.c ; do echo; echo '#ifdef MG_ENABLE_LINES'; echo "#line 1 \"$$F\""; echo '#endif'; cat $$F | sed -e 's,#include ".*,,'; done))> $@

mongoose.h: $(HDRS) Makefile
	(cat src/license.h; echo; echo '#ifndef MONGOOSE_H'; echo '#define MONGOOSE_H'; echo; cat src/version.h ; echo; echo '#ifdef __cplusplus'; echo 'extern "C" {'; echo '#endif'; cat src/arch.h src/arch_*.h src/config.h src/str.h src/log.h src/timer.h src/util.h src/fs.h src/url.h src/iobuf.h src/base64.h src/md5.h src/sha1.h src/event.h src/net.h src/http.h src/ssi.h src/tls.h src/tls_mbed.h src/tls_openssl.h src/ws.h src/sntp.h src/mqtt.h src/dns.h | sed -e 's,#include ".*,,' -e 's,^#pragma once,,'; echo; echo '#ifdef __cplusplus'; echo '}'; echo '#endif'; echo '#endif  // MONGOOSE_H')> $@

clean:
	rm -rf $(PROG) *.o *.dSYM unit_test* ut fuzzer *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb slow-unit* _CL_* infer-out data.txt crash-* test/packed_fs.c pack
	@for X in $(EXAMPLES); do $(MAKE) -C $$X clean; done
