# MIP webserver over FreeRTOS on EK-TM4C1294XL

This firmware uses MIP, an experimental TCP/IP stack of the Mongoose Network Library, running as a FreeRTOS task.
It implements the following:

- Minimal elementary web server, as simple as possible
- No dependencies: no HAL, no CMSIS
- Hand-written [mcu.h](mcu.h) header based on the [datasheet](https://www.ti.com/lit/pdf/spms433)
- Interrupt-driven [Ethernet driver](../../../drivers/mip_driver_tm4c.c)
- LED blinky, based on another FreeRTOS task
- Debug log on UART0 (ICDI)

## Requirements

- [GNU make](http://mongoose.ws/tutorials/tools/#gnu-make)
- [ARM GCC](http://mongoose.ws/tutorials/tools/#arm-gcc)
- [Uniflash](https://mongoose.ws/tutorials/ti/ek-tm4c1294xl-baremetal/#build-and-run) for flashing

The links above will send you to tutorials on how to install each of those tools in your workstation for Linux, Mac, and Windows.

You'll also need _git_ so the Makefile can clone the FreeRTOS-Kernel repository. We assume you have it since you got to get this repository somehow. If you don't, and don't want to install it, just get the proper [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) version (see the Makefile) from its repository, as you did with the Mongoose repository.

## Usage

Plugin your board into USB, and attach an Ethernet cable.
To build and flash:

```sh
$ make clean flash
```

To see debug log, use any serial monitor program like `picocom` at 115200 bps and configure it to insert carriage returns after line feeds:

```sh
$ picocom /dev/ttyACM0 -i -b 115200 --imap=lfcrlf
```

There is also a [detailed tutorial on this example](https://mongoose.ws/tutorials/ti/ek-tm4c1294xl-freertos-mip/)

For more details and benchmark data on MIP, check the [F746ZG baremetal example](../../stm32/nucleo-f746zg-baremetal/)
