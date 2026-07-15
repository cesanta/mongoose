# JWT Bearer Authentication

## Build and run:

```sh
make
```

## HS256:

This path signs and verifies JWTs with the shared secret from `main.c`.

```sh
curl -i http://127.0.0.1:8000/login-hs256
TOKEN=$(curl -s http://127.0.0.1:8000/login-hs256 | sed -n 's/.*"token":"\([^"]*\)".*/\1/p')
curl -i -H "Authorization: Bearer $TOKEN" http://127.0.0.1:8000/
```

```txt
HTTP/1.1 200 OK
Content-Type: application/json
Content-Length: 47         

{"sub":"admin","scope":"dash:read dash:write"}
```

`scope` here is a JSON string value containing a space-delimited permission list; as an example.

## ES256 (Requires MG_TLS_BUILTIN):

This path parses the EC private key from `main.c`, signs the JWT with it, and
derives the public key for verification to keep the example self-contained.

```sh
curl -i http://127.0.0.1:8000/login-es256
TOKEN=$(curl -s http://127.0.0.1:8000/login-es256 | sed -n 's/.*"token":"\([^"]*\)".*/\1/p')
curl -i -H "Authorization: Bearer $TOKEN" http://127.0.0.1:8000/
```

```txt
HTTP/1.1 200 OK
Content-Type: application/json
Content-Length: 36         

{"sub":"admin","scope":"dash:read"}
```

`scope` here is a JSON string value containing a space-delimited permission list; as an example.

## Unauthorized access attempt example

```sh
curl -i -H "Authorization: Bearer bad.token.here" http://127.0.0.1:8000/
```

```txt
HTTP/1.1 401 Unauthorized
WWW-Authenticate: Bearer
Content-Length: 13         

Unauthorized
```
