# HARD EXCLUSIONS
**Automatically exclude findings matching these patterns:**
1. The attacker must be inside the process, run local code, attach a debugger, or directly invoke Mongoose internals.
2. The attacker must call internal C functions directly, pass arbitrary API arguments from local code, construct fake callback data, or manually invoke event handlers.
3. The exploit depends on NULL, dangling, fake, or corrupted pointers/objects supplied by a local caller, including invalid `mg_connection`, `mg_mgr`, `mg_str`, iobufs, or callback data.
4. The exploit requires manually mutating internal structs, flags, parser offsets, descriptor indexes, connection state, TLS state, MQTT/WebSocket state, or filesystem state into impossible combinations.
5. The bug exists only in tests, fuzz harnesses, benchmarks, synthetic unit tests, or debug-only code, with no shared production logic.
6. The issue is only in a tutorial/demo/example with no realistic production relevance or sensitive operation.
7. The attacker must edit source code, build flags, compile-time macros, TLS backend choices, filesystem roots, or local device configuration.
8. The attacker must directly write hardware registers, MMIO, DMA descriptors, descriptor ownership bits, IRQ state, cache registers, or peripheral configuration.
9. Generic DoS from high traffic volume, many valid connections, slow clients, missing quotas, missing rate limits, or ordinary resource saturation.
10. Pure hardening advice: add logging, monitoring, sanitizers, compiler flags, stack canaries, ASLR, comments, or broader validation without a concrete reachable vulnerability.
11. Theoretical memory-safety trivia with no realistic external trigger, no sensitive data exposure, and no security-relevant crash or corruption.
12. Generic crypto advice with no concrete impact on authentication, confidentiality, integrity, downgrade resistance, hostname verification, certificate validation, key handling, nonces/tags, or plaintext fallback.
13. Speculative protocol claims without a specific malformed input shape, reachable parser path, and concrete security impact.
14. Pure application policy gaps: no default auth, no CSRF, no user access model, no account lockout, no audit logs, or no password policy.

# SIGNAL QUALITY CRITERIA
**For remaining findings, assess:**
1. What external attacker-controlled input reaches the code: HTTP, MQTT, WebSocket, DNS/mDNS, TLS, Ethernet/IP/TCP/UDP, upload data, path, query, header, body, or malicious peer response?
2. How does that input reach the vulnerable code through normal Mongoose execution?
3. Does the exploit respect the external-attacker model, without local code execution, fake pointers, struct mutation, debugger access, or direct hardware access?
4. What concrete security impact occurs: memory corruption, OOB read/write, UAF, smuggling, auth bypass, file access, firmware/config overwrite, data exposure, TLS bypass, crash, livelock, state corruption, etc.?
5. Does the issue affect Mongoose library code, protocol parsers, TCP/IP stack, TLS integration, filesystem helpers, upload handling, MQTT, WebSocket, DNS/mDNS, OTA, dashboards, or embedded backends?
6. Does the report provide exact files, functions, line numbers, data flow, bounds reasoning, state reasoning, or a minimal malformed input?
7. Is the malformed input shape clear, such as CL+TE HTTP, oversized MQTT SUBSCRIBE, malformed WebSocket length, DNS compression loop, wildcard TLS certificate, invalid packet header length, or encoded path traversal?
8. Is the severity justified by reachability and impact?
9. Is this a library/helper/default/example-pattern vulnerability rather than only arbitrary application misuse?
10. Is the fix specific enough to act on: reject CL+TE, bound arrays, validate DNS pointers, enforce one-label wildcard matching, check packet lengths, canonicalize paths, or bound RX loops?
11. Is a DoS finding caused by a concrete implementation flaw rather than volume, slow clients, missing quotas, or capacity limits?
12. In embedded context, can a small malformed input reliably crash, wedge, corrupt, or permanently disrupt a device/process?

