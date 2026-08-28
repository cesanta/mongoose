# Security Scan Area: net_builtin

## Target

Area: net_builtin

This area covers the built-in TCP/IP stack and its selected L2 implementations.

Primary files:
- `src/l2_*.c`
- `src/net_builtin.c`

Perform a focused security review centered on these files and this security surface.

Supporting code outside these files may be inspected when necessary to establish reachability, data flow, state, validation, mitigation, or impact, but do not independently review unrelated areas.

## Area-Specific Security Guidance

**Built-In TCP/IP Stack, Packet Parsing, and Embedded Network Drivers:**
- Review Ethernet, ARP, IP, IPv6, ICMP, UDP, TCP, DHCP, SNTP, and driver receive/transmit paths for packet-length validation before reading protocol headers.
- Check IP/TCP/UDP header length fields, options length, fragmentation/reassembly behavior, checksum handling, and payload offset calculations for underflow, overflow, or out-of-bounds access.
- Look for malformed packets that can corrupt descriptor rings, RX/TX buffers, queue state, ARP tables, connection state, or timers.
- Review interrupt handlers and bare-metal driver code for bounded processing loops. Flag IRQ paths where attacker-controlled traffic can cause livelock, starvation, descriptor ownership loss, or repeated processing of the same malformed frame.
- Check DMA descriptor ownership transitions, memory barriers, cache coherency handling, and buffer length fields for race-like bugs that can cause memory corruption or stale packet reuse.
- Review platform-specific drivers such as STM32, NXP, TI, Microchip, Renesas, Infineon, Wiznet, Cypress WiFi, cellular, and other built-in stack integrations for inconsistent bounds checks or endian conversions.
- Treat a remotely reachable malformed Ethernet/IP/TCP/UDP packet that crashes, wedges, or corrupts an embedded target as reportable.
