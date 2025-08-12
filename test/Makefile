SRCS = mongoose.c unit_test.c packed_fs.c
HDRS = $(wildcard ../src/*.h) $(wildcard ../src/drivers/*.h)
DEFS ?= -DMG_MAX_HTTP_HEADERS=7 -DMG_ENABLE_LINES -DMG_ENABLE_PACKED_FS=1 -DMG_ENABLE_SSI=1 -DMG_ENABLE_ASSERT=1
WARN ?= -pedantic -W -Wall -Werror -Wshadow -Wdouble-promotion -fno-common -Wconversion -Wundef
OPTS ?= -O3 -g3
INCS ?= -Isrc -I.
SSL ?=
CWD ?= $(realpath $(CURDIR))
ROOT_DIR = $(realpath $(CWD)/..)
ENV ?=  -e Tmp=. -e WINEDEBUG=-all
DOCKER_BIN ?= docker
# use net=host so all tests have the same network and can access localhost and ad hoc services
DOCKER ?= $(DOCKER_BIN) run --platform linux/amd64 --net=host --rm $(ENV) -v $(ROOT_DIR):$(ROOT_DIR) -w $(CWD)
VCFLAGS ?= /nologo /W3 /O2 /MD /I.
IPV6 ?= 1
ASAN ?= -fsanitize=address,undefined,alignment -fno-sanitize-recover=all -fno-omit-frame-pointer -fno-common
ASAN_OPTIONS ?= detect_leaks=1
# This does not include ESP32, ESP8266, Microchip; all examples that are directly placed under an OS tutorial. Also, it is needed that way...
TUTORIALS_EMBEDDED := $(dir $(wildcard ../tutorials/infineon/*/Makefile)) $(dir $(wildcard ../tutorials/nxp/*/Makefile)) $(dir $(wildcard ../tutorials/pico-sdk/*/Makefile)) $(dir $(wildcard ../tutorials/renesas/*/Makefile)) $(dir $(wildcard ../tutorials/stm32/*/Makefile)) $(dir $(wildcard ../tutorials/ti/*/Makefile)) $(dir $(wildcard ../tutorials/wch/*/Makefile))
TUTORIALS := ../tutorials/micropython/ $(filter-out $(TUTORIALS_EMBEDDED), $(dir $(wildcard ../tutorials/*/*/Makefile)))
TUTORIALS_QUICK := $(filter-out ../tutorials/micropython/ ../tutorials/tcpip/pcap-driver/, $(TUTORIALS))
TUTORIALS_MAC := $(filter-out ../tutorials/micropython/ ../tutorials/tcpip/pcap-driver/ ../tutorials/tcpip/tap-driver/, $(TUTORIALS))
TUTORIALS_WIN := $(dir $(wildcard ../tutorials/http/file-*/Makefile) $(wildcard ../tutorials/http/http-*/Makefile) $(wildcard ../tutorials/mqtt/*/Makefile) $(wildcard ../tutorials/websocket/*/Makefile) $(wildcard ../tutorials/webui/*/Makefile))
PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include
VERSION ?= $(shell cut -d'"' -f2 ../src/version.h)
COMMON_CFLAGS ?= $(C_WARN) $(WARN) $(INCS) $(DEFS) -DMG_ENABLE_IPV6=$(IPV6) $(TFLAGS) -pthread
CFLAGS ?= $(OPTS) $(ASAN) $(COMMON_CFLAGS)
VALGRIND_CFLAGS ?= $(OPTS) $(COMMON_CFLAGS)
VALGRIND_RUN ?= valgrind --tool=memcheck --gen-suppressions=all --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --error-exitcode=1 --exit-on-first-error=yes --fair-sched=yes
.PHONY: clean_tutorials tutorials mip_test test valgrind

ifeq "$(findstring ++,$(CC))" ""
# $(CC) does not end with ++, i.e. we're using C. Apply C flags
C_WARN ?= -Wmissing-prototypes -Wstrict-prototypes
else
# $(CC) ends with ++, i.e. we're using C++. Apply C++ flags
C_WARN ?= -Wno-deprecated
endif

ifeq "$(SSL)" "OPENSSL"
ifeq "$(OPENSSL)" ""
CFLAGS  += -DMG_TLS=MG_TLS_OPENSSL $(shell pkg-config openssl --cflags)
LDFLAGS += $(shell pkg-config openssl --libs)
else
CFLAGS  += -DMG_TLS=MG_TLS_OPENSSL -I$(OPENSSL)/include
LDFLAGS += -L$(OPENSSL)/lib -lssl -lcrypto
endif
endif

ifeq "$(SSL)" "MBEDTLS"
ifeq "$(MBEDTLS)" ""
# Ubuntu 22.04 does not provide 'pc' files for MbedTLS (others may)
#CFLAGS  += -DMG_TLS=MG_TLS_MBED $(shell pkg-config mbedtls --cflags) $(shell pkg-config mbedcrypto --cflags) $(shell pkg-config mbedx509 --cflags)
#LDFLAGS += $(shell pkg-config mbedtls --libs) $(shell pkg-config mbedcrypto --libs) $(shell pkg-config mbedx509 --libs)
CFLAGS  += -DMG_TLS=MG_TLS_MBED -I/usr/include
LDFLAGS += -lmbedtls -lmbedcrypto -lmbedx509
else
CFLAGS  += -DMG_TLS=MG_TLS_MBED -I$(MBEDTLS)/include -I/usr/include
LDFLAGS += -L$(MBEDTLS)/lib -lmbedtls -lmbedcrypto -lmbedx509
endif
endif

ifeq "$(SSL)" "WOLFSSL"
# WolfSSL requires overriding the include path when used in OpenSSL compatibility mode (we do)
#ifeq "$(WOLFSSL)" ""
#CFLAGS  += -DMG_TLS=MG_TLS_WOLFSSL -DEXTERNAL_OPTS_OPENVPN $(shell pkg-config openssl --cflags)
#LDFLAGS += $(shell pkg-config wolfssl --libs)
#else
WOLFSSL ?= $(shell pkg-config wolfssl --variable=prefix)
CFLAGS  += -DMG_TLS=MG_TLS_WOLFSSL -DEXTERNAL_OPTS_OPENVPN -I$(WOLFSSL)/include -I$(WOLFSSL)/include/wolfssl
LDFLAGS += -L$(WOLFSSL)/lib -lwolfssl
#endif
endif

ifeq "$(SSL)" "BUILTIN"
DEFS += -DMG_TLS=MG_TLS_BUILTIN
endif

all:
	$(MAKE) -C ../tutorials/http/http-server

tall: mg_prefix unamalgamated test valgrind fuzz fuzz_tls vc98 vc17 vc22 mingw mingw++ arm armhf riscv s390 tutorials tutorials_win tutorials_embedded mip_test mip_vc98

mip_test: mip_test.c mongoose.c mongoose.h packed_fs.c Makefile
	$(CC) mip_test.c packed_fs.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

mip_tap_test: mip_tap_test.c mongoose.c mongoose.h packed_fs.c Makefile tls_multirec/server
	$(CC) mip_tap_test.c packed_fs.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

mip_vc98: mip_test.c mongoose.c mongoose.h packed_fs.c Makefile
	$(DOCKER) mdashnet/vc98 wine cl mip_test.c packed_fs.c $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) mdashnet/vc98 wine $@.exe

packed_fs.c: Makefile data/ssi.h fuzz.c data/a.txt data/ca.pem certs/ca.crt certs/server.crt certs/server.key
	$(CC) $(CFLAGS) pack.c -o pack
	$(RUN) ./pack Makefile data/ssi.h fuzz.c data/a.txt data/range.txt data/ca.pem certs/ca.crt certs/server.crt certs/server.key certs/client.key certs/client.crt > $@

# Check that all external (exported) symbols have "mg_" prefix
mg_prefix: mongoose.c mongoose.h
	$(CC) mongoose.c $(CFLAGS) -c -o /tmp/x.o && nm /tmp/x.o | grep ' T ' | grep -v 'mg_' ; test $$? = 1

musl: test
musl: ASAN =
musl: WARN += -Wno-sign-conversion
musl: CC = $(DOCKER) mdashnet/cc1 gcc
musl: RUN = $(DOCKER) mdashnet/cc1

# Make sure we can build from unamalgamated sources.
unamalgamated: CFLAGS += -DMG_ENABLE_MD5=1
unamalgamated: $(HDRS) Makefile packed_fs.c
	$(CC) ../src/*.c packed_fs.c unit_test.c $(CFLAGS) $(LDFLAGS) -g -o unit_test

fuzz: ASAN = -fsanitize=fuzzer,signed-integer-overflow,address,undefined
fuzz: mongoose.c mongoose.h Makefile fuzz.c
	$(CC) fuzz.c $(OPTS) $(WARN) $(INCS) $(TFLAGS) $(ASAN) -o fuzzer
	$(RUN) ./fuzzer $(ARGS)

FUZZDATA ?= /tmp/fuzzdata
fuzz2: mongoose.c mongoose.h Makefile fuzz.c
	$(CC) fuzz.c -DMAIN $(OPTS) $(WARN) $(ASAN) $(INCS) -o fuzzer
	$(RUN) ./fuzzer $(FUZZDATA)

fuzz_tls: ASAN = -fsanitize=fuzzer,signed-integer-overflow,address,undefined
fuzz_tls: mongoose.c mongoose.h Makefile fuzz_tls.c
	$(CC) fuzz_tls.c $(OPTS) $(WARN) $(INCS) $(TFLAGS) $(ASAN) -o fuzzer_tls
	$(RUN) ./fuzzer_tls -max_len=17000 $(ARGS)

fuzz_tls2: mongoose.c mongoose.h Makefile fuzz_tls.c
	$(CC) fuzz_tls.c -DMAIN $(OPTS) $(WARN) $(ASAN) $(INCS) -o fuzzer_tls
	$(RUN) ./fuzzer_tls $(FUZZDATA)



test: Makefile mongoose.h $(SRCS) tls_multirec/server
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) -o unit_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./unit_test

tls_multirec/server: FORCE
ifneq "$(SSL)" ""
ifeq "$(MULTIREC)" ""
	$(MAKE) -C tls_multirec CC=gcc
endif
endif
	true

FORCE:
	true

coverage: CFLAGS += -coverage
coverage: test
	gcov -l -n *.gcno | sed '/^$$/d' | sed 'N;s/\n/ /'
	gcov -t mongoose.c > mongoose.gcov

upload-coverage: coverage
	curl -s https://codecov.io/bash | /bin/bash

valgrind: Makefile mongoose.h $(SRCS)
	$(CC) $(SRCS) $(VALGRIND_CFLAGS) $(LDFLAGS) -g -o unit_test
	$(VALGRIND_RUN) ./unit_test

misra:
	cppcheck --addon=misra -DMG_ARCH=MG_ARCH_UNIX -DMG_ENABLE_PACKED_FS=1 -DMG_ENABLE_SSI=1 mongoose.c >/tmp/x 2>&1
	cppcheck --addon=misra -DMG_ARCH=MG_ARCH_CUSTOM -DMG_ENABLE_LINES=1 -DTLS=MG_TLS_BUILTIN -DMG_ENABLE_TCPIP=1 mongoose.c  >>/tmp/x 2>&1
	cppcheck --addon=misra --clang -DMG_ENABLE_LINES=1 -DTLS=MG_TLS_BUILTIN -DMG_ENABLE_TCPIP=1 mongoose.c  >>/tmp/x 2>&1
	cppcheck --addon=misra --clang -DMG_ENABLE_LINES=1 -DMG_ENABLE_SSI=1 mongoose.c  >>/tmp/x 2>&1
	less /tmp/x

# https://ddanilov.me/how-signals-are-handled-in-a-docker-container
armhf: ASAN=
armhf: IPV6=0
armhf: CC = $(DOCKER) mdashnet/armhf cc
armhf: RUN = $(DOCKER) --init mdashnet/armhf
armhf: test

s390: ASAN=
s390: IPV6=0
s390: CFLAGS += -Wno-stringop-overflow
s390: DEFS += -DMG_ENABLE_CHACHA20=0
s390: CC = $(DOCKER) mdashnet/s390 cc
s390: RUN = $(DOCKER) --init mdashnet/s390
s390: test

arm: DEFS += -DMG_ENABLE_POSIX_FS=0 -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_TCPIP_DRIVER_INIT=0 -DMG_ARCH=MG_ARCH_ARMGCC -DNO_SLEEP_ABORT
arm: mongoose.h $(SRCS)
	$(DOCKER) mdashnet/armgcc arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb $(SRCS) $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o unit_test -nostartfiles --specs nosys.specs -e 0

riscv: DEFS += -DMG_ENABLE_POSIX_FS=0 -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_TCPIP_DRIVER_INIT=0 -DMG_ARCH=MG_ARCH_ARMGCC -DNO_SLEEP_ABORT
riscv: mongoose.h $(SRCS)
	$(DOCKER) mdashnet/riscv riscv-none-elf-gcc -march=rv32imc -mabi=ilp32 $(SRCS) $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o unit_test

vc98: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc98 wine cl $(SRCS) $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) mdashnet/vc98 wine $@.exe

vc17: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc17 wine64 cl $(SRCS) $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) mdashnet/vc17 wine64 $@.exe

vc22: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc22 wine64 cl $(SRCS) $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) mdashnet/vc22 wine64 $@.exe

mingw: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw x86_64-w64-mingw32-gcc $(SRCS) -W -Wall -Werror -I. $(DEFS) $(TFLAGS) -lwsock32 -lbcrypt -o $@.exe
	$(DOCKER) mdashnet/mingw wine64 $@.exe

mingw++: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw x86_64-w64-mingw32-g++ $(SRCS) -W -Wall -Werror -I. $(DEFS) $(TFLAGS) -lwsock32 -lbcrypt -o $@.exe
	@echo "NOT RUNNING"

linux-libs: CFLAGS += -fPIC
linux-libs: LDFLAGS += -Wl,-soname,libmongoose.so.$(VERSION)
linux-libs: mongoose.o
	$(CC) mongoose.o $(LDFLAGS) -shared -o libmongoose.so.$(VERSION)
	$(AR) rcs libmongoose.a mongoose.o

install: linux-libs
	install -Dm644 libmongoose.a libmongoose.so.$(VERSION) $(DESTDIR)$(LIBDIR)
	ln -s libmongoose.so.$(VERSION) $(DESTDIR)$(LIBDIR)/libmongoose.so
	install -Dm644 mongoose.h $(DESTDIR)$(INCLUDEDIR)/mongoose.h

uninstall:
	rm -rf $(DESTDIR)$(LIBDIR)/libmongoose.a $(DESTDIR)$(LIBDIR)/libmongoose.so.$(VERSION) $(DESTDIR)$(INCLUDEDIR)/mongoose.h $(DESTDIR)$(LIBDIR)/libmongoose.so

mongoose.c: Makefile $(wildcard ../src/*.c) $(wildcard ../src/drivers/*.c)
	cd .. && (export LC_ALL=C ; cat src/license.h; echo; echo '#include "mongoose.h"' ; (for F in src/*.c src/drivers/*.c ; do echo; echo '#ifdef MG_ENABLE_LINES'; echo "#line 1 \"$$F\""; echo '#endif'; cat $$F | sed -e 's,#include ".*,,'; done))> $@

mongoose.h: $(HDRS) Makefile
	cd .. && (cat src/license.h; echo; echo '#ifndef MONGOOSE_H'; echo '#define MONGOOSE_H'; echo; cat src/version.h ; echo; echo '#ifdef __cplusplus'; echo 'extern "C" {'; echo '#endif'; cat src/arch.h src/arch_*.h src/net_ft.h src/net_lwip.h src/net_rl.h src/config.h src/profile.h src/str.h src/queue.h src/fmt.h src/printf.h src/log.h src/timer.h src/fs.h src/util.h src/url.h src/iobuf.h src/base64.h src/md5.h src/sha1.h src/sha256.h src/event.h src/net.h src/http.h src/ssi.h src/tls.h src/tls_x25519.h src/tls_aes128.h src/tls_uecc.h src/tls_chacha20.h src/tls_rsa.h src/tls_mbed.h src/tls_openssl.h src/ws.h src/sntp.h src/mqtt.h src/dns.h src/json.h src/rpc.h src/ota.h src/flash.h src/wifi.h src/net_builtin.h src/drivers/*.h | sed -e '/keep/! s,#include ".*,,' -e 's,^#pragma once,,'; echo; echo '#ifdef __cplusplus'; echo '}'; echo '#endif'; echo '#endif  // MONGOOSE_H')> $@


clean: clean_tutorials clean_tutorials_embedded
	rm -rf $(PROG) *.exe *.o *.dSYM *_test ut fuzzer *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb slow-unit* _CL_* infer-out data.txt crash-* packed_fs.c pack
	#find tutorials -maxdepth 3 -name zephyr -prune -o -name Makefile -print | xargs dirname | xargs -n1 make clean -C


tutorials:
	@for X in $(TUTORIALS); do test -f $$X/Makefile || continue; $(MAKE) -C $$X example || exit 1; done
clean_tutorials:
	for X in $(TUTORIALS); do test -f $$X/Makefile || continue; $(MAKE) -C $$X clean || exit 1; done

tutorials_essential:
	@for X in $(TUTORIALS_QUICK); do test -f $$X/Makefile || continue; $(MAKE) -C $$X example || exit 1; done
clean_tutorials_essential:
	for X in $(TUTORIALS_QUICK); do test -f $$X/Makefile || continue; $(MAKE) -C $$X clean || exit 1; done

tutorials_mac:
	@for X in $(TUTORIALS_MAC); do test -f $$X/Makefile || continue; $(MAKE) -C $$X example || exit 1; done
clean_tutorials_mac:
	for X in $(TUTORIALS_MAC); do test -f $$X/Makefile || continue; $(MAKE) -C $$X clean || exit 1; done

tutorials_win:
	$(foreach X, $(TUTORIALS_WIN), $(MAKE) -C $(X) example &)
clean_tutorials_win:
	$(foreach X, $(TUTORIALS_WIN), $(MAKE) -C $(X) clean &)


tutorials_embedded:
	@for X in $(TUTORIALS_EMBEDDED); do test -f $$X/Makefile || continue; $(MAKE) -C $$X build || exit 1; done
clean_tutorials_embedded:
	for X in $(TUTORIALS_EMBEDDED); do test -f $$X/Makefile || continue; $(MAKE) -C $$X clean || exit 1; done

