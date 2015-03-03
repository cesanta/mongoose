# Mongoose Embedding Guide

Embedding Mongoose is done in two steps:

   1. Copy
    [mongoose.c](https://raw.github.com/cesanta/mongoose/master/mongoose.c) and
    [mongoose.h](https://raw.github.com/cesanta/mongoose/master/mongoose.h)
    to your application's source tree and include them in the build.
   2. Somewhere in the application code, call `mg_create_server()` to create
    a server, configure it with `mg_set_option()` and loop with
    `mg_poll_server()` until done. Call `mg_destroy_server()` to cleanup.

Here's a minimal application `app.c` that embeds mongoose:

    #include "mongoose.h"

    int main(void) {
      struct mg_server *server = mg_create_server(NULL, NULL);
      mg_set_option(server, "document_root", ".");      // Serve current directory
      mg_set_option(server, "listening_port", "8080");  // Open port 8080

      for (;;) {
        mg_poll_server(server, 1000);   // Infinite loop, Ctrl-C to stop
      }
      mg_destroy_server(&server);

      return 0;
    }

To compile it, put `mongoose.c`, `mongoose.h` and `app.c` into one
folder, start terminal on UNIX or Visual Studio command line prompt on Windows,
and run the following command:

    cc app.c mongoose.c -pthread -o app     # on Unix
    cl.exe app.c mongoose.c /TC /MD         # on Windows

When run, this simple application opens port 8080 and serves static files,
CGI files and lists directory content in the current working directory.

It is possible to generate HTML page content. Mongoose can call user-defined
function when certain events occur.
That function is called _an event handler_, and it is the second parameter
to `mg_create_server()` function. Here is the example event handler function:

    int event_handler(struct mg_connection *conn, enum mg_event ev) {
      switch (ev) {
        case MG_AUTH: return MG_TRUE;
        default: return MG_FALSE;
      }
    }

Event handler is called by Mongoose with `struct mg_connection *`
pointer and an event number. `struct mg_connection *conn`
has all information about the request: HTTP headers, POST or websocket
data buffer, etcetera. `enum mg_event ev` tells which exactly event is sent.
For each event, an event handler returns a value which tells Mongoose how
to behave.

The sequence of events for every connection is this:

   * `MG_AUTH` - Mongoose asks whether this connection is authorized. If event
      handler returns `MG_FALSE`, then Mongoose does not serve the request but
      sends authorization request to the client. If `MG_TRUE` is returned,
      then Mongoose continues on with the request.
   * `MG_REQUEST` - Mongoose asks event handler to serve the request. If
      event handler serves the request by sending a reply,
      it should return `MG_TRUE`. Otherwise,
      it should return `MG_FALSE` which tells Mongoose that request is not
      served and Mongoose should serve it. For example, event handler might
      choose to serve only RESTful API requests with URIs that start with
      certain prefix, and let Mongoose serve all static files.
      If event handler decides to serve the request, but doesn't have
      all the data at the moment, it should return `MG_MORE`. That tells
      Mongoose to keep the connection open after callback returns.

      `mg_connection::connection_param` pointer is a placeholder to keep
      user-specific data. For example, handler could decide to open a DB
      connection and store DB connection handle in `connection_param`.
   * `MG_POLL` is sent to every connection on every iteration of
      `mg_poll_server()`. Event handler should return `MG_FALSE` to ignore
      this event. If event handler returns `MG_TRUE`, then Mongoose assumes
      that event handler has finished sending data, and Mongoose will
      close the connection.
   * `MG_HTTP_ERROR` sent when Mongoose is about to send HTTP error back
      to the client. Event handler can choose to send a reply itself, in which
      case event handler must return `MG_TRUE`. Otherwise, event handler must
      return `MG_FALSE`
   * `MG_CLOSE` is sent when the connection is closed. This event is used
      to cleanup per-connection state stored in `connection_param`
      if it was allocated.

Let's extend our minimal application example and
create an URI that will be served by user's C code. The app will handle
`/hello` URI by showing a hello message. So, when app is run,
http://127.0.0.1:8080/hello will say hello, and here's the code:

    #include <string.h>
    #include "mongoose.h"

    static int event_handler(struct mg_connection *conn, enum mg_event ev) {
      if (ev == MG_AUTH) {
        return MG_TRUE;   // Authorize all requests
      } else if (ev == MG_REQUEST && !strcmp(conn->uri, "/hello")) {
        mg_printf_data(conn, "%s", "Hello world");
        return MG_TRUE;   // Mark as processed
      } else {
        return MG_FALSE;  // Rest of the events are not processed
      }
    }

    int main(void) {
      struct mg_server *server = mg_create_server(NULL, event_handler);
      mg_set_option(server, "document_root", ".");
      mg_set_option(server, "listening_port", "8080");

      for (;;) {
        mg_poll_server(server, 1000);  // Infinite loop, Ctrl-C to stop
      }
      mg_destroy_server(&server);

      return 0;
    }

## Example code

Mongoose source code contains number of examples, located in the
[examples](https://github.com/cesanta/mongoose/blob/master/examples/) directory.
To build any example, go to the respective directory and run `make`.

## Compilation flags

Below is the list of compilation flags that enable or disable certain
features. By default, some features are enabled, and could be disabled
by setting appropriate `NO_*` flag. Features that are disabled by default
could be enabled by setting appropriate `USE_*` flag. Bare bones Mongoose
is quite small, about 30 kilobytes of compiled x86 code. Each feature adds
a couple of kilobytes to the executable size, and also has some runtime penalty.

Note that some flags start with `NS_` prefix. This is because Mongoose uses
[Net Skeleton](http://github.com/cesanta/net_skeleton) as a low-level
networking engine. If user code has `#include <net_skeleton.h>`, then
all Net Skeleton functions will be available too.


    -DMONGOOSE_NO_AUTH          Disable MD5 authorization support
    -DMONGOOSE_NO_CGI           Disable CGI support
    -DMONGOOSE_NO_DAV           Disable WebDAV support
                                (PUT, DELETE, MKCOL, PROPFIND methods)
    -DMONGOOSE_NO_DIRECTORY_LISTING  Disable directory listing
    -DMONGOOSE_NO_FILESYSTEM    Disables all file IO, serving from memory only
    -DMONGOOSE_NO_LOGGING       Disable access/error logging
    -DMONGOOSE_ENABLE_THREADS   Enable mg_start_thread() function
    -DMONGOOSE_NO_WEBSOCKET     Disable WebSocket support
    -DMONGOOSE_NO_USER          No concept of a user on used platform.
                                (Platform does not provide getpwnam, setgid or setuid)

    -DMONGOOSE_USE_IDLE_TIMEOUT_SECONDS=X Idle connection timeout, default is 30
    -DMONGOOSE_USE_LUA          Enable Lua scripting
    -DMONGOOSE_USE_LUA_SQLITE3  Enable sqlite3 binding for Lua
    -DMONGOOSE_USE_POST_SIZE_LIMIT=X      POST requests larger than X will be
                                          rejected, not set by default
    -DMONGOOSE_USE_EXTRA_HTTP_HEADERS=X   Append X to the HTTP headers
                                          for static files, empty by default

    -DNS_ENABLE_DEBUG         Enables debug messages on stdout, very noisy
    -DNS_ENABLE_SSL           Enable SSL
    -DNS_ENABLE_IPV6          Enable IPv6 support
    -DNS_ENABLE_HEXDUMP       Enables hexdump of sent and received traffic
    -DNS_STACK_SIZE=X         Sets stack size to X for  ns_start_thread()
    -DNS_DISABLE_THREADS      Disable threads support
    -DNS_DISABLE_SOCKETPAIR   For systems without loopback interface
    -DMONGOOSE_SEND_NS_EVENTS Send Net Skeleton events to the event handler
                              in addition to the Mongoose events
