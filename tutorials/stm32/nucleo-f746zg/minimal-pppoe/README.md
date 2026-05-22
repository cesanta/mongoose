# PPPoE on NUCLEO-F746ZG

This example shows a device with a very simple web server (the NUCLEO board) that connects to a PPPoE server. The device web server can then be accessed from the computer running the web server. For more complete scenarios, proper server setting and routing would be needed, thing a network admin will do.

## Configure and start your PPPoE server

This is just a test example in Linux, we use _pppoe-server_ for this, as it is easy to configure and serves our purpose. For production scenarios, ask your network admin.

```sh
$ cat /etc/ppp/pppoe-server-options
# PPP options for the PPPoE server
# LIC: GPL
#require-pap
#login
lcp-echo-interval 10
lcp-echo-failure 2
$ sudo pppoe-server -F -I yourethernetinterface
```

## Flash your NUCLEO

```sh
$ make
$ make flash
```

## Check and test

```sh
$ ifconfig ppp0
ppp0: flags=4305<UP,POINTOPOINT,RUNNING,NOARP,MULTICAST>  mtu 1492
        inet 10.0.0.1  netmask 255.255.255.255  destination 10.67.15.1
        ppp  txqueuelen 3  (Point-to-Point Protocol)
        RX packets 3  bytes 36 (36.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 28  bytes 7003 (6.8 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

$ route
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
default         _gateway        0.0.0.0         UG    0      0        0 virbr0
10.67.15.1      0.0.0.0         255.255.255.255 UH    0      0        0 ppp0

$ curl http://10.67.15.1/
hi
$ 
```

```sh
$ minicom -D /dev/ttyACM0
```

```text
0      3 mongoose.c:5306:mg_mgr_init    MG_IO_SIZE: 512, TLS: builtin
6      2 main.c:48:main                 Init TCP/IP stack ...
b      3 mongoose.c:27645:guess_mdc_cr  HCLK: 216000000, CR: 4
11     2 mongoose.c:25494:mg_phy_init   PHY ID: 0x07 0xc131 (LAN87x)
18     3 mongoose.c:5223:mg_listen      1 0 tcp://0.0.0.0:80
1d     2 main.c:57:main                 Init done, starting main loop ...
7e7    3 mongoose.c:27757:mg_tcpip_driv Link is 100M full-duplex
7ec    3 mongoose.c:4035:mg_l2_pppoe_po Sent PADI
7f1    1 mongoose.c:7220:mg_tcpip_poll  Network is down
7f6    3 mongoose.c:4083:mg_l2_pppoe_rx Sent PADR
7fa    3 mongoose.c:4088:mg_l2_pppoe_rx PPPoE session 0x0001 started
bcf    3 mongoose.c:27757:mg_tcpip_driv Link is 100M full-duplex
bd4    1 mongoose.c:7220:mg_tcpip_poll  Network is down
c0d    3 mongoose.c:3821:ppp_handle_lcp LCP config request of 14 bytes, rejecting...
c15    3 mongoose.c:3815:ppp_handle_lcp LCP config request of 4 bytes, acknowledging...
c1e    3 mongoose.c:3836:ppp_handle_lcp LCP echo request of 8 bytes, replying...
c26    3 mongoose.c:3943:ppp_rx         unknown 40-byte PPP frame with proto 0x80fd:
0000   80 fd 01 01 00 0f 1a 04 78 00 18 04 78 00 15 03   ........x...x...
0010   2f 00 00 00 00 00                                 /.....          
c3a    3 mongoose.c:3874:ppp_handle_ipc IPCP ack, GW IP: 10.0.0.1
c40    3 mongoose.c:3897:ppp_handle_ipc IPCP ack, IP: 10.67.15.1
fb7    2 mongoose.c:5712:onstatechange  READY, IP: 10.67.15.1
fbc    2 mongoose.c:5713:onstatechange         GW: 10.0.0.1
fc1    2 mongoose.c:5715:onstatechange        MAC: 02:35:42:56:22:2e
33d3   3 mongoose.c:3836:ppp_handle_lcp LCP echo request of 8 bytes, replying...
5b8b   3 mongoose.c:3836:ppp_handle_lcp LCP echo request of 8 bytes, replying...
8348   3 mongoose.c:3836:ppp_handle_lcp LCP echo request of 8 bytes, replying...
aafa   3 mongoose.c:3836:ppp_handle_lcp LCP echo request of 8 bytes, replying...
d2b7   3 mongoose.c:3836:ppp_handle_lcp LCP echo request of 8 bytes, replying...
fa74   3 mongoose.c:3836:ppp_handle_lcp LCP echo request of 8 bytes, replying...
12196  3 mongoose.c:6583:accept_conn    2 accepted 10.0.0.1:50846
1219d  3 mongoose.c:5159:mg_close_conn  2 0 closed
```
