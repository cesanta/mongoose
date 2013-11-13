# Mongoose Configuration Options

Every option is followed by it's default value.
If default value is not present, then it is empty.

### cgi_pattern `**.cgi$|**.pl$|**.php$`
All files that match `cgi_pattern` are treated as CGI files. Default pattern
allows CGI files be anywhere. To restrict CGIs to a certain directory,
use `/path/to/cgi-bin/**.cgi` as pattern. Note that full file path is
matched against the pattern, not the URI.

### cgi_environment
Extra environment variables to be passed to the CGI script in
addition to standard ones. The list must be comma-separated list
of name=value pairs, like this: `VARIABLE1=VALUE1,VARIABLE2=VALUE2`.

### put\_delete\_auth\_file
Passwords file for PUT and DELETE requests. Without it, PUT and DELETE requests
will fail.

### cgi_interpreter
Path to an executable to use as CGI interpreter for __all__ CGI scripts
regardless script extension. If this option is not set (which is a default),
Mongoose looks at first line of a CGI script,
[shebang line](http://en.wikipedia.org/wiki/Shebang_(Unix\)),
for an interpreter.

For example, if both PHP and perl CGIs are used, then
`#!/path/to/php-cgi.exe` and `#!/path/to/perl.exe` must be first lines of the
respective CGI scripts. Note that paths should be either full file paths,
or file paths relative to the current working directory of mongoose server.
If mongoose is started by mouse double-click on Windows, current working
directory is a directory where mongoose executable is located.

If all CGIs use the same interpreter, for example they are all PHP, then
`cgi_interpreter` can be set to the path to `php-cgi.exe` executable and
shebang line in the CGI scripts can be omitted.
Note that PHP scripts must use `php-cgi.exe` executable, not `php.exe`.

### protect_uri
Comma separated list of URI=PATH pairs, specifying that given
URIs must be protected with respected password files. Paths must be full
file paths.

### authentication_domain `mydomain.com`
Authorization realm used in `.htpasswd` authorization.

### ssi_pattern `**.shtml$|**.shtm$`
All files that match `ssi_pattern` are treated as SSI.

Server Side Includes (SSI) is a simple interpreted server-side scripting
language which is most commonly used to include the contents of a file into
a web page. It can be useful when it is desirable to include a common piece
of code throughout a website, for example, headers and footers.

In order for a webpage to recognize an SSI-enabled HTML file, the filename
should end with a special extension, by default the extension should be
either `.shtml` or `.shtm`.

Unknown SSI directives are silently ignored by mongoose. Currently, two SSI
directives are supported, `<!--#include ...>` and
`<!--#exec "command">`. Note that `<!--#include ...>` directive supports
three path specifications:

    <!--#include virtual="path">  Path is relative to web server root
    <!--#include abspath="path">  Path is absolute or relative to
                                  web server working dir
    <!--#include file="path">,    Path is relative to current document
    <!--#include "path">

The `include` directive may be used to include the contents of a file or the
result of running a CGI script. The `exec` directive is used to execute a
command on a server, and show command's output. Example:

    <!--#exec "ls -l" -->

For more information on Server Side Includes, take a look at the Wikipedia:
[Server Side Includes](http://en.wikipedia.org/wiki/Server_Side_Includes)

### throttle
Limit download speed for clients.  `throttle` is a comma-separated
list of key=value pairs, where key could be:

    *                   limit speed for all connections
    x.x.x.x/mask        limit speed for specified subnet
    uri_prefix_pattern  limit speed for given URIs

The value is a floating-point number of bytes per second, optionally
followed by a `k` or `m` character, meaning kilobytes and
megabytes respectively. A limit of 0 means unlimited rate. The
last matching rule wins. Examples:

    *=1k,10.0.0.0/8=0   limit all accesses to 1 kilobyte per second,
                        but give connections from 10.0.0.0/8 subnet
                        unlimited speed

    /downloads/=5k      limit accesses to all URIs in `/downloads/` to
                        5 kilobytes per secods. All other accesses are unlimited

### access\_log\_file
Path to a file for access logs. Either full path, or relative to current
working directory. If absent (default), then accesses are not logged.

### error\_log\_file
Path to a file for error logs. Either full path, or relative to current
working directory. If absent (default), then errors are not logged.

### enable\_directory\_listing `yes`
Enable directory listing, either `yes` or `no`.

### enable\_keep\_alive `no`
Enable connection keep alive, either `yes` or `no`.

Experimental feature. Allows clients to reuse TCP connection for
subsequent HTTP requests, which improves performance.
For this to work when using request handlers it's important to add the correct
Content-Length HTTP header for each request. If this is forgotten the client
will time out.


### global\_auth\_file
Path to a global passwords file, either full path or relative to the current
working directory. If set, per-directory `.htpasswd` files are ignored,
and all requests are authorised against that file.

The file has to include the realm set through `authentication_domain` and the password in digest format:

    user:realm:digest
    test:test.com:ce0220efc2dd2fad6185e1f1af5a4327

(e.g. use [this generator](http://www.askapache.com/online-tools/htpasswd-generator))

### index_files `index.html,index.htm,index.cgi,index.shtml,index.php`
Comma-separated list of files to be treated as directory index
files.

### access\_control\_list
An Access Control List (ACL) allows restrictions to be put on the list of IP
addresses which have access to the web server. In the case of the Mongoose
web server, the ACL is a comma separated list of IP subnets, where each
subnet is prepended by either a `-` or a `+` sign. A plus sign means allow,
where a minus sign means deny. If a subnet mask is omitted, such as `-1.2.3.4`,
this means to deny only that single IP address.

Subnet masks may vary from 0 to 32, inclusive. The default setting is to allow
all accesses. On each request the full list is traversed, and
the last match wins. Examples:

    -0.0.0.0/0,+192.168/16    deny all acccesses, only allow 192.168/16 subnet

To learn more about subnet masks, see the
[Wikipedia page on Subnetwork](http://en.wikipedia.org/wiki/Subnetwork)

### extra\_mime\_types
Extra mime types to recognize, in form `extension1=type1,exten-
sion2=type2,...`. Extension must include dot.  Example:
`.cpp=plain/text,.java=plain/text`

### listening_ports `8080`
Comma-separated list of ports to listen on. If the port is SSL, a
letter `s` must be appeneded, for example, `80,443s` will open
port 80 and port 443, and connections on port 443 will be SSL-ed.
For non-SSL ports, it is allowed to append letter `r`, meaning 'redirect'.
Redirect ports will redirect all their traffic to the first configured
SSL port. For example, if `listening_ports` is `80r,443s`, then all
HTTP traffic coming at port 80 will be redirected to HTTPS port 443.

It is possible to specify an IP address to bind to. In this case,
an IP address and a colon must be prepended to the port number.
For example, to bind to a loopback interface on port 80 and to
all interfaces on HTTPS port 443, use `127.0.0.1:80,443s`.

### document_root `.`
A directory to serve. By default, currect directory is served. Current
directory is commonly referenced as dot (`.`).

### ssl_certificate
Path to SSL certificate file. This option is only required when at least one
of the `listening_ports` is SSL. The file must be in PEM format,
and it must have both private key and certificate, see for example
[ssl_cert.pem](https://github.com/cesanta/mongoose/blob/master/build/ssl_cert.pem)

### num_threads `50`
Number of worker threads. Mongoose handles each incoming connection in a
separate thread. Therefore, the value of this option is effectively a number
of concurrent HTTP connections Mongoose can handle.

### run\_as\_user
Switch to given user credentials after startup. Usually, this option is
required when mongoose needs to bind on privileged port on UNIX. To do
that, mongoose needs to be started as root. But running as root is a bad idea,
therefore this option can be used to drop privileges. Example:

    mongoose -listening_ports 80 -run_as_user nobody

### request\_timeout\_ms `30000`
Timeout for network read and network write operations, in milliseconds.
If client intends to keep long-running connection, either increase this value
or use keep-alive messages.


### url\_rewrite\_patterns
Comma-separated list of URL rewrites in the form of
`uri_pattern=file_or_directory_path`. When Mongoose receives the request,
it constructs the file name to show by combining `document_root` and the URI.
However, if the rewrite option is used and `uri_pattern` matches the
requested URI, then `document_root` is ignored. Insted,
`file_or_directory_path` is used, which should be a full path name or
a path relative to the web server's current working directory. Note that
`uri_pattern`, as all mongoose patterns, is a prefix pattern.

This makes it possible to serve many directories outside from `document_root`,
redirect all requests to scripts, and do other tricky things. For example,
to redirect all accesses to `.doc` files to a special script, do:

    mongoose -url_rewrite_patterns **.doc$=/path/to/cgi-bin/handle_doc.cgi

Or, to imitate user home directories support, do:

    mongoose -url_rewrite_patterns /~joe/=/home/joe/,/~bill=/home/bill/

### hide\_files\_patterns
A pattern for the files to hide. Files that match the pattern will not
show up in directory listing and return `404 Not Found` if requested. Pattern
must be for a file name only, not including directory name. Example:

    mongoose -hide_files_patterns secret.txt|even_more_secret.txt


