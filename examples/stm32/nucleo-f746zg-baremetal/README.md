# Baremetal webserver on NUCLEO-F746ZG

This firmware uses MIP, an experimental TCP/IP stack of the Mongoose Network Library,
which implements the following:

- A complete [HTTP device dashboard](../../device-dashboard) with:
  - User authentication
  - Real-time device data graph
  - Coninfiguration display and update
  - MQTT communication with a remote MQTT server
- Implements SNTP time synchronisation
- No dependencies: no HAL, no CMSIS, no RTOS
- Hand-written [mcu.h](mcu.h) header based on the [datasheet](https://www.st.com/resource/en/reference_manual/rm0385-stm32f75xxx-and-stm32f74xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- Interrupt-driven [Ethernet driver](../../../mip/driver_stm32.c)
- Blue LED blinky, based on SysTick interrupt
- User button handler, turns off/on green LED, based on EXTI, interrupt-driven 
- HardFault handler that blinks red LED
- Debug log on UART3 (st-link)

## Requirements

- [GNU make](http://mongoose.ws/tutorials/tools/#gnu-make)
- [ARM GCC](http://mongoose.ws/tutorials/tools/#arm-gcc)
- [stlink](http://mongoose.ws/tutorials/tools/#stlink) for flashing

The Makefile defaults to using Docker for the compiler, so you don't actually need to install it if you are using a Linux/Mac workstation. If you are not, or you want to run your local ARM compiler, just append `DOCKER=` to the make commands depicted below to call the compiler directly; it must be in your executable path.

In any case, the links above will send you to tutorials on how to install each of those tools in your workstation for Linux, Mac, and Windows.

## Usage

Plugin your Nucleo board into USB, and attach an Ethernet cable.
To build and flash:

```sh
$ make clean flash
```

To see debug log, use any serial monitor program like `picocom` at 115200 bps and configure it to insert carriage returns after line feeds:

```sh
$ picocom /dev/ttyACM0 -i -b 115200 --imap=lfcrlf
```

There is also a [detailed tutorial on this example](https://mongoose.ws/tutorials/stm32/nucleo-f746zg-baremetal/)

## Benchmark

A quick comparison is made with several other implementations.
Note: `IP` in the table below is the IP address printed on the console after
boot. The benchmark command is the same: `siege -c 5 -t 5s http://IP`

|                     | Zephyr   | LWIP sockets | LWIP raw | MIP  |
| ------------------- | -------- | ------------ | -------- | ---- |
| Requests per second | 3        | 16           | 286      | 1080 |
| Firmware size       | 117k (*) | 160k         | 114k     | 28k  |

- Zephyr: uses Zehypr's RTOS and TCP stack, with Mongoose library on top,
  [source code](https://github.com/cesanta/mongoose/tree/master/examples/zephyr/http-server).
  (*) By default, Zephyr example is TLS-enabled. To compare sizes, a TLS-disabled
  build was done by disabling TLS in `prj.conf` and `CMakeLists.txt`
- LWIP sockets: uses FreeRTOS and LWIP with sockets support, with Mongoose
  library on top, [source code](https://github.com/mongoose-examples/stm32-nucleo-f746z).
  built with `#define MG` in `Core/main.c`
- LWIP raw: uses FreeRTOS and LWIP without sockets, LWIP's httpd server,
  [source code](https://github.com/mongoose-examples/stm32-nucleo-f746z).
  built with `#define MG` line commented out in `Core/main.c`
- MIP: this repository

