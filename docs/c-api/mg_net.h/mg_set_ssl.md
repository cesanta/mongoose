---
title: "mg_set_ssl()"
decl_name: "mg_set_ssl"
symbol_kind: "func"
signature: |
  const char *mg_set_ssl(struct mg_connection *nc, const char *cert,
                         const char *ca_cert);
---

Note: This function is deprecated. Please, use SSL options in
mg_connect_opt.

Enables SSL for a given connection.
`cert` is a server certificate file name for a listening connection
or a client certificate file name for an outgoing connection.
The certificate files must be in PEM format. The server certificate file
must contain a certificate, concatenated with a private key, optionally
concatenated with DH parameters.
`ca_cert` is a CA certificate or NULL if peer verification is not
required.
Return: NULL on success or error message on error. 

