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
- `struct mg_mesh` has `init`, `tick` and `free` callbacks. The most important
  one is the `tick()` callback which is called with a microsecond timestamp,
  for which we introduce `mg_micros()`, and is responsible for rouing,
  mesh management, etc. Mongoose driver `tx()` function takes a frame, splits
  it to radio frames and sends, and the `rx()` function reassembles.

The exact IO mechanism is up to a "mesh strategy" that implements `mg_mesh`.
Currently we have two strategies (see detailed section below):
  - A: implemented in `mesh_a.c`, all nodes compete for the air time
  - B: a bridge acts as a coordinator. It sends beacons when nodes can join
    the mesh, and it gives each joined node an IO time, round-robin. Kind of
    like token ring. This drives collision time to minimum, cause when a bridge
    asked a node to speak, all others keep their traps zipped.

Mongoose calls driver's `poll()`. Radio driver calls `mg_micros()` to get
the current timestamp and calls `mg_mesh :: tick()`.

The goal is to swap radios and mesh strategies without changing application
code. Both current star strategies deliberately omit radio ACKs to avoid
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
from the radio to the LAN. It may answer proxy ARP, subject to `-a 0|1` arg.

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

The simulator keeps pending and active transmissions in a singly linked
list owned by `struct simulator`. Frames from the same sender are served in
submission order; different senders contend for the shared channel. Queued
payload is limited to 64 KiB across all senders; allocation failure or a full
queue makes `radio_tx()` return zero.

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
The breakdown table's `lost` column counts frames missed by the
intended radio next hop because it was transmitting. For the current
single-bridge topology, node uplinks target the bridge; bridge downlinks
target the destination node. DHCP replies use the client MAC in `chaddr`,
and broadcast ARP requests use the target IP. Unrelated listeners are not
counted. This inference affects statistics only, not packet forwarding.
Overlaps can also count as collisions. Sensing/turnaround losses and other
broadcasts without an identifiable target are excluded from this counter.

The 802.15.4 definition uses 250 kbit/s, 8 bytes overhead, 320 us backoff
slots, 128 us channel sensing and 100 us turnaround. The initial backoff
exponent is 3 (0–7 slots), increasing to 5 (0–31 slots) on busy checks.
After transmitting, a sender waits the radio's inter-frame spacing,
`mg_radio::ifs_us`, 640 us for 802.15.4, before starting its next frame's
backoff. This wait applies even if the frame collided or the sender's
queue was temporarily empty. Reception remains enabled during the
inter-frame wait.
The backoff generator is seeded from the wall clock.

Both strategies limit each transmission to 125 bytes plus the 2-byte FCS.
Overhead and inter-frame spacing apply to each fragment.
A receiver must be within `-range METRES` of the sender (default 185 m).
`-loss PERCENT` adds independent random loss for each in-range reception
(default 0%). The radio summary counts range and random losses together.
Collisions and channel sensing still apply across the whole simulated air.

## Files

| File | What |
|------|------|
| `l2_mesh.h`, `l2_mesh.c` | `struct mg_radio`, `struct mg_mesh`, shared node initialization, and IEEE 802.15.4 radio |
| `mesh_b.c`, `mesh_a.c` | Slotted and unslotted star strategies |
| `simulator.h`, `simulator.c` | The simulator |
| `Makefile` | Builds and runs the simulator |

## Where we are

Both strategies currently use direct radio hops between nodes and a bridge.
The simulator models airtime, half-duplex reception and collisions.

- Frames outside reception range are not received, so nodes out of a bridge's
  range never join. But collisions and channel sensing are still global:
  every overlap is a collision, wherever the receiver is, and a busy
  channel is busy for everyone.
- Mesh routing remains unimplemented: bridge selection, forwarding through other
  nodes and recovery are not implemented.

Next steps include distance-aware collision and channel sensing, mesh
routing and bridge filtering.

## Mesh strategies

### `A` — unslotted (default)

Nodes attach TCP/IP to a mesh Ethernet interface. The mesh splits outgoing
Ethernet frames into broadcast radio fragments tagged by burst and
reassembles incoming fragments separately for each sender. The driver uses randomized
backoff and channel sensing before transmitting. A busy channel triggers
another backoff. Frames can collide when senders both sense idle before
either starts transmitting.

The bridge forwards Ethernet frames between LAN and radio in both directions.
Nodes can obtain DHCP addresses and answer HTTP requests. Use one bridge
for now because multiple bridges can circulate broadcasts.

### `B` — slotted

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
turns in a row is removed, with its queued downlink frames. With no
members, the bridge opens JOIN windows back to back. A node recognizes its
first `DATA_DOWN` as
confirmation that it joined.

Slotted nodes have a separate TCP/IP interface that reports link up after
the first grant. The bridge uses the Ethernet MAC stored at join time
to queue LAN frames per member. It directs DHCP replies by
the client MAC in `chaddr` and copies other multicast and broadcast
frames to all members. JOIN messages fit in a single radio frame. A DATA
burst carries one Ethernet frame across up to 14 radio frames. Each
fragment has a 9-byte short-address MAC header, a one-byte type, and a
byte containing the fragment index and final index in the high and low
nibbles. Empty DATA messages use a single header-only frame. The node
starts its uplink after receiving the whole downlink, and the bridge
forwards only complete uplinks to the LAN.

Optional proxy ARP lets the slotted bridge answer LAN ARP requests for
members whose IPv4 addresses it has learned from uplink traffic. It
replies with the member's Ethernet MAC and keeps the request off the
radio. Requests for unknown addresses still cross the radio.

## Running

```sh
make                          # 100x100 m, a bridge, 11x11 nodes
make X=200 Y=50 ROWS=2 COLS=8 # Another area and grid
```

`make` puts a bridge in the middle of the area, and a node at every corner
of a `ROWS` x `COLS` grid over it. The simulator itself takes:

```
./mesh_sim -area X,Y,Z -bridge X,Y,Z -node X,Y,Z ... -t SEC -w SEC -m A|B -a 0|1 -range METRES -loss PERCENT
```

Select a mesh strategy by name with `-m A|B`.

```sh
make ARGS="-m A"
make ARGS="-m B"
make ARGS="-m B -a 1"  # Enable proxy ARP (default: -a 0)
```

Positions are in metres; `X,Y` means `Z` is 0. Repeat `-bridge` and `-node`
for more.
