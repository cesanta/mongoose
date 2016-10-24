# nRF52 example project

This example shows how to use mongoose on nRF52 boards.
To run it you will need:

- [nRF52 DK](https://www.nordicsemi.com/Products/Getting-started-with-the-nRF52-Development-Kit) dev board
- Linux machine with 6lowpan module enabled. There are a few tutorials out there,
  e.g [this one](https://devzone.nordicsemi.com/blogs/972/setting-up-ipv6-over-ble-using-nrf52-series-and-co/)
- [Docker](https://docs.docker.com/engine/installation/linux/)

## Building firmware and cleaning

Nothing special: `make` and `make clean`.

## Flashing

`make flash`

## Watch debug output

This is a two-step process:

- In a separate terminal, invoke `make jlinkexe`. It will run `JLinkExe`
  utility, in which you'll need to type `connect`. You can actually do a lot of
  things there, including halting and stopping the target, flashing firmware,
  etc. You can type `?` to get the list of all available commands.
- Now, in another terminal, invoke `make rtt`. It will start RTT client which
  will connect to the running `JLinkExe` and will show logs from the device.

## Debug with GDB

This is a two-step process:

- In a separate terminal, invoke `make gdbserver`. Note that `make jlinkexe`
  and `make gdbserver`, unfortunately, cannot run sumultaneously.
- Now, in another terminal, invoke `make gdb`. It will start GDB session which
  will connect to the running GDB server.

