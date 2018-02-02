APP_LDFLAGS ?=
CC_WRAPPER ?=
GENFILES_LIST ?=
CC = $(XC32_PATH)/bin/xc32-gcc
AR = $(XC32_PATH)/bin/xc32-ar
LD = $(XC32_PATH)/bin/xc32-gcc
BIN2HEX = $(XC32_PATH)/bin/xc32-bin2hex

CFLAGS = -fframe-base-loclist -mprocessor=32MZ2048EFM064 \
         -ffunction-sections -Wall -g -no-legacy-libc \
         -O1

CFLAGS += -DCS_ENABLE_DEBUG

LDFLAGS = -mprocessor=32MZ2048EFM064

# TODO(dfrank) enable that conditionally
CFLAGS  += -D__DEBUG  -D__MPLAB_DEBUGGER_ICD3=1
LDFLAGS += -mdebugger -D__MPLAB_DEBUGGER_ICD3=1

OBJCOPY = $(XC32_PATH)/bin/xc32-objcopy

# cc flags,file
define cc
	$(vecho) "XC32   $2 -> $@"
	$(Q) $(CC_WRAPPER) $(CC) -c $1 -o $@ $2
endef

# cc flags,file
define asm
	$(vecho) "XC32 ASM   $2 -> $@"
	$(Q) $(CC) -c $1 -o $@ $2
endef

# ar files
define ar
	$(vecho) "AR    $@"
	$(Q) $(AR) cru $@ $1
endef

# link script,flags,objs
define link
	$(vecho) "LD    $@ (ignoring linker script $1)"
	$(Q) $(CC_WRAPPER) $(LD) -o $@ $2 $3 -no-legacy-libc \
    -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=_min_heap_size=44960,--gc-sections
endef

define bin2hex
	$(vecho) "HEX   $1 -> $@"
	$(Q) $(BIN2HEX) $1
endef
