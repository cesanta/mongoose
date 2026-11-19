# UDP CPQ Patch Assessment

## Scope

This assesses the CPQ patch applied to `master`, with only this receive-side
change:

```c
if (c->is_bsd && c->recv.len > 0) return true;
```

It deliberately does not require the bsdudp TX framing, peer filtering, or
neighbor-cache work. The rule keeps the first unread nonempty datagram and
discards later ones.

## What The CPQ Patch Provides

When two nonempty UDP datagrams arrive before a BSD application calls
`recv()` or `recvfrom()`, the first remains in `c->recv` and later packets are
discarded. Therefore the first payload is no longer concatenated with the
second one, and its source address remains in `c->rem` until it is read.

That is a valid deliberately lossy receive policy. It only addresses this one
case. The rest of the odd/incorrect behaviors found when initially writing code in the bsdudp branch are detailed in subsequent paragraphs. The rest of the missing functionality also observed before is NOT described here, only what is claimed to be functional is evaluated; these are somehow enumerated at the end of this document, just before the appendixes

See [B.1](#b1-cpq-unread-datagram-policy) for the test evidence.

## Rate Limit Of The One-Datagram Policy

The retained datagram is a one-slot loss queue: while the BSD application has
not called `recv()` or `recvfrom()`, every later nonempty datagram is dropped.
If the application needs 4 ms to read it, independent traffic has these
approximate collision probabilities:

| Aggregate UDP rate | Probability of another datagram within 4 ms |
| ---: | ---: |
| 25 packets/s | 9.5% |
| 50 packets/s | 18.1% |
| 100 packets/s | 33.0% |
| 250 packets/s | 63.2% |
| 1000 packets/s | 98.2% |

This is basic queueing theory under an independent-arrivals assumption. See
[Appendix A](#appendix-a-one-slot-queue-model) for the model and limits of the
estimate.

## Baseline Failures

`bsd_udp_cpq_fail.c` and `bsd_udp_cpq_peer.c` print the wire-level effects for
short receives, empty datagrams, connected peers, and `sendto()`.

The target runs Mongoose's built-in TCP/IP stack. A bridge connects its packet
interface to the LAN because this path cannot use BSD sockets: BSD is the API
being tested.

The default runner is self-contained for CI: it starts the peer on the same
host (Workstation) and exercises every failure except a mismatched Ethernet destination. To
show that L2 failure, run the primary peer on a second machine (Laptop):

```text
Laptop:      ./bsd_udp_cpq_peer --remote <Mongoose-IP>
Workstation: CPQ_REMOTE_PEER=<Laptop-IP> ./bsd_cpq_test
```

Remote mode starts a Workstation helper automatically. The Laptop and helper
have distinct MAC addresses. The helper first redirects a connected-socket
reply, then populates the unconnected `sendto()` socket with its MAC. The
following `sendto()` names the Laptop IP, but `TRACE_UDP=1` shows the helper's
Ethernet destination. The evidence below was collected with Workstation IP
forwarding disabled, so the Laptop cannot receive a frame addressed to the
helper MAC.

### Short `recvfrom()` turns one datagram into two reads

The CPQ code removes only the copied byte count from `c->recv`. Reading two
bytes of `ABCDE` leaves `CDE` in the buffer, so the next `recvfrom()` returns
it as if it were a new UDP datagram. UDP must discard the unread tail of a
datagram after a non-peek short receive. Dropping later packets does not repair
this: the tail is already in the retained packet.

See [B.2](#b2-short-recvfrom) for the test evidence.

### A connected UDP socket accepts traffic from any sender

`rx_udp()` overwrites `c->rem` and the return L2 address for every accepted
packet. It does not check that an incoming peer matches the peer selected by
`connect()`. An unrelated sender can therefore place data in a connected
socket and replace its reply destination. The demonstrator sends `intruder`
from a third peer: the connected receive succeeds when it should return
`EAGAIN`, and the subsequent `send()` goes to the intruder rather than the
connected peer.

See [B.3](#b3-connected-udp-accepts-another-peer) for the test evidence.

### Unconnected `send()` reports success

`bsd_dispatch(BSD_OP_SEND)` passes UDP bytes to `mg_send()` without checking
whether the socket has a peer. A real UDP socket has nowhere to send in this
state and reports `EDESTADDRREQ`. Here, the API reports that the datagram was
accepted even though it has no valid destination.

See [B.4](#b4-unconnected-send) for the test evidence.

### `sendto()` uses one stale L2 destination

`BSD_OP_SENDTO` changes the destination IP and UDP port in `c->rem`, but calls
`mg_send()` with the one L2 MAC currently stored for the socket. That MAC was
learned when a packet arrived, or while a UDP `connect()` resolved its peer;
there is no resolution or MAC lookup for each `sendto()` destination. An
application using `sendto()` for several peers therefore has one usable L2
destination, not one per peer.

- A single-destination application can appear to work: its only peer is the
  one that caused the MAC to be learned. This is likely the tested case.
- Sending to another local-LAN host changes the IP address but keeps that first
  host's MAC. The Ethernet frame reaches the first host, which drops it
  because its IP destination names the second host. `sendto()` still reports
  success.
- Several off-link destinations can also appear to work if the stored MAC is
  the common router MAC. The router is the correct L2 next hop for all of
  them. This does not make the socket generally correct.
- A socket that has neither received a datagram nor been connected has no
  usable destination MAC. Its first proactive `sendto()` reports success but
  cannot be delivered correctly.

Receiving an unrelated datagram can also break a destination that was working.
For example, A sends to this UDP port, so the socket stores A's MAC and a
later `sendto(A)` works. B then sends any datagram to the same port.
`rx_udp()` replaces the stored MAC with B's. The next `sendto(A)` still names
A in the IP header, but addresses the Ethernet frame to B; A no longer gets
it. The same overwrite occurs after UDP `connect()`, because the CPQ code does
not filter non-peer senders. Remote traffic through the same router can hide
this again because its Ethernet source MAC remains the router's MAC.

The two-host remote test covers both cases: [B.5](#b5-first-unconnected-sendto)
shows the first `sendto()` without an L2 address, and
[B.6](#b6-sendto-corrupts-later-sends-and-uses-a-stale-mac) shows a Laptop IP
sent with the helper MAC. Two sockets on one Linux host are not sufficient for
the latter because they share one NIC MAC. The bridge prints each outgoing UDP
IP destination beside its Ethernet destination MAC, independently confirming
the wrong L2 destination.

(In branch bsdudp, we fixed this by using a framing transporting TX records between the BSD side and the Mongoose side; a correct `sendto()` must wait
for ARP resolution without losing that destination, so those records/frames are queued at the send buffer and the extraction then can decide whether to start an ARP, thing that is minimized with a small cache-like table holding 4 entries)

See [B.5](#b5-first-unconnected-sendto) and
[B.6](#b6-sendto-corrupts-later-sends-and-uses-a-stale-mac) for the test
evidence.

### `sendto()` changes the meaning of later `send()` calls

The same direct update makes an unconnected socket look connected after a
`sendto()`. A later `send()` succeeds and uses the previous `sendto()` target,
although BSD requires `EDESTADDRREQ` until `connect()` selects a peer. It is
state corruption from the application's point of view: an operation intended
to name one datagram's destination changes the socket's default destination.

See [B.6](#b6-sendto-corrupts-later-sends-and-uses-a-stale-mac) for the test
evidence.

### `bind()` followed by UDP `connect()` loses the bound port

`mg_connect_resolved()` assigns an ephemeral local port unconditionally.
Calling `connect()` on a UDP socket already bound to a specific port replaces
that port. This is independent of receive buffering; the local identity used
by replies changes after `connect()`.

See [B.7](#b7-bind-followed-by-udp-connect) for the test evidence.

### `sendto()` on a connected UDP socket reports success

A connected UDP socket already has a selected peer. `sendto()` must fail with
`EISCONN`, but the CPQ implementation accepts it and changes that peer.

See [B.8](#b8-sendto-on-a-connected-udp-socket) for the test evidence.

### Oversized UDP writes report success after truncation

The CPQ code passes an oversized payload through `mg_send()`. The built-in UDP
path trims to its payload capacity and transmits only that prefix. BSD callers
are told that the whole datagram was sent. A UDP datagram must be sent whole
or fail; partial transmission is not meaningful.

See [B.9](#b9-oversized-udp-write) for the test evidence.

### Empty datagrams disappear

An empty UDP datagram does not increase `c->recv.len`. No later `recv()` can
distinguish it from no packet, so it returns `EAGAIN` or blocks instead of
returning zero. The proposed condition is also false for an empty retained
datagram, so it cannot represent or protect one.

Yes, it has been agreed that is a known and accepted behavior.

For completeness, see [B.10](#b10-empty-datagram) for the test evidence.

## bsdudp Branch Fixes and Additions

These are additional capabilities, not prerequisites for the one-datagram
drop policy above:

- RX drops the complete retained datagram after a short non-peek read.
- RX filters packets from non-peer senders on connected UDP sockets.
- TX records carry each UDP destination, so queued `sendto()` calls retain
  their individual targets.
- TX triggers and reuses L2 resolution for `sendto()` destinations.
- Unconnected `send()` returns `EDESTADDRREQ`; connected UDP `sendto()`
  returns `EISCONN`.
- `bind()` followed by UDP `connect()` keeps the selected local port.
- BSD writes reject oversized datagrams with `EMSGSIZE` before queuing them.

Empty UDP receives still require an explicit marker or framing record and are
not solved by either the CPQ patch or the current bsdudp simplification.

## Appendix A: One-Slot Queue Model

Let `D` be the time from receiving a datagram until the BSD application reads
it. Let `N` peers each send at `r` packets/s, so the aggregate arrival rate is
`lambda = N * r`. For independent arrivals approximated as a Poisson process,
the probability that at least one later datagram arrives during the unread
window is:

```text
P(collision) = 1 - exp(-lambda * D)
```

The table above uses `D = 4 ms`. It is a collision probability, not a precise
packet-loss percentage: the latter also depends on the scheduler and on when
the application calls `recv()`.

For evenly spaced periodic traffic, the aggregate packet spacing must be at
least `D` to avoid drops. Therefore each of `N` equal-rate peers needs a period
of at least `N * D`: with `D = 4 ms`, 10 peers need at least 40 ms per packet
per peer, while 100 peers need at least 400 ms.

## Appendix B: Two-Host Test Evidence

This run used Mongoose at `192.168.69.231`, the Laptop at
`192.168.69.73`, and a Workstation helper at `192.168.69.11`. The Laptop MAC
learned for normal control traffic was `3c:0a:f3:8c:ce:ef`; the helper MAC was
`06:3f:ad:7e:3f:1c`. Startup and
DHCP are omitted below.

### B.1 CPQ unread-datagram policy

**Test.** Two peers send `one` and `two` before the application reads port
`1234`.

**Expected.** The CPQ rule retains `one` and drops `two`.

**Observed.**

```text
Workstation: PASS drop: kept one, dropped two
Laptop:      no separate observation
Bridge:      no separate observation
```

**Internal reason.** `rx_udp()` returns once a BSD connection has a nonempty
`c->recv`. It is a one-datagram loss queue, not a UDP receive queue.

### B.2 Short `recvfrom()`

**Test.** A peer sends one five-byte datagram, `ABCDE`. The application first
reads two bytes, then reads again.

**Expected.** The first call returns `AB`; UDP discards the unread `CDE` tail.
The second call returns `EAGAIN` until another datagram arrives.

**Observed.**

```text
Workstation: FAIL short: expected EAGAIN, got 3 "CDE": tail retained
Laptop:      sent ABCDE
Bridge:      no separate observation
```

**Internal reason.** `BSD_OP_RECVFROM` removes only the copied byte count from
the stream-shaped `c->recv` iobuf. It has no record length to discard the rest
of the datagram.

### B.3 Connected UDP accepts another peer

**Test.** The socket connects to the Laptop. The Workstation helper then sends
`intruder` to that connected socket.

**Expected.** The helper datagram is ignored. A following `send()` still goes
to the Laptop.

**Observed.**

```text
Workstation: FAIL peer: expected EAGAIN, got 8 "intruder": no peer filter
Mongoose:    mg_connect_ 7 192.168.69.231:64103 -> 192.168.69.73:14000
Mongoose:    mg_connect_ 7 ARP lookup...
Mongoose:    rx_arp 7 ARP resolved 192.168.69.73 -> 3c:0a:f3:8c:ce:ef
Workstation: reply: sent 5 bytes; peer reports its destination below
Laptop:      FAIL reply: expected control, got none: intruder replaced peer
Helper:      FAIL connected peer: helper received reply
Bridge:      UDP IP dst 192.168.69.11 -> Ethernet dst 06:3f:ad:7e:3f:1c
```

**Internal reason.** `mg_connect_resolved()` correctly starts an ARP lookup for
the configured Laptop peer and stores its MAC. `rx_udp()` then accepts every
sender matching the local port. Before the BSD read runs, it overwrites
`c->rem` and the connection return MAC with the helper's source. The later
`send()` therefore replies to the helper instead of the peer that was resolved
at `connect()` time.

### B.4 Unconnected `send()`

**Test.** Send one byte on a bound UDP socket that has never called
`connect()` or `sendto()`.

**Expected.** `send()` fails with `EDESTADDRREQ`: there is no peer.

**Observed.**

```text
Workstation: FAIL send: expected EDESTADDRREQ, got 1: no peer check
Laptop:      no packet expected
Bridge:      UDP IP dst 0.0.0.0 -> Ethernet dst 00:00:00:00:00:00
```

**Internal reason.** `BSD_OP_SEND` calls `mg_send()` without checking whether
the UDP socket has a selected peer. The zeroed remote address reaches the L2
transmit path.

### B.5 First unconnected `sendto()`

**Test.** Call `sendto()` to the Laptop before this socket has received any
packet or connected to a peer.

**Expected.** The implementation resolves the Laptop L2 address, waits if
needed, then delivers the datagram.

**Observed.**

```text
Workstation: FAIL sendto: expected delivery, got 4: no resolution
Laptop:      FAIL sendto: expected packet, got none
Bridge:      UDP IP dst 192.168.69.73 -> Ethernet dst 00:00:00:00:00:00
```

**Internal reason.** `BSD_OP_SENDTO` replaces only `c->rem` and immediately
calls `mg_send()`. Unlike the preceding `connect()` cases, this operation
produces no `mg_connect_ ... ARP lookup...` or `rx_arp ... ARP resolved ...`
log for the Laptop destination. It neither performs ARP/L2 resolution nor
retains the destination while resolution would be pending. The zero Ethernet
destination in the bridge trace is the direct result.

### B.6 `sendto()` corrupts later sends and uses a stale MAC

**Test.** The helper sends a datagram to the unconnected socket, establishing
the helper MAC. The application then calls `sendto()` to the Laptop and, after
that, calls `send()` on the same socket.

**Expected.** `sendto()` sends to the Laptop MAC without changing the socket's
default peer. The later `send()` fails with `EDESTADDRREQ`.

**Observed.**

```text
Workstation: sendto after RX: got 5
Workstation: FAIL send: expected EDESTADDRREQ, got 4: sendto set peer
Laptop:      FAIL stale MAC: expected packet, got none
Bridge:      UDP IP dst 192.168.69.73 -> Ethernet dst 3c:0a:f3:8c:ce:ef
Bridge:      UDP IP dst 192.168.69.73 -> Ethernet dst 06:3f:ad:7e:3f:1c
```

The first bridge line is normal Laptop control traffic. The second names the
same Laptop IP but uses the helper MAC. The Laptop receives no packet.

**Internal reason.** The implementation keeps only one mutable `c->rem` and
one return MAC per Mongoose UDP connection. Receiving the helper datagram
updates that one MAC to `06:3f:ad:7e:3f:1c`. `sendto()` overwrites `c->rem`,
but does not start an ARP lookup for the Laptop and therefore retains the
helper MAC. There is no `mg_connect_ ... ARP lookup...` or `rx_arp ... ARP
resolved ...` sequence between this `sendto()` and the second bridge line. No
per-datagram destination record or neighbor cache exists.

### B.7 `bind()` followed by UDP `connect()`

**Test.** Bind local port `1238`, then connect to the Laptop and query the
local address.

**Expected.** The socket remains bound to `1238`.

**Observed.**

```text
Workstation: FAIL bind: expected 1238, got 64102: connect replaced bind
Mongoose:    mg_connect_ 5 192.168.69.231:64102 -> 192.168.69.73:14000
Mongoose:    mg_connect_ 5 ARP lookup...
Mongoose:    rx_arp 5 ARP resolved 192.168.69.73 -> 3c:0a:f3:8c:ce:ef
Laptop:      normal ARP/control peer
```

**Internal reason.** `mg_connect_resolved()` assigns an ephemeral local port
without preserving the BSD socket's existing bind state.

### B.8 `sendto()` on a connected UDP socket

**Test.** After UDP `connect()`, call `sendto()` with a different destination.

**Expected.** Fail with `EISCONN`; a connected UDP socket uses `send()`.

**Observed.**

```text
Workstation: FAIL sendto: expected EISCONN, got 1: connected socket accepted it
Laptop:      no separate observation
Bridge:      no separate observation
```

**Internal reason.** `BSD_OP_SENDTO` has no connected-UDP state check and
blindly replaces the peer address. The connected socket's ARP lookup happened
at `connect()` time in B.3; this operation does not validate that state or
start resolution for its replacement destination.

### B.9 Oversized UDP write

**Test.** Send a 1600-byte datagram.

**Expected.** Fail with `EMSGSIZE`; UDP cannot partially send one datagram.

**Observed.**

```text
Workstation: FAIL oversize: expected EMSGSIZE, got 1600: later truncated
Laptop:      no receiving socket in this run
Bridge:      no payload-length assertion in this run
```

**Internal reason.** The BSD layer does not reject an over-capacity datagram.
The built-in UDP transmit path later limits payload to its available capacity,
while the BSD caller has already been told all 1600 bytes were accepted.

This run proves the false-success API result. A receiver-side payload-length
assertion would additionally demonstrate the transmitted prefix length.

### B.10 Empty datagram

**Test.** Send a zero-byte UDP datagram, then call `recvfrom()`.

**Expected.** `recvfrom()` returns zero and preserves the source address.

**Observed.**

```text
Workstation: FAIL empty: expected 0, got -1 errno 11: no marker
Laptop:      sent a zero-byte datagram
Bridge:      no separate observation
```

**Internal reason.** An empty payload does not increase `c->recv.len`; the
stream iobuf cannot distinguish an empty datagram from no pending data.
