
**NOTE: THIS MANUAL IS WORK IN PROGRESS**

## Overview

Mongoose is small and easy to use web server. It is self-contained, and does
not require any external software to run.

On Windows, mongoose iconifies itself to the system tray icon when started.
Right-click on the icon pops up a menu, where it is possible to stop
mongoose, or configure it, or install it as Windows service.

On UNIX and Mac, mongoose is a command line utility. Running `mongoose` in
terminal, optionally followed by configuration parameters
(`mongoose [OPTIONS]`) or configuration file name
(`mongoose [config_file_name]`) starts the
web server. Mongoose does not detach from terminal. Pressing `Ctrl-C` keys
would stop the server.

When started, mongoose first searches for the configuration file.
If configuration file is specified explicitly in the command line, i.e.
`mongoose path_to_config_file`, then specified configuration file is used.
Otherwise, mongoose would search for file `mongoose.conf` in the same directory
where binary is located, and use it.
Configuration file is a sequence of lines, each line containing
command line argument name and it's value. Empty lines, and lines beginning
with `#`, are ignored. Here is the example of `mongoose.conf` file:

    # mongoose.conf file
    document_root c:\www
    listening_ports 8080,8043s
    ssl_certificate c:\mongoose\ssl_cert.pem

When configuration file is processed, mongoose process command line arguments,
if they are specified. Command line arguments therefore can override
configuration file settings. Command line arguments must start with `-`.
For example, if `mongoose.conf` has line
`document_root /var/www`, and mongoose has been started as
`mongoose -document_root /etc`, then `/etc` directory will be served as
document root, because command line options take priority over
configuration file.

Mongoose can also be used to modify `.htpasswd` passwords file:

    mongoose -A <htpasswd_file> <realm> <user> <passwd>

## Usage Examples

