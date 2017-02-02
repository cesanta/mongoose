---
title: Overview
---

Mongoose uses non-blocking DNS resolver. For each name to be resolved,
it first checks the `/etc/hosts` file (or, `hosts` on Windows).
If the entry is not found there, then the `8.8.8.8` DNS server is queried.
When IP address is found, Mongoose proceeds with making the connection
with the resolved IP address.
