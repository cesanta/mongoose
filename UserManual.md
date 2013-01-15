
**NOTE: THIS MANUAL IS WORK IN PROGRESS**

Overview
--------

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

Usage Examples
--------------

- How to share a Windows folder: copy mongoose executable to a folder and
  double-click the executable. The folder should be accessible via
  [http://localhost:8080](http://localhost:8080) in any browser.
- How to start mongoose at UNIX startup time in daemon mode, serving
  directory `/var/www`: put this line in the system startup script:
  `/path/to/mongoose -listening_ports 80 -document_root /var/www &`

Command Line Options
--------------------
`-A <htpasswd_file> <domain_name> <user_name> <password>`

    Add/edit user's password in the passwords file. Deleting users
    can be done with any text editor. Functionality is similar to
    Apache's `htdigest` utility.

Common Problems
---------------

Embedding
---------

Other Resources
---------------
- Presentation made by Arnout Vandecappelle at FOSDEM 2011 on 2011-02-06
  in Brussels, Belgium, called
  "Creating secure web based user interfaces for Embedded Devices"
  ([pdf](http://mind.be/content/110206_Web-ui.pdf) |
   [odp](http://mind.be/content/110206_Web-ui.odp))
- Linux Journal article by Michel J.Hammel, 2010-04-01, called
  [Mongoose: an Embeddable Web Server in C](http://www.linuxjournal.com/article/10680)
