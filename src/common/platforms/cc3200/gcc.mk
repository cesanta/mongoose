APP_LDFLAGS ?=
CC_WRAPPER ?=
GENFILES_LIST ?=
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
AR = arm-none-eabi-ar
NM = arm-none-eabi-nm

IPATH += $(SDK_PATH)/third_party/FreeRTOS/source/portable/GCC/ARM_CM4
VPATH += $(SDK_PATH)/third_party/FreeRTOS/source/portable/GCC/ARM_CM4

C_CXX_FLAGS = -mthumb -mcpu=cortex-m4 -ffunction-sections -fdata-sections \
              -MD -Os -ggdb -Wall -Werror -Dgcc
CFLAGS += -std=c99 $(C_CXX_FLAGS)
CXXFLAGS += -std=g++11 $(C_CXX_FLAGS)

AR = arm-none-eabi-ar
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
LIBGCC := ${shell ${CC} -mthumb ${CFLAGS} -print-libgcc-file-name}
LIBC := ${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM := ${shell ${CC} ${CFLAGS} -print-file-name=libm.a}

# Disable certain warnings on SDK sources, we have no control over them anyway.
# We also force-include platform.h which resolves some symbol conflicts
# between system includes and simplelink.h
$(SDK_OBJS): CFLAGS += -Wno-missing-braces -Wno-strict-aliasing -Wno-parentheses -Wno-unused-variable -Wno-builtin-macro-redefined
$(SDK_OBJS): CFLAGS += -include common/platform.h

# cc flags,file
define cc
	$(vecho) "GCC   $2"
	$(Q) $(CC_WRAPPER) $(CC) -c $1 -o $@ $2
endef
define cxx
	$(vecho) "G++   $2"
	$(Q) $(CC_WRAPPER) $(CXX) -c $1 -o $@ $2
endef

# ar files
define ar
	$(vecho) "AR    $@"
	$(Q) $(AR) cru $@ $1
endef

# link script,flags,objs
define link
	$(vecho) "LD    $@"
	$(Q) $(CC_WRAPPER) $(LD) \
	  --gc-sections -o $@ -T $1 $2 $3 \
	  $(LIBM) $(LIBC) $(LIBGCC)
endef
