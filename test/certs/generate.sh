#/bin/sh
#
# Geneate self-signed ECC certificates

cd `dirname $0`

# Create cnf for adding SAN DNS:localhost
# See https://security.stackexchange.com/questions/190905/subject-alternative-name-in-certificate-signing-request-apparently-does-not-surv
cat > cnf <<EOF
[SAN]
subjectAltName=DNS:localhost
EOF

# Generate CA
# Important: CN names must be different for CA and client/server certs
openssl ecparam -noout -name prime256v1 -genkey -out ca.key
openssl req -x509 -new -key ca.key -days 3650 -subj /CN=Mongoose -out ca.crt

# Generate server cert
openssl ecparam -noout -name prime256v1 -genkey -out server.key
openssl req -new -sha256 -key server.key -days 3650 -subj /CN=server -out server.csr
openssl x509 -req -sha256 -in server.csr -extensions SAN -extfile cnf \
  -CAkey ca.key -CA ca.crt -CAcreateserial -days 3650 -out server.crt

# Generate client cert
openssl ecparam -noout -name prime256v1 -genkey -out client.key
openssl req -new -sha256 -key client.key -days 3650 -subj /CN=client -out client.csr
openssl x509 -req -sha256 -in client.csr -extensions SAN -extfile cnf \
  -CAkey ca.key -CA ca.crt -CAcreateserial -days 3650 -out client.crt

# Verify
openssl verify -verbose -CAfile ca.crt server.crt
openssl verify -verbose -CAfile ca.crt client.crt

# Inspect
# openssl x509 -text -noout -ext subjectAltName -in server.crt