- How to share a Windows folder: copy mongoose executable to a folder and
  double-click the executable. The folder should be accessible via
  [http://localhost:8080](http://localhost:8080) in any browser.

- How to start mongoose at UNIX startup time in daemon mode, serving
  directory `/var/www`: put this line in the system startup script:
  `/path/to/mongoose -listening_ports 80 -document_root /var/www &`

- How to serve user home directories using URL rewrite:
  `mongoose -url_rewrite_patterns /~joe/=/home/joe/,/~bill=/home/bill/`

## Configuration Options
```
     -C cgi_pattern
         All files that fully match cgi_pattern are treated as CGI.
         Default pattern allows CGI files be anywhere. To restrict CGIs to
         certain directory, use e.g. "-C /cgi-bin/**.cgi".  Default:
         "**.cgi$|**.pl$|**.php$"

     -E cgi_environment
         Extra environment variables to be passed to the CGI script in
         addition to standard ones. The list must be comma-separated list
         of X=Y pairs, like this: "VARIABLE1=VALUE1,VARIABLE2=VALUE2".
         Default: ""

     -G put_delete_passwords_file
         PUT and DELETE passwords file. This must be specified if PUT or
         DELETE methods are used. Default: ""

     -I cgi_interpreter
         Use cgi_interpreter as a CGI interpreter for all CGI scripts
         regardless script extension.  Mongoose decides which interpreter
         to use by looking at the first line of a CGI script.  Default:
         "".

     -P protect_uri
         Comma separated list of URI=PATH pairs, specifying that given
         URIs must be protected with respected password files. Default: ""

     -R authentication_domain
         Authorization realm. Default: "mydomain.com"

     -S ssi_pattern
         All files that fully match ssi_pattern are treated as SSI.
         Unknown SSI directives are silently ignored. Currently, two SSI
         directives are supported, "include" and "exec".  Default:
         "**.shtml$|**.shtm$"

     -T throttle
         Limit download speed for clients.  throttle is a comma-separated
         list of key=value pairs, where key could be a '*' character
         (limit for all connections), a subnet in form x.x.x.x/mask (limit
         for a given subnet, for example 10.0.0.0/8), or an URI prefix
         pattern (limit for the set of URIs, for example /foo/**). The
         value is a floating-point number of bytes per second, optionally
         followed by a `k' or `m' character, meaning kilobytes and
         megabytes respectively. A limit of 0 means unlimited rate. The
         last matching rule wins. For example, "*=1k,10.0.0.0/8" means
         limit everybody to 1 kilobyte per second, but give people from
         10/8 subnet unlimited speed. Default: ""

     -a access_log_file
         Access log file. Default: "", no logging is done.

     -d enable_directory_listing
         Enable/disable directory listing. Default: "yes"

     -e error_log_file
         Error log file. Default: "", no errors are logged.

     -g global_passwords_file
         Location of a global passwords file. If set, per-directory
         .htpasswd files are ignored, and all requests must be authorised
         against that file.  Default: ""

     -i index_files
         Comma-separated list of files to be treated as directory index
         files.  Default: "index.html,index.htm,index.cgi"

     -l access_control_list
         Specify access control list (ACL). ACL is a comma separated list
         of IP subnets, each subnet is prepended by '-' or '+' sign. Plus
         means allow, minus means deny. If subnet mask is omitted, like
         "-1.2.3.4", then it means single IP address. Mask may vary from 0
         to 32 inclusive. On each request, full list is traversed, and
         last match wins. Default setting is to allow all. For example, to
         allow only 192.168/16 subnet to connect, run "mongoose
         -0.0.0.0/0,+192.168/16".  Default: ""

     -m extra_mime_types
         Extra mime types to recognize, in form "extension1=type1,exten-
         sion2=type2,...". Extension must include dot.  Example: "mongoose
         -m .cpp=plain/text,.java=plain/text". Default: ""

     -p listening_ports
         Comma-separated list of ports to listen on. If the port is SSL, a
         letter 's' must be appeneded, for example, "-p 80,443s" will open
         port 80 and port 443, and connections on port 443 will be SSL-ed.
         It is possible to specify an IP address to bind to. In this case,
         an IP address and a colon must be prepended to the port number.
         For example, to bind to a loopback interface on port 80 and to
         all interfaces on HTTPS port 443, use "mongoose -p
         127.0.0.1:80,443s". Default: "8080"

     -r document_root
         Location of the WWW root directory. Default: "."

     -s ssl_certificate
         Location of SSL certificate file. Default: ""

     -t num_threads
         Number of worker threads to start. Default: "10"

     -u run_as_user
         Switch to given user's credentials after startup. Default: ""

     -w url_rewrite_patterns
         Comma-separated list of URL rewrites in the form of "pattern=sub-
         stitution,..." If the "pattern" matches some prefix of the
         requested URL, then matched prefix gets substituted with "substi-
         tution".  For example, "-w /con-
         fig=/etc,**.doc|**.rtf=/path/to/cgi-bin/handle_doc.cgi" will
         serve all URLs that start with "/config" from the "/etc" direc-
         tory, and call handle_doc.cgi script for .doc and .rtf file
         requests. If some pattern matches, no further matching/substitu-
         tion is performed (first matching pattern wins). Use full paths
         in substitutions. Default: ""

     -x hide_files_patterns
         A prefix pattern for the files to hide. Files that match the pat-
         tern will not show up in directory listing and return 404 Not
         Found if requested. Default: ""

```

## Common Problems
---------------
- PHP doesn't work - getting empty page, or 'File not found' error. The
  reason for that is wrong paths to the interpreter. Remember that with PHP,
  correct interpreter is `php-cgi.exe` (`php-cgi` on UNIX). Solution: specify
  full path to the PHP interpreter, e.g.
  `mongoose -cgi_interpreter /full/path/to/php-cgi`

## Embedding
Embedding Mongoose is easy. Somewhere in the application code, `mg_start()`
function must be called. That starts the web server in a separate thread.
When it is not needed anymore, `mg_stop()` must be called.  Application code
can pass configuration options to `mg_start()`, and also specify callback
functions that Mongoose should call at certain events.
[hello.c](http://a.c) provides a minimalistic example.

Common pattern is to implement `begin_request` callback, and serve static files
from memory, and/or construct dynamic replies on the fly. Here is
my [embed.c](https://gist.github.com/valenok/4714740) gist
that shows how to easily any data can be embedded
directly into the executable. If such data needs to be encrypted, then
encrypted database or encryption dongles would be a better choice.


## Other Resources
- Presentation made by Arnout Vandecappelle at FOSDEM 2011 on 2011-02-06
  in Brussels, Belgium, called
  "Creating secure web based user interfaces for Embedded Devices"
  ([pdf](http://mind.be/content/110206_Web-ui.pdf) |
   [odp](http://mind.be/content/110206_Web-ui.odp))
- Linux Journal article by Michel J.Hammel, 2010-04-01, called
  [Mongoose: an Embeddable Web Server in C](http://www.linuxjournal.com/article/10680)
