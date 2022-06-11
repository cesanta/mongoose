# MIP - experimental TCP/IP stack for Mongoose Library

This bare-metal embedded TCP/IP stack is designed specifically for Mongoose.
It does not implement BSD sockets interface - instead, it implements
an event-based API tailored for Mongoose Library. As a result, it does not
need to use synchronisation primitives. It is very fast, and can work
with or without RTOS.

See example usage at [examples/stm32/nucleo-f746zg-baremetal](../examples/stm32/nucleo-f746zg-baremetal)
