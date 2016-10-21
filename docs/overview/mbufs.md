---
title: Memory buffers
---

Each connection has a send and receive buffer, `struct mg_connection::send_mbuf`
and `struct mg_connection::recv_mbuf` respectively. When data arrives,
Mongoose appends received data to the `recv_mbuf` and triggers an `MG_EV_RECV`
event. The user may send data back by calling one of the output functions, like
`mg_send()` or `mg_printf()`. Output functions append data to the `send_mbuf`.
When Mongoose successfully writes data to the socket, it discards data from
`struct mg_connection::send_mbuf` and sends an `MG_EV_SEND` event. When the connection
is closed, an `MG_EV_CLOSE` event is sent.

![](media/mbuf.png)
