# nRF52 example project

This is a Mongoose "Hello, world" that can be compiled under nRF IoT SDK.

To make it work, you should download [nRF5 IoT SDK](http://developer.nordicsemi.com/nRF5_IoT_SDK/)
and unpack it in the current directory as `nrf5_iot_sdk` (so that
`nrf5_iot_sdk` contains `components`, `external`, etc).

In order to build it with the provided Makefile and ARM GCC, you also need to
edit the file `nrf5_iot_sdk/components/toolchain/gcc/Makefile.posix` (or
`Makefile.windows` if you use Windows) : adjust arm-none-eabi path and version
accordingly to your installation.

The project structure replicates that of the examples provided with the SDK:
Keil uVision project is located at `boards/pca10040/arm5_no_packs`; ARM-GCC
Makefile is located in `boards/pca10040/armgcc`.

E.g. in order to build it with ARM GCC, navigate to
`http/boards/pca10040/armgcc` and type `make`. In order to flash: `make flash`.

For details, refer to an [article on Hackster](https://www.hackster.io/dfrank/mongoose-embedded-networking-library-on-nrf51-and-nrf52-547b15).
