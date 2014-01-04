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
      struct mg_server *server = mg_create_server(NULL);
      mg_set_option(server, "document_root", ".");
      mg_set_option(server, "listening_port", "8080");
      for (;;) mg_poll_server(server, 1000);  // Infinite loop, Ctrl-C to stop
      mg_destroy_server(&server);
      return 0;
    }

To compile it, put `mongoose.c`, `mongoose.h` and `minimal.c` into one
folder, start terminal on UNIX or Visual Studio command line prompt on Windows,
and run the following command:

    cc app.c mongoose.c -o app    # on Unix
    cl app.c mongoose.c /TC /MD   # on Windows

When run, this simple application opens port 8080 and serves static files,
CGI files and lists directory content in the current working directory.

Mongoose can call user-defined functions when certain URIs are requested.
These functions are _called uri handlers_.  `mg_add_uri_handler()` registers
an URI handler, and there is no restriction exist on the number of URI handlers.
Also, mongoose can call a user-defined function when it is about to send
HTTP error back to client. That function is called _http error handler_ and
can be registered by `mg_set_http_error_handler()`. Handlers are called
by Mongoose with `struct mg_connection *` pointer as a parameter, which
has all information about the request: HTTP headers, POST or websocket
data buffer, etcetera.

Let's extend our minimal application example and
create an URI that will be served by user's C code. The app will handle
`/hello` URI by showing a hello message. So, when app is run,
http://127.0.0.1:8080/hello will say hello, and here's the code:

    #include <string.h>
    #include "mongoose.h"

    static int handle_hello(struct mg_connection *conn) {
      static const char *reply = "Hello world!\n";
      mg_write_data(conn, reply, strlen(reply));
      return 1;
    }

    int main(void) {
      struct mg_server *server = mg_create_server(NULL);
      mg_set_option(server, "document_root", ".");
      mg_set_option(server, "listening_port", "8080");
      mg_add_uri_handler(server, "/hello", &handle_hello);
      for (;;) mg_poll_server(server, 1000);  // Infinite loop, Ctrl-C to stop
      mg_destroy_server(&server);
      return 0;
    }

Below is the list of compilation flags that enable or disable certain
features. By default, some features are enabled, and could be disabled
by setting appropriate `NO_*` flag. Features that are disabled by default
could be enabled by setting appropriate `USE_*` flag. Bare bones Mongoose
is quite small, about 30 kilobytes of compiled x86 code. Each feature adds
a couple of kilobytes to the executable size, and also has some runtime penalty.

    -DNO_AUTH         Disable MD5 authorization support
    -DNO_CGI          Disable CGI support
    -DNO_DAV          Disable WebDAV support (PUT, DELETE, MKCOL, PROPFIND)
    -DNO_DIRECTORY_LISTING  Disable directory listing
    -DNO_LOGGING      Disable access/error logging
    -DNO_WEBSOCKET    Disable WebSocket support

    -DUSE_IPV6          Enable IPv6 support
    -DUSE_LUA           Enable Lua scripting
    -DUSE_LUA_SQLITE3   Enable sqlite3 binding for Lua
    -DUSE_SSL           Enable SSL
    -DUSE_POST_SIZE_LIMIT=X     POST requests larger than X will be rejected
    -DUSE_EXTRA_HTTP_HEADERS=X  Append X to the HTTP headers for static files

Mongoose source code contains a well-commented example code, listed below:

   * [hello.c](https://github.com/cesanta/mongoose/blob/master/examples/hello.c)
   is a minimalisting hello world example
   * [post.c](https://github.com/cesanta/mongoose/blob/master/examples/post.c)
   shows how to handle form input
