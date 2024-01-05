################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c \
../utilities/fsl_str.c 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d \
./utilities/fsl_str.d 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o \
./utilities/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MIMXRT1062DVL6B -DCPU_MIMXRT1062DVL6B_cm7 -DSDK_OS_BAREMETAL -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/board" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/source" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/drivers" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/xip" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/CMSIS" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/device" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/serial_manager" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/uart" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/utilities" -I"/root/Documents/MCUXpresso_11.8.1_1197/workspace/rt1060-evk-xpresso-baremetal-builtin/component/lists" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o ./utilities/fsl_str.d ./utilities/fsl_str.o

.PHONY: clean-utilities

