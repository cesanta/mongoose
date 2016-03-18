---
title: Connection flags
---

Each connection has a `flags` bit field. Some flags are set by Mongoose, for
example if a user creates an outbound UDP connection using `udp://1.2.3.4:5678`
address, Mongoose is going to set `MG_F_UDP` flag for that connection. Other
flags are meant to be set only by user event handler to tell Mongoose how to
behave.  Below is a list of connection flags that are meant to be set by event
handlers:

* `MG_F_FINISHED_SENDING_DATA` tells Mongoose that all data has been appended
  to the `send_mbuf`. As soon as Mongoose sends it to the socket, the
  connection will be closed.
* `MG_F_BUFFER_BUT_DONT_SEND` tells Mongoose to append data to the `send_mbuf`
  but hold on sending it, because the data will be modified later and then will
  be sent by clearing `MG_F_BUFFER_BUT_DONT_SEND` flag.
* `MG_F_CLOSE_IMMEDIATELY` tells Mongoose to close the connection immediately,
  usually after some error
* `MG_F_USER_1`, `MG_F_USER_2`, `MG_F_USER_3`, `MG_F_USER_4` could be used by a
  developer to store application-specific state

Flags below are set by Mongoose:

* `MG_F_SSL_HANDSHAKE_DONE` SSL only, set when SSL handshake is done.
* `MG_F_CONNECTING` set when connection is in connecting state after
  `mg_connect()` call but connect did not finish yet.
* `MG_F_LISTENING` set for all listening connections
* `MG_F_UDP` set if connection is UDP
* `MG_F_IS_WEBSOCKET` set if connection is a Websocket connection
* `MG_F_WEBSOCKET_NO_DEFRAG` should be set by a user if user wants to switch
  off automatic Websocket frame defragmentation
