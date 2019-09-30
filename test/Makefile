# vim: ts=8 noet sw=8 ai cin smarttab

PROG = unit_test

MONGOOSE_DIR = ..

SRC_DIR = ../src

COMMON_DIR_PUB = ../src/common
COMMON_DIR_DEV = ../../common

# Our dev repo and public mongoose repo have different layouts, so here
# we make it work on both
ifneq ("$(wildcard ../../common.mk)","")
COMMON_PARENT = ../..
else
COMMON_PARENT = $(SRC_DIR)
endif

TEST_SOURCES = unit_test.c test_util.c test_main.c fuzz.c
AMALGAMATED_SOURCES = ../mongoose.c
KRYPTON_PATH = $(REPO_ROOT)/krypton

VPATH = $(MONGOOSE_DIR) $(SRC_DIR) $(COMMON_DIR_PUB) $(COMMON_DIR_DEV) $(COMMON_PARENT)

# or Krypton, or mbedTLS
SSL ?= OpenSSL

COMMON_FEATURE_FLAGS = \
	-DKR_MODULE_LINES \
	-DMG_ENABLE_COAP \
	-DMG_ENABLE_DNS_SERVER \
	-DMG_ENABLE_HTTP_SSI_EXEC \
	-DMG_ENABLE_HTTP_WEBDAV \
	-DMG_ENABLE_MQTT_BROKER \
	-DMG_ENABLE_SOCKS \
	-DMG_ENABLE_SYNC_RESOLVER \
	-DMG_ENABLE_THREADS \
	-DMG_DISABLE_DAV_AUTH \
	-DMG_INTERNAL="" \
	-DMG_MODULE_LINES \
	-DMG_CALLOC=test_calloc \
	-DMG_MALLOC=test_malloc \
	-DMG_ENABLE_POLL_UNTIL \
	-DMG_ENABLE_SNTP -DMG_SNTP_NO_DELAY_CORRECTION \
	-DMG_ENABLE_HTTP_STREAMING_MULTIPART
UNIX_FEATURE_FLAGS=-DMG_ENABLE_IPV6 -DMG_ENABLE_SSL
# TODO: remove -Wno-deprecated-declarations once deprecated
# `mg_http_parse_header()` is removed from mongoose.
CFLAGS = -W -Wall -Wundef -Werror -Wno-deprecated-declarations -g -O0 -Wno-multichar -D__USE_MISC \
	 $(COMMON_FEATURE_FLAGS) $(UNIX_FEATURE_FLAGS) \
	 $(patsubst %,-I%,$(subst :, ,$(VPATH))) \
	 -include unit_test.h -pthread $(CFLAGS_EXTRA)

LDFLAGS = -lm

ifeq "$(SSL)" "OpenSSL"
# OpenSSL is the default
LDFLAGS += -lcrypto -lssl
endif
ifeq "$(SSL)" "Krypton"
CFLAGS += $(KRYPTON_PATH)/krypton.c -I$(KRYPTON_PATH)
endif
ifeq "$(SSL)" "mbedTLS"
CFLAGS += -DMG_SSL_IF=MG_SSL_IF_MBEDTLS -DMG_SSL_MBED_DUMMY_RANDOM
LDFLAGS += -lmbedcrypto -lmbedtls -lmbedx509
endif

include test.mk
include $(SRC_DIR)/mg_modules.mk

# http://crossgcc.rts-software.org/doku.php?id=compiling_for_win32
MINGW_GCC=/usr/local/gcc-4.8.0-qt-4.8.4-for-mingw32/win32-gcc/bin/i586-mingw32-gcc

.PHONY: $(PROG).exe $(PROG)_mingw.exe

$(PROG)_mingw.exe: Makefile
	$(MINGW_GCC) $(AMALGAMATED_SOURCES) -o $(PROG)_mingw.exe -W -Wall -Werror

$(PROG).exe: $(AMALGAMATED_SOURCES) data/cgi/index.cgi.exe test_sources_resolved
ifndef VC6_DIR
	$(error Please set VC6_DIR)
endif
	Include=$(VC6_DIR)/include Lib=$(VC6_DIR)/lib \
	wine $(VC6_DIR)/bin/cl \
	  $(TEST_SOURCES_RESOLVED) $(AMALGAMATED_SOURCES) \
	  /MD /I. /I.. /Zi \
	  /FIunit_test.h \
	  /I$(REPO_ROOT) \
	  $(COMMON_FEATURE_FLAGS) $(CFLAGS_EXTRA) \
	  -DMG_ENABLE_SSL $(KRYPTON_PATH)/krypton.c -I$(KRYPTON_PATH) \
	  kernel32.lib advapi32.lib \
	  /Fe$@
	cp data/cgi/index.cgi.exe data/cgi/index.cgi

win: $(PROG).exe
	wine $(PROG).exe $(TEST_FILTER)

# CGI test program.

$(PROG)-%: data/cgi/index.cgi

data/cgi/index.cgi: index_cgi.c ccgi-1.2/ccgi.c
	@echo -e "CC\tindex.cgi"
	@mkdir -p data/cgi
	@$(CC) -Wall -Werror -o $@ $^ -Iccgi-1.2

data/cgi/index.cgi.exe: index_cgi.c ccgi-1.2/ccgi.c
	@echo -e "CC\tindex.cgi.exe"
	@mkdir -p data/cgi
	Include=$(VC6_DIR)/include Lib=$(VC6_DIR)/lib \
	wine $(VC6_DIR)/bin/cl \
	  /MD /Iccgi-1.2 /Zi index_cgi.c ccgi-1.2/ccgi.c /Fe$@

clean: clean_index_cgi

clean_index_cgi:
	@echo -e "CLEAN\tindex.cgi"
	@rm -f data/cgi/index.cgi*

fuzz:
	$(CC) fuzz.c ../mongoose.c -o /tmp/$@ -fsanitize=fuzzer,address -g -I..
	/tmp/$@

# Interactive:
#   docker run -v $(CURDIR)/../..:/cesanta -t -i --entrypoint=/bin/bash cesanta/mongoose_test
docker:
	docker run --rm -v $(CURDIR)/../..:/cesanta cesanta/mongoose_test
