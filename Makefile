SRCS = mongoose.c test/unit_test.c test/packed_fs.c
HDRS = $(wildcard src/*.h) $(wildcard mip/*.h)
DEFS ?= -DMG_MAX_HTTP_HEADERS=7 -DMG_ENABLE_LINES -DMG_ENABLE_PACKED_FS=1 -DMG_ENABLE_SSI=1
WARN ?= -pedantic -W -Wall -Werror -Wshadow -Wdouble-promotion -fno-common -Wconversion -Wundef
OPTS ?= -O3 -g3
INCS ?= -Isrc -I.
SSL ?=
CWD ?= $(realpath $(CURDIR))
ENV ?=  -e Tmp=. -e WINEDEBUG=-all 
DOCKER ?= docker run --platform linux/amd64 --rm $(ENV) -v $(CWD):$(CWD) -w $(CWD)
VCFLAGS = /nologo /W3 /O2 /MD /I. $(DEFS) $(TFLAGS)
IPV6 ?= 1
ASAN ?= -fsanitize=address,undefined,alignment -fno-sanitize-recover=all -fno-omit-frame-pointer -fno-common
ASAN_OPTIONS ?= detect_leaks=1
EXAMPLES := $(dir $(wildcard examples/*/Makefile)) $(wildcard examples/stm32/nucleo-*)
PREFIX ?= /usr/local
VERSION ?= $(shell cut -d'"' -f2 src/version.h)
COMMON_CFLAGS ?= $(C_WARN) $(WARN) $(INCS) $(DEFS) -DMG_ENABLE_IPV6=$(IPV6) $(TFLAGS)
CFLAGS ?= $(OPTS) $(ASAN) $(COMMON_CFLAGS)
VALGRIND_CFLAGS ?= $(OPTS) $(COMMON_CFLAGS)
VALGRIND_RUN ?= valgrind --tool=memcheck --gen-suppressions=all --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --error-exitcode=1 --exit-on-first-error=yes
.PHONY: examples test valgrind mip_test

ifeq "$(findstring ++,$(CC))" ""
# $(CC) does not end with ++, i.e. we're using C. Apply C flags
C_WARN ?= -Wmissing-prototypes -Wstrict-prototypes
else
# $(CC) ends with ++, i.e. we're using C++. Apply C++ flags
C_WARN ?= -Wno-deprecated
endif

ifeq "$(SSL)" "MBEDTLS"
MBEDTLS ?= /usr/local
CFLAGS  += -DMG_ENABLE_MBEDTLS=1 -I$(MBEDTLS)/include -I/usr/include
LDFLAGS ?= -L$(MBEDTLS)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif

ifeq "$(SSL)" "OPENSSL"
OPENSSL ?= /usr/local
CFLAGS  += -DMG_ENABLE_OPENSSL=1 -I$(OPENSSL)/include
LDFLAGS ?= -L$(OPENSSL)/lib -lssl -lcrypto
endif

all: mg_prefix unamalgamated test mip_test arm examples vc98 vc17 vc22 mingw mingw++ fuzz

mip_test: test/mip_test.c mongoose.c mongoose.h Makefile
	$(CC) test/mip_test.c $(INCS) $(WARN) $(OPTS) $(C_WARN) $(ASAN) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

examples:
	@for X in $(EXAMPLES); do test -f $$X/Makefile || continue; $(MAKE) -C $$X example || exit 1; done

test/packed_fs.c: Makefile src/ssi.h test/fuzz.c test/data/a.txt
	$(CC) $(CFLAGS) test/pack.c -o pack
	$(RUN) ./pack Makefile src/ssi.h test/fuzz.c test/data/a.txt test/data/range.txt > $@

DIR ?= test/data
OUT ?= packed_fs.c
mkfs:
	$(CC) $(CFLAGS) test/pack.c -o pack
	$(RUN) ./pack -s $(DIR) `find $(DIR) -type f` > $(OUT)
#	find $(DIR) -type f | sed -e s,^$(DIR),,g -e s,^/,,g

# Check that all external (exported) symbols have "mg_" prefix
mg_prefix: mongoose.c mongoose.h
	$(CC) mongoose.c $(CFLAGS) -c -o /tmp/x.o && nm /tmp/x.o | grep ' T ' | grep -v 'mg_' ; test $$? = 1

musl: test
musl: ASAN =
musl: WARN += -Wno-sign-conversion
musl: CC = $(DOCKER) mdashnet/cc1 gcc
musl: RUN = $(DOCKER) mdashnet/cc1

