# Mongoose FAQ

## My Antivirus Software reports Mongoose as a security threat

Mongoose doesn't contain any malicious logic. Antivirus reports a
[false positive](http://en.wikipedia.org/wiki/Type_I_and_type_II_errors#False_positive_error).
This is when certain byte sequence in Mongoose accidentally matches
virus signature in the Antivirus database.

## Download page doesn't work

Please make sure Javascript is enabled in your browser, and that the
antivirus software is not blocking the download.


## MacOS message: "Mongoose.app is damaged and can’t be opened. You should move it to the Trash"

This happens on newer MacOS systems. The reason for the message
is the fact Mongoose.app is not digitally signed.
Mongoose download procedure changes the app on the fly by injecting
user information in the binary, making any prior digital signature void.
Open "System Preferences" -> "Security"  and set "Allow apps downloaded from"
to "Anywhere".  Revert the settings once Mongoose is installed.

## PHP doesn't work: getting empty page, or 'File not found' error

The reason for that is wrong paths to the interpreter. Remember that with PHP,
correct interpreter is `php-cgi.exe` (`php-cgi` on UNIX). Solution: specify
full path to the PHP interpreter, e.g.:

    mongoose -cgi_interpreter /full/path/to/php-cgi

## Mongoose fails to start

If Mongoose exits immediately when run, this
usually indicates a syntax error in the configuration file
(named `mongoose.conf` by default) or the command-line arguments.
Syntax checking is omitted from Mongoose to keep its size low. However,
the Manual should be of help. Note: the syntax changes from time to time,
so updating the config file might be necessary after executable update.

### Embedding with OpenSSL on Windows might fail because of calling convention

To force Mongoose to use `__stdcall` convention, add `/Gz` compilation
flag to the Visual Studio project settings.
