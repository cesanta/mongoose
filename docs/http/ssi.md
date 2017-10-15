# SSI

Server Side Includes (SSI) is a simple interpreted server-side scripting
language which is most commonly used to include the contents of a file
into a web page. It can be useful when it is desirable to include a common
piece of code throughout a website, for example, headers and footers.

In order to use SSI, call `mg_serve_http()` function and use
`.shtml` file extension for the SSI files. To be more precise,
all files that match `ssi_pattern` setting in the
`struct mg_serve_http_opts` are treated as SSI.
If `ssi_pattern` is NULL, `**.shtml$|**.shtm$` is used.

Unknown SSI directives are silently ignored by Mongoose. Currently,
the following SSI directives are supported:

- `<!--#include FILE_TO_INCLUDE -->` - inject the content of some other file
- `<!--#exec "COMMAND_TO_EXECUTE" -->` - runs a command and inject the output
- `<!--#call COMMAND -->` - triggers `MG_EV_SSI_CALL` event

Note that `<!--#include ... -->` directive supports three path specifications:

- `<!--#include virtual="path" -->`  Path is relative to web server root
- `<!--#include abspath="path" -->`  Path is absolute or relative to the
  web server working dir
- `<!--#include file="path" -->`, `<!--#include "path" -->`
  Path is relative to current document

The include directive may be used to include the contents of a file or
the result of running a CGI script.

The exec directive is used to execute a command on a server,
and show command's output. Example: `<!--#exec "ls -l" -->`

The call directive is a way to invoke a C handler from the HTML page.
On each occurrence of `<!--#call PARAMS -->` directive,
Mongoose calls a registered event handler with `MG_EV_SSI_CALL` event.
Event parameter will point to the `PARAMS` string.
An event handler can output any text, for example by calling
`mg_printf()`. This is a flexible way of generating a web page on
server side by calling a C event handler. Example:

`<!--#call foo -->  <!--#call bar -->`

In the event handler:

```c
   case MG_EV_SSI_CALL: {
     const char *param = (const char *) ev_data;
     if (strcmp(param, "foo") == 0) {
       mg_printf(c, "hello from foo");
     } else if (strcmp(param, "bar") == 0) {
       mg_printf(c, "hello from bar");
     }
     break;
   }
```
