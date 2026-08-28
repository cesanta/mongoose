# Security Scan Area: common_misc

## Target

Area: common_misc

This area covers common utility code, parsing/string/formatting helpers, URL handling, hash/base64 helpers, filesystem backends, little filesystem integration, and flash support.

Primary files:
- `src/base64.c`
- `src/fmt.c`
- `src/json.c`
- `src/log.c`
- `src/md5.c`
- `src/printf.c`
- `src/sha1.c`
- `src/sha256.c`
- `src/str.c`
- `src/url.c`
- `src/util.c`
- `src/fs*.c`
- `src/lfs.c`
- `src/flash.c`

Perform a focused security review centered on these files and this security surface.

Supporting code outside these files may be inspected when necessary to establish reachability, data flow, state, validation, mitigation, or impact, but do not independently review unrelated areas.

## Area-Specific Security Guidance

**Filesystem Backends:**
- Review packed filesystem code and filesystem backends for traversal, unauthorized file read/write, overwrite, or truncation.
- Check path canonicalization across POSIX, Windows, embedded filesystems, packed filesystems, and custom `mg_fs` implementations.
- Look for trailing slash confusion, NUL byte truncation, and case sensitivity differences in backend path handling.

**URL Parsing:**
- Review URL parsing for scheme, host, port, IPv6 literal, userinfo, percent-encoding, embedded NUL, empty host, default port, and path/query boundary confusion.
- Check for SSRF-relevant behavior only when attacker input can control the destination host, protocol, or network boundary. Path-only control is not sufficient.
