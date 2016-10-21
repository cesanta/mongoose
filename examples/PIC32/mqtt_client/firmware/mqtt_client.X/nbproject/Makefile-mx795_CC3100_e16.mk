#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-mx795_CC3100_e16.mk)" "nbproject/Makefile-local-mx795_CC3100_e16.mk"
include nbproject/Makefile-local-mx795_CC3100_e16.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=mx795_CC3100_e16
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netcfg.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/device.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/driver.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/fs.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/wlan.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/socket.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netapp.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/nonos.c ../src/system_config/mx795_CC3100_e16/framework/driver/rtcc/src/drv_rtcc_static.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_mapping.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_tasks.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_sys_queue.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_rm_tasks.c ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_mapping.c ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static.c ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static_byte_model.c ../src/system_config/mx795_CC3100_e16/framework/system/clk/src/sys_clk_static.c ../src/system_config/mx795_CC3100_e16/framework/system/console/src/sys_console_static.c ../src/system_config/mx795_CC3100_e16/framework/system/ports/src/sys_ports_static.c ../src/system_config/mx795_CC3100_e16/system_init.c ../src/system_config/mx795_CC3100_e16/system_interrupt.c ../src/system_config/mx795_CC3100_e16/system_exceptions.c ../src/system_config/mx795_CC3100_e16/system_tasks.c ../src/app.c ../src/main.c ../src/cc3100_port.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/debug/src/sys_debug.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon_pic32mx.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/int/src/sys_int_pic32.c ../../../../../mongoose.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1831220292/netcfg.o ${OBJECTDIR}/_ext/1831220292/device.o ${OBJECTDIR}/_ext/1831220292/driver.o ${OBJECTDIR}/_ext/1831220292/fs.o ${OBJECTDIR}/_ext/1831220292/wlan.o ${OBJECTDIR}/_ext/1831220292/socket.o ${OBJECTDIR}/_ext/1831220292/netapp.o ${OBJECTDIR}/_ext/1831220292/nonos.o ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o ${OBJECTDIR}/_ext/453830916/drv_usart_static.o ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o ${OBJECTDIR}/_ext/1510159531/sys_console_static.o ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o ${OBJECTDIR}/_ext/1396533431/system_init.o ${OBJECTDIR}/_ext/1396533431/system_interrupt.o ${OBJECTDIR}/_ext/1396533431/system_exceptions.o ${OBJECTDIR}/_ext/1396533431/system_tasks.o ${OBJECTDIR}/_ext/1360937237/app.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/cc3100_port.o ${OBJECTDIR}/_ext/1952159421/sys_debug.o ${OBJECTDIR}/_ext/1921105363/sys_devcon.o ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o ${OBJECTDIR}/_ext/948168960/mongoose.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1831220292/netcfg.o.d ${OBJECTDIR}/_ext/1831220292/device.o.d ${OBJECTDIR}/_ext/1831220292/driver.o.d ${OBJECTDIR}/_ext/1831220292/fs.o.d ${OBJECTDIR}/_ext/1831220292/wlan.o.d ${OBJECTDIR}/_ext/1831220292/socket.o.d ${OBJECTDIR}/_ext/1831220292/netapp.o.d ${OBJECTDIR}/_ext/1831220292/nonos.o.d ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o.d ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o.d ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o.d ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o.d ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o.d ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o.d ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o.d ${OBJECTDIR}/_ext/453830916/drv_usart_static.o.d ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o.d ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o.d ${OBJECTDIR}/_ext/1510159531/sys_console_static.o.d ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o.d ${OBJECTDIR}/_ext/1396533431/system_init.o.d ${OBJECTDIR}/_ext/1396533431/system_interrupt.o.d ${OBJECTDIR}/_ext/1396533431/system_exceptions.o.d ${OBJECTDIR}/_ext/1396533431/system_tasks.o.d ${OBJECTDIR}/_ext/1360937237/app.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/cc3100_port.o.d ${OBJECTDIR}/_ext/1952159421/sys_debug.o.d ${OBJECTDIR}/_ext/1921105363/sys_devcon.o.d ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o.d ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o.d ${OBJECTDIR}/_ext/948168960/mongoose.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1831220292/netcfg.o ${OBJECTDIR}/_ext/1831220292/device.o ${OBJECTDIR}/_ext/1831220292/driver.o ${OBJECTDIR}/_ext/1831220292/fs.o ${OBJECTDIR}/_ext/1831220292/wlan.o ${OBJECTDIR}/_ext/1831220292/socket.o ${OBJECTDIR}/_ext/1831220292/netapp.o ${OBJECTDIR}/_ext/1831220292/nonos.o ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o ${OBJECTDIR}/_ext/453830916/drv_usart_static.o ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o ${OBJECTDIR}/_ext/1510159531/sys_console_static.o ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o ${OBJECTDIR}/_ext/1396533431/system_init.o ${OBJECTDIR}/_ext/1396533431/system_interrupt.o ${OBJECTDIR}/_ext/1396533431/system_exceptions.o ${OBJECTDIR}/_ext/1396533431/system_tasks.o ${OBJECTDIR}/_ext/1360937237/app.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/cc3100_port.o ${OBJECTDIR}/_ext/1952159421/sys_debug.o ${OBJECTDIR}/_ext/1921105363/sys_devcon.o ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o ${OBJECTDIR}/_ext/948168960/mongoose.o