# PRECEDENTS
**Mongoose-specific filtering decisions:**
1. Keep malformed HTTP framing issues that can cause CL+TE smuggling, parser disagreement, body confusion, desynchronization, memory corruption, data exposure, or crash.
2. Keep duplicate/conflicting `Content-Length`, duplicate/conflicting `Transfer-Encoding`, malformed chunking, and HTTP version framing issues with concrete security impact.
3. Keep malformed MQTT packet bugs in CONNECT, PUBLISH, SUBSCRIBE, UNSUBSCRIBE, properties, remaining length, topics, QoS, packet IDs, or response construction when externally reachable.
4. Keep MQTT SUBSCRIBE topic-count overflow into fixed response buffers.
5. Keep WebSocket length, masking, fragmentation, continuation, control-frame, and state-confusion bugs with concrete security impact.
6. Keep TLS certificate validation bypasses, missing hostname verification, insecure backend differences, invalid certificate acceptance, and plaintext/unverified fallback.
7. Keep wildcard certificate overmatching, such as `*.example.com` matching `a.b.example.com`, when used for TLS identity or another security boundary.
8. Keep unsafe `mg_match` or glob use at security boundaries: certificate identity, route authorization, filesystem policy, MQTT topic authorization, or access control.
9. Keep DNS/mDNS label, compression pointer, pointer loop, RR length, transaction matching, spoofing, OOB, or infinite-loop issues with external reachability.
10. Keep filesystem traversal through exposed serving/upload/SSI paths, including encoding, double decoding, backslashes, dot-dot, absolute paths, drive letters, symlinks, NULs, alias/root confusion, and upload filename traversal.
11. Keep OTA/update issues allowing unauthenticated or improperly validated writes to firmware, flash, config, filesystem, boot state, device identity, or update metadata.
12. Keep embedded packet parser bugs in Ethernet, ARP, IP, IPv6, ICMP, UDP, TCP, DHCP, SNTP, or drivers when malformed traffic causes OOB, overflow, descriptor corruption, queue corruption, state corruption, or crash.
13. Keep IRQ, polling, RX descriptor, and packet-loop livelock/starvation/wedge bugs caused by received traffic.
14. Keep event-lifecycle bugs reachable from external input: UAF, stale pointer, double close, callback lifetime error, iobuf mutation after close, protocol transition confusion, timer/wakeup after free.
15. Keep externally reachable `mg_str` null-termination mistakes causing OOB read, data leak, auth bypass, parser confusion, or crash.
16. Keep integer conversion bugs where external lengths affect allocation, copying, parsing, or bounds checks.
17. Filter parser/helper reports based only on a local caller passing invalid pointers, fake lengths, fake structs, or corrupted objects.
18. Filter reports requiring direct mutation of connection internals such as protocol flags, closing flags, handler pointers, iobuf fields, callback pointers, or user data.
19. Filter reports requiring attacker control of `MG_ENABLE_*`, TLS backend macros, filesystem backend macros, compiler flags, debug flags, sanitizer settings, or platform defines.
20. Filter direct hardware-access reports requiring writes to registers, DMA descriptors, MMIO, IRQ state, cache controls, or peripheral configuration.
21. Filter generic “Mongoose lacks authentication” reports unless a helper, example, route pattern, auth parser, MQTT broker behavior, dashboard, upload handler, or OTA endpoint creates a concrete issue.
22. Filter generic rate-limit/quota/connection-cap/upload-cap findings unless a specific bug lets a small malformed input trigger crash, corruption, infinite loop, or disproportionate exhaustion.
23. Filter purely theoretical UB, portability, unaligned-access, or signed-overflow reports with no supported-platform impact and no realistic external trigger.
24. Filter RFC non-compliance without concrete smuggling, bypass, memory corruption, data exposure, state corruption, or crash.
25. Keep security-relevant production-like examples: dashboards, upload servers, MQTT brokers, firmware update flows, auth examples, TLS examples, and filesystem-serving examples.
26. Keep client-side findings caused by malicious servers, brokers, DNS responders, TLS peers, WebSocket peers, or HTTP peers.
27. Keep callback-related findings only when Mongoose invokes the callback normally and then unsafely continues using freed or mutated state.
28. Keep availability findings when one malformed packet, request, certificate, DNS response, MQTT message, WebSocket frame, or small sequence can reliably crash, wedge, corrupt, or disrupt a Mongoose target.
29. Keep custom backend findings only when the issue is in Mongoose’s backend interface, default backend, documented backend contract, or common backend implementation.
30. When uncertain, keep findings with clear external input, reachable code path, and concrete impact in Mongoose library/protocol/driver code; filter internal-access, impossible-state, local-misuse, generic-hardening, and speculative reports.
31. For dashboard reports, do not infer unauthenticated access from a route-specific check alone; account for the broader authentication flow. Keep concrete reachable bugs such as path traversal, unsafe decoded paths, or file read/write/delete issues.
32. Filter reports regarding DNS transaction ID predictability or lack of strict question-name matching in DNS response processing, as these are acknowledged design limitations of the current implementation.