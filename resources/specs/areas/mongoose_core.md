# Security Scan Area: mongoose_core

## Target

Area: mongoose_core

This area covers the core event/network/connection machinery, iobuf handling, queues, socket transport, timers, and BSD abstraction.

Primary files:
- `src/event.c`
- `src/iobuf.c`
- `src/net.c`
- `src/queue.c`
- `src/sock.c`
- `src/timer.c`
- `src/bsd.c`

Perform a focused security review centered on these files and this security surface.

Supporting code outside these files may be inspected when necessary to establish reachability, data flow, state, validation, mitigation, or impact, but do not independently review unrelated areas.

## Area-Specific Security Guidance

**Protocol State Machine and Event-Driven Lifecycle Bugs:**
- Review transitions between listening, accepted, connecting, resolving, TLS handshaking, HTTP, WebSocket, MQTT, draining, closing, and closed states.
- Check whether malformed input can cause protocol handlers to process data in the wrong state, process stale data after close, or invoke callbacks with invalid event data.
- Look for use-after-free or stale pointer use when callbacks call `mg_close_conn`, mark a connection closing, mutate iobufs, change protocol handlers, or initiate TLS/WebSocket/MQTT transitions.
- Verify that partial reads/writes, backpressure, `is_full`, draining, TLS pending states, and send-buffer mutations cannot cause duplicated sends, skipped validation, or parser confusion.
- Review timer and wakeup handling for stale user data pointers, cross-thread queue misuse, and callbacks firing after associated connections/state have been freed.
- Only report concurrency or race issues when a concrete event ordering can trigger memory corruption, unauthorized action, or reliable device/process crash.
