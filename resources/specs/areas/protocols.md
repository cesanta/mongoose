# Security Scan Area: protocols

## Target

Area: protocols

This area covers the smaller externally controlled protocol parsers and state machines other than HTTP, WebSocket, and TLS.

Primary files:
- `src/modbus.c`
- `src/rpc.c`
- `src/sntp.c`
- `src/mqtt.c`
- `src/dns.c`

Perform a focused security review centered on these files and this security surface.

Supporting code outside these files may be inspected when necessary to establish reachability, data flow, state, validation, mitigation, or impact, but do not independently review unrelated areas.

## Area-Specific Security Guidance

**MQTT Parser and Broker/Client State Machine Vulnerabilities:**
- Review MQTT fixed header parsing, remaining-length decoding, variable-length integer loops, packet identifier handling, QoS transitions, topic extraction, property parsing, and MQTT 3.x vs MQTT 5 differences.
- Look for malformed MQTT packets that can trigger out-of-bounds reads/writes, stack buffer overflow, heap corruption, assertion/crash, or parser desynchronization.
- Specifically check SUBSCRIBE and UNSUBSCRIBE handling for unbounded numbers of topics, malformed topic length fields, missing QoS bytes, invalid QoS values, and writes to fixed-size response arrays.
- Verify that PUBLISH handling enforces topic length, packet length, QoS rules, retained flag handling, packet identifier requirements, and message bounds before accessing fields.
- Check broker examples and protocol handlers for authorization bypass, cross-topic data exposure, wildcard topic mishandling, `$SYS` or reserved topic confusion, and unintended publish/subscribe access.
- Review MQTT reconnect/resubscribe logic for stale connection state, packet identifier reuse bugs, and memory lifetime problems after close/reconnect.
- Treat unauthenticated single-packet broker crash, client crash from malicious broker response, or memory corruption during MQTT parsing as reportable.

**DNS and mDNS Parsing and Name Resolution:**
- Review DNS and mDNS message parsing for label length validation, compression pointer loops, pointer bounds, recursive expansion limits, integer overflow, malformed resource records, and out-of-bounds reads.
- Check that DNS answers are matched to outstanding requests by transaction ID, question name/type/class, and source expectations where applicable.
- Look for cache poisoning, response spoofing, or name confusion if DNS response validation is weak in client or resolver code.
- Verify that DNS failures, timeouts, and retries cannot leave dangling active request state or corrupt connection state.

**Modbus, RPC, and SNTP:**
- Review externally controlled lengths, framing, message parsing, and protocol state for malformed peer input that can cause parser desynchronization, state corruption, or a remotely triggerable crash or wedge.
