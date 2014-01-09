# Mongoose Configuration Options

### access\_control\_list
An Access Control List (ACL) allows restrictions to be put on the list of IP
addresses which have access to the web server. In the case of the Mongoose
web server, the ACL is a comma separated list of IP subnets, where each
subnet is prepended by either a `-` or a `+` sign. A plus sign means allow,
where a minus sign means deny. If a subnet mask is omitted, such as `-1.2.3.4`,
this means to deny only that single IP address.

Subnet masks may vary from 0 to 32, inclusive. The default setting is to allow
all accesses. On each request the full list is traversed, and
the last match wins. Example: `$ mongoose -access_control_list -0.0.0.0/0,+192.168/16` to deny all acccesses except from 192.168/16 subnet. To learn
more about subnet masks, see the
[Wikipedia page on Subnetwork](http://en.wikipedia.org/wiki/Subnetwork)

Default: not set, all accesses are allowed.

### access\_log\_file
Path to a file for access logs. Either full path, or relative to the
mongoose executable. Default: not set, no query logging is done.

### auth_domain
Authorization realm used in `.htpasswd` authorization. Default: `mydomain.com`

### cgi_interpreter
Path to an executable to use as CGI interpreter for __all__ CGI scripts
regardless script extension.
for an interpreter. Default: not set, Mongoose looks at
[shebang line](http://en.wikipedia.org/wiki/Shebang_(Unix\).

For example, if both PHP and perl CGIs are used, then
`#!/path/to/php-cgi.exe` and `#!/path/to/perl.exe` must be first lines of the
respective CGI scripts. Note that paths should be either full file paths,
or file paths relative to the directory where mongoose server is located.

If all CGIs use the same interpreter, for example they are all PHP, then
`cgi_interpreter` option can be set to the path to `php-cgi.exe` executable and
shebang line in the CGI scripts can be omitted.
Note that PHP scripts must use `php-cgi.exe` executable, not `php.exe`.

### cgi_pattern
All files that match `cgi_pattern` are treated as CGI files. Default pattern
allows CGI files be anywhere. To restrict CGIs to a certain directory,
use `/path/to/cgi-bin/**.cgi` as pattern. Note that full file path is
matched against the pattern, not the URI. Note: if `MONGOOSE_CGI` environment
variable is set, then Mongoose passes it to the CGI script.

Default: `**.cgi$|**.pl$|**.php$`

### dav\_auth\_file
Authentication file for WebDAV mutation requests: `PUT`, `DELETE`, `MKCOL`.
The format of that file is the same as for the `.htpasswd` file
used for digest authentication. It can be created and managed by
`mongoose -A` command. Default: not set, WebDAV mutations are disallowed.

### document_root
A directory to serve. Default: current working directory.

### enable\_directory\_listing
Enable directory listing, either `yes` or `no`. Default: `yes`.

### error\_log\_file
Path to a file for error logs. Either full path, or relative to the
mongoose executable. Default: not set, no errors are logged.

### extra\_mime\_types
Extra mime types to recognize, in form `extension1=type1,extension2=type2,...`.
Extension must include dot.  Example:
`mongoose -extra_mime_types .cpp=plain/text,.java=plain/text`. Default: not set.


### global\_auth\_file
Path to a global passwords file, either full path or relative to the mongoose
executable. If set, per-directory `.htpasswd` files are ignored,
and all requests are authorised against that file. Use `mongoose -A` to
manage passwords, or third party utilities like
[htpasswd-generator](http://www.askapache.com/online-tools/htpasswd-generator).
Default: not set, per-directory `.htpasswd` files are respected.

### hide\_files\_patterns
A pattern for the files to hide. Files that match the pattern will not
show up in directory listing and return `404 Not Found` if requested. Pattern
must be for a file name only, not including directory name, e.g.
`mongoose -hide_files_patterns secret.txt|even_more_secret.txt`. Default:
not set.

### idle\_timeout\_ms
Timeout for idle connections. Default: 30000 (30 seconds)

### index_files
Comma-separated list of files to be treated as directory index
files. Default: `index.html,index.htm,index.cgi,index.shtml,index.php`

### ssl_certificate
Path to SSL certificate file. The file must be in PEM format,
and it must have both private key and certificate, see for example
[ssl_cert.pem](https://github.com/cesanta/mongoose/blob/master/build/ssl_cert.pem). If this option is set, then Mongoose serves SSL connections on
`listening_port`. Default: not set.

### listening_port
Port to listen on. Port could be prepended by the specific IP address to bind
to, e.g. `mongoose -listening_port 127.0.0.1:8080`. Otherwise Mongoose
will bind to all addresses.  Default: 8080.

### run\_as\_user
Switch to given user credentials after startup. UNIX-only. This option is
required when mongoose needs to bind on privileged port on UNIX, e.g.

    $ sudo mongoose -listening_ports 80 -run_as_user nobody

Default: not set.

### url\_rewrites
Comma-separated list of URL rewrites in the form of
`uri_pattern=file_or_directory_path`. When Mongoose receives the request,
it constructs the file name to show by combining `document_root` and the URI.
However, if the rewrite option is used and `uri_pattern` matches the
requested URI, then `document_root` is ignored. Insted,
`file_or_directory_path` is used, which should be a full path name or
a path relative to the web server's current working directory. Note that
`uri_pattern`, as all mongoose patterns, is a prefix pattern. If `uri_patters`
is a number, then it is treated as HTTP error code, and `file_or_directory_path`
should be an URI to redirect to. Mongoose will issue `302` temporary redirect
to the specified URI, appending two parameters:
`?code=<http_error_code&orig_uri=<original_uri>`.

This makes it possible to serve many directories outside from `document_root`,
redirect all requests to scripts, and do other tricky things. Examples:

    # Redirect all accesses to `.doc` files to a special script
    mongoose -url_rewrites **.doc$=/path/to/cgi-bin/handle_doc.cgi

    # Implement user home directories support
    mongoose -url_rewrites /~joe/=/home/joe/,/~bill=/home/bill/

    # Redirect 404 errors to a specific error page
    mongoose -url_rewrites 404=/cgi-bin/error.cgi

Default: not set.
