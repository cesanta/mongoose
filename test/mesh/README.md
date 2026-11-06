# Ethernet over a radio mesh

## The idea

Small radio devices, e.g. IEEE 802.15.4, sub-GHz or LoRa boards, should run
Mongoose as if they were on an ordinary Ethernet LAN: get an IP address over
DHCP, answer HTTP, talk MQTT. No 6LoWPAN, no Thread, no special stack.

To do that, the radio mesh looks like an Ethernet switch to Mongoose.
One or more bridges connect the radio network to the wired LAN: a bridge has
an Ethernet port and a radio. A node in range of a bridge talks to it
directly; a node out of range reaches a bridge through other nodes, which
forward its frames. That makes the radio network a mesh. With several
bridges, each node uses one of them, e.g. the nearest; the mesh covers a
bigger area, and keeps working if a bridge goes down. A PC on the LAN sees
the radio nodes as regular hosts on the same LAN.

```
  PC --- LAN --- bridge ~~~ radio nodes
```

## Two parts plug in

- `struct mg_radio` describes frame size, bitrate, airtime overhead, signal
  parameters and channel-access timings.
- `struct mg_mesh` has `init`, `tick`, `free`, `tx` and `rx` callbacks. The most important
  one is the `tick()` callback which is called with a microsecond timestamp,
  obtained from `mg_micros()`, and handles routing and mesh management.
  The mesh queues and fragments Ethernet frames and reassembles incoming radio
  frames. Driver `tx()` queues Ethernet into the mesh; `rx()` returns a
  reassembled Ethernet frame. A private callback transmits one radio frame.

The default slotted mesh in `../../src/l2_mesh.c` uses the bridge as a coordinator.
It opens windows for new nodes to join and gives joined nodes DATA turns
in round-robin order.

Mongoose calls driver's `poll()`. Radio driver calls `mg_micros()` to get
the current timestamp and calls `mg_mesh :: tick()`.

The goal is to swap radios and mesh strategies without changing application
code. The slotted star deliberately omits radio ACKs to avoid
the extra airtime and radio turnaround. Other strategies may use them.

## Why a simulator

Real boards are slow to test with, and you can't put 500 of them on a desk.
The simulator runs a whole network in one process: every node is a real
Mongoose instance, only the radio and the air between nodes are emulated.
That lets us try mesh strategies with many nodes, and measure how long it
takes nodes to join, get an address, and answer a request.

## Virtual time

The simulator exposes its virtual clock through `mg_micros()`.
TCP/IP reads the same clock through `mg_millis()`, rounded down to milliseconds.
Nothing waits for real time. `-t SEC` sets the duration, 10 minutes by default.

Radio channel checks, transmission starts and ends run at their scheduled
microsecond timestamps. The clock jumps to the earliest radio event, mesh
deadline, or periodic stack poll.

`STEP_MICROSECONDS` is 1000: periodic polling services Mongoose's internal
stack timers every millisecond. Packet delivery triggers immediate work:
the simulator repeatedly polls DHCP, the tester, bridges and nodes at the
same virtual timestamp until that work drains. Ethernet forwarding and
radio receive processing therefore do not wait for the periodic tick.
Internal timer handling can still be delayed by up to about 1 ms.

## The setup

```
  DHCP server ---------+
   10.0.0.1            |
   02:01:00:00:00:00   +-- LAN ---- bridge ~~~ nodes
                       |
  test node    --------+
   10.0.0.2
   02:02:00:00:00:00
```

The LAN is assumed infinitely fast, see "How frames move on the LAN" below.
Two nodes are on it:

- The DHCP server, 10.0.0.1, MAC 02:01:00:00:00:00, gives each node an
  address made from its MAC: 02:00:00:X:Y:Z gets 10.X+1.Y.Z, so node
  addresses never clash with the LAN nodes' 10.0.0.x. It also answers
  ARP requests for its advertised gateway IP, 10.0.0.1, so nodes can resolve
  the gateway MAC and stop retrying. It does not implement IP routing,
  and does not have a full Mongoose instance
