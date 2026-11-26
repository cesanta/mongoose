# Ethernet over radio simulator

The simulator runs real Mongoose instances with a simulated 802.15.4 radio.
A bridge forwards Ethernet frames between the LAN and radio nodes. Each node
gets an address from a LAN DHCP server and answers HTTP requests from a LAN
client. The current protocol uses one bridge and direct radio hops.

## Shared driver

`src/drivers/radio.[ch]` implements joining, membership, turn scheduling,
Ethernet fragmentation/reassembly and bridge forwarding. `MG_RADIO` selects
raw BLE 2 Mbit/s or IEEE 802.15.4 framing and timing at compile time.

Each hardware driver passes three functions to `mg_radio_init()`:

- `hw_init(ifp, addr)`: initialize the radio; `addr` is the radio address.
- `hw_tx(ifp, buf, len)`: start one radio frame asynchronously; return its
  length if accepted, or zero if busy.
- `hw_poll(ifp)`: service the hardware and report one TX completion or abort.

Received frames go through `mg_radio_rx_queue()` at arrival time, including
when reception runs in an interrupt. PHY length bytes and CRC are excluded.
The queue preserves arrival timestamps so a delayed poll cannot answer an
expired grant.

A hardware driver's init calls `mg_radio_init()` with those functions; its
`mg_tcpip_driver` uses `mg_radio_tx()`, `mg_radio_rx()` and `mg_radio_poll()`
directly. `mg_radio_init()` allocates the interface's
private state and stores it in `ifp->driver_data`. There is one built-in
scheduling protocol and no strategy callback table.

The interface's event handler, `ifp->fn`, gets `MG_TCPIP_EV_PHY_RX` for each
received radio frame and `MG_TCPIP_EV_PHY_TX` for each radio frame just before
it goes to the hardware. `ev_data` is a `struct mg_str *`, the frame.

Call `mg_radio_bridge(radio, lan)` after both interfaces initialize. The bridge
reads the LAN receive queue and passes those Ethernet frames through the
radio interface's normal driver `tx()` function. Driver wrappers can therefore
count or translate Ethernet frames. Frames received from radio nodes are
forwarded through the LAN driver's `tx()` function.

The simulator embeds each radio interface as the first member of its hardware
state and calls the production API, including the timestamped receive queue.
It compiles `mongoose.c` into `simulator.c`, to read the private radio state
for its virtual clock. Radio state lives as long as the program.

## Scheduling

The bridge opens JOIN windows and grants DATA turns to members in round-robin
order. Nodes only send their data during grants. Joining uses randomized reply
timing. The protocol does not request radio ACKs or use CSMA.

Ethernet frames are fragmented into radio frames and reassembled at the
receiver. The bridge sends broadcasts separately to each member. A member is
removed after `MG_RADIO_TIMEOUT_MS` without an answer, counted from its first
missed turn; a node leaves after the same interval without a turn.
`MG_RADIO_ANSWER_TIMEOUT_US` bounds answers and `MG_RADIO_TX_TIMEOUT_US` bounds
hardware transmissions. All devices must share channel, network and PHY settings.

## Running

```sh
make -C test/mesh
# Smaller grid, 60 simulated seconds, 15 seconds to settle:
make -C test/mesh ROWS=2 COLS=2 ARGS="-t 60 -w 15"
# Explicit topology:
./test/mesh/simulator -bridge 50,50 -node 40,40 -node 60,60 -t 60 -w 15
# Reception loss or limited range:
./test/mesh/simulator -bridge 50,50 -node 40,40 -loss 5 -t 60 -w 15
./test/mesh/simulator -bridge 50,50 -node 99,99 -range 20 -t 60 -w 15
```

The simulator requires exactly one bridge. Positions are `X,Y` or `X,Y,Z` in
metres, within the `-area` box. Default radio range is 185 metres; `-loss` sets
independent reception loss in percent. Overlapping transmissions collide
across the simulated channel. Reception is half duplex. Nodes outside range
cannot join. There is no multi-hop forwarding.

Time is virtual: the clock jumps between radio events, protocol deadlines and
periodic stack polls. `mg_micros()` exposes that clock; periodic stack polls
occur every millisecond. The LAN is infinitely fast. A LAN DHCP server uses
10.0.0.1 and the HTTP tester uses 10.0.0.2. After `-w` seconds the tester cycles
through nodes until five seconds before the end of `-t`. The summary reports
successful/failed requests, latency, radio transmissions, collisions, reception
loss and how many nodes joined and acquired addresses.

## Hardware examples

- `tutorials/nordic/xiao-nrf52840/usb-to-ble-bridge`: USB Ethernet to BLE bridge.
- `tutorials/nxp/frdm-rw612/ethernet-to-802.15.4-bridge`: Ethernet to radio bridge.
- `tutorials/nordic/xiao-nrf52840/minimal`: radio node.
- `tutorials/silabs/xiao-mg24/minimal`: radio node.
- `tutorials/nxp/frdm-mcxw71/minimal`: radio node.

Hardware backends are `nrf52`, `mg24`, `mcxw71`, `rw612w`, and `esp32` in
`src/drivers`. Board HALs provide the microsecond clock. The bridge owns LAN
initialization, its receive queue and any promiscuous-mode configuration.

MCXW71 supports raw BLE 2 Mbit/s through GENFSK, without a Bluetooth stack,
and 802.15.4 through ZLL. Its minimal example defaults to BLE:

```sh
make -C tutorials/nxp/frdm-mcxw71/minimal build
make -C tutorials/nxp/frdm-mcxw71/minimal build CFLAGS_EXTRA=-DMG_RADIO=MG_RADIO_802154
```

The HAL initializes the selected PHY and BLE whitening seed. Other boards
using this driver must provide equivalent transceiver setup. All communicating
nodes must use the same PHY, channel and network/access address.
