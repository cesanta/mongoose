CFLAGS_EXTRA ?=

XTENSA_TOOLS_ROOT ?= /opt/Espressif/crosstool-NG/builds/xtensa-lx106-elf/bin
SDK_PATH ?= /opt/Espressif/ESP8266_SDK
ESPTOOL ?= esptool.py
ESPPORT ?= /dev/ttyACM0
ESPSPEED ?= 230400
# For flash = > 16Mbit
ESPFLASHARGS = --flash_mode dio --flash_size 32m

VERBOSE ?= 0

CC := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
CXX := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-g++
AR := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-ar
LD := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
OBJCOPY := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-objcopy
NM := $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-nm
CC_WRAPPER ?=

define link
$(vecho) "LD    $@"
$(Q) $(CC_WRAPPER) $(LD) $(LIBDIRS) -T$(LD_SCRIPT) $(LDFLAGS) -o $@ \
-Wl,-Map=$@.map -Wl,--start-group $1 -Wl,--end-group
endef

define compile_params
$(vecho) "$5    $1"
$(Q) $(CC_WRAPPER) $3 -MD -MP $(INCDIRS) $4 -c $1 -o $2
endef

define compile
$(call compile_params,$<,$@, $(CC), $(CFLAGS),"CC")
endef

define compile_cxx
$(call compile_params,$<,$@, $(CXX), $(CXXFLAGS),"CXX")
endef

# some of these flags works around for gdb 7.5.x stacktrace issue
# while still allowing -Os to remove padding between data in .rodata
# section, allowing us to gain about 1k of ram.
# text section is 4k bigger, but we care more about ram at the moment.
# TODO(mkm): figure out which flag(s).
NO_Os_FLAGS= -fno-expensive-optimizations -fno-thread-jumps \
             -fno-align-functions -fno-align-jumps \
             -fno-align-loops -fno-align-labels -fno-caller-saves \
             -fno-crossjumping -fno-cse-follow-jumps -fno-cse-skip-blocks \
             -fno-delete-null-pointer-checks -fno-devirtualize  \
             -fno-gcse -fno-gcse-lm -fno-hoist-adjacent-loads \
             -fno-inline-small-functions -fno-indirect-inlining -fno-partial-inlining \
             -fno-ipa-cp -fno-ipa-sra -fno-peephole2 -fno-optimize-sibling-calls -fno-optimize-strlen \
             -fno-reorder-blocks -fno-reorder-blocks-and-partition -fno-reorder-functions \
             -fno-sched-interblock -fno-sched-spec -fno-rerun-cse-after-loop \
             -fno-schedule-insns -fno-schedule-insns2 -fno-strict-aliasing -fno-strict-overflow \
             -fno-tree-builtin-call-dce -fno-tree-switch-conversion -fno-tree-tail-merge \
             -fno-tree-pre -fno-tree-vrp

C_CXX_FLAGS  = -W -Wall -Werror -Wundef -Wno-comment -Wno-variadic-macros -Wpointer-arith \
               -Wno-missing-field-initializers \
               -pipe -Os $(NO_Os_FLAGS) -g3 \
               -Wl,-EL -fno-inline-functions \
               -D_XOPEN_SOURCE=500 \
               -nostdlib -mlongcalls -mtext-section-literals -D__ets__ -DSTATIC=static \
               -Wno-parentheses \
               -DIRAM='__attribute__((section(".fast.text")))' \
               -DICACHE_RAM_ATTR=IRAM \
               -DNOINSTR='__attribute__((no_instrument_function))' \
               -DCS_PLATFORM=3 \
               -ffunction-sections -fdata-sections

CFLAGS = -std=gnu99 $(C_CXX_FLAGS)
CXXFLAGS = -std=gnu++11 -fno-exceptions $(C_CXX_FLAGS)

# linker flags used to generate the main object file
LDFLAGS = -nostdlib -Wl,--no-check-sections -u call_user_start \
          -Wl,-static -Wl,--gc-sections