# Source Files
SOURCEFILES=../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netcfg.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/device.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/driver.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/fs.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/wlan.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/socket.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netapp.c ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/nonos.c ../src/system_config/mx795_CC3100_e16/framework/driver/rtcc/src/drv_rtcc_static.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_mapping.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_tasks.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_sys_queue.c ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_rm_tasks.c ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_mapping.c ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static.c ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static_byte_model.c ../src/system_config/mx795_CC3100_e16/framework/system/clk/src/sys_clk_static.c ../src/system_config/mx795_CC3100_e16/framework/system/console/src/sys_console_static.c ../src/system_config/mx795_CC3100_e16/framework/system/ports/src/sys_ports_static.c ../src/system_config/mx795_CC3100_e16/system_init.c ../src/system_config/mx795_CC3100_e16/system_interrupt.c ../src/system_config/mx795_CC3100_e16/system_exceptions.c ../src/system_config/mx795_CC3100_e16/system_tasks.c ../src/app.c ../src/main.c ../src/cc3100_port.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/debug/src/sys_debug.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon_pic32mx.c ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/int/src/sys_int_pic32.c ../../../../../mongoose.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-mx795_CC3100_e16.mk dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1831220292/netcfg.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netcfg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netcfg.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netcfg.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/netcfg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/netcfg.o.d" -o ${OBJECTDIR}/_ext/1831220292/netcfg.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netcfg.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/device.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/device.o.d" -o ${OBJECTDIR}/_ext/1831220292/device.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/device.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/driver.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/driver.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/driver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/driver.o.d" -o ${OBJECTDIR}/_ext/1831220292/driver.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/driver.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/fs.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/fs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/fs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/fs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/fs.o.d" -o ${OBJECTDIR}/_ext/1831220292/fs.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/fs.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/wlan.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/wlan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/wlan.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/wlan.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/wlan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/wlan.o.d" -o ${OBJECTDIR}/_ext/1831220292/wlan.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/wlan.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/socket.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/socket.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/socket.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/socket.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/socket.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/socket.o.d" -o ${OBJECTDIR}/_ext/1831220292/socket.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/socket.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/netapp.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netapp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netapp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netapp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/netapp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/netapp.o.d" -o ${OBJECTDIR}/_ext/1831220292/netapp.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netapp.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/nonos.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/nonos.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/nonos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/nonos.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/nonos.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/nonos.o.d" -o ${OBJECTDIR}/_ext/1831220292/nonos.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/nonos.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o: ../src/system_config/mx795_CC3100_e16/framework/driver/rtcc/src/drv_rtcc_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1227741461" 
	@${RM} ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o.d" -o ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o ../src/system_config/mx795_CC3100_e16/framework/driver/rtcc/src/drv_rtcc_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_mapping.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_tasks.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_sys_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_sys_queue.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_rm_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_rm_tasks.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o: ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/453830916" 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o.d" -o ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_mapping.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/453830916/drv_usart_static.o: ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/453830916" 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/453830916/drv_usart_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/453830916/drv_usart_static.o.d" -o ${OBJECTDIR}/_ext/453830916/drv_usart_static.o ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o: ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static_byte_model.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/453830916" 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o.d 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o.d" -o ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static_byte_model.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1456866848/sys_clk_static.o: ../src/system_config/mx795_CC3100_e16/framework/system/clk/src/sys_clk_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1456866848" 
	@${RM} ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1456866848/sys_clk_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1456866848/sys_clk_static.o.d" -o ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o ../src/system_config/mx795_CC3100_e16/framework/system/clk/src/sys_clk_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1510159531/sys_console_static.o: ../src/system_config/mx795_CC3100_e16/framework/system/console/src/sys_console_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1510159531" 
	@${RM} ${OBJECTDIR}/_ext/1510159531/sys_console_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1510159531/sys_console_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1510159531/sys_console_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1510159531/sys_console_static.o.d" -o ${OBJECTDIR}/_ext/1510159531/sys_console_static.o ../src/system_config/mx795_CC3100_e16/framework/system/console/src/sys_console_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1602879792/sys_ports_static.o: ../src/system_config/mx795_CC3100_e16/framework/system/ports/src/sys_ports_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1602879792" 
	@${RM} ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1602879792/sys_ports_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1602879792/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o ../src/system_config/mx795_CC3100_e16/framework/system/ports/src/sys_ports_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_init.o: ../src/system_config/mx795_CC3100_e16/system_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_init.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_init.o ../src/system_config/mx795_CC3100_e16/system_init.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_interrupt.o: ../src/system_config/mx795_CC3100_e16/system_interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_interrupt.o ../src/system_config/mx795_CC3100_e16/system_interrupt.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_exceptions.o: ../src/system_config/mx795_CC3100_e16/system_exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_exceptions.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_exceptions.o ../src/system_config/mx795_CC3100_e16/system_exceptions.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_tasks.o: ../src/system_config/mx795_CC3100_e16/system_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_tasks.o ../src/system_config/mx795_CC3100_e16/system_tasks.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1360937237/cc3100_port.o: ../src/cc3100_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cc3100_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cc3100_port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/cc3100_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/cc3100_port.o.d" -o ${OBJECTDIR}/_ext/1360937237/cc3100_port.o ../src/cc3100_port.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1952159421/sys_debug.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/debug/src/sys_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1952159421" 
	@${RM} ${OBJECTDIR}/_ext/1952159421/sys_debug.o.d 
	@${RM} ${OBJECTDIR}/_ext/1952159421/sys_debug.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1952159421/sys_debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1952159421/sys_debug.o.d" -o ${OBJECTDIR}/_ext/1952159421/sys_debug.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/debug/src/sys_debug.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1921105363/sys_devcon.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1921105363" 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1921105363/sys_devcon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1921105363/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/1921105363/sys_devcon.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1921105363" 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o.d" -o ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon_pic32mx.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/993332121/sys_int_pic32.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/int/src/sys_int_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/993332121" 
	@${RM} ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/993332121/sys_int_pic32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/993332121/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/int/src/sys_int_pic32.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/948168960/mongoose.o: ../../../../../mongoose.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/948168960" 
	@${RM} ${OBJECTDIR}/_ext/948168960/mongoose.o.d 
	@${RM} ${OBJECTDIR}/_ext/948168960/mongoose.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/948168960/mongoose.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/948168960/mongoose.o.d" -o ${OBJECTDIR}/_ext/948168960/mongoose.o ../../../../../mongoose.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
