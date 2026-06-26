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


SOURCES = bsd_shm_test.c ../../mongoose.c

# FreeRTOS
SOURCES += FreeRTOS-Kernel/portable/MemMang/heap_4.c
SOURCES += FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/port.c
SOURCES += FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c
CFLAGS += -IFreeRTOS-Kernel/include 
CFLAGS += -IFreeRTOS-Kernel/portable/ThirdParty/GCC/Posix -Wno-conversion -Wno-undef

# Mongoose options are defined in mongoose_config.h, this forces loading and skips OS detection
CFLAGS_MONGOOSE += -DMG_ARCH=MG_ARCH_CUSTOM


all:  shm_tap_bridge bsd_client FreeRTOS-Kernel bsd_test
	ASAN_OPTIONS=$(ASAN_OPTIONS) $(RUN) ./bsd_test $(ARGS)

# requires shm_tap_bridge to be running
bsd_test: $(SOURCES) bsd_x_test.c ../../mongoose.h
	$(CC) $(SOURCES) $(wildcard FreeRTOS-Kernel/*.c) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) -o $@

bsd_client: bsd_client.c
	$(CC) bsd_client.c -pthread -o $@

# bridge shm queue <--> TAP interface
shm_tap_bridge: shm_tap_bridge.c
	$(CC) shm_tap_bridge.c -pthread -o $@

clean:                    # Cleanup. Delete built program and all build artifacts
	rm -rf bsd_test bsd_client *.o *.obj FreeRTOS-Kernel

FreeRTOS-Kernel:  # FreeRTOS sources
	git clone -q -c advice.detachedHead=false --depth 1 -b V11.2.0 https://github.com/FreeRTOS/FreeRTOS-Kernel $@
