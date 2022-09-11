# MIP - a built-in TCP/IP stack for Mongoose Library

This bare-metal embedded TCP/IP stack is designed specifically for Mongoose.
Works with or without RTOS. Makes it possible to implement networking without
any extra software - just drop `mongoose.c` and `mongoose.h` into your sources,
and you're done!

## Implemented drivers

- STM32F4xx
- STM32F7xx
- W5500

## Example usage

- [nucleo-f746 bare metal](../examples/stm32/nucleo-f746zg-baremetal)
- [nucleo-f429 bare metal](../examples/stm32/nucleo-f429zi-baremetal)
- (VIDEO) [stm32f7 with CubeIDE](https://www.youtube.com/watch?v=8htC_TSBeO0)
- [Xiao M0 SAMD21 Arduino + W5500](../examples/arduino/w5500)
