# Digest Authentication

Mongoose has a built-in Digest (MD5) authentication support. In order to
enable Digest authentication, create a file `.htpasswd` in the directory
you would like to protect. That file should be in the format that Apache's
`htdigest` utility.

You can use either Apache `htdigest` utility, or
Mongoose pre-build binary at https://www.cesanta.com/binary.html
to add new users into that file:

```
mongoose -A /path/to/.htdigest mydomain.com joe joes_password
```
