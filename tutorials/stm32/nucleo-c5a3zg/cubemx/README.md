# Nucleo-C5A3ZI Mongoose Integration

The `.ioc2` file in this directory is pre-configured. Note that after the
project generation, you should do the following to complete integraion:

1. Edit the linker script `.ld` file and increase heap and stack sizes to
   0x20000 and 0x4000 respectively
2. Add Mongoose to the `main.c`, see https://mongoose.ws/docs/guides/stm32-tcpip-stack/#modify-mainc

