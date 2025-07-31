SRCS = mongoose.c unit_test.c packed_fs.c
HDRS = $(wildcard ../src/*.h) $(wildcard ../src/drivers/*.h)
DEFS ?= -DMG_MAX_HTTP_HEADERS=7 -DMG_ENABLE_LINES -DMG_ENABLE_SSI=1 -DMG_ENABLE_ASSERT=1 -DMG_ENABLE_IPV6=$(IPV6)
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
TUTORIALS_EMBEDDED := $(dir $(wildcard ../tutorials/infineon/*/Makefile)) $(dir $(wildcard ../tutorials/nxp/*/Makefile)) $(dir $(wildcard ../tutorials/rp/*/Makefile)) $(dir $(wildcard ../tutorials/renesas/*/Makefile)) $(dir $(wildcard ../tutorials/stm32/*/*/Makefile)) $(dir $(wildcard ../tutorials/ti/*/Makefile)) $(dir $(wildcard ../tutorials/wch/*/Makefile))
TUTORIALS := ../tutorials/micropython/ $(filter-out $(TUTORIALS_EMBEDDED), $(dir $(wildcard ../tutorials/*/*/Makefile)))
TUTORIALS_QUICK := $(filter-out ../tutorials/micropython/ ../tutorials/tcpip/pcap-driver/, $(TUTORIALS))
TUTORIALS_MAC := $(filter-out ../tutorials/micropython/ ../tutorials/tcpip/pcap-driver/ ../tutorials/tcpip/tap-driver/, $(TUTORIALS))
TUTORIALS_WIN := $(dir $(wildcard ../tutorials/http/file-*/Makefile) $(wildcard ../tutorials/http/http-*/Makefile) $(wildcard ../tutorials/mqtt/*/Makefile) $(wildcard ../tutorials/websocket/*/Makefile) $(wildcard ../tutorials/webui/*/Makefile))
PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include
VERSION ?= $(shell cut -d'"' -f2 ../src/version.h)
COMMON_CFLAGS ?= $(C_WARN) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -pthread
CFLAGS ?= $(OPTS) $(ASAN) $(COMMON_CFLAGS)
VALGRIND_CFLAGS ?= $(OPTS) $(COMMON_CFLAGS)
VALGRIND_RUN ?= valgrind --tool=memcheck --gen-suppressions=all --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --error-exitcode=1 --exit-on-first-error=yes --fair-sched=yes
.PHONY: tutorials mip_test eth_test ppp_test atcmd_test eth_vc98 ppp_vc98 atcmd_vc98 eth_s390 ppp_s390 atcmd_s390 test valgrind

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

tls_multirec/server: FORCE
ifneq "$(SSL)" ""
ifeq "$(MULTIREC)" ""
	$(MAKE) -C tls_multirec CC=gcc
endif
endif
	true

FORCE:
	true


all:
	$(MAKE) -C ../tutorials/http/http-server

tall: mg_prefix unamalgamated test valgrind fuzz fuzz_tls vc98 vc17 vc22 mingw mingw++ arm armhf riscv \
      diabish ppc_diabish s390 ppc mips tutorials tutorials_win tutorials_embedded

tmip: mip_test eth_test ppp_test atcmd_test mip_tap_test pppoe_tap_test mip_port_test mip_diabish \
      mip_vc98 eth_vc98 ppp_vc98 atcmd_vc98 mip_ppc_diabish mip_s390 eth_s390 ppp_s390 \
      atcmd_s390 mip_port_s390 mip_ppc eth_ppc ppp_ppc atcmd_ppc mip_port_ppc mip_mips \
      eth_mips ppp_mips atcmd_mips mip_port_mips

# Amalgamated source files
mongoose.c: Makefile $(wildcard ../src/*.c) $(wildcard ../src/drivers/*.c)
	cd .. && (export LC_ALL=C ; cat src/license.h; echo; echo '#include "mongoose.h"' ; (for F in src/*.c src/drivers/*.c ; do echo; echo '#ifdef MG_ENABLE_LINES'; echo "#line 1 \"$$F\""; echo '#endif'; cat $$F | sed -e 's,#include ".*,,'; done))> $@

mongoose.h: $(HDRS) Makefile
	cd .. && (export LC_ALL=C ; cat src/license.h; echo; echo '#ifndef MONGOOSE_H'; echo '#define MONGOOSE_H'; echo; cat src/version.h ; echo; echo '#ifdef __cplusplus'; echo 'extern "C" {'; echo '#endif'; cat src/arch.h src/arch_*.h src/os_*.h src/net_ft.h src/net_lwip.h src/net_rl.h src/config.h src/profile.h src/str.h src/queue.h src/fmt.h src/printf.h src/log.h src/timer.h src/fs.h src/util.h src/url.h src/iobuf.h src/base64.h src/md5.h src/sha1.h src/sha256.h src/event.h src/net.h src/http.h src/ssi.h src/tls.h src/tls_x25519.h src/tls_aes128.h src/tls_uecc.h src/tls_chacha20.h src/tls_rsa.h src/tls_mbed.h src/tls_openssl.h src/ws.h src/sntp.h src/mqtt.h src/dns.h src/modbus.h src/json.h src/rpc.h src/dash.h src/ota.h src/flash.h src/wifi.h src/l2.h src/net_builtin.h src/bsd.h src/drivers/*.h | sed -e '/keep/! s,#include ".*,,' -e 's,^#pragma once,,'; echo; echo '#ifdef __cplusplus'; echo '}'; echo '#endif'; echo '#endif  // MONGOOSE_H')> $@

# Check that all external (exported) symbols have "mg_" prefix
mg_prefix: mongoose.c mongoose.h
	$(CC) mongoose.c $(CFLAGS) -c -o /tmp/x.o && nm /tmp/x.o | grep ' T ' | grep -v 'mg_' ; test $$? = 1

test: Makefile mongoose.h $(SRCS) tls_multirec/server
	$(CC) $(SRCS) $(CFLAGS) $(LDFLAGS) -o unit_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./unit_test

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
	cppcheck --addon=misra -DMG_ARCH=MG_ARCH_UNIX -DMG_ENABLE_SSI=1 mongoose.c >/tmp/x 2>&1
	cppcheck --addon=misra -DMG_ARCH=MG_ARCH_CUSTOM -DMG_ENABLE_LINES=1 -DTLS=MG_TLS_BUILTIN -DMG_ENABLE_TCPIP=1 mongoose.c  >>/tmp/x 2>&1
	cppcheck --addon=misra --clang -DMG_ENABLE_LINES=1 -DTLS=MG_TLS_BUILTIN -DMG_ENABLE_TCPIP=1 mongoose.c  >>/tmp/x 2>&1
	cppcheck --addon=misra --clang -DMG_ENABLE_LINES=1 -DMG_ENABLE_SSI=1 mongoose.c  >>/tmp/x 2>&1
	less /tmp/x

arm: DEFS += -DMG_ENABLE_POSIX_FS=0 -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_TCPIP_DRIVER_INIT=0 -DMG_ARCH=MG_ARCH_ARMGCC -DNO_SLEEP_ABORT
arm: mongoose.h $(SRCS) mip_test.c
	$(DOCKER) mdashnet/armgcc arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb $(SRCS) $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o unit_test -nostartfiles --specs nosys.specs -e 0
	$(DOCKER) mdashnet/armgcc arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb mip_test.c packed_fs.c $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o mip_test -nostartfiles --specs nosys.specs -e 0

riscv: DEFS += -DMG_ENABLE_POSIX_FS=0 -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_TCPIP_DRIVER_INIT=0 -DMG_ARCH=MG_ARCH_ARMGCC -DNO_SLEEP_ABORT
riscv: mongoose.h $(SRCS) mip_test.c
	$(DOCKER) mdashnet/riscv riscv-none-elf-gcc -march=rv32imc -mabi=ilp32 $(SRCS) $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o unit_test
	$(DOCKER) mdashnet/riscv riscv-none-elf-gcc -march=rv32imc -mabi=ilp32 mip_test.c packed_fs.c $(OPTS) $(WARN) $(INCS) $(DEFS) $(TFLAGS) -o mip_test

# https://ddanilov.me/how-signals-are-handled-in-a-docker-container
armhf: ASAN=
armhf: IPV6=0
armhf: CC = $(DOCKER) mdashnet/armhf cc
armhf: RUN = $(DOCKER) --init mdashnet/armhf
armhf: test

# C89 FOLKS
diabish: CC = gcc
diabish: CFLAGS += -std=gnu89 -Werror=declaration-after-statement -Werror=vla -Wno-pedantic
diabish: test
# end-of

# C89 + BIG-ENDIAN FOLKS
ppc_diabish: ASAN=
ppc_diabish: CFLAGS += -std=gnu89 -Werror=declaration-after-statement -Werror=vla -Wno-pedantic
ppc_diabish: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static
ppc_diabish: RUN = qemu-ppc-static
ppc_diabish: test
# end-of

# BIG-ENDIAN FOLKS
s390: ASAN=
s390: IPV6=0
s390: CFLAGS += -Wno-stringop-overflow
s390: CC = $(DOCKER) mdashnet/s390 cc
s390: RUN = $(DOCKER) --init mdashnet/s390
s390: test

ppc: ASAN=
ppc: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static
ppc: RUN = $(DOCKER) --init vicamo/debian:stable-powerpc # doesn't have curl
ppc: test

mips: ASAN=
mips: CC = $(DOCKER) dockcross/linux-mips mips-unknown-linux-gnu-gcc -static
mips: RUN = $(DOCKER) --init vicamo/debian:stable-mips # not working in GH (?)
mips: test
# end-of

# Windows
vc98: IPV6=0  # VC98 does NOT support IPV6 sockets
vc98: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc98 wine cl $(SRCS) $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) -e WINEPREFIX=/tmp/wineprefix mdashnet/vc98 wine $@.exe

vc17: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc17 wine64 cl $(SRCS) $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) mdashnet/vc17 wine64 $@.exe

vc22: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/vc22 wine64 cl $(SRCS) $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) mdashnet/vc22 wine64 $@.exe

mingw: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw x86_64-w64-mingw32-gcc $(SRCS) -W -Wall -Werror -I. $(DEFS) $(TFLAGS) -lws2_32 -lbcrypt -o $@.exe
	$(DOCKER) mdashnet/mingw wine64 $@.exe

mingw++: Makefile mongoose.h $(SRCS)
	$(DOCKER) mdashnet/mingw x86_64-w64-mingw32-g++ $(SRCS) -W -Wall -Werror -I. $(DEFS) $(TFLAGS) -lws2_32 -lbcrypt -o $@.exe
	@echo "NOT RUNNING"
# end-of

# Linux easter eggs
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


packed_fs.c: Makefile data/ssi.h fuzz.c data/a.txt data/ca.pem certs/ca.crt certs/server.crt certs/server.key
	$(CC) $(CFLAGS) pack.c -o pack
	$(RUN) ./pack Makefile data/ssi.h fuzz.c data/a.txt data/range.txt data/ca.pem certs/ca.crt certs/server.crt certs/server.key certs/client.key certs/client.crt > $@


clean: clean_tutorials clean_tutorials_embedded
	rm -rf $(PROG) *.exe *.o *.dSYM *_test ut fuzzer *.gcov *.gcno *.gcda *.obj *.exe *.ilk *.pdb slow-unit* _CL_* infer-out data.txt crash-* packed_fs.c pack
	$(MAKE) -C tls_multirec $@
	$(MAKE) -C ../resources/I-CUBE-Mongoose $@
	#find tutorials -maxdepth 3 -name zephyr -prune -o -name Makefile -print | xargs dirname | xargs -n1 make clean -C


# MIP (net_builtin + L2 + driver) TESTS

mip_test: mip_test.c mongoose.c mongoose.h packed_fs.c Makefile
	$(CC) mip_test.c packed_fs.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

eth_test: eth_test.c mongoose.c mongoose.h Makefile
	$(CC) eth_test.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

ppp_test: ppp_test.c mongoose.c mongoose.h Makefile
	$(CC) ppp_test.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

atcmd_test: atcmd_test.c mongoose.c mongoose.h Makefile
	$(CC) atcmd_test.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

mip_tap_test: mip_x_test.c mip_tap_test.c mongoose.c mongoose.h packed_fs.c Makefile tls_multirec/server
	$(CC) mip_tap_test.c packed_fs.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

pppoe_tap_test: pppoe_tap_test.c mongoose.c mongoose.h Makefile
	$(CC) pppoe_tap_test.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

# bridge UDP port <--> TAP interface
port_tap_bridge: port_tap_bridge.c
	cc port_tap_bridge.c -o $@

# requires port_tap_bridge to be running
mip_port_test: mip_x_test.c mip_port_test.c mongoose.c mongoose.h packed_fs.c Makefile tls_multirec/server
	$(CC) mip_port_test.c packed_fs.c $(CFLAGS) $(LDFLAGS) -o $@
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./$@

# C89 FOLKS
mip_diabish: CC = gcc
mip_diabish: CFLAGS += -std=gnu89 -Werror=declaration-after-statement -Werror=vla -Wno-pedantic
mip_diabish: mip_test

mip_vc98: mip_test.c mongoose.c mongoose.h packed_fs.c Makefile
	$(DOCKER) mdashnet/vc98 wine cl mip_test.c packed_fs.c $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) -e WINEPREFIX=/tmp/wineprefix mdashnet/vc98 wine $@.exe

eth_vc98: eth_test.c mongoose.c mongoose.h Makefile
	$(DOCKER) mdashnet/vc98 wine cl eth_test.c $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) -e WINEPREFIX=/tmp/wineprefix mdashnet/vc98 wine $@.exe

ppp_vc98: ppp_test.c driver_mock.c mongoose.c mongoose.h Makefile
	$(DOCKER) mdashnet/vc98 wine cl ppp_test.c $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) -e WINEPREFIX=/tmp/wineprefix mdashnet/vc98 wine $@.exe

atcmd_vc98: atcmd_test.c mongoose.c mongoose.h Makefile
	$(DOCKER) mdashnet/vc98 wine cl atcmd_test.c $(VCFLAGS) $(DEFS) $(TFLAGS) /Fe$@.exe
	$(DOCKER) -e WINEPREFIX=/tmp/wineprefix mdashnet/vc98 wine $@.exe
# end-of

# C89 + BIG-ENDIAN FOLKS
mip_ppc_diabish: ASAN=
mip_ppc_diabish: CFLAGS += -std=gnu89 -Werror=declaration-after-statement -Werror=vla -Wno-pedantic
mip_ppc_diabish: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static
mip_ppc_diabish: RUN = qemu-ppc-static
mip_ppc_diabish: mip_test
# end-of

# BIG-ENDIAN FOLKS
mip_s390: ASAN=
mip_s390: CFLAGS += -Wno-stringop-overflow
mip_s390: CC = $(DOCKER) mdashnet/s390 cc
mip_s390: RUN = $(DOCKER) --init mdashnet/s390
mip_s390: mip_test

eth_s390: ASAN=
eth_s390: CC = $(DOCKER) mdashnet/s390 cc
eth_s390: RUN = $(DOCKER) --init mdashnet/s390
eth_s390: eth_test

ppp_s390: ASAN=
ppp_s390: CC = $(DOCKER) mdashnet/s390 cc
ppp_s390: RUN = $(DOCKER) --init mdashnet/s390
ppp_s390: ppp_test

atcmd_s390: ASAN=
atcmd_s390: CC = $(DOCKER) mdashnet/s390 cc
atcmd_s390: RUN = $(DOCKER) --init mdashnet/s390
atcmd_s390: atcmd_test

mip_port_s390: ASAN=
mip_port_s390: CFLAGS += -Wno-stringop-overflow -DNO_HTTPSERVER_TEST # This test requires curl, not present in this image
mip_port_s390: CC = $(DOCKER) mdashnet/s390 cc
mip_port_s390: RUN = $(DOCKER) --init mdashnet/s390
mip_port_s390: mip_port_test

mip_ppc: ASAN=
mip_ppc: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static -Wno-maybe-uninitialized
mip_ppc: RUN = qemu-ppc-static
mip_ppc: mip_test

eth_ppc: ASAN=
eth_ppc: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static -Wno-maybe-uninitialized
eth_ppc: RUN = qemu-ppc-static
eth_ppc: eth_test

ppp_ppc: ASAN=
ppp_ppc: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static -Wno-maybe-uninitialized
ppp_ppc: RUN = qemu-ppc-static
ppp_ppc: ppp_test

atcmd_ppc: ASAN=
atcmd_ppc: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static -Wno-maybe-uninitialized
atcmd_ppc: RUN = qemu-ppc-static
atcmd_ppc: atcmd_test

mip_port_ppc: ASAN=
mip_port_ppc: CFLAGS += -DNO_HTTPSERVER_TEST # *** avoid system() calls, no OS image !
mip_port_ppc: CC = $(DOCKER) dockcross/linux-ppc powerpc-unknown-linux-gnu-gcc -static -Wno-maybe-uninitialized
mip_port_ppc: RUN = qemu-ppc-static
mip_port_ppc: mip_port_test

mip_mips: ASAN=
mip_mips: CC = $(DOCKER) dockcross/linux-mips mips-unknown-linux-gnu-gcc -static
mip_mips: RUN = qemu-mips-static
mip_mips: mip_test

eth_mips: ASAN=
eth_mips: CC = $(DOCKER) dockcross/linux-mips mips-unknown-linux-gnu-gcc -static
eth_mips: RUN = qemu-mips-static
eth_mips: eth_test

ppp_mips: ASAN=
ppp_mips: CC = $(DOCKER) dockcross/linux-mips mips-unknown-linux-gnu-gcc -static
ppp_mips: RUN = qemu-mips-static
ppp_mips: ppp_test

atcmd_mips: ASAN=
atcmd_mips: CC = $(DOCKER) dockcross/linux-mips mips-unknown-linux-gnu-gcc -static
atcmd_mips: RUN = qemu-mips-static
atcmd_mips: atcmd_test

mip_port_mips: ASAN=
mip_port_mips: CFLAGS += -DNO_HTTPSERVER_TEST # *** avoid system() calls, no OS image !
mip_port_mips: CC = $(DOCKER) dockcross/linux-mips mips-unknown-linux-gnu-gcc -static
mip_port_mips: RUN = qemu-mips-static
mip_port_mips: mip_port_test
# end-of

# TUTORIALs TESTS

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
