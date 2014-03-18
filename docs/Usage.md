# Mongoose User Guide

Mongoose is small and easy to use web server built on top of
mongoose library. It is designed with maximum simplicity in mind. For example,
to share any directory, just drop mongoose executable in that directory,
double-click it (on UNIX, run it from shell) and launch a browser at
[http://localhost:8080](http://localhost:8080) Note that 'localhost' should
be changed to a machine's name if a folder is accessed from other computer.

On Windows and Mac, Mongoose iconifies itself to the system tray when started.
Right-click on the icon to pop up a menu, where it is possible to stop
mongoose, or configure it.

On UNIX, `mongoose` is a command line utility. Running `mongoose` in
terminal, optionally followed by configuration parameters
(`mongoose [OPTIONS]`) or configuration file name
(`mongoose [config_file_name]`) starts the
web server:

    $ mongoose -document_root /var/www  # Running mongoose with cmdline options
    $ mongoose /etc/my_config.txt       # Running mongoose with config file
    $ mongoose                          # Running with no parameters. This will
                                        # serve current directory on port 8080

Mongoose does not detach from terminal. Pressing `Ctrl-C` keys
stops the server.

When started, mongoose first searches for the configuration file.
If configuration file is specified explicitly in the command line, then
specified configuration file is used.
Otherwise, mongoose would search for file `mongoose.conf` in the same directory
where binary is located, and use it. Configuration file can be absent.

Configuration file is a sequence of lines, each line containing
command line argument name and it's value. Empty lines and lines beginning
with `#` are ignored. Here is the example of `mongoose.conf` file:

    # This is a comment
    document_root C:\www
    listening_port 80
    ssl_certificate C:\mongoose\ssl_cert.pem

Command line arguments are highest priority and can override
configuration file settings. For example, if `mongoose.conf` has line
`document_root /var/www`, and mongoose has been started as
`mongoose -document_root /etc`, then `/etc` directory will be used as
document root.

Note that configuration options on the command line must start with `-`,
and their names are the same as in the config file. Exampli gratia,
the following two setups are equivalent:

    $ mongoose -listening_port 1234 -document_root /var/www

    $ cat > mongoose.conf
    listening_ports 1234
    document_root /var/www
    ^D
    $ mongoose

Mongoose can also be used to modify `.htpasswd` passwords file:

    $ mongoose -A .htpasswd mydomain.com user_name user_password

Unlike other web servers, mongoose does not require CGI scripts be located in
a special directory. CGI scripts can be anywhere. CGI (and SSI) files are
recognized by the file name pattern. Mongoose uses shell-like glob
patterns. Pattern match starts at the beginning of the string, so essentially
patterns are prefix patterns. Syntax is as follows:

     **         Matches everything
     *          Matches everything but slash character, '/'
     ?          Matches any character
     $          Matches the end of the string
     |          Matches if pattern on the left side or the right side matches.

All other characters in the pattern match themselves. Examples:

    # Pattern   Meaning
    **.cgi$     Any string that ends with .cgi
    /foo        Any string that begins with /foo
    **a$|**b$   Any string that ends with a or b

To restrict CGI files only to `/cgi-bin/` directory, use this setting:

    $ mongoose -cgi_pattern /cgi-bin/*.cgi # Emulate /cgi-bin/ restriction
