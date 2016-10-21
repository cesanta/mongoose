---
title: Enabling flags
---

- `MG_ENABLE_SSL` Enable [SSL/TLS support](https://docs.cesanta.com/mongoose/master/#/http/ssl.md/) (OpenSSL API)
- `MG_ENABLE_IPV6` Enable IPv6 support
- `MG_ENABLE_MQTT` enable [MQTT client](https://docs.cesanta.com/mongoose/master/#/mqtt/client_example.md/) (on by default, set to 0 to disable)
- `MG_ENABLE_MQTT_BROKER` enable [MQTT broker](https://docs.cesanta.com/mongoose/master/#/mqtt/server_example.md/)
- `MG_ENABLE_DNS_SERVER` enable DNS server
- `MG_ENABLE_COAP` enable CoAP protocol
- `MG_ENABLE_HTTP` Enable HTTP protocol support (on by default, set to 0 to disable)
- `MG_ENABLE_HTTP_CGI` Enable [CGI](https://docs.cesanta.com/mongoose/master/#/http/cgi.md/) support
- `MG_ENABLE_HTTP_SSI` Enable [Server Side Includes](https://docs.cesanta.com/mongoose/master/#/http/ssi.md/) support
- `MG_ENABLE_HTTP_SSI_EXEC` Enable SSI `exec` operator
- `MG_ENABLE_HTTP_WEBDAV` enable WebDAV extensions to HTTP
- `MG_ENABLE_HTTP_WEBSOCKET` enable WebSocket extension to HTTP (on by default, =0 to disable)
- `MG_ENABLE_BROADCAST` enable `mg_broadcast()` API
- `MG_ENABLE_GETADDRINFO` enable `getaddrinfo()` in `mg_resolve2()`
- `MG_ENABLE_THREADS` enable `mg_start_thread()` API
