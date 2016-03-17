---
title: Memory buffers
---

Each connection has send and receive buffer,
[`struct mg_connection::send_mbuf`](#TODO)
and
[`struct mg_connection::recv_mbuf`](#ODO) respectively.
When data arrives,
Mongoose appends received data to the `recv_mbuf` and
triggers `MG_EV_RECV` event. User may send data back by calling one of the
output functions, like [`mg_send()`](#TODO) or
[`mg_printf()`](#TODO). Output functions append data to the
`send_mbuf`. When Mongoose
successfully writes data to the socket, it discards data from
[`mg_connection::send_mbuf`](#TODO) and
sends `MG_EV_SEND` event. When connection is closed, `MG_EV_CLOSE` event is sent.

![](../../static/img/mongoose/mbuf.png)