- The test node, 10.0.0.2, MAC 02:02:00:00:00:00, is a Mongoose
  instance. It waits for the mesh to settle, 60 seconds by default
  (`-w SEC`), then sends `GET /` to each node in turn, one at a time, at the
  node's own IP address. It wraps back to the first node and keeps querying
  until five seconds before the end, when it stops starting new requests.
  An in-flight request may still complete or time out during the remaining
  time. Runs lasting five seconds or less start no requests. A node with
  no address, a connection error, or a request exceeding 10 seconds counts
  as failed. The summary reports successful and failed attempts, plus
  minimum, mean and maximum response times for successful requests. A
  request still pending at the end is not included in those counts.

Bridges are on the LAN too, and get every frame, as if in promiscuous mode:
a frame for one of their radio nodes isn't addressed to the bridge itself.
A bridge forwards every frame from the LAN to the radio and every frame
from the radio to the LAN.

Bridges and radio nodes get MACs 02:00:00:xx:xx:xx, numbered from 1 in
command-line order: 02:00:00:00:00:01, 02:00:00:00:00:02, and so on.

### How frames move on the LAN

Every LAN node, the DHCP server, the test node and each bridge, has a
LAN interface, `lif`: a `struct mg_tcpip_if` with a receive queue. All of
them use the same driver, `s_lan_driver`. There are no cables and no switch:

- Sending. A node sends a frame with its driver's `tx`, which is
  `lan_tx()`. It looks at the destination MAC and copies the frame into the
  receive queues of the nodes it is for:
  - a broadcast or multicast goes to every node but the sender;
  - a unicast goes to the node with that MAC;
  - bridges get every frame, as a real bridge in promiscuous mode would:
    a frame for a radio node is addressed to the node, not to its bridge.
- Receiving. Each node takes the frames from its own queue when
  the main loop polls it:
  - the test node runs Mongoose, which reads the queue by itself, as it
    would with a real Ethernet driver that queues frames from an interrupt;
  - the DHCP server reads it in `dhcp_poll()`, and replies by sending
    through its own `lif`;
  - bridges read it in `bridges_poll()`, and forward it to the radio.

Enqueuing marks immediate work pending. The main loop drains that work
without advancing virtual time, so the LAN has no simulated propagation,
serialization or polling delay. Sending never calls receiver code directly;
replies are processed by subsequent polls at the same timestamp.

For example, a node asking for an address:

1. The node's DHCP DISCOVER crosses the radio to a bridge, which sends it
   out of its `lif`. It is a broadcast, so `lan_tx()` puts it into the
   queues of the DHCP server, the test node and the other bridges.
2. On its next poll, the DHCP server reads it and sends an OFFER, also a
   broadcast, which lands in the test node's and the bridges' queues.
3. On their next poll, the bridges read the OFFER and send it over the
   radio to the node. The test node's Mongoose drops it: the test node
   has a fixed address and no DHCP client.

### How frames move on the radio

The simulator keeps one accepted transmission per sender in a singly linked
list. A busy sender's driver returns zero, so the mesh retains the fragment
until the driver accepts it. Different senders can still overlap on air.

1. A sender waits a random number of backoff slots, then listens for
   `cca_us` to check whether the channel was idle throughout that interval.
2. If busy, it increases its backoff exponent up to `backoff_max`, waits
   again, and repeats. There is no channel-access attempt limit yet.
3. If idle, it schedules transmission after `turnaround_us`. Another sender
   can also pass its check before either starts: the channel check does not
   reserve the air.
4. Transmission takes `(len + overhead) * 8 / bitrate` seconds, rounded up
   to a microsecond. Overlapping transmissions are both marked collided.
   Channel checks at the same timestamp run before transmission starts,
   so list order cannot make one simultaneous check win.
