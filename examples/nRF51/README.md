# nRF51 example project

This is a Mongoose "Hello, world" that can be compiled under nRF51 IoT SDK.

To make it work, you should download [nRF51 IoT SDK](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF51-IoT-SDK)
and unpack it in the current directory as `nrf51_iot_sdk` (so that
`nrf51_iot_sdk` contains `components`, `external`, etc).

In order to build it with the provided Makefile and ARM GCC, you also need to
edit the file `nrf51_iot_sdk/components/toolchain/gcc/Makefile.posix` (or
`Makefile.windows` if you use Windows) : adjust arm-none-eabi path and version
accordingly to your installation.

The project structure replicates that of the examples provided with the SDK:
Keil uVision project is located at `boards/pca10028/arm5_no_packs`, ARM-GCC
Makefile is located in `boards/pca10028/armgcc`..

E.g. in order to build it with ARM GCC, navigate to
`http/boards/pca10028/armgcc` and type `make`. In order to flash: `make flash`.

For details, refer to an [article on Hackster](https://www.hackster.io/dfrank/mongoose-embedded-networking-library-on-nrf51-and-nrf52-547b15).
