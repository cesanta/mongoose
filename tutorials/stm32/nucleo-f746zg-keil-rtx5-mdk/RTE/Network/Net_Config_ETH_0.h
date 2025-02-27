/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Interface
 * Copyright (c) 2004-2021 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Net_Config_ETH_0.h
 * Purpose: Network Configuration for ETH Interface
 * Rev.:    V7.4.0
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>Ethernet Network Interface 0
#define ETH0_ENABLE             1

//   <o>Connect to hardware via Driver_ETH# <0-255>
//   <i>Select driver control block for MAC and PHY interface
#define ETH0_DRIVER             0

//   <s.17>MAC Address
//   <i>Ethernet MAC Address in text representation
//   <i>Value FF-FF-FF-FF-FF-FF is not allowed,
//   <i>LSB of first byte must be 0 (an ethernet Multicast bit).
//   <i>Default: "1E-30-6C-A2-45-5E"
#define ETH0_MAC_ADDR           "1E-30-6C-A2-45-5E"

//   <e>VLAN
//   <i>Enable or disable Virtual LAN
#define ETH0_VLAN_ENABLE        0

//     <o>VLAN Identifier <1-4093>
//     <i>A unique 12-bit numeric value
//     <i>Default: 1
#define ETH0_VLAN_ID            1
//   </e>

//   <e>IPv4
//   <i>Enable IPv4 Protocol for Network Interface
#define ETH0_IP4_ENABLE         1

//     <s.15>IP Address
//     <i>Static IPv4 Address in text representation
//     <i>Default: "192.168.0.100"
#define ETH0_IP4_ADDR           "192.168.0.100"

//     <s.15>Subnet mask
//     <i>Local Subnet mask in text representation
//     <i>Default: "255.255.255.0"
#define ETH0_IP4_MASK           "255.255.255.0"

//     <s.15>Default Gateway
//     <i>IP Address of Default Gateway in text representation
//     <i>Default: "192.168.0.254"
#define ETH0_IP4_GATEWAY        "192.168.0.254"

//     <s.15>Primary DNS Server
//     <i>IP Address of Primary DNS Server in text representation
//     <i>Default: "8.8.8.8"
#define ETH0_IP4_PRIMARY_DNS    "8.8.8.8"

//     <s.15>Secondary DNS Server
//     <i>IP Address of Secondary DNS Server in text representation
//     <i>Default: "8.8.4.4"
#define ETH0_IP4_SECONDARY_DNS  "8.8.4.4"

//     <e>IP Fragmentation
//     <i>This option enables fragmentation of outgoing IP datagrams,
//     <i>and reassembling the fragments of incoming IP datagrams.
//     <i>Default: enabled
#define ETH0_IP4_FRAG_ENABLE    1

//       <o>MTU size <576-1500>
//       <i>Maximum Transmission Unit in bytes
//       <i>Default: 1500
#define ETH0_IP4_MTU            1500
//     </e>

//     <h>ARP Address Resolution
//     <i>ARP cache and node address resolver settings
//       <o>Cache Table size <5-100>
//       <i>Number of cached MAC/IP addresses
//       <i>Default: 10
#define ETH0_ARP_TAB_SIZE       10

//       <o>Cache Timeout in seconds <5-255>
//       <i>A timeout for cached hardware/IP addresses
//       <i>Default: 150
#define ETH0_ARP_CACHE_TOUT     150

//       <o>Number of Retries <0-20>
//       <i>Number of Retries to resolve an IP address
//       <i>before ARP module gives up
//       <i>Default: 4
#define ETH0_ARP_MAX_RETRY      4

//       <o>Resend Timeout in seconds <1-10>
//       <i>A timeout to resend the ARP Request
//       <i>Default: 2
#define ETH0_ARP_RESEND_TOUT    2

//       <q>Send Notification on Address changes
//       <i>When this option is enabled, the embedded host
//       <i>will send a Gratuitous ARP notification at startup,
//       <i>or when the device IP address has changed.
//       <i>Default: Disabled
#define ETH0_ARP_NOTIFY         0
//     </h>

//     <e>IGMP Group Management
//     <i>Enable or disable Internet Group Management Protocol
#define ETH0_IGMP_ENABLE        0

//       <o>Membership Table size <2-50>
//       <i>Number of Groups this host can join
//       <i>Default: 5
#define ETH0_IGMP_TAB_SIZE      5
//     </e>

//     <q>NetBIOS Name Service
//     <i>When this option is enabled, the embedded host can be
//     <i>accessed by its name on local LAN using NBNS protocol.
#define ETH0_NBNS_ENABLE        0

//     <e>Dynamic Host Configuration
//     <i>When this option is enabled, local IP address, Net Mask
//     <i>and Default Gateway are obtained automatically from
//     <i>the DHCP Server on local LAN.
#define ETH0_DHCP_ENABLE        1

//       <s.40>Vendor Class Identifier
//       <i>This value is optional. If specified, it is added
//       <i>to DHCP request message, identifying vendor type.
//       <i>Default: ""
#define ETH0_DHCP_VCID          ""

