#include <shared/utils/encryption.h>
#include <stddef.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <string.h> //Only needed for strlen().
#include "common.h"

char *encode_base64(const char *str) {
  BIO *encoder = BIO_new(BIO_f_base64());
  BIO *marshal = BIO_new(BIO_s_mem());
  BUF_MEM *marshal_buffer;

  BIO_push(encoder, marshal);
  BIO_set_flags(encoder, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(encoder, str, (int) strlen(str));
  BIO_flush(encoder);
  BIO_get_mem_ptr(marshal, &marshal_buffer);

  BIO_set_close(marshal, BIO_NOCLOSE);
  BIO_free_all(encoder);

  BUF_MEM_grow(marshal_buffer, (*marshal_buffer).length + 1);
  (*marshal_buffer).data[(*marshal_buffer).length] = '\0';
  return (*marshal_buffer).data;
}

char *decode_base64(const char *str) {
  char *decoded = malloc((strlen(str) * 3) / 4 + 1);
  BIO *encoder = BIO_new(BIO_f_base64());
  BIO *marshal = BIO_new(BIO_s_mem());

  BIO_write(marshal, str, (int) strlen(str));
  BIO_push(encoder, marshal);
  BIO_set_flags(encoder, BIO_FLAGS_BASE64_NO_NL);

  int decoded_byte_index = 0;
  while (0 < BIO_read(encoder, decoded + decoded_byte_index++, 1));

  BIO_free_all(encoder);
  return decoded;
}

char *encode_sha1(const char *str) {
  char *hash = malloc(SHA_DIGEST_LENGTH);
  SHA1((unsigned char *) str, strlen(str), (unsigned char *) hash);
  return hash;
}

static const char *const SocketMagicKey = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
char *websocket(const char *key) {
  var key_copy = str(key);
  key_copy = realloc(key_copy, strlen(key) + strlen(SocketMagicKey) - 1);
  strcat(key_copy, SocketMagicKey);

  let sha1 = encode_sha1(key_copy);
  let base64 = encode_base64((char *) sha1);
  free(sha1);
  free(key_copy);
  return base64;
}

const struct encryption_lib encryption = {
        .decode_base64=decode_base64,
        .encode_base64=encode_base64,
        .encode_sha1=encode_sha1,
        .websocket=websocket,
};