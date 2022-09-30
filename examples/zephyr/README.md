# Zephyr Project examples

## Building

- Run `make zephyr`, this will clone the Zephyr Project repo or update it if you've already done it before
- Now you can build the project, follow one of this options: 

### Trying on a simulated board

The default board is an x86 machine that can be simulated with QEMU, and runs inside a _Docker_ container, you only need to have _Docker_ installed. Its Ethernet is also simulated and uses the TUN/TAP interface. 
You'll need a TUN/TAP interface (default name is _tap0_) on your host machine; follow the instructions [here](https://github.com/cesanta/mongoose/tree/master/examples/mip-tap#linux-setup) to configure your network to provide a suitable networking environment. 
- Run `make build`. The interface configuration on the simulated machine side is in the file _overlay-e1000.conf_, and defaults to _tap0_. You can change this overlay file adding an `OVERLAY` argument (e.g.: *make build OVERLAY="-DOVERLAY_CONFIG=something"*).
- Once the build succeeds, run `make run` to run the target. The default _Docker_ configuration is to share _/dev/net/tun_; if this does not work for you, pass the necessary _Docker_ parameters inside `SHARETUN` (e.g.: *make run SHARETUN=something*).

### Using a real board

- Run `make build BOARD=yourboardname` (e.g.: *make build BOARD=nucleo_f746zg*)
- Once the build succeeds, run `make flash`. The Makefile shares the USB bus with the _Docker_ container, this works well with ST-Link and J-Link devices. If this does not work for you, pass the necessary _Docker_ parameters inside `SHAREUSB` (e.g.: *make flash SHAREUSB=something*)
- The generated ELF file is at _build/zephyr_

## Cleaning up

- You can run `make clean` to clean up the build files but keep the configuration files, which speeds the next build
- If you do major changes (like compiling for a different board...), run `make pristine` to clean up everything under the _build_ directory.
