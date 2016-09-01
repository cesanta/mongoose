---
title: Disabling flags
---

- `MG_DISABLE_HTTP_WEBSOCKET` disable HTTP + WebSocket protocol support
- `MG_DISABLE_HTTP_DIGEST_AUTH` disable HTTP Digest (MD5) authorisation support
- `MG_DISABLE_MQTT` disable MQTT support
- `MG_DISABLE_SHA1` disable SHA1 support (used by WebSocket)
- `MG_DISABLE_MD5` disable MD5 support (used by HTTP auth)
- `MG_DISABLE_SOCKETPAIR` disable `mg_broadcast()` API
- `MG_DISABLE_HTTP_KEEP_ALIVE` useful for embedded systems to save resources