else
${OBJECTDIR}/_ext/1831220292/netcfg.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netcfg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netcfg.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netcfg.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/netcfg.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/netcfg.o.d" -o ${OBJECTDIR}/_ext/1831220292/netcfg.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netcfg.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/device.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/device.o.d" -o ${OBJECTDIR}/_ext/1831220292/device.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/device.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/driver.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/driver.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/driver.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/driver.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/driver.o.d" -o ${OBJECTDIR}/_ext/1831220292/driver.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/driver.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/fs.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/fs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/fs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/fs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/fs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/fs.o.d" -o ${OBJECTDIR}/_ext/1831220292/fs.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/fs.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/wlan.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/wlan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/wlan.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/wlan.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/wlan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/wlan.o.d" -o ${OBJECTDIR}/_ext/1831220292/wlan.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/wlan.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/socket.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/socket.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/socket.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/socket.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/socket.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/socket.o.d" -o ${OBJECTDIR}/_ext/1831220292/socket.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/socket.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/netapp.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netapp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netapp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/netapp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/netapp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/netapp.o.d" -o ${OBJECTDIR}/_ext/1831220292/netapp.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/netapp.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1831220292/nonos.o: ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/nonos.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1831220292" 
	@${RM} ${OBJECTDIR}/_ext/1831220292/nonos.o.d 
	@${RM} ${OBJECTDIR}/_ext/1831220292/nonos.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1831220292/nonos.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1831220292/nonos.o.d" -o ${OBJECTDIR}/_ext/1831220292/nonos.o ../../../../../../../../../CC3100SDK_1.2.0/cc3100-sdk/simplelink/source/nonos.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o: ../src/system_config/mx795_CC3100_e16/framework/driver/rtcc/src/drv_rtcc_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1227741461" 
	@${RM} ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o.d" -o ${OBJECTDIR}/_ext/1227741461/drv_rtcc_static.o ../src/system_config/mx795_CC3100_e16/framework/driver/rtcc/src/drv_rtcc_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_mapping.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_mapping.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_tasks.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_tasks.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_sys_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_sys_queue.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_sys_queue.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o: ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_rm_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1474605348" 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o.d" -o ${OBJECTDIR}/_ext/1474605348/drv_spi_static_rm_tasks.o ../src/system_config/mx795_CC3100_e16/framework/driver/spi/static/src/drv_spi_static_rm_tasks.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o: ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/453830916" 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o.d" -o ${OBJECTDIR}/_ext/453830916/drv_usart_mapping.o ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_mapping.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/453830916/drv_usart_static.o: ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/453830916" 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/453830916/drv_usart_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/453830916/drv_usart_static.o.d" -o ${OBJECTDIR}/_ext/453830916/drv_usart_static.o ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o: ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static_byte_model.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/453830916" 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o.d 
	@${RM} ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o.d" -o ${OBJECTDIR}/_ext/453830916/drv_usart_static_byte_model.o ../src/system_config/mx795_CC3100_e16/framework/driver/usart/src/drv_usart_static_byte_model.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1456866848/sys_clk_static.o: ../src/system_config/mx795_CC3100_e16/framework/system/clk/src/sys_clk_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1456866848" 
	@${RM} ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1456866848/sys_clk_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1456866848/sys_clk_static.o.d" -o ${OBJECTDIR}/_ext/1456866848/sys_clk_static.o ../src/system_config/mx795_CC3100_e16/framework/system/clk/src/sys_clk_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1510159531/sys_console_static.o: ../src/system_config/mx795_CC3100_e16/framework/system/console/src/sys_console_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1510159531" 
	@${RM} ${OBJECTDIR}/_ext/1510159531/sys_console_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1510159531/sys_console_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1510159531/sys_console_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1510159531/sys_console_static.o.d" -o ${OBJECTDIR}/_ext/1510159531/sys_console_static.o ../src/system_config/mx795_CC3100_e16/framework/system/console/src/sys_console_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1602879792/sys_ports_static.o: ../src/system_config/mx795_CC3100_e16/framework/system/ports/src/sys_ports_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1602879792" 
	@${RM} ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1602879792/sys_ports_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1602879792/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/1602879792/sys_ports_static.o ../src/system_config/mx795_CC3100_e16/framework/system/ports/src/sys_ports_static.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_init.o: ../src/system_config/mx795_CC3100_e16/system_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_init.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_init.o ../src/system_config/mx795_CC3100_e16/system_init.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_interrupt.o: ../src/system_config/mx795_CC3100_e16/system_interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_interrupt.o ../src/system_config/mx795_CC3100_e16/system_interrupt.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_exceptions.o: ../src/system_config/mx795_CC3100_e16/system_exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_exceptions.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_exceptions.o ../src/system_config/mx795_CC3100_e16/system_exceptions.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1396533431/system_tasks.o: ../src/system_config/mx795_CC3100_e16/system_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1396533431" 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1396533431/system_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1396533431/system_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1396533431/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1396533431/system_tasks.o ../src/system_config/mx795_CC3100_e16/system_tasks.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1360937237/cc3100_port.o: ../src/cc3100_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cc3100_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/cc3100_port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/cc3100_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1360937237/cc3100_port.o.d" -o ${OBJECTDIR}/_ext/1360937237/cc3100_port.o ../src/cc3100_port.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1952159421/sys_debug.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/debug/src/sys_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1952159421" 
	@${RM} ${OBJECTDIR}/_ext/1952159421/sys_debug.o.d 
	@${RM} ${OBJECTDIR}/_ext/1952159421/sys_debug.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1952159421/sys_debug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1952159421/sys_debug.o.d" -o ${OBJECTDIR}/_ext/1952159421/sys_debug.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/debug/src/sys_debug.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1921105363/sys_devcon.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1921105363" 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1921105363/sys_devcon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1921105363/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/1921105363/sys_devcon.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1921105363" 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o.d" -o ${OBJECTDIR}/_ext/1921105363/sys_devcon_pic32mx.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/devcon/src/sys_devcon_pic32mx.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/993332121/sys_int_pic32.o: ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/int/src/sys_int_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/993332121" 
	@${RM} ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/993332121/sys_int_pic32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/993332121/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/993332121/sys_int_pic32.o ../../../../../../../../../microchip/harmony/v1_08_01/framework/system/int/src/sys_int_pic32.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
