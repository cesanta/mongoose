# Mongoose SSL guide

SSL is a protocol that makes web communication secure. To enable SSL
in mongoose, 3 steps are required:

   1. Valid certificate file must be created
   2. `ssl_certificate` options must be set to contain path to the
       certificate file.
   3. `listening_ports` option must contain a port number with letter `s`
        appended to it, which instructs Mongoose to use SSL for all connections
        made to that port.

Below is the `mongoose.conf` file snippet for typical SSL setup:

    document_root     www_root        # Serve files in www_root directory
    listening_ports   80r,443s        # Redirect all HTTP requests to HTTPS
    ssl_certificate   ssl_cert.pem    # Location of certificate file

## How to create SSL certificate file

SSL certificate file is a text file that must contain at least two
sections:

   1. A private key
   2. A certificate

Both sections should be chunks of text in PEM format. When PEM file is
opened in a text editor, it looks like this:

    -----BEGIN RSA PRIVATE KEY-----
    MIIEogIBAAKCAQEAwONaLOP7EdegqjRuQKSDXzvHmFMZfBufjhELhNjo5KsL4ieH
    hYN0Zii2yTb63jGxKY6gH1R/r9dL8kXaJmcZrfSa3AgywnteJWg=
    -----END RSA PRIVATE KEY-----
    -----BEGIN CERTIFICATE-----
    MIIDBjCCAe4CCQCX05m0b053QzANBgkqhkiG9w0BAQQFADBFMQswCQYDVQQGEwJB
    SEGI4JSxV56lYg==
    -----END CERTIFICATE-----

Two aforementioned sections are clearly seen. Typically, those section
are bigger then in the example shown. The text between the `BEGIN` and
`END` is the text representation of binary data, a private key and a
certificate. Therefore, in order to create a certificate file,

   * private key must be converted to PEM format
   * certificate must be converted to PEM format
   * those two should be concatenated into a single file

If the certificate chain in used, a chain file also needs to be
converted into PEM format and appended to the certificate file.

## How SSL works

SSL is a protocol that can encrypt communication between two parties. If third
party observes all messages passed by, it would be very
hard for the third party (though not impossible) to decrypt the communication.

The idea is based on so-called public key encryption. Communicating parties
have two keys: a public key and a private key. A public key is advertised
to everybody, and it is contained in a certificate. A private key is kept
secret. Security algorithm works in a way that anybody can encrypt
a message using public key, and only private key can decrypt it.

This is why web server needs both private key and certificate: private key
is used to decrypt incoming messages, and certificate is used to tell the
public key to the other party. When communication starts, parties exchange
their public keys, and keep private keys to themselves. Man-in-the-middle
who observes the communication is unable to decrypt the messages cause
private keys are required for decryption.

Encryption algorithms are built on top of hard mathematical problem, which
makes it very expensive for man-in-the-middle to compute private keys.
For example, RSA algorithm is based on a mathematical problem of factorization.
It is easy to generate two very large prime numbers `P` and `Q` and make
a product `P * Q`. But given a product, it is very hard to recover these
two prime numbers - this is called factorization.
