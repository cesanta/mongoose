# RP2040 / RP2350 Mongoose example

This example can be built for both RP2040 and RP2350 targets.

## Set target board

By default, it builds for RP2040 target. If you want to change the target
board, edit `CMakeKists.txt` and set the board, for example:

```cmake
set(PICO_BOARD pico2_w)
```

## Edit WiFi credentials

For WiFi examples, edit `wifi.c` and set WiFi network name and password.

## Build and run

Make sure you have `picotool` installed. Then run:

```sh
make build flash
```

Start serial console to see the debug output.

See detailed documentation at https://mongoose.ws/docs/guides/rp-tcpip-stack/