# Make sure we can build from an unamalgamated sources
unamalgamated: $(HDRS) Makefile test/packed_fs.c
	$(CC) src/*.c test/packed_fs.c test/unit_test.c $(CFLAGS) $(LDFLAGS) -g -o unit_test

fuzz: ASAN = -fsanitize=fuzzer,signed-integer-overflow,address,undefined
fuzz: mongoose.c mongoose.h Makefile test/fuzz.c
	$(CC) test/fuzz.c $(OPTS) $(WARN) $(INCS) $(TFLAGS) $(ASAN) -o fuzzer
	$(RUN) ./fuzzer

fuzz2: mongoose.c mongoose.h Makefile test/fuzz.c
	$(CC) test/fuzz.c -DMAIN $(OPTS) $(WARN) $(ASAN) $(INCS) -o fuzzer
	$(RUN) ./fuzzer /tmp/fuzzdata

test: Makefile mongoose.h $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) -o unit_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./unit_test

coverage: CFLAGS += -coverage
coverage: test
	gcov -l -n *.gcno | sed '/^$$/d' | sed 'N;s/\n/ /'
	gcov -t mongoose.c > mongoose.gcov

upload-coverage: coverage
	curl -s https://codecov.io/bash | /bin/bash

valgrind: Makefile mongoose.h mongoose.c
	$(CC) $(SRCS) $(VALGRIND_CFLAGS) $(LDFLAGS) -g -o unit_test
	$(VALGRIND_RUN) ./unit_test

arm: DEFS += -DMG_ENABLE_FILE=0 -DMG_ENABLE_MIP=1 -DMG_ARCH=MG_ARCH_NEWLIB 
arm: mongoose.h $(SRCS)
	$(DOCKER) mdashnet/armgcc arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb $(SRCS) $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o unit_test -nostartfiles --specs nosys.specs -e 0

riscv: DEFS += -DMG_ENABLE_FILE=0 -DMG_ENABLE_MIP=1 -DMG_ARCH=MG_ARCH_NEWLIB 
riscv: mongoose.h $(SRCS)
	$(DOCKER) mdashnet/riscv riscv-none-elf-gcc -march=rv32imc -mabi=ilp32 $(SRCS) $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o unit_test

vc98: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc98 wine cl $(SRCS) $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(DOCKER) mdashnet/vc98 wine $@.exe

vc17: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc17 wine64 cl $(SRCS) $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(DOCKER) mdashnet/vc17 wine64 $@.exe

vc22: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc22 wine64 cl $(SRCS) $(VCFLAGS) ws2_32.lib /Fe$@.exe
	$(DOCKER) mdashnet/vc22 wine64 $@.exe

mingw: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw x86_64-w64-mingw32-gcc $(SRCS) -W -Wall -Werror -I. $(DEFS) -lwsock32 -o $@.exe
	$(DOCKER) mdashnet/mingw wine64 $@.exe

mingw++: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw x86_64-w64-mingw32-g++ $(SRCS) -W -Wall -Werror -I. $(DEFS) -lwsock32 -o $@.exe

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

mongoose.c: Makefile $(wildcard src/*) $(wildcard mip/*.c)
	(cat src/license.h; echo; echo '#include "mongoose.h"' ; (for F in src/*.c mip/*.c ; do echo; echo '#ifdef MG_ENABLE_LINES'; echo "#line 1 \"$$F\""; echo '#endif'; cat $$F | sed -e 's,#include ".*,,'; done))> $@

mongoose.h: $(HDRS) Makefile
	(cat src/license.h; echo; echo '#ifndef MONGOOSE_H'; echo '#define MONGOOSE_H'; echo; cat src/version.h ; echo; echo '#ifdef __cplusplus'; echo 'extern "C" {'; echo '#endif'; cat src/arch.h src/arch_*.h src/config.h src/str.h src/fmt.h src/log.h src/timer.h src/fs.h src/util.h src/url.h src/iobuf.h src/base64.h src/md5.h src/sha1.h src/event.h src/net.h src/http.h src/ssi.h src/tls.h src/tls_mbed.h src/tls_openssl.h src/ws.h src/sntp.h src/mqtt.h src/dns.h src/json.h src/rpc.h mip/mip.h mip/driver_*.h | sed -e '/keep/! s,#include ".*,,' -e 's,^#pragma once,,'; echo; echo '#ifdef __cplusplus'; echo '}'; echo '#endif'; echo '#endif  // MONGOOSE_H')> $@

clean:
	rm -rf $(PROG) *.exe *.o *.dSYM *_test* ut fuzzer *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb slow-unit* _CL_* infer-out data.txt crash-* test/packed_fs.c pack
	@for X in $(EXAMPLES); do $(MAKE) -C $$X clean; done
