#MIP TCP / IP stack over pcap

This example allows to use Mongoose's MIP built-in TCP/IP stack on systems
that support pcap API, i.e. have libpcap library installed. The application
opens an interface and uses `pcap_next_ex()` for reading packets from the
interface, and `pcap_inject()` to write packets to the interface.

NOTE: depending on the libcap implementation, the injected packets may
or may not be looped back to the interface's TCP/IP stack. If they are
not looped back, then it is necessary to create a separate interface that
is bridged to the target interface - see MacOS example below.

## MacOS setup

MacOS has support for `feth` (fake ethernet) interfaces. One can create a pair
of `feth` interfaces and interlink them. Once a `feth` interface is assigned
a peer and an IP address, anything that gets injected to it, appears on a
peer interface and vice versa.

```sh
$ sudo ifconfig feth0 create
$ sudo ifconfig feth1 create
$ sudo ifconfig feth1 peer feth 0     # Link two fake ethernet ifaces together
$ sudo ifconfig feth1 10.10           # Assign 10.0.0.10 to feth1
$ sudo ifconfig feth0 up
$ sudo ifconfig feth1 up
```

Now we have two Ethernet interfaces, `feth0` and `feth1`, interlinked and
active.  On your Mac, go to "System Preferences" / Sharing, enable "Internet
Sharing" and choose "Thunderbolt bridge". This enables DHCP on the `bridge0`
interface, which is bridge for all Thunderbolt devices, and adds necessary
routes to the WiFi interface. On my system it gets `192.168.2.1` IP address,
and serves `192.168.2/24` net.  We should add one of our fake interfaces to
this bridge:

```sh
$ sudo ifconfig bridge0 addm feth1
```

We cat start an example using the `feth0`:

```sh
$ make -C examples/mip-pcap/ clean all ARGS="-i feth0" 
...
2386718 3 mip.c:279:arp_cache_add       ARP cache: added 0xc0a80201 @ 36:77:4d:be:e0:80
2386718 2 mip.c:300:onstatechange       READY, IP: 192.168.2.17
```

The diagram of the setup is below:

```
                                                    ┼

        ┌──────────►   192.168.2.1     bridge0
        │ routing      DHCP               │
        │              ┌──────────────────┴──────────────────┐
        ▼              │                                     │       192.168.2.17
                       │              ┌─────────┐ fake ethernet pair ┌─────────┐
  192.168.0.10         │              │  feth1  ├────────────┬───────┤  feth0  │
  ┌─────────┐          │              └─────────┘            │       └────┬────┘
  │   en0   │          │                                     │            │
  └────┬────┘          │ ┌─────────┐ ┌─────────┐ ┌─────────┐ │            │
       │               │ │   en1   │ │   en2   │ │   en3   │ │         mip-pcap
                       │ └────┬────┘ └────┬────┘ └────┬────┘ │
      WiFi             │      │           │           │      │
                       └──────┼───────────┼───────────┼──────┘
       │                      │           │           │
       ▼
   Internets                      Thunderbolt Ethernet
```