${OBJECTDIR}/_ext/948168960/mongoose.o: ../../../../../mongoose.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/948168960" 
	@${RM} ${OBJECTDIR}/_ext/948168960/mongoose.o.d 
	@${RM} ${OBJECTDIR}/_ext/948168960/mongoose.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/948168960/mongoose.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"/home/alex/CC3100SDK_1.2.0/cc3100-sdk/simplelink/include" -ffunction-sections -O1 -I"../src" -I"../src/system_config/mx795_CC3100_e16" -I"../src/mx795_CC3100_e16" -I"../../../../framework" -I"../src/system_config/mx795_CC3100_e16/framework" -I"../../../../../../../../../microchip/harmony/v1_08_01/framework" -MMD -MF "${OBJECTDIR}/_ext/948168960/mongoose.o.d" -o ${OBJECTDIR}/_ext/948168960/mongoose.o ../../../../../mongoose.c    -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../../../../../../microchip/harmony/v1_08_01/bin/framework/peripheral/PIC32MX795F512L_peripherals.a ../../../../../../../../../microchip/harmony/v1_08_01/framework/tcpip/src/crypto/aes_pic32mx.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../../../../../../../../../microchip/harmony/v1_08_01/bin/framework/peripheral/PIC32MX795F512L_peripherals.a ../../../../../../../../../microchip/harmony/v1_08_01/framework/tcpip/src/crypto/aes_pic32mx.a      -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI $(COMPARISON_BUILD)    -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=2048,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../../../../../../microchip/harmony/v1_08_01/bin/framework/peripheral/PIC32MX795F512L_peripherals.a ../../../../../../../../../microchip/harmony/v1_08_01/framework/tcpip/src/crypto/aes_pic32mx.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../../../../../../../../../microchip/harmony/v1_08_01/bin/framework/peripheral/PIC32MX795F512L_peripherals.a ../../../../../../../../../microchip/harmony/v1_08_01/framework/tcpip/src/crypto/aes_pic32mx.a      -DXPRJ_mx795_CC3100_e16=$(CND_CONF)  -no-legacy-libc  -DEXT_LIB_REGISTERED_GENERAL_EVENTS -DSL_FULL -DCS_PLATFORM=6 -DMG_SIMPLELINK_NO_OSI $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/mqtt_client.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/mx795_CC3100_e16
	${RM} -r dist/mx795_CC3100_e16

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
