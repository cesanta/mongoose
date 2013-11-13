# Mongoose Usage Guide

Mongoose is small and easy to use web server. It is self-contained, and does
not require any external software to run.

On Windows, mongoose iconifies itself to the system tray icon when started.
Right-click on the icon pops up a menu, where it is possible to stop
mongoose, or configure it, or install it as Windows service. The easiest way
to share a folder on Windows is to copy `mongoose.exe` to a folder,
double-click the exe, and launch a browser at
[http://localhost:8080](http://localhost:8080). Note that 'localhost' should
be changed to a machine's name if a folder is accessed from other computer.

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
where binary is located, and use it. Configuration file can be absent.


Configuration file is a sequence of lines, each line containing
command line argument name and it's value. Empty lines, and lines beginning
with `#`, are ignored. Here is the example of `mongoose.conf` file:

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
configuration file. Configuration options section below provide a good
overview of Mongoose features.

Note that configuration options on the command line must start with `-`,
but their names are the same as in the config file. All option names are
listed in the next section. Thus, the following two setups are equivalent:

    # Using command line arguments
    $ mongoose -listening_ports 1234 -document_root /var/www

    # Using config file
    $ cat mongoose.conf
    listening_ports 1234
    document_root /var/www
    $ mongoose

Mongoose can also be used to modify `.htpasswd` passwords file:

    mongoose -A <htpasswd_file> <realm> <user> <passwd>

Unlike other web servers, mongoose does not require CGI scripts be located in
a special directory. CGI scripts can be anywhere. CGI (and SSI) files are
recognized by the file name pattern. Mongoose uses shell-like glob
patterns. Pattern match starts at the beginning of the string, so essentially
patterns are prefix patterns. Syntax is as follows:

     **      Matches everything
     *       Matches everything but slash character, '/'
     ?       Matches any character
     $       Matches the end of the string
     |       Matches if pattern on the left side or the right side matches.

All other characters in the pattern match themselves. Examples:

    **.cgi$      Any string that ends with .cgi
    /foo         Any string that begins with /foo
    **a$|**b$    Any string that ends with a or b


