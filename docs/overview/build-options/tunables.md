---
title: Tunables
---

- `MG_MALLOC`, `MG_CALLOC`, `MG_REALLOC`, `MG_FREE` allow you to a use custom
  memory allocator, e.g. `-DMG_MALLOC=my_malloc`
- `MG_USE_READ_WRITE` when set replaces calls to `recv` with `read` and `send` with `write`,
  thus enabling to add any kind of file descriptor (files, serial devices) to an event manager.
- `MG_SSL_CRYPTO_MODERN`, `MG_SSL_CRYPTO_OLD` - choose either "Modern" or "Old" ciphers
  instead of the default "Intermediate" setting.
  See [this article](https://wiki.mozilla.org/Security/Server_Side_TLS#Recommended_configurations) for details.
