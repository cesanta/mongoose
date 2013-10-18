# Mongoose FAQ

### PHP doesn't work: getting empty page, or 'File not found' error

The reason for that is wrong paths to the interpreter. Remember that with PHP,
correct interpreter is `php-cgi.exe` (`php-cgi` on UNIX). Solution: specify
full path to the PHP interpreter, e.g.:

    mongoose -cgi_interpreter /full/path/to/php-cgi

### Mongoose fails to start

If Mongoose exits immediately when run, this
usually indicates a syntax error in the configuration file
(named `mongoose.conf` by default) or the command-line arguments.
Syntax checking is omitted from Mongoose to keep its size low. However,
the Manual should be of help. Note: the syntax changes from time to time,
so updating the config file might be necessary after executable update.

### Embedding with OpenSSL on Windows might fail because of calling convention

To force Mongoose to use `__stdcall` convention, add `/Gz` compilation
flag to the Visual Studio project settings.
