# Ethernet over a radio mesh

## The idea

Small radio devices, e.g. IEEE 802.15.4, sub-GHz or LoRa boards, should run
Mongoose as if they were on an ordinary Ethernet LAN: get an IP address over
DHCP, answer HTTP, talk MQTT. No 6LoWPAN, no Thread, no special stack.

To do that, the radio interface looks like Ethernet to Mongoose
(`MG_TCPIP_L2_ETH`). Its driver splits each Ethernet frame into radio
frames, sends them over the air, and the receiver puts them back together.
One or more bridges connect the radio network to the wired LAN: a bridge has
an Ethernet port and a radio. A node in range of a bridge talks to it
directly; a node out of range reaches a bridge through other nodes, which
forward its frames. That makes the radio network a mesh. With several
bridges, each node uses one of them, e.g. the nearest; the mesh covers a
bigger area, and keeps working if a bridge goes down. A PC on the LAN sees
the radio nodes as regular hosts on the same LAN.

```
             +---- bridge 1 ~~~ node ~~~ node
             |                    ~
  PC ---- LAN                    node
             |                    ~
             +---- bridge 2 ~~~ node ~~~ node ~~~ node
```

`~~~` and `~` are radio hops. Nodes far from a bridge send their frames
through nearer nodes. A node in the middle can reach either bridge.

## Two parts plug in

- `struct mg_radio`, a radio standard: how big a radio frame is, how
  addresses are written into it. `mg_radio_802154` is the first one.
- `struct mg_mesh`, a mesh strategy: how nodes find a bridge, and where each
  frame goes next. `mg_mesh_star` is the first and simplest one: every node
  talks to a bridge directly, one hop.

Everything else, splitting and reassembling frames, sending again when a
frame is lost, is common code that works the same for every radio and every
mesh. The goal is to swap radios and mesh strategies without touching
anything else.

## Why a simulator

Real boards are slow to test with, and you can't put 500 of them on a desk.
The simulator runs a whole network in one process: every node is a real
Mongoose instance, only the radio and the air between nodes are emulated.
That lets us try mesh strategies with many nodes, and measure how long it
takes nodes to join, get an address, and answer a request.

## Virtual time

The simulator has its own clock, `simulator::now`, in microseconds, and
Mongoose reads it through `mg_millis()`. Nothing waits for real time: the
simulator moves the clock forward itself, 250 us per step, and polls every
station once per step. A minute of network traffic takes about a second to
simulate. `-t SEC` sets how long to run, 10 minutes by default.

The step is a trade-off. Each step polls every station's Mongoose, and even
an idle poll costs something, so a smaller step means a slower run. A
bigger step makes frames wait longer in queues, see below. Measured with
121 nodes and 10 simulated minutes:

| Step | Request time, mean | Run time |
|------|--------------------|----------|
| 1 ms | 6.1 ms | 0.9 s |
| 250 us | 1.5 ms | 3.0 s |
| 100 us | 0.6 ms | 7.2 s |
| 10 us | 0.06 ms | 72 s |

## The setup

```
  DHCP server ---------+
   10.0.0.1            |
   02:01:00:00:00:00   +-- LAN ---- bridge ~~~ nodes
                       |
  test station --------+
   10.0.0.2
   02:02:00:00:00:00
```

The LAN is assumed infinitely fast, see "How frames move on the LAN" below.
Two stations are on it:

- The DHCP server, 10.0.0.1, MAC 02:01:00:00:00:00, gives each node an
  address made from its MAC: 02:00:00:X:Y:Z gets 10.X+1.Y.Z, so node
  addresses never clash with the LAN stations' 10.0.0.x.
- The test station, 10.0.0.2, MAC 02:02:00:00:00:00, is a Mongoose
  instance. It waits for the mesh to settle, 60 seconds by default
  (`-w SEC`), then sends `GET /` to each node in turn, one at a time, at the
  node's own IP address. A node with no address, or one that doesn't answer
  within 10 seconds, counts as failed. At the end it prints how many
  requests succeeded and how long they took.

