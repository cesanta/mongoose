# Mongoose TCP/IP stack on a tap interface

This example allows the use of Mongoose's built-in TCP/IP stack in systems that support a TUN/TAP device. The application opens a TAP device and uses the standard socket interface to read and write on it.

The interface can be created by the example itself, but it will also disappear when the example exits. Since we usually need to do something before, like setting up a DHCP server or establishing some connections to the rest of the world, we'll manually create a specific TAP interface and the example will open it; this way, we can have everything setup in advance.

## Linux setup

Create a TAP interface; you can use it as is, attach it to a virtual bridge, or forward/masquerade its traffic.

```sh
$ sudo ip tuntap add dev tap0 mode tap
$ sudo ip link set tap0 up
```

In some systems, you might need superuser privileges to open the device with the example. In others, you can add `user yourusername` or `group yourgroup` at creation time, to grant access to it.

### Use as a virtual interface

In this scenario, Mongoose is only reachable from the hosting machine, but you can add forwarding and masquerading later. Once you have your TAP interface up and running with an IP address, you can configure any services on it.

- Add an IP address
  ```sh
  $ sudo ip addr add 192.168.0.1/24 dev tap0
  ```
- Start your DHCP server at that interface serving that subnet, or otherwise configure Mongoose to use a fixed address within that subnet.
- Now start the example opening the very same TAP interface:
  ```sh
  $ make -C examples/mip-tap/ clean all
  ```
  If you are using a different interface name than `tap0`, add `ARGS="-i ifcname"`, and use the proper interface name

```
  [DHCP server for 192.168.0.x]
                 192.168.0.1                    192.168.0.x
                 ┌─────────┐                    
                 │  tap0   ├────────────────────  mip-tap
                 └─────────┘                       
```

As you can't access any other host than your workstation, you need to add any required services (as DNS) there, and configure Mongoose appropriately. This is a foundation, you can expand it by choosing one of the solutions that follow.

### Bridge to your network

If you happen to have a virtual bridge interface (for linking several virtual machines together, for example) (probably `br0` or `virbr0`), we can take advantage of it.

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
- Attach the TAP interface to the bridge:
  ```sh
  $ sudo ip link set tap0 master virbr0
  ```
- You won't see your interface state is UP until it is open, but you can check you see something like this:
  ```sh
  $ ip link show master virbr0
  2: enp9s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast master virbr0 state UP mode DEFAULT group default qlen 1000
    link/ether 30:5a:3a:08:db:90 brd ff:ff:ff:ff:ff:ff
  6: tap0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast master virbr0 state DOWN mode DEFAULT group default qlen 1000
    link/ether 66:91:e2:5f:d7:ed brd ff:ff:ff:ff:ff:ff
  ```
- Now start the example
  ```sh
  $ make -C examples/mip-tap/ clean all
  ```
  If you are using a different interface name than `tap0`, add `ARGS="-i ifcname"`, and use the proper interface name
- Now if everything is fine, besides the example working, you'll see your interface state is UP:
  ```sh
  $ ip link show master virbr0
  2: enp9s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast master virbr0 state UP mode DEFAULT group default qlen 1000
    link/ether 30:5a:3a:08:db:90 brd ff:ff:ff:ff:ff:ff
  6: tap0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast master virbr0 state UP mode DEFAULT group default qlen 1000
    link/ether 66:91:e2:5f:d7:ed brd ff:ff:ff:ff:ff:ff
  ```

```
                                   10.1.0.10
                                    virbr0
                                       │
                    ┌──────────────────┴──────────────────┐
                    │                                     │         10.1.0.x
                    │              ┌────────┐             │
                    │              │  tap0  ├─────────────┼───────  mip-tap
                    │              └────────┘             │
                    │                                     │
                    │ ┌─────────┐ ┌─────────┐ ┌─────────┐ │
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

If you have _Docker_ running, it may introduce firewall rules that will disrupt your bridging.

### Forward/Masquerade

Once you have your virtual interface up and running with an IP address, you can use your Linux to NAT and forward Mongoose traffic to another interface, for example the one that connects you to the Internet. In this case, Mongoose will have access to the Internet and will also be reachable from your own workstation; but not from any other hosts in your local network (if there is one). You can use this setup with a direct connection to the Internet or being part of a network, when you want to isolate Mongoose from the rest of your network.

- We assume you already have a firewall in place (you should); to configure masquerading and forwarding do:
  ```sh
  $ sudo iptables -t nat -A POSTROUTING -o enp9s0 -j MASQUERADE
  $ echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward
  ```
  Remember to use the proper network interface name
- Now start the example
  ```sh
  $ make -C examples/mip-tap/ clean all
  ```
  If you are using a different interface name than `tap0`, add `ARGS="-i ifcname"`, and use the proper interface name

```
       [DHCP server for 192.168.0.x]
      ┌────────────►  192.168.0.1                    192.168.0.x
      │ forwarding    ┌─────────┐  
      │ masquerading  │  tap0   ├────────────────────  mip-tap
      │               └─────────┘
      ▼
   Your IP (LAN / Public)
 ┌─────────┐
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
                       virbr0         forwarding     ┌─────────┐
                          │          masquerading    │  tap0   ├────────────────────  mip-tap
                          │                          └─────────┘
       ┌──────────────────┴──────────────────┐
       │                                     │
       │ ┌─────────┐ ┌─────────┐ ┌─────────┐ │
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
