# Pico 2 / XIAO RP2040: USB Ethernet and serial with Mongoose

One USB cable gives the board a CDC-NCM Ethernet interface and a CDC-ACM
serial port that carries the Mongoose log. The native RP2040/RP2350 USB
controller is driven directly: no PIO, Ethernet PHY, TinyUSB, or lwIP.
Mongoose provides TCP/IP, a DHCP server, and HTTP/HTTPS servers.

The USB code is a Mongoose driver, `src/drivers/usb_rp.c`, enabled by
`MG_ENABLE_DRIVER_USB_RP` in `mongoose_config.h`. `mg_mgr_init()` starts it,
so `main.c` only sets up the LED and the HTTP listeners.

## Build and flash

Install the Pico SDK, CMake, Arm GNU toolchain, and picotool. The Makefile
uses `~/src/pico-sdk`; override `PICO_SDK_PATH` if necessary.
The default target is `PICO_BOARD=pico2` (RP2350, Arm Cortex-M33).

```sh
make                                # Pico 2
make PICO_BOARD=seeed_xiao_rp2040   # XIAO RP2040
```

Each board has its own build directory, `build/pico2/firmware.uf2` or
`build/seeed_xiao_rp2040/firmware.uf2`, so RP2040 and RP2350 CMake settings
never mix.

On Pico 2, hold BOOTSEL while connecting the USB cable. On XIAO, hold BOOT
while pressing and releasing RESET, then release BOOT. Flash:

```sh
make flash
```

The firmware also enters BOOTSEL when its serial port is opened at 1200 baud
and DTR drops. `picotool -f` alone cannot reboot it: the driver does not
implement the Pico SDK vendor reset interface.

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

Use the actual serial device path. The baud rate is ignored, except that
1200 baud followed by DTR dropping requests BOOTSEL. To exit screen, press
Ctrl-A, then backslash, and confirm. The serial port is an output console;
received bytes are discarded.

Linux normally provides NCM and ACM drivers. Windows 11 provides NCM and ACM;
the driver includes Microsoft OS 2.0 compatible ID descriptors for driver
binding. Linux and Windows have not been tested here. Windows 10 has no
built-in NCM driver.

## Implementation

* `main.c`: LED blink task and the HTTP/HTTPS handler.
* `../../../src/drivers/usb_rp.c`: RP2040/RP2350 USB device controller.
* `../../../src/drivers/usb.c`: controller independent NCM + ACM:
  descriptors, control requests, NTB16 negotiation, framing, parser, and the
  serial log buffer. Shared with the SAMD21 driver, `usb_samd21.c`.
* Network control/data interfaces 0/1; ACM control/data interfaces 2/3.
* Network endpoints: interrupt IN 0x81, bulk OUT/IN 0x02/0x82.
* Serial endpoints: interrupt IN 0x83, bulk OUT/IN 0x04/0x84.
* Full-speed USB, 64-byte packets. Transfers span multiple packets and end
  with short packets or ZLPs. Ethernet FCS is not sent on USB.
* NTB16 with no CRC, 4 KiB maximum IN/OUT blocks, and up to 16 incoming
  datagrams per block, including chained NDPs. The whole block is validated
  before any frame is delivered. NTB32 and CRC mode are not advertised.
* One Ethernet frame per outgoing block, within the host's negotiated input
  size. Outgoing frames are not aggregated.
* Host and device MAC addresses and the USB serial number come from the
  board's flash ID. One outgoing block can be in flight; incoming frames go
  to Mongoose's 8 KiB queue. The serial log is a nonblocking 2 KiB ring; old
  logs are overwritten when the host is not reading.
* Buffer sizes can be changed with `MG_USB_NCM_NTB_SIZE`,
  `MG_USB_NCM_LOG_SIZE` and `MG_USB_NCM_QUEUE_SIZE`.
* Single core, polled from `mg_mgr_poll()`. No host mode, RNDIS, suspend,
  remote wake, or Ethernet bridging.

The development VID/PID is `CAFE:40E4`, not an allocation for shipped
products. Get a legitimate VID/PID before distributing hardware.
