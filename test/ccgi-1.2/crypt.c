/*
 * C CGI Library version 1.2
 *
 * Author:  Stephen C. Losen,  University of Virginia
 *
 * Copyright 2015 Stephen C. Losen.  Distributed under the terms
 * of the GNU Lesser General Public License (LGPL 2.1)
 *
 * CGI_encrypt() converts arbitrary input data to a secure,
 * encrypted string, using a password provided by the user.
 * CGI_decrypt() reverses the process and returns the original
 * data.  The secure string includes a message digest for
 * verifying the data.
 */

#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <ccgi.h>

#define SALT_SIZE    8
#define DIGEST_SIZE 20

/*
 * init_salt() places 8 random bytes into salt with RAND_bytes()
 * or else uses SHA1 digest of current time and process ID
 */

static void
init_salt(unsigned char *salt) {
    EVP_MD_CTX ctx;
    unsigned char md[DIGEST_SIZE];
    struct timeval tv;
    pid_t pid;
    unsigned int rlen;

    if (RAND_bytes(salt, SALT_SIZE) == 1) {
        return;
    }
    gettimeofday(&tv, 0);
    pid = getpid();
    EVP_DigestInit(&ctx, EVP_sha1());
    EVP_DigestUpdate(&ctx, &tv, sizeof(tv));
    EVP_DigestUpdate(&ctx, &pid, sizeof(pid));
    EVP_DigestFinal(&ctx, md, &rlen);
    memcpy(salt, md, SALT_SIZE);
}

/*
 * digest() feeds input p of length len, the password and
 * the salt to SHA1 and returns the digest in md.
 */

static void
digest(const void *p, int len, const char *password,
    const unsigned char *salt, unsigned char *md)
{
    EVP_MD_CTX ctx;
    unsigned int rlen;

    EVP_DigestInit(&ctx, EVP_sha1());
    EVP_DigestUpdate(&ctx, salt, SALT_SIZE);
    EVP_DigestUpdate(&ctx, password, strlen(password));
    EVP_DigestUpdate(&ctx, p, len);
    EVP_DigestFinal(&ctx, md, &rlen);
}

/*
 * CGI_encrypt() returns an encrypted base64 string using input p
 * of length len and the password.  We generate a random 8 byte
 * salt.  We compute a SHA1 message digest using p, the password,
 * and the salt.  We generate a cipher key using the password and
 * salt. We encrypt the digest and input with the AES-256-CBC cipher
 * and key. We base64 encode the return string, which consists of
 * the unencrypted salt, encrypted digest and encrypted input.
 */

char *
CGI_encrypt(const void *p, int len, const char *password) {
    EVP_CIPHER_CTX ctx;
    unsigned char md[DIGEST_SIZE];
    unsigned char iv[EVP_MAX_IV_LENGTH];
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char *out;
    char *b64;
    int offset, rlen;

    if (p == 0 || len <= 0 || password == 0 || *password == 0) {
        return 0;
    }
    out = malloc(SALT_SIZE + DIGEST_SIZE + len + EVP_MAX_BLOCK_LENGTH);
    init_salt(out);
    digest(p, len, password, out, md);
    EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), out,
        (unsigned char *) password, strlen(password), 1, key, iv);
    EVP_EncryptInit(&ctx, EVP_aes_256_cbc(), key, iv);
    offset = SALT_SIZE;
    EVP_EncryptUpdate(&ctx, out + offset, &rlen, md, DIGEST_SIZE);
    offset += rlen;
    EVP_EncryptUpdate(&ctx, out + offset, &rlen, p, len);
    offset += rlen;
    EVP_EncryptFinal(&ctx, out + offset, &rlen);
    b64 = CGI_encode_base64(out, offset + rlen);
    free(out);
    return b64;
}

/*
 * CGI_decrypt() returns a pointer to data decrypted from
 * base64 string p and the password. p was previously encrypted
 * with CGI_encrypt().  We base64 decode p and the first 8 bytes
 * is the salt followed by the encrypted message digest and
 * encrypted data.  We generate the cipher key using the salt
 * and the password.  We decrypt the message digest and data
 * using the AES-256-CBC cipher and key.  We re-compute the
 * message digest using the decrypted data, password, and salt.
 * If the two digests match then we return a pointer to the data
 * and place its length in *len.  Otherwise we return null.
 */

void *
CGI_decrypt(const char *p, int *len, const char *password) {
    EVP_CIPHER_CTX ctx;
    unsigned char md[DIGEST_SIZE];
    unsigned char iv[EVP_MAX_IV_LENGTH];
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char *ret, *out;
    int offset, rlen = 0;

    if (p == 0 || *p == 0 || password == 0 || *password == 0) {
        return 0;
    }
    ret = CGI_decode_base64(p, &rlen);
    if (rlen <= DIGEST_SIZE + SALT_SIZE) {
        free(ret);
        return 0;
    }
    out = malloc(rlen + EVP_MAX_BLOCK_LENGTH);
    EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), ret,
        (unsigned char *) password, strlen(password), 1, key, iv);
    EVP_DecryptInit(&ctx, EVP_aes_256_cbc(), key, iv);
    EVP_DecryptUpdate(&ctx, out, &offset, ret + SALT_SIZE,
        rlen - SALT_SIZE);
    EVP_DecryptFinal(&ctx, out + offset, &rlen);
    rlen += offset - DIGEST_SIZE;

    /*
     * The salt is in ret, the decrypted digest and decrypted
     * data are in out, and the data length is rlen.
     */

    if (rlen > 0) {
        digest(out + DIGEST_SIZE, rlen, password, ret, md);
    }
    if (rlen > 0 && memcmp(out, md, DIGEST_SIZE) == 0) {
        memcpy(ret, out + DIGEST_SIZE, rlen);
        ret[rlen] = 0;
        if (len != 0) {
            *len = rlen;
        }
    }
    else {
        free(ret);
        ret = 0;
    }
    free(out);
    return ret;
}
