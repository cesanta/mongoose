
# Device Dashboard via RNDIS on an RP2040

Your headless Raspberry Pi Pico-based hardware can also have a device dashboard on a web browser when you connect it to your computer via USB

## Build and run

Clone Mongoose repo, go to this example, and build it:

```sh
git clone https://github.com/cesanta/mongoose
cd mongoose/examples/rp2040/pico-rndis-dashboard
make build
```

This will generate a firmware file: `build/firmware.uf2`. Reboot your Pico board in bootloader mode, and copy it to the RPI disk.

The device will reboot, register as a USB RNDIS device, and add a network to your computer.
Run a browser, and open 192.168.3.1, you should see a device dashboard

Note: USB stdio in the Pico-SDK is done in background with TinyUSB and an interrupt to hide it from the user and periodically call tusb_task(). When we use TinyUSB, that code is removed from the compilation list; so this example uses UART stdio (UART 0) to keep things simple and focused on the RNDIS example. If you want to see the console output, connect an USB-to-UART or a low-voltage-TTL-to-RS-232 adapter to GPIO0
