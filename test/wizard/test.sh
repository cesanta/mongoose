#!/bin/bash

RTOSES="baremetal FreeRTOS"
WIZARD_URL=""
test "$1" != "" && WIZARD_URL="WIZARD_URL=$1"

dotest ()
{
	echo "BOARD=$1 IDE=$2 RTOS=$3" >> test.log
	make BOARD=$1 IDE=$2 RTOS=$3 $4 $WIZARD_URL || ( \
		echo "^^^ FAILED ^^^" >> test.log && rm -rf wizard* workspace )
}


cd `dirname "$0"`
rm test.log


STM32="f207 f429 f439 f746 f756 f767 h563 h573 h723 h735 h743 h745 h747 h753 h755"
STIDES="GCC+make CubeIDE"
# Keil: f756
for board in $STM32; do
	for ide in $STIDES; do
		for rtos in $RTOSES; do
			dotest $board $ide $rtos
		done
	done
done
dotest h7s3l8 GCC+make baremetal


NXP="rt1020 rt1024 rt1040 rt1060 rt1064 rt1170 mcxn947"
NXPIDES="GCC+make MCUXpresso"
for board in $NXP; do
	for ide in $NXPIDES; do
		for rtos in $RTOSES; do
			dotest $board $ide $rtos
		done
	done
done
NXP="rw612"
for board in $NXP; do
	for ide in $NXPIDES; do
		for rtos in "baremetal"; do
			dotest $board $ide $rtos
		done
	done
done


TI="tm4c129"
TIIDES="GCC+make"
for board in $TI; do
	for ide in $TIIDES; do
		for rtos in $RTOSES; do
			dotest $board $ide $rtos
		done
	done
done
dotest "tms570" "CGT+make" "baremetal"


INFINEON="xmc4400 xmc4700 xmc7200"
INFINEONIDES="GCC+make ModusIDE"
for board in $INFINEON; do
	for ide in $INFINEONIDES; do
		for rtos in "baremetal"; do
			dotest $board $ide $rtos
		done
	done
done


ESP="esp32"
for board in $ESP; do
	dotest $board "ESP-IDF" "baremetal"
done


PICO="evb-pico evb-pico2 evb-pico2-w5100 pico-w pico2-w"
for board in $PICO; do
	for rtos in "baremetal"; do
		dotest $board "Pico-SDK" $rtos
	done
done


#NORDIC

RENESAS="ra6 ra8m1"
RENESASIDES="GCC+make e2studio"
for board in $RENESAS; do
	for ide in $RENESASIDES; do
		for rtos in "baremetal"; do
			dotest $board $ide $rtos
		done
	done
done


# f439 not supported
# h755 not supported in 3.7.0 branch; master branch currently not building
# other ST boards (PHY address != 0) might build and not work
ZEPHYR="zephyr zephyr-w5500 f207 f429 f746 f756 f767 h563 h573 h723 h735 h743 h745 h747 h753 mcxn947 rt1060 rt1064 evb-pico nrf91"
for board in $ZEPHYR; do
	dotest $board "Zephyr" "baremetal"
done


#ARDUINO="arduino arduino-esp32 teensy41"
#for board in $ARDUINO; do
#	dotest $board "Arduino" "baremetal"
#done

rm -rf workspace pico-sdk mcuxpresso .cache .eclipse .p2 build
cd -
