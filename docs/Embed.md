# Mongoose Embedding Guide

Embedding Mongoose is done in two steps:

   1. Copy
    [mongoose.c](https://raw.github.com/cesanta/mongoose/master/mongoose.c) and
    [mongoose.h](https://raw.github.com/cesanta/mongoose/master/mongoose.h)
    to your application's source tree and include these two files in the build.
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
folder, then run the following UNIX command:

    cc app.c mongoose.c -o app

If you're on Windows, run this in a Visual Studio shell:

    cl app.c mongoose.c /TC /MD

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
      static const char *reply = "HTTP/1.0 200 OK\r\n\r\nHello world!\n";
      mg_write(conn, reply, strlen(reply));
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

Note that URI handler must output valid HTTP response, which includes
the reply line with status code `HTTP/1.0 200 OK`, HTTP headers which are
empty in our example, and message body `Hello world!\n`. Note that reply
line is ended with `\r\n`, and HTTP headers are also ended with `\r\n`.

Mongoose source code contains a well-commented example code, listed below:

   * [hello.c](https://github.com/cesanta/mongoose/blob/master/examples/hello.c)
   shows how to handle form input, file upload, websocket communication, get
   cookie values.

   * [chat](https://github.com/cesanta/mongoose/blob/master/examples/chat)
   implements basic online chat functionality using Lua scripting capabilities
   of Mongoose. Not a single line of C is written for that example.
   Demostrates usage of database, cookie-based authentication, session
   support, RESTful interface. No additional software is required to run it
   on any platform.