//       <q>Bootfile Name
//       <i>This value is optional. If enabled, the Bootfile Name
//       <i>(option 67) is also requested from DHCP server.
//       <i>Default: disabled
#define ETH0_DHCP_BOOTFILE      0

//       <q>NTP Servers
//       <i>This value is optional. If enabled, a list of NTP Servers
//       <i>(option 42) is also requested from DHCP server.
//       <i>Default: disabled
#define ETH0_DHCP_NTP_SERVERS   0
//     </e>

//     Disable ICMP Echo response
#define ETH0_ICMP_NO_ECHO       0
//   </e>

//   <e>IPv6
//   <i>Enable IPv6 Protocol for Network Interface
#define ETH0_IP6_ENABLE         0

//     <s.40>IPv6 Address
//     <i>Static IPv6 Address in text representation
//     <i>Use unspecified address "::" when static
//     <i>IPv6 address is not used.
//     <i>Default: "fec0::2"
#define ETH0_IP6_ADDR           "fec0::2"

//     <o>Subnet prefix-length <1-128>
//     <i>Number of bits that define network address
//     <i>Default: 64
#define ETH0_IP6_PREFIX_LEN     64

//     <s.40>Default Gateway
//     <i>Default Gateway IPv6 Address in text representation
//     <i>Default: "fec0::1"
#define ETH0_IP6_GATEWAY        "fec0::1"

//     <s.40>Primary DNS Server
//     <i>Primary DNS Server IPv6 Address in text representation
//     <i>Default: "2001:4860:4860::8888"
#define ETH0_IP6_PRIMARY_DNS    "2001:4860:4860::8888"

//     <s.40>Secondary DNS Server
//     <i>Secondary DNS Server IPv6 Address in text representation
//     <i>Default: "2001:4860:4860::8844"
#define ETH0_IP6_SECONDARY_DNS  "2001:4860:4860::8844"

//     <e>IPv6 Fragmentation
//     <i>This option enables fragmentation of outgoing IPv6 datagrams,
//     <i>and reassembling the fragments of incoming IPv6 datagrams.
//     <i>Default: enabled
#define ETH0_IP6_FRAG_ENABLE    1

//       <o>MTU size <1280-1500>
//       <i>Maximum Transmission Unit in bytes
//       <i>Default: 1500
#define ETH0_IP6_MTU            1500
//     </e>

//     <h>Neighbor Discovery
//     <i>Neighbor cache and node address resolver settings
//       <o>Cache Table size <5-100>
//       <i>Number of cached node addresses
//       <i>Default: 5
#define ETH0_NDP_TAB_SIZE       5

//       <o>Cache Timeout in seconds <5-255>
//       <i>Timeout for cached node addresses
//       <i>Default: 150
#define ETH0_NDP_CACHE_TOUT     150

//       <o>Number of Retries <0-20>
//       <i>Number of retries to resolve an IP address
//       <i>before NDP module gives up
//       <i>Default: 4
#define ETH0_NDP_MAX_RETRY      4

//       <o>Resend Timeout in seconds <1-10>
//       <i>A timeout to resend Neighbor Solicitation
//       <i>Default: 2
#define ETH0_NDP_RESEND_TOUT    2
//     </h>

//     <e>Dynamic Host Configuration
//     <i>When this option is enabled, local IPv6 address is
//     <i>automatically configured.
#define ETH0_DHCP6_ENABLE       1

//       <o>DHCPv6 Client Mode  <0=>Stateless Mode <1=>Statefull Mode
//       <i>Stateless DHCPv6 Client uses router advertisements
//       <i>for IPv6 address autoconfiguration (SLAAC).
//       <i>Statefull DHCPv6 Client connects to DHCPv6 server for a
//       <i>leased IPv6 address and DNS server IPv6 addresses.
#define ETH0_DHCP6_MODE         1

//       <e>Vendor Class Option
//       <i>If enabled, Vendor Class option is added to DHCPv6
//       <i>request message, identifying vendor type.
//       <i>Default: disabled
#define ETH0_DHCP6_VCLASS_ENABLE 0

//         <o>Enterprise ID
//         <i>Enterprise-number as registered with IANA.
//         <i>Default: 0 (Reserved)
#define ETH0_DHCP6_VCLASS_EID   0

//         <s.40>Vendor Class Data
//         <i>This string identifies vendor type.
//         <i>Default: ""
#define ETH0_DHCP6_VCLASS_DATA  ""
//       </e>
//     </e>

//     Disable ICMP6 Echo response
#define ETH0_ICMP6_NO_ECHO      0
//   </e>

//   <h>OS Resource Settings
//   <i>These settings are used to optimize usage of OS resources.
//     <o>Interface Thread Stack Size <512-65535:4>
//     <i>Default: 512 bytes
#define ETH0_THREAD_STACK_SIZE  512

//        Interface Thread Priority
#define ETH0_THREAD_PRIORITY    osPriorityAboveNormal

//   </h>
// </h>

//------------- <<< end of configuration section >>> ---------------------------
