#define MG_ENABLE_SOCKET 1
#define MG_ENABLE_LOG 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_TCPIP 0
#define MG_IO_SIZE (1 * 1024 * 1024)  // Big IO size for fast resizes
#define MG_TLS MG_TLS_BUILTIN

#include "mongoose.c"

#ifdef __cplusplus
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#else
int LLVMFuzzerTestOneInput(const uint8_t *, size_t);
#endif
typedef int (*f)(struct mg_connection *);

f f_[] = {
  mg_tls_server_recv_hello,
#if 0
  mg_tls_client_recv_hello,
  mg_tls_client_recv_ext, 
  mg_tls_client_recv_cert,
  mg_tls_client_recv_cert_verify
#endif
};

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  struct mg_connection c_[sizeof(f_)/sizeof(f)], *c = &c_[0];
  struct tls_data tls_[sizeof(f_)/sizeof(f)];
  int i;
  if (size == 0) return 0;
  mg_log_set(MG_LL_INFO);
  memset(c, 0, sizeof(*c));
  c->send.align = c->recv.align = c->rtls.align = MG_IO_SIZE;  
  c->is_tls = c->is_tls_hs = 1;
  for (i = 0; i < (int)(sizeof(f_)/sizeof(f)); i++) {
    struct mg_iobuf *io;
    c = &c_[i];
    io = &c->rtls;
    if (i > 0) memcpy(c, &c_[0], sizeof(*c)); // copy from 1st one
    if (i > 0) c->is_client = 1;  // from 1 on, client functions
    memset(&tls_[i], 0, sizeof(struct tls_data));
    if (io->size - io->len < size && !mg_iobuf_resize(io, io->len + size)) {
      mg_error(c, "oom");
      return 0; // drop it
    }
    memcpy(&io->buf[io->len], data, size);
    io->len += size;
    c->tls = &tls_[i];
    f_[i](&c[i]);
    mg_iobuf_free(io);
  }
  
  return 0;
}

#if defined(MAIN)
int main(int argc, char *argv[]) {
  int res = EXIT_FAILURE;
  if (argc > 1) {
    struct mg_str data = mg_file_read(&mg_fs_posix, argv[1]);
    if (data.buf != NULL) {
      LLVMFuzzerTestOneInput((uint8_t *) data.buf, data.len);
      res = EXIT_SUCCESS;
    }
    free(data.buf);
  }
  return res;
}
#endif
