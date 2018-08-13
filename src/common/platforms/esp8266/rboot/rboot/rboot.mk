#
# Makefile for rBoot
# https://github.com/raburton/esp8266
#

ESPTOOL2 ?= ../../build/esptool2
SDK_BASE ?= /opt/Espressif/ESP8266_SDK
SPI_SIZE ?=
RBOOT_INTEGRATION ?=
RBOOT_EXTRA_INCDIR ?=

# RBOOT_BUILD_BASE and RBOOT_GEN_BASE should be provided via makefile parameters
RBOOT_BUILD_BASE ?=
# RBOOT_GEN_BASE is the directory for generated inputs
RBOOT_GEN_BASE ?=

ifndef XTENSA_BINDIR
CC := xtensa-lx106-elf-gcc
LD := xtensa-lx106-elf-gcc
else
CC := $(addprefix $(XTENSA_BINDIR)/,xtensa-lx106-elf-gcc)
LD := $(addprefix $(XTENSA_BINDIR)/,xtensa-lx106-elf-gcc)
endif
CC_WRAPPER ?=

CFLAGS    = -Os -Wpointer-arith -Wundef -Werror -Wl,-EL \
            -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals \
            -D__ets__ -DIRAM='__attribute__((section(".fast.text")))' \
            -DNOINSTR='__attribute__((no_instrument_function))' \
            -DICACHE_FLASH $(CFLAGS_EXTRA)

LDFLAGS   = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -L $(SDK_BASE)/ld/

LD_SCRIPT = rboot.ld

E2_OPTS = -quiet -bin -boot0

ifeq ($(RBOOT_BIG_FLASH),1)
	CFLAGS += -DBOOT_BIG_FLASH
endif
ifeq ($(RBOOT_INTEGRATION),1)
	CFLAGS += -DRBOOT_INTEGRATION
endif
ifeq ($(SPI_SIZE), 256K)
	E2_OPTS += -256
else ifeq ($(SPI_SIZE), 512K)
	E2_OPTS += -512
else ifeq ($(SPI_SIZE), 1M)
	E2_OPTS += -1024
else ifeq ($(SPI_SIZE), 2M)
	E2_OPTS += -2048
else ifeq ($(SPI_SIZE), 4M)
	E2_OPTS += -4096
endif

RBOOT_EXTRA_INCDIR := $(addprefix -I,$(RBOOT_EXTRA_INCDIR))

.SECONDARY:

all: $(RBOOT_BUILD_BASE)/rboot.bin

$(RBOOT_BUILD_BASE)/rboot-stage2a.o: rboot-stage2a.c rboot-private.h rboot.h
	@echo "CC $<"
	@$(CC_WRAPPER) $(CC) $(CFLAGS) $(RBOOT_EXTRA_INCDIR) -c $< -o $@

$(RBOOT_BUILD_BASE)/rboot-stage2a.elf: $(RBOOT_BUILD_BASE)/rboot-stage2a.o
	@echo "LD $@"
	@$(CC_WRAPPER) $(LD) -Trboot-stage2a.ld $(LDFLAGS) -Wl,--start-group $^ -Wl,--end-group -o $@

$(RBOOT_GEN_BASE)/rboot-hex2a.h: $(RBOOT_BUILD_BASE)/rboot-stage2a.elf
	@echo "E2 $@"
	@$(ESPTOOL2) -quiet -header $< $@ .text

$(RBOOT_BUILD_BASE)/rboot.o: rboot.c rboot-private.h rboot.h $(RBOOT_GEN_BASE)/rboot-hex2a.h
	@echo "CC $<"
	@$(CC) $(CFLAGS) -I$(RBOOT_GEN_BASE) $(RBOOT_EXTRA_INCDIR) -c $< -o $@

$(RBOOT_BUILD_BASE)/%.o: %.c %.h
	@echo "CC $<"
	@$(CC) $(CFLAGS) $(RBOOT_EXTRA_INCDIR) -c $< -o $@

$(RBOOT_BUILD_BASE)/%.elf: $(RBOOT_BUILD_BASE)/%.o
	@echo "LD $@"
	@$(LD) -T$(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $^ -Wl,--end-group -o $@

$(RBOOT_BUILD_BASE)/%.bin: $(RBOOT_BUILD_BASE)/%.elf
	@echo "E2 $@"
	@$(ESPTOOL2) $(E2_OPTS) $< $@ .text .rodata

