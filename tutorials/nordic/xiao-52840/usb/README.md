# XIAO nRF52840: USB Ethernet and serial with Mongoose

One USB cable gives the board a CDC-NCM Ethernet interface and a CDC-ACM
serial port that carries the Mongoose log. The nRF52840 USB device controller
is driven directly: no TinyUSB, SoftDevice, Ethernet PHY, or lwIP. Mongoose
provides TCP/IP, a DHCP server, and HTTP/HTTPS servers.

The USB code is a Mongoose driver, `src/drivers/usb_nrf52.c`. In
`mongoose_config.h`, `MG_ENABLE_DRIVER_USB_NRF52` selects it, and
`MG_ENABLE_USB_ETHERNET` and `MG_ENABLE_USB_SERIAL` turn on its two
functions. Mongoose runs it from `mg_mgr_init()` and `mg_mgr_poll()`, so
`main.c` only sets up the clock, the LED, and the HTTP listeners.

## Build and flash

Install the Arm GNU toolchain and `adafruit-nrfutil`
(`pip install adafruit-nrfutil`). The first build fetches CMSIS and Nordic's
nrfx, for the device headers and the startup code.

```sh
make
make flash PORT=/dev/cu.usbmodemXXXX
```

The board keeps its Adafruit bootloader. The application starts at 0x27000,
above the area reserved for the SoftDevice, which is not used. `make flash`
opens the serial port at 1200 baud, which reboots the firmware into the
bootloader, then uploads over serial DFU. If no firmware with a USB serial
port is running, double-tap RESET to enter the bootloader first.

## Use on macOS

Connect with a USB data cable. macOS creates an Ethernet service and a
`/dev/cu.usbmodem*` serial port with built-in drivers. Allow the USB accessory
if prompted, and leave the network service set to DHCP.

`mongoose_config.h` gives the device a static address, and the driver serves
DHCP to the host:

* Device: `192.168.7.1/24`
* HTTP: <http://192.168.7.1/>
* HTTPS: <https://192.168.7.1/> (self-signed demonstration certificate)
* JSON tick: <http://192.168.7.1/api/tick>

Change `MG_TCPIP_IP` and `MG_TCPIP_MASK` if the subnet conflicts with another
network. Remove them to make the board a DHCP client instead, for example
behind macOS Internet Sharing.

```sh
curl --noproxy '*' http://192.168.7.1/api/tick
curl --noproxy '*' -k https://192.168.7.1/api/tick
screen /dev/cu.usbmodemXXXX 115200
```

The baud rate is ignored, except that 1200 baud followed by DTR dropping
reboots into the bootloader. The serial port is an output console; received
bytes are discarded.

## Implementation

* `main.c`: LED blink task and the HTTP/HTTPS handler.
* `hal.h`, `hal.c`: clock, SysTick, RNG, LED, newlib stubs.
* `../../../../src/drivers/usb_nrf52.c`: nRF52840 USB device controller,
  polled. EasyDMA moves each 64-byte packet, one transfer at a time.
  Applies Nordic's errata 171, 187 and 199 workarounds.
* `../../../../src/drivers/usb.c`: controller independent NCM + ACM, shared
  with the RP2040/RP2350 and SAMD21 drivers.
