The NXP lwIP Port
===============================================================================

Below is description of possible settings of the port layer and an overview of 
a few helper functions.

The best place for redefinition of any mentioned macro is `lwipopts.h`.

The declaration of every mentioned function is in `ethernetif.h`. Please check 
the doxygen comments of those functions before.


## Link state
Physical link state (up/down) and its speed and duplex must be read out from 
PHY over MDIO bus. Especially link information is useful for lwIP stack so it
can for example send DHCP discovery immediately when a link becomes up.

To simplify this port layer offers a function `ethernetif_probe_link()` which 
reads those data from PHY and forwards them into lwIP stack.

In almost all examples this function is called every `ETH_LINK_POLLING_INTERVAL_MS`
(1500ms) by a function `probe_link_cyclic()`. 

By setting `ETH_LINK_POLLING_INTERVAL_MS` to 0 polling will be disabled.
On FreeRTOS, `probe_link_cyclic()` will be then called on an interrupt generated 
by PHY. GPIO port and pin for the interrupt line must be set in the 
`ethernetifConfig` struct passed to `ethernetif_init()`. On bare metal interrupts
are not supported right now.


## Rx task
To improve the reaction time of the app, reception of packets is done in
a dedicated task. The rx task stack size can be set by `ETH_RX_TASK_STACK_SIZE`
macro, its priority by `ETH_RX_TASK_PRIO`.

If you want to save memory you can set reception to be done in an interrupt by
setting `ETH_DO_RX_IN_SEPARATE_TASK` macro to 0.


## Disabling Rx interrupt when out of buffers
If `ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS` is set to 1, then when the port
gets out of Rx buffers, Rx enet interrupt will be disabled for a particular 
controller. Everytime Rx buffer is freed, Rx interrupt will be enabled.

This prevents your app from never getting out of Rx interrupt when the network
is flooded with traffic.

`ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS` is by default turned on, on FreeRTOS
and off on bare metal.

## Limit the number of packets read out from the driver at once on bare metal.
You may define macro `ETH_MAX_RX_PKTS_AT_ONCE` to limit the number of received 
packets read out from the driver at once. 

In case of heavy Rx traffic, lowering this number improves the realtime
behaviour of an app. Increasing improves Rx throughput. 

Setting it to value < 1 or not defining means "no limit".

## Helper functions
If your application needs to wait for the link to become up you can use one of
the following functions:
- `ethernetif_wait_linkup()`- Blocks until the link on the passed netif is not up.
- `ethernetif_wait_linkup_array()` - Blocks until the link on at least one netif
  from the passed list of netifs becomes up.

If your app needs to wait for the IPv4 address on a particular netif to become
different than "ANY" address (255.255.255.255) function
`ethernetif_wait_ipv4_valid()` does this.