5. At the end, a collided frame is dropped. Otherwise it is queued for all
   other nodes for broadcast/multicast, or the matching node for unicast.
   Every other bridge receives it too. Receive processing happens at the
   same virtual timestamp.

A receiver drops a frame if any part of that frame overlaps its own channel
backoff, sensing, turnaround or transmission interval, even if it is
available again when the frame ends.
The radio summary reports total sent and collided frames, and lost receptions.
Losses include range or random loss, plus missed unicast receptions when the
destination was transmitting.

The 802.15.4 definition uses 250 kbit/s, 8 bytes overhead, 320 us backoff
slots, 128 us channel sensing and 100 us turnaround. The initial backoff
exponent is 3 (0–7 slots), increasing to 5 (0–31 slots) on busy checks.
After transmitting, a sender waits the radio's inter-frame spacing,
`mg_radio::ifs_us`, 640 us for 802.15.4, before starting its next frame's
backoff. This wait applies even if the frame collided or the sender's
queue was temporarily empty. Reception remains enabled during the
inter-frame wait.
The backoff generator is seeded from the wall clock.

The mesh limits each transmission to 125 bytes plus the 2-byte FCS.
Overhead and inter-frame spacing apply to each fragment.
A receiver must be within `-range METRES` of the sender (default 185 m).
`-loss PERCENT` adds independent random loss for each in-range reception
(default 0%). The radio summary counts range and random losses together.
Collisions and channel sensing still apply across the whole simulated air.

## Files

| File | What |
|------|------|
| `../../src/l2_mesh.h`, `../../src/l2_mesh.c` | `struct mg_radio`, `struct mg_mesh`, shared node initialization, and IEEE 802.15.4 radio |
| `mesh_a.c` | Alternative unslotted star strategy |
| `simulator.h`, `simulator.c` | The simulator |
| `Makefile` | Builds and runs the simulator |

## Where we are

The slotted strategy uses direct radio hops between nodes and a bridge.
The simulator models airtime, half-duplex reception and collisions.

- Frames outside reception range are not received, so nodes out of a bridge's
  range never join. But collisions and channel sensing are still global:
  every overlap is a collision, wherever the receiver is, and a busy
  channel is busy for everyone.
- Mesh routing remains unimplemented: bridge selection, forwarding through other
  nodes and recovery are not implemented.

Next steps include distance-aware collision and channel sensing, mesh
routing and bridge filtering.

## Strategy A — unslotted (simulator default)

Each sender contends for the channel using CSMA. The mesh queues radio
fragments and submits them one at a time. The receiver reassembles bursts
per sender, and the bridge forwards Ethernet frames between LAN and radio.

## Default strategy — slotted mesh

One bridge coordinates the network and starts with an empty member list.
It broadcasts a JOIN beacon to open a 3 ms window. Unjoined nodes choose
windows at random and add up to 640 µs of jitter before sending their JOIN
request with their Ethernet MAC to the bridge. The bridge adds each new
radio address and its Ethernet MAC to the head of its member list.

After the JOIN window, the bridge grants each member a DATA turn in list
order, then opens another JOIN window. The bridge advances as soon as it
reassembles a valid uplink. If the reply is missing, the turn times out
after allowing for the downlink and a maximum-size uplink burst, including
inter-frame spacing. A member must answer every turn,
with an empty `DATA_UP` if it has nothing to send. A member that misses 3
turns in a row is removed. With no
members, the bridge opens JOIN windows back to back. A node recognizes its
first `DATA_DOWN` as
confirmation that it joined.

