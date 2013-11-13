# Mongoose Lua Server Pages

Pre-built Windows and Mac mongoose binaries have built-in Lua Server Pages
support. That means it is possible to write PHP-like scripts with mongoose,
using Lua programming language instead of PHP. Lua is known
for it's speed and small size. Mongoose uses Lua version 5.2.1, the
documentation for it can be found at
[Lua 5.2 reference manual](http://www.lua.org/manual/5.2/).

To create a Lua Page, make sure a file has `.lp` extension. For example,
let's say it is going to be `my_page.lp`. The contents of the file, just like
with PHP, is HTML with embedded Lua code. Lua code must be enclosed in
`<?  ?>` blocks, and can appear anywhere on the page. For example, to
print current weekday name, one can write:

    <p>
      <span>Today is:</span>
      <? mg.write(os.date("%A")) ?>
    </p>

Note that this example uses function `mg.write()`, which prints data to the
web page. Using function `mg.write()` is the way to generate web content from
inside Lua code. In addition to `mg.write()`, all standard library functions
are accessible from the Lua code (please check reference manual for details),
and also information about the request is available in `mg.request_info` object,
like request method, all headers, etcetera. Please refer to
`struct mg_request_info` definition in
[mongoose.h](https://github.com/cesanta/mongoose/blob/master/mongoose.h)
to see what kind of information is present in `mg.request_info` object. Also,
[page.lp](https://github.com/cesanta/mongoose/blob/master/test/page.lp) and
[prime_numbers.lp](https://github.com/cesanta/mongoose/blob/master/examples/lua/prime_numbers.lp)
contains some example code that uses `request_info` and other functions(form submitting for example).

Mongoose exports the following to the Lua server page:

    mg.read()         -- reads a chunk from POST data, returns it as a string
    mg.write(str)     -- writes string to the client
    mg.include(path)  -- sources another Lua file
    mg.redirect(uri)  -- internal redirect to a given URI
    mg.onerror(msg)   -- error handler, can be overridden
    mg.version        -- a string that holds Mongoose version
    mg.request_info   -- a table with request information

    -- Connect to the remote TCP server. This function is an implementation
    -- of simple socket interface. It returns a socket object with three
    -- methods: send, recv, close, which are synchronous (blocking).
    -- connect() throws an exception on connection error.
    connect(host, port, use_ssl)

    -- Example of using connect() interface:
    local host = 'code.google.com'  -- IP address or domain name
    local ok, sock = pcall(connect, host, 80, 1)
    if ok then
      sock:send('GET /p/mongoose/ HTTP/1.0\r\n' ..
                'Host: ' .. host .. '\r\n\r\n')
      local reply = sock:recv()
      sock:close()
      -- reply now contains the web page https://code.google.com/p/mongoose
    end


**IMPORTANT: Mongoose does not send HTTP headers for Lua pages. Therefore,
every Lua Page must begin with HTTP reply line and headers**, like this:

    <? print('HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n') ?>
    <html><body>
      ... the rest of the web page ...

To serve Lua Page, mongoose creates Lua context. That context is used for
all Lua blocks within the page. That means, all Lua blocks on the same page
share the same context. If one block defines a variable, for example, that
variable is visible in the block that follows.


