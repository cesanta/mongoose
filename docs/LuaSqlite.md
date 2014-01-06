# Mongoose Lua Server Pages

Pre-built Windows and Mac mongoose binaries support Lua Server Pages
functionality.
That means it is possible to write PHP-like scripts with mongoose
using Lua programming language instead of PHP. Lua is known
for it's speed and small size. Mongoose uses Lua version 5.2.1, the
documentation for it can be found at
[Lua 5.2 reference manual](http://www.lua.org/manual/5.2/).

To create a Lua Page, make a file that is called `<something>.lp`. For example,
let's say it is going to be `my_page.lp`. It is important to have a file
name that ends up with `.lp`, cause this is the way mongoose recognises
Lua Page file. The contents of the file, just like
with PHP, is HTML with embedded Lua code. Lua code must be enclosed within
`<?  ?>` blocks, and can appear anywhere on the page.

Mongoose does not send HTTP headers for Lua pages. Therefore,
every Lua Page must begin with HTTP status line and headers, like this:

    <? mg.write('HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n') ?>
    <html><body>
      <span>Today is:</span> <? mg.write(os.date("%A")) ?>
    </body></html>

Note that this example uses function `mg.write()`, which prints data to the
web page. Using function `mg.write()` is the way to generate web content from
inside Lua code. In addition to `mg.write()`, all standard library functions
are accessible from the Lua code (please check reference manual for details).
Information about the request is available in `mg.request_info` object.
I contains request method, all headers, etcetera. Please refer to
`struct mg_request_info` definition in
[mongoose.h](https://github.com/cesanta/mongoose/blob/master/mongoose.h)
to see what is available in `mg.request_info`. Also,
[page.lp](https://github.com/cesanta/mongoose/blob/master/test/page.lp) and
[prime_numbers.lp](https://github.com/cesanta/mongoose/blob/master/examples/lua/prime_numbers.lp)
contain some example code that uses `request_info` and other functions,
like form submission.

Mongoose exports the following to the Lua server page:

    mg.write(str)     -- writes string to the client
    mg.onerror(msg)   -- error handler, can be overridden
    mg.request_info   -- a table with request information
    sqlite3           -- Sqlite3 interface

slite3 functions are documented at [lua.sqlite.org](http://lua.sqlite.org/),
and usage example is at
[page.lp](https://github.com/cesanta/mongoose/blob/master/test/page.lp).

Using Lua scripting it is easy to emulate SSI functionality. For example,
to include the content of another file, one can write:

    <? mg.write(io.open('MY_FILE.TXT'):read('*all')) ?>

To serve a Lua Page, mongoose creates Lua context. That context is used for
all Lua blocks within the page. That means, all Lua blocks on the same page
share the same context. If one block defines a variable, for example, that
variable is visible in the block that follows.
