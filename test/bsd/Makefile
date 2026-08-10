WARN ?= -pedantic -W -Wall -Werror -Wshadow -Wdouble-promotion -fno-common -Wconversion -Wundef
OPTS ?= -O3 -g3
ASAN ?= -fsanitize=address,undefined,alignment -fno-sanitize-recover=all -fno-omit-frame-pointer -fno-common
ASAN_OPTIONS ?= detect_leaks=1
COMMON_CFLAGS ?= $(C_WARN) $(WARN) -I. -I../.. $(TFLAGS)
CFLAGS ?= $(OPTS) $(ASAN) $(COMMON_CFLAGS)

ifeq "$(findstring ++,$(CC))" ""
# $(CC) does not end with ++, i.e. we're using C. Apply C flags
C_WARN ?= -Wmissing-prototypes -Wstrict-prototypes
else
# $(CC) ends with ++, i.e. we're using C++. Apply C++ flags
C_WARN ?= -Wno-deprecated
endif

.PHONY: all bsd_test bsd_client shm_tap_bridge shm_port_bridge bsd_s390 clean

TEST_SOURCES = bsd_shm_test.c
# A C++ application compiles its sources as C++, linking Mongoose and FreeRTOS as C.
# FreeRTOS does not build as C++, so C sources are compiled separately.
C_SOURCES = ../../mongoose.c
C_SOURCES += FreeRTOS-Kernel/portable/MemMang/heap_4.c
C_SOURCES += FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/port.c
C_SOURCES += FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
C_SOURCES += $(wildcard FreeRTOS-Kernel/*.c)
C_OBJECTS = $(notdir $(C_SOURCES:.c=.o))
C_INPUTS = $(foreach S,$(C_SOURCES),-x c $(S))
CFLAGS += -IFreeRTOS-Kernel/include 
CFLAGS += -IFreeRTOS-Kernel/portable/ThirdParty/GCC/Posix -Wno-conversion -Wno-undef

ifeq "$(findstring ++,$(CC))" ""
TEST_LANGUAGE = c
else
TEST_LANGUAGE = c++
endif

DOCKER_BIN ?= docker
CWD ?= $(realpath $(CURDIR))
ROOT_DIR = $(realpath $(CWD)/../..)
ENV ?= -e Tmp=. -e WINEDEBUG=-all
DOCKER ?= $(DOCKER_BIN) run --platform linux/amd64 --net=host --rm $(ENV) -v $(ROOT_DIR):$(ROOT_DIR) -w $(CWD)

# Mongoose options are defined in mongoose_config.h, this forces loading and skips OS detection
CFLAGS_MONGOOSE += -DMG_ARCH=MG_ARCH_CUSTOM


all bsd: shm_tap_bridge bsd_client FreeRTOS-Kernel bsd_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./bsd_test $(ARGS)

# requires shm_tap_bridge to be running
bsd_test: FreeRTOS-Kernel
	$(CC) $(C_INPUTS) -c $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA)
	$(CC) -x $(TEST_LANGUAGE) -c $(TEST_SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA)
	$(CC) $(C_OBJECTS) $(TEST_SOURCES:.c=.o) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) -o $@

bsd_client: bsd_client.c
	$(CC) bsd_client.c -pthread -o $@

# bridge shm queue <--> TAP interface
shm_tap_bridge: shm_tap_bridge.c shm_queue.h
	$(CC) shm_tap_bridge.c -pthread -o $@

# requires test/port_tap_bridge to be running
bsd_s390: ASAN=
bsd_s390: CC = $(DOCKER) mdashnet/s390 cc
bsd_s390: RUN = $(DOCKER) --init mdashnet/s390
bsd_s390: shm_port_bridge bsd_client FreeRTOS-Kernel bsd_test
	$(RUN) sh -c './shm_port_bridge & pid=$$!; sleep 1; ./bsd_test; rc=$$?; kill $$pid >/dev/null 2>&1; wait $$pid >/dev/null 2>&1; exit $$rc'

shm_port_bridge: shm_port_bridge.c shm_queue.h
	$(CC) shm_port_bridge.c $(CFLAGS) -pthread -o $@

clean:                    # Cleanup. Delete built program and all build artifacts
	rm -rf bsd_test bsd_client shm_port_bridge *.o *.obj FreeRTOS-Kernel

FreeRTOS-Kernel:  # FreeRTOS sources
	git clone -q -c advice.detachedHead=false --depth 1 -b V11.2.0 https://github.com/FreeRTOS/FreeRTOS-Kernel $@
