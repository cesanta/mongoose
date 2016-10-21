---
title: CGI
---

[CGI](https://en.wikipedia.org/wiki/Common_Gateway_Interface)
is a simple mechanism to generate dynamic content.
In order to use CGI, call `mg_serve_http()` function and use
`.cgi` file extension for the CGI files. To be more precise,
all files that match `cgi_file_pattern` setting in the
`struct mg_serve_http_opts` are treated as CGI.
If `cgi_file_pattern` is NULL, `**.cgi$|**.php$` is used.

If Mongoose recognises a file as CGI, it executes it, and sends the output
back to the client. Therefore,
CGI file must be executable. Mongoose honours the shebang line - see
http://en.wikipedia.org/wiki/Shebang_(Unix).

For example, if both PHP and Perl CGIs are used, then
``#!/path/to/php-cgi.exe` and ``#!/path/to/perl.exe` must be the first lines
of the respective CGI scripts.

It is possible to hardcode the path to the CGI interpreter for all
CGI scripts and disregard the shebang line. To do that, set the
`cgi_interpreter` setting in the `struct mg_serve_http_opts`.

NOTE: PHP scripts must use `php-cgi.exe` as CGI interpreter, not `php.exe`.
Example:

```c
  opts.cgi_interpreter = "C:\\ruby\\ruby.exe";
```
