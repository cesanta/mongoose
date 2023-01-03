
# RMII with the PIO in an RP2040 for 10Mbps Ethernet

- Full-duplex 10Mbps
- RMII Tx implemented with a PIO state machine and single-buffer DMA transfers
  - Once the DMA starts sending to the PIO, subsequent attempts to tx will block 
- RMII Rx implemented with a PIO state machine and ping-pong buffer DMA transfers
  - PIO code handles the CSR_DV signal, stops and raises an IRQ on frame ending. CRC is calculated later for minimum dead time between frames
- SMI implemented with a PIO state machine
- The RP2040 is clocked from the LAN8720 and works at 50MHz
- CRC calculation uses a fast table-based algorithm that doesn't require much storage (nibble calculation)

## Hardware connections

You can change most pins at will, bear in mind that each function requires consecutive pins, see main.c
The RETCLK (should be REFCLK...) pin can go to either GPIO20 or GPIO22

| LAN8720 | | Raspberry Pi Pico |  |
| ----------- | ----------- |----------------- | -- |
| 1,2 | VCC | 3V3 | 36 |
| 3,4 | GND | GND | 38 |
| 5 | MDC | GPIO15 | 20 |
| 6 | MDIO | GPIO14 | 19 |
| 7 | CRS | GPIO8 | 11 |
| 8 | nINT / RETCLK | GPIO20 | 26 |
| 9 | RX1 | GPIO7 | 10 |
| 10 | RX0 | GPIO6 | 9 |
| 11 | TX0 | GPIO10 | 14 |
| 12 | TX-EN | GPIO12 | 16 |
| 13 |
| 14| TX1 | GPIO11 | 15 |


## Build and run

Clone Mongoose repo, go to this example, and build it:

```sh
git clone https://github.com/cesanta/mongoose
cd mongoose/examples/rp2040/pico-rmii
make build
```

This will generate a firmware file: `build/firmware.uf2`. Reboot your Pico board in bootloader mode, and copy it to the RPI disk.

Attach to the serial console, wait a couple of seconds, then plug in your Ethernet cable:

```
$ picocom /dev/ttyACM0 -i -b 115200 --imap=lfcrlf
6213 1 mongoose.c:6757:onstatechange    Link up
6217 3 mongoose.c:6840:tx_dhcp_discover DHCP discover sent
621b 3 mongoose.c:6723:arp_cache_add    ARP cache: added 192.168.69.2 @ 52:54:...
6222 2 mongoose.c:6749:onstatechange    READY, IP: 192.168.69.243
6227 2 mongoose.c:6750:onstatechange           GW: 192.168.69.1
622b 2 mongoose.c:6752:onstatechange           Lease: 21600 sec
```

Note the acquired IP address printed (you can also check your DHCP server). Point your browser to that address, or run curl, you should see an "ok" message:

```
$ curl 192.168.69.243
ok
```
