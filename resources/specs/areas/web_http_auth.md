# Security Scan Area: web_http_auth

## Target

Area: web_http_auth

This area covers HTTP parsing and framing, HTTP-facing filesystem behavior, SSI, WebSocket, authentication/token/dashboard code, and HTTP-based OTA behavior.

Primary files:
- `src/http.c`
- `src/ssi.c`
- `src/ws.c`
- `src/jwt.c`
- `src/dash.c`
- `src/ota.c`

Perform a focused security review centered on these files and this security surface.

Supporting code outside these files may be inspected when necessary to establish reachability, data flow, state, validation, mitigation, or impact, but do not independently review unrelated areas.

For OTA behavior, follow operations into TLS, flash, filesystem, JSON, URL, or other support code when necessary, while keeping the review centered on the `web_http_auth` surface.

## Area-Specific Security Guidance

**HTTP Parser, Request Smuggling, and Message Framing:**
- Verify that HTTP requests or responses containing both `Content-Length` and `Transfer-Encoding` are rejected or handled in a way that cannot cause front-end/back-end desynchronization. Treat CL+TE coexistence as security-relevant request smuggling risk.
- Verify that duplicate `Content-Length`, duplicate `Transfer-Encoding`, conflicting transfer codings, malformed chunk sizes, chunk extensions, premature EOF, and body length mismatches cannot cause inconsistent body parsing.
- Check whether HTTP/1.0, HTTP/1.1, and malformed protocol versions alter CL/TE handling unsafely. A message with both `Content-Length` and `Transfer-Encoding` should not be accepted merely because of version differences.
- Look for parsing paths where headers are normalized differently from later lookup paths, including case folding, whitespace around colon, obs-fold/line folding, embedded NUL, duplicate headers, or partial header names.
- Review URL decoding, query parsing, multipart parsing, upload handling, range handling, and chunked responses for bounds mistakes, truncation, or inconsistent decoded-vs-raw path decisions.
- Check static file serving and SSI handling for path traversal through percent-encoding, double decoding, backslashes on Windows, mixed separators, dot-dot segments, absolute paths, drive letters, symlinks, embedded NUL, or alias/root_dir confusion.
- Review HTTP authentication helper paths for parsing ambiguities in Basic auth, header extraction, credential truncation, empty username/password acceptance, or incorrect comparison behavior.
- Check that HTTP upgrade to WebSocket validates method, headers, key, version, and connection semantics consistently before switching protocol state.

**WebSocket Framing and Upgrade Vulnerabilities:**
- Review WebSocket frame parsing for extended payload length handling, 16-bit/64-bit length conversion, masking rules, fragmentation, continuation frames, control-frame length limits, close/ping/pong handling, and compression-related assumptions if present.
- Check that client-to-server frames require masking and that malformed mask/payload boundaries cannot read or write out of bounds.
- Look for state-machine confusion after HTTP upgrade, especially where partial frames, fragmented messages, or mixed HTTP/WebSocket bytes can cause parser desynchronization.
- Check whether large or malformed frame lengths can cause integer overflow, memory corruption, uncontrolled allocation, connection lockup, or single-connection/device crash.
- Verify that WebSocket message APIs preserve attacker-controlled binary data length correctly and do not pass non-null-terminated payloads to C-string functions.

**HTTP-Facing Filesystem Serving, Uploads, and SSI:**
- Review `mg_http_serve_dir`, `mg_http_serve_file`, upload APIs, and SSI expansion for traversal, unauthorized file read/write, overwrite, truncation, or unexpected executable/include behavior.
- Look for inconsistent handling of decoded and undecoded paths, double decoding, query string leakage into path, trailing slash confusion, NUL byte truncation, and case sensitivity differences.
- Review multipart upload parsing for boundary confusion, filename traversal, field length overflow, partial-write behavior, and max-size enforcement.
- Check SSI include handling for directory escape, recursive include loops, unintended disclosure of local files, and parsing of attacker-controlled SSI directives.
- Verify that range requests and static response headers cannot cause out-of-bounds reads or disclose memory.

**OTA/Firmware Update and Device Dashboard Security:**
- Review firmware update endpoints and examples for missing authentication, authorization bypass, path confusion, arbitrary write, unsafe flash offset/length handling, and failure to verify firmware authenticity where the code claims secure update behavior.
- Check update chunk parsing for integer overflow, flash boundary crossing, rollback/downgrade exposure, incomplete image bootability, and crash/power-loss unsafe state transitions.
- Flag any code path where unauthenticated network input can write firmware, configuration, filesystem contents, flash pages, bootloader state, or device identity material.
- Review device dashboard examples for security-sensitive default routes, credentials, upload handlers, debug endpoints, config mutation endpoints, and CORS/header behavior when they are likely to be copied into production.
- Do not report ordinary insecure demo behavior unless it creates a realistic copy-paste production vulnerability or affects library code.

**Authentication, Authorization, and Access-Control Patterns in Mongoose Integrations:**
- Review examples and helper APIs that implement HTTP auth, cookies, sessions, tokens, device dashboards, REST APIs, upload endpoints, and firmware update endpoints.
- Flag authentication bypasses caused by prefix route matching, glob matching, decoded-vs-raw URI confusion, case normalization mistakes, missing boundary checks, or default-open sensitive endpoints.
- Check for authorization decisions made in client-side JavaScript only. Client-side checks are not security boundaries.
- Review CORS and origin checks only when they protect credentialed sensitive operations and the exploit path is concrete.
- Look for credential comparison bugs, truncation of usernames/passwords, accepting empty credentials, parsing only part of an Authorization header, or treating malformed credentials as anonymous-but-authorized.
- Flag sensitive data exposure when secrets, credentials, tokens, private keys, firmware signing material, PII, or device identity values can be read by unauthorized remote clients or logged from attacker-triggered paths.