Bridges are on the LAN too, and get every frame, as if in promiscuous mode:
a frame for one of their radio nodes isn't addressed to the bridge itself.
A bridge forwards every frame it gets from the LAN to the radio, and every
frame from the radio to the LAN, with no filtering yet.

Bridges and radio nodes get MACs 02:00:00:xx:xx:xx, numbered from 1 in
command-line order: 02:00:00:00:00:01, 02:00:00:00:00:02, and so on.

### How frames move on the LAN

Every LAN station, the DHCP server, the test station and each bridge, has a
LAN interface, `lif`: a `struct mg_tcpip_if` with a receive queue. All of
them use the same driver, `s_lan_driver`. There are no cables and no switch:

- **Sending (tx).** A station sends a frame with its driver's `tx`, which is
  `lan_tx()`. It looks at the destination MAC and copies the frame into the
  receive queues of the stations it is for:
  - a broadcast or multicast goes to every station but the sender;
  - a unicast goes to the station with that MAC;
  - bridges get every frame, as a real bridge in promiscuous mode would:
    a frame for a radio node is addressed to the node, not to its bridge.

  That's all `tx` does. Nobody's code runs on the receiving side yet.
- **Receiving (rx).** Each station takes the frames from its own queue when
  the main loop polls it:
  - the test station runs Mongoose, which reads the queue by itself, as it
    would with a real Ethernet driver that queues frames from an interrupt;
  - the DHCP server reads it in `dhcp_poll()`, and replies by sending
    through its own `lif`;
  - bridges read it in `bridges_poll()`, and forward it to the radio.

So a frame waits in a queue until its receiver's next poll, at most one
step of virtual time, 250 us. Ethernet is hundreds of times faster than the radio, so that
doesn't matter for what we measure. And since sending never calls the
receiver's code, a reply is never sent from inside another station's send.

For example, a node asking for an address:

1. The node's DHCP DISCOVER crosses the radio to a bridge, which sends it
   out of its `lif`. It is a broadcast, so `lan_tx()` puts it into the
   queues of the DHCP server, the test station and the other bridges.
2. On its next poll, the DHCP server reads it and sends an OFFER, also a
   broadcast, which lands in the test station's and the bridges' queues.
3. On their next poll, the bridges read the OFFER and send it over the
   radio to the node. The test station's Mongoose drops it: the test station
   has a fixed address and no DHCP client.

## Files

| File | What |
|------|------|
| `l2_mesh.h`, `l2_mesh.c` | `struct mg_radio`, `struct mg_mesh`, common code |
| `l2_802154.h`, `l2_802154.c` | `mg_radio_802154`, IEEE 802.15.4 |
| `star.h`, `star.c` | `mg_mesh_star`, the star mesh |
| `simulator.h`, `simulator.c` | The simulator |
| `Makefile` | Builds and runs the simulator |

## Where we are

It works end to end: nodes get addresses over DHCP, and the test station's
requests reach them through the bridges. But the radio is ideal for now:

- The radio driver works like the LAN's: it puts whole Ethernet frames
  straight into receive queues, all of them for a broadcast, else the one
  with the destination MAC, and bridges get every frame. No range, no
  loss, no airtime.
- Bridges forward everything, with no filtering.
- So use one bridge for now. With two, a broadcast from the LAN goes to the
  radio through both, each hears the other's copy and sends it back to the
  LAN, and that never ends. Deciding who forwards what, without loops, is
  the job of `mg_mesh`.

`mg_radio` and `mg_mesh` are still stubs. Next steps, one at a time: the
air (range, loss, airtime), fragmenting frames into radio frames with
`mg_radio`, choosing a bridge and forwarding through nodes with `mg_mesh`,
and filtering at the bridges.

## Running

```sh
make                          # 100x100 m, a bridge, 11x11 nodes
make X=200 Y=50 ROWS=2 COLS=8 # Another area and grid
```

`make` puts a bridge in the middle of the area, and a node at every corner
of a `ROWS` x `COLS` grid over it. The simulator itself takes:

```
./mesh_sim -area X,Y,Z -bridge X,Y,Z -node X,Y,Z ... -t SEC -w SEC
```

Positions are in metres; `X,Y` means `Z` is 0. Repeat `-bridge` and `-node`
for more.