Slotted nodes have a separate TCP/IP interface that reports link up after
the first grant. Each mesh interface has one 2 KiB TX queue of split
radio bursts. The bridge shares that queue among members and sends the
head burst in its destination's turn. It directs DHCP replies by the
client MAC in `chaddr` and keeps one copy of other multicast and broadcast
bursts until each member has had its turn. A frame that does not fit is
rejected in full. The hardware and simulator drivers accept one raw frame
at a time. JOIN messages fit in a single radio frame. A DATA
burst carries one Ethernet frame across up to 14 radio frames. Each
fragment has a 9-byte short-address MAC header, a one-byte type, and a
byte containing the fragment index and final index in the high and low
nibbles. Empty DATA messages use a single header-only frame. The node
starts its uplink after receiving the whole downlink, and the bridge
forwards only complete uplinks to the LAN.

## Running

```sh
make                          # 100x100 m, a bridge, 11x11 nodes
make X=200 Y=50 ROWS=2 COLS=8 # Another area and grid
```

`make` puts a bridge in the middle of the area, and a node at every corner
of a `ROWS` x `COLS` grid over it. The simulator itself takes:

```
./mesh_sim -area X,Y,Z -bridge X,Y,Z -node X,Y,Z ... -t SEC -w SEC -m A|default -range METRES -loss PERCENT
```

Positions are in metres; `X,Y` means `Z` is 0. Repeat `-bridge` and `-node`
for more.

## Hardware

The FRDM-RW612 example is the Ethernet bridge. The XIAO MG24 and
FRDM-MCXW71 examples are radio clients. All three use the slotted mesh.

```sh
make -C tutorials/nxp/frdm-rw612/bridge
make -C tutorials/silabs/xiao-mg24/minimal
make -C tutorials/nxp/frdm-mcxw71/minimal
```

Use each tutorial's `flash` target. Connect the
bridge to an Ethernet LAN with a DHCP server. Find the client's assigned
IP in its serial log, then send an HTTP GET to that IP from the LAN.

All three boards use channel 15 and PAN `0xabcd` by default. Each board derives its radio address from its Ethernet MAC.
MG24 is a node only; its radio address is derived from its MAC.

The drivers in `src/drivers` send and receive raw 802.15.4 frames.
They use the same slotted mesh implementation as the simulator.
The board driver is initialized by `mg_mgr_init()`. The RW612 bridge
has its own `bridge/main.c`, which initializes and polls the two drivers
directly, without an event manager or local IP stack.

## Hardware integration

The default mesh and radio drivers are included in `mongoose.c` and `mongoose.h`.
Radio drivers live in `src/drivers/`: `mg24`, `mcxw71`, `rw612w`,
and `nrf52`. MG24 driver data contains its mesh state and `strategy`; it has
no bridge LAN interface.
A NULL strategy selects `mg_mesh_default`, named `"default"`; assigning another
`const struct mg_mesh *` overrides it. Hardware callbacks transmit and receive
raw radio frames; the selected strategy owns joining, scheduling and Ethernet
fragmentation. Driver initialization goes through `mg_tcpip_init()`.

Board examples are under `tutorials/nxp/frdm-rw612/bridge`,
`tutorials/nxp/frdm-mcxw71/minimal`, `tutorials/silabs/xiao-mg24/minimal`, and
`tutorials/nordic/xiao-52840/minimal`. Their HALs supply `mg_micros()`.

The simulator and all radio drivers keep the hardware driver installed throughout the
interface lifetime. Mesh state belongs to `driver_data`. Driver `tx()` queues
Ethernet frames; `poll()` feeds raw received frames to the mesh and calls
strategy `tick()`. The strategy sends scheduled fragments through the driver's
private `radio_tx` callback. Driver `rx()` returns a reassembled Ethernet frame
using the stack's existing receive buffer. There is no mesh polling timer,
copied radio interface, or mesh-specific change in `net_builtin.*`.
The simulator explicitly frees its mesh state after `mg_mgr_free()`; board
drivers keep their state for the firmware lifetime. MG24, MCXW71 and nRF52 are
radio nodes. The RW612 bridge application owns Ethernet initialization, its RX
queue, promiscuous mode and link polling, and attaches the LAN to the mesh.
The RW612W driver owns only the radio interface.
