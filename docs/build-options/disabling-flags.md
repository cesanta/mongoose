---
title: Disabling flags
---

- `MG_DISABLE_HTTP_WEBSOCKET` disable HTTP + Websocket protocol support
- `MG_DISABLE_HTTP_DIGEST_AUTH` disable HTTP Digest (MD5) authorization support
- `MG_DISABLE_MQTT` disable MQTT support
- `MG_DISABLE_SHA1` disable SHA1 support (used by Websocket)
- `MG_DISABLE_MD5` disable MD5 support (used by HTTP auth)
- `MG_DISABLE_JSON_RPC` disable JSON-RPC support
- `MG_DISABLE_SOCKETPAIR` disable `mg_broadcast()` API
- `MG_DISABLE_HTTP_KEEP_ALIVE` useful for embedded systems to save resources
