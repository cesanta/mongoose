# Mongoose TCP/IP stack over pcap

This example allows to use Mongoose's built-in TCP/IP stack on systems
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
$ sudo ifconfig feth1 peer feth0     # Link two fake ethernet ifaces together
$ sudo ifconfig feth1 10.10          # Assign 10.0.0.10 to feth1
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

We can start an example using the `feth0`:

```sh
$ make -C examples/mip-pcap/ clean all ARGS="-i feth0" 
...
2386718 3 mip.c:279:arp_cache_add       ARP cache: added 0xc0a80201 @ 36:77:4d:be:e0:80
2386718 2 mip.c:300:onstatechange       READY, IP: 192.168.2.17
```

The diagram of the setup is below:

```
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
  Internet                     Thunderbolt Ethernet
```

## Linux setup

The quick and simple options are:

- Open your loopback device (probably `lo`), in which case Mongoose is only reachable from your machine, and it also sees echoes of its own traffic.
- Open your Ethernet/WiFi device (probably `eth0` or `enpXsY` for Ethernet), in which case Mongoose can access the network and the Internet but is not reachable from within your machine
- If you happen to have a virtual bridge interface (for linking several virtual machines together, for example) (probably `br0` or `virbr0`), you can open it, but it will behave as opening the underlying physical interface.

For best results, you should create a virtual interface, which you can use as is, attach to a virtual bridge, or forward/masquerade its traffic.

```sh
$ sudo ip link add my0 type veth peer [name] my1
$ sudo ip link set my0 up
$ sudo ip link set my1 up
```

In some systems, like for example RHEL7, _name_ is mandatory; in others you might need not to add it, that is why we represented it here using square brackets.

### Use a virtual interface

This is similar to using the loopback device, Mongoose is only reachable from the hosting machine, but you can add forwarding and masquerading later. Once you have your virtual interface up and running with an IP address, you can configure any services on one end, let's say `my1`, while you open the other end, `my0` with Mongoose.

- Add an IP address to one end of the pipe
  ```sh
  $ sudo ip addr add 192.168.0.1/24 dev my1
  ```
- Start your DHCP server at that interface serving that subnet, or otherwise configure Mongoose to use a fixed address within that subnet.
- Now start the example opening the other end of the virtual interface:
  ```sh
  $ make -C examples/mip-pcap/ clean all ARGS="-i my0" 
  ```
  - In some systems, you will probably need superuser privileges to open the device:
    ```sh
    $ sudo examples/mip-pcap/example -i my0
    ```

```
  [DHCP server for 192.168.0.x]
                 192.168.0.1                    192.168.0.x
                 ┌─────────┐  virtual eth pair  ┌─────────┐
                 │   my1   ├────────────────────┤   my0   │
                 └─────────┘                    └────┬────┘
                                                     │
                                                     │
                                                  mip-pcap
```

As you can't access any other host than your workstation, you need to add any required services (as DNS) there, and configure Mongoose appropriately. This is a foundation, you can expand it by choosing one of the solutions that follow.

### Bridge to your network

We will attach one end of the virtual interface to a bridge, which will also be attached to your network interface. In this case, Mongoose will have access to your network (and through it, to the Internet) and will also be reachable from your own workstation and other hosts in your network. You don't need to add IP addresses as in the example above, unless you don't have a DHCP server in your network, in which case you will configure Mongoose for a fixed address in your subnet.

- If you don't already have a virtual bridge interface, as mentioned above, you'll have to create it and attach your network interface (NIC) to it. Your IP address has to be assigned to the bridge, instead of the NIC. If you are using DHCP, the client must run on the bridge interface instead of the NIC.
  ```sh
  $ ip link add virbr0 type bridge
  $ sudo ip link set virbr0 up
  $ sudo ip addr del 10.1.0.10/24 dev enp9s0
  $ sudo ip link set enp9s0 master virbr0
  $ sudo ip addr add 10.1.0.10/24 dev virbr0
  ```
  Check using `ifconfig`, and try to ping some host in your network
- Attach one end of the virtual interface to the bridge:
  ```sh
  $ sudo ip link set my1 master virbr0
  ```
- If everything is fine, you'll see your interface pair state is UP:
  ```sh
  $ ip link show master virbr0
  2: enp9s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast master virbr0 state UP mode DEFAULT group default qlen 1000
    link/ether 30:5a:3a:08:db:90 brd ff:ff:ff:ff:ff:ff
  396: my1@my0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue master virbr0 state UP mode DEFAULT group default qlen 1000
    link/ether b6:ee:bc:53:f2:6e brd ff:ff:ff:ff:ff:ff
  ```
