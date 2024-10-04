#!/bin/bash

RTOSES="baremetal FreeRTOS"


dotest ()
{
	echo "BOARD=$1 IDE=$2 RTOS=$3" >> test.log
	make BOARD=$1 IDE=$2 RTOS=$3 || ( \
		echo "^^^ FAILED ^^^" >> test.log && rm -rf wizard* workspace )
}


cd `dirname "$0"`
rm test.log


STM32="f207 f429 f439 f746 f756 f767 h563 h573 h723 h735 h743 h745 h747 h753 h755"
STIDES="GCC+make CubeIDE"
# Zephyr: !f439
# Keil: f756
for board in $STM32; do
	for ide in $STIDES; do
		for rtos in $RTOSES; do
			dotest $board $ide $rtos
		done
	done
done


NXP="rt1020 rt1024 rt1040 rt1060 rt1064 rt1170 mcxn947"
NXPIDES="GCC+make"
#MCUXpresso: mcxn947
for board in $NXP; do
	for ide in $NXPIDES; do
		for rtos in "baremetal"; do
			dotest $board $ide $rtos
		done
	done
done


INFINEON="xmc4400 xmc4700 xmc7200"
INFINEONIDES="GCC+make"
for board in $INFINEON; do
	for ide in $INFINEONIDES; do
		for rtos in "baremetal"; do
			dotest $board $ide $rtos
		done
	done
done


PICO="evb-pico"
#Zephyr
for board in $PICO; do
	for rtos in "baremetal"; do
		dotest $board "Pico-SDK" $rtos
	done
done


ESP="esp32"
for board in $ESP; do
	dotest $board "ESP-IDF" "baremetal"
done


ARDUINO="teensy41"

rm -rf workspace pico-sdk
cd -
