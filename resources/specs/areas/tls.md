# Security Scan Area: tls

## Target

Area: tls

This area covers the complete Mongoose TLS implementation represented by the primary files.

Primary files:
- `src/tls_aes128.c`
- `src/tls_builtin.c`
- `src/tls_chacha20.c`
- `src/tls_dummy.c`
- `src/tls_mbed.c`
- `src/tls_openssl.c`
- `src/tls_rsa.c`
- `src/tls_uecc.c`
- `src/tls_x25519.c`

Perform a focused security review centered on these files and this security surface.

Supporting code outside these files may be inspected when necessary to establish reachability, data flow, state, validation, mitigation, or impact, but do not independently review unrelated areas.

Connection, core, network, and helper code may be inspected when needed to understand the complete TLS execution path.

## Area-Specific Security Guidance

### Mandatory High-Risk Coverage: Built-In TLS

The built-in TLS implementation (`MG_TLS_BUILTIN`) is a mandatory high-risk review target. Perform a substantial security analysis of all security-relevant built-in TLS code, including parsing, cryptographic processing, authentication and certificate validation, length and buffer handling, protocol state, cross-call behavior, and failure paths.

Trace attacker-controlled TLS peer input through normal client and server execution, including traffic received before peer authentication is complete. Report any credible memory-safety, authentication, confidentiality, integrity, state-corruption, or remotely triggerable availability vulnerability.

**TLS, Certificate, Hostname Verification, and Cryptographic Validation:**
- Review all TLS backends, including built-in TLS, mbedTLS, OpenSSL, WolfSSL, and custom TLS integration, for equivalent security behavior where possible.
- Flag certificate verification bypasses, accidental `skip_verification` use in security-sensitive examples or defaults, missing hostname verification when `name`/SNI is available, or inconsistent validation across TLS backends.
- For hostname verification, check wildcard handling carefully. A wildcard certificate identity must not match multiple DNS labels. For example, `*.example.com` must not match `a.b.example.com`.
- Verify that wildcard matching is applied only to certificate presented identifiers, not to arbitrary reference identifiers or general DNS glob matching.
- Review `mg_match` or other glob/matching helpers if used for certificate identity checks, authorization checks, filesystem paths, routing, or topic matching. Flag cases where glob semantics are broader than the security policy requires.
- Check certificate parsing for DER/PEM boundary errors, length overflows, malformed ASN.1, extension parsing mistakes, name constraints, SAN vs CN precedence, embedded NUL, and invalid string type handling.
- Review TLS record parsing, handshake transcript handling, key schedule, random generation, ECC/X25519 operations, AES-GCM nonce/tag handling, and error handling for memory safety and authentication failures.
- Flag any behavior that silently falls back from verified TLS to unverified TLS or plaintext.
- Check that TLS receive/send error paths cannot corrupt connection state, reuse freed buffers, or continue processing unauthenticated plaintext as encrypted data.