- Now start the example opening the other end of the virtual interface:
  ```sh
  $ make -C examples/mip-pcap/ clean all ARGS="-i my0" 
  ```
  - In some systems, you will probably need superuser privileges to open the device:
    ```sh
    $ sudo examples/mip-pcap/example -i my0
    ```

```
                                   10.1.0.10
                                    virbr0
                                        │
                     ┌──────────────────┴──────────────────┐
                     │                                     │         10.1.0.x
                     │              ┌─────────┐  virtual eth pair  ┌─────────┐
                     │              │   my1   ├────────────┬───────┤   my0   │
                     │              └─────────┘            │       └────┬────┘
                     │                                     │            │
                     │ ┌─────────┐ ┌─────────┐ ┌─────────┐ │            │
                     │ │  vnet1  │ │ enp9s0  │ │  vnet2  │ │         mip-pcap
                     │ └────┬────┘ └────┬────┘ └────┬────┘ │
                     │      │           │           │      │
                     └──────┼───────────┼───────────┼──────┘
                            │           │           │
                                        │
                                     Ethernet [WiFi]     
                                        │    Local LAN 10.1.0.x
                                        │    DHCP server, router
                                        ▼	
                                     Internet
```

If you have _Docker_ running, it may introduce firewall rules that will disrupt your bridging.

### Forward/Masquerade

Once you have your virtual interface up and running with an IP address, you can use your Linux to NAT and forward Mongoose traffic to another interface, for example the one that connects you to the Internet. In this case, Mongoose will have access to the Internet and will also be reachable from your own workstation; but not from any other hosts in your local network (if there is one). You can use this setup with a direct connection to the Internet or being part of a network, when you want to isolate Mongoose from the rest of your network.

- We assume you already have a firewall in place (you should); to configure masquerading and forwarding do:
  ```sh
  $ sudo iptables -t nat -A POSTROUTING -o enp9s0 -j MASQUERADE
  $ echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward
  ```
  Remember to use the proper network interface name
- Now start the example opening the other end of the virtual interface:
  ```sh
  $ make -C examples/mip-pcap/ clean all ARGS="-i my0" 
  ```
  - In some systems, you will probably need superuser privileges to open the device:
    ```sh
    $ sudo examples/mip-pcap/example -i my0
    ```

```
       [DHCP server for 192.168.0.x]
      ┌────────────►  192.168.0.1                    192.168.0.x
      │ forwarding    ┌─────────┐  virtual eth pair  ┌─────────┐
      │ masquerading  │   my1   ├────────────────────┤   my0   │
      │               └─────────┘                    └────┬────┘
      ▼                                                   │
   Your IP (LAN / Public)                                 │
 ┌─────────┐                                           mip-pcap
 │ enp9s0  │ 
 └────┬────┘       
      │ 
      │ 
   Ethernet
      │ 
      ▼
   Internet
```

You can also use this setup if your NIC is part of a bridge (for example: you have virtual machines); in that case:
- use the bridge interface for masquerading, instead of the Ethernet (your IP now belongs to the virtual bridge, not the NIC):
  ```sh
  $ sudo iptables -t nat -A POSTROUTING -o virbr0 -j MASQUERADE
  $ echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward
  ```
  Remember to use the proper bridge interface name

```
                                      [DHCP server for 192.168.0.x]
                      10.1.0.10    ◀──────────────►  192.168.0.1                    192.168.0.x
                       virbr0         forwarding     ┌─────────┐  virtual eth pair  ┌─────────┐
                          │          masquerading    │   my1   ├────────────────────┤   my0   │
                          │                          └─────────┘                    └────┬────┘
       ┌──────────────────┴──────────────────┐	                                         │
       │                                     │                                           │
       │ ┌─────────┐ ┌─────────┐ ┌─────────┐ │                                        mip-pcap
       │ │  vnet1  │ │ enp9s0  │ │  vnet2  │ │ 
       │ └────┬────┘ └────┬────┘ └────┬────┘ │
       │      │           │           │      │
       └──────┼───────────┼───────────┼──────┘
              │           │           │
                          │
                       Ethernet [WiFi]     
                          │    Local LAN 10.1.0.x
                          │    DHCP server, router
                          ▼	
                       Internet
```
