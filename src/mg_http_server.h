/*
 * === Server API reference
 */

#ifndef CS_MONGOOSE_SRC_HTTP_SERVER_H_
#define CS_MONGOOSE_SRC_HTTP_SERVER_H_

#if MG_ENABLE_HTTP

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Parses a HTTP message.
 *
 * `is_req` should be set to 1 if parsing a request, 0 if reply.
 *
 * Returns the number of bytes parsed. If HTTP message is
 * incomplete `0` is returned. On parse error, a negative number is returned.
 */
int mg_parse_http(const char *s, int n, struct http_message *hm, int is_req);

/*
 * Searches and returns the header `name` in parsed HTTP message `hm`.
 * If header is not found, NULL is returned. Example:
 *
 *     struct mg_str *host_hdr = mg_get_http_header(hm, "Host");
 */
struct mg_str *mg_get_http_header(struct http_message *hm, const char *name);

/*
 * Parses the HTTP header `hdr`. Finds variable `var_name` and stores its value
 * in the buffer `*buf`, `buf_size`. If the buffer size is not enough,
 * allocates a buffer of required size and writes it to `*buf`, similar to
 * asprintf(). The caller should always check whether the buffer was updated,
 * and free it if so.
 *
 * This function is supposed to parse cookies, authentication headers, etc.
 * Example (error handling omitted):
 *
 *     char user_buf[20];
 *     char *user = user_buf;
 *     struct mg_str *hdr = mg_get_http_header(hm, "Authorization");
 *     mg_http_parse_header2(hdr, "username", &user, sizeof(user_buf));
 *     // ... do something useful with user
 *     if (user != user_buf) {
 *       free(user);
 *     }
 *
 * Returns the length of the variable's value. If variable is not found, 0 is
 * returned.
 */
int mg_http_parse_header2(struct mg_str *hdr, const char *var_name, char **buf,
                          size_t buf_size);

/*
 * DEPRECATED: use mg_http_parse_header2() instead.
 *
 * Same as mg_http_parse_header2(), but takes buffer as a `char *` (instead of
 * `char **`), and thus it cannot allocate a new buffer if the provided one
 * is not enough, and just returns 0 in that case.
 */
int mg_http_parse_header(struct mg_str *hdr, const char *var_name, char *buf,
                         size_t buf_size)
#ifdef __GNUC__
    __attribute__((deprecated))
#endif
    ;

/*
 * Gets and parses the Authorization: Basic header
 * Returns -1 if no Authorization header is found, or if
 * mg_parse_http_basic_auth
 * fails parsing the resulting header.
 */
int mg_get_http_basic_auth(struct http_message *hm, char *user, size_t user_len,
                           char *pass, size_t pass_len);

/*
 * Parses the Authorization: Basic header
 * Returns -1 iif the authorization type is not "Basic" or any other error such
 * as incorrectly encoded base64 user password pair.
 */
int mg_parse_http_basic_auth(struct mg_str *hdr, char *user, size_t user_len,
                             char *pass, size_t pass_len);

/*
 * Parses the buffer `buf`, `buf_len` that contains multipart form data chunks.
 * Stores the chunk name in a `var_name`, `var_name_len` buffer.
 * If a chunk is an uploaded file, then `file_name`, `file_name_len` is
 * filled with an uploaded file name. `chunk`, `chunk_len`
 * points to the chunk data.
 *
 * Return: number of bytes to skip to the next chunk or 0 if there are
 *         no more chunks.
 *
 * Usage example:
 *
 * ```c
 *    static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *      switch(ev) {
 *        case MG_EV_HTTP_REQUEST: {
 *          struct http_message *hm = (struct http_message *) ev_data;
 *          char var_name[100], file_name[100];
 *          const char *chunk;
 *          size_t chunk_len, n1, n2;
 *
 *          n1 = n2 = 0;
 *          while ((n2 = mg_parse_multipart(hm->body.p + n1,
 *                                          hm->body.len - n1,
 *                                          var_name, sizeof(var_name),
 *                                          file_name, sizeof(file_name),
 *                                          &chunk, &chunk_len)) > 0) {
 *            printf("var: %s, file_name: %s, size: %d, chunk: [%.*s]\n",
 *                   var_name, file_name, (int) chunk_len,
 *                   (int) chunk_len, chunk);
 *            n1 += n2;
 *          }
 *        }
 *        break;
 * ```
 */
size_t mg_parse_multipart(const char *buf, size_t buf_len, char *var_name,
                          size_t var_name_len, char *file_name,
                          size_t file_name_len, const char **chunk,
                          size_t *chunk_len);

/*
 * Fetches a HTTP form variable.
 *
 * Fetches a variable `name` from a `buf` into a buffer specified by `dst`,
 * `dst_len`. The destination is always zero-terminated. Returns the length of
 * a fetched variable. If not found, 0 is returned. `buf` must be valid
 * url-encoded buffer. If destination is too small or an error occured,
 * negative number is returned.
 */
int mg_get_http_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len);

#if MG_ENABLE_FILESYSTEM
/*
 * This structure defines how `mg_serve_http()` works.
 * Best practice is to set only required settings, and leave the rest as NULL.
 */
struct mg_serve_http_opts {
  /* Path to web root directory */
  const char *document_root;

  /* List of index files. Default is "" */
  const char *index_files;

  /*
   * Leave as NULL to disable authentication.
   * To enable directory protection with authentication, set this to ".htpasswd"
   * Then, creating ".htpasswd" file in any directory automatically protects
   * it with digest authentication.
   * Use `mongoose` web server binary, or `htdigest` Apache utility to
   * create/manipulate passwords file.
   * Make sure `auth_domain` is set to a valid domain name.
   */
  const char *per_directory_auth_file;

  /* Authorization domain (domain name of this web server) */
  const char *auth_domain;

  /*
   * Leave as NULL to disable authentication.
   * Normally, only selected directories in the document root are protected.
   * If absolutely every access to the web server needs to be authenticated,
   * regardless of the URI, set this option to the path to the passwords file.
   * Format of that file is the same as ".htpasswd" file. Make sure that file
   * is located outside document root to prevent people fetching it.
   */
  const char *global_auth_file;

  /* Set to "no" to disable directory listing. Enabled by default. */
  const char *enable_directory_listing;

  /*
   * SSI files pattern. If not set, "**.shtml$|**.shtm$" is used.
   *
   * All files that match ssi_pattern are treated as SSI.
   *
   * Server Side Includes (SSI) is a simple interpreted server-side scripting
   * language which is most commonly used to include the contents of a file
   * into a web page. It can be useful when it is desirable to include a common
   * piece of code throughout a website, for example, headers and footers.
   *
   * In order for a webpage to recognize an SSI-enabled HTML file, the
   * filename should end with a special extension, by default the extension
   * should be either .shtml or .shtm
   *
   * Unknown SSI directives are silently ignored by Mongoose. Currently,
   * the following SSI directives are supported:
   *    &lt;!--#include FILE_TO_INCLUDE --&gt;
   *    &lt;!--#exec "COMMAND_TO_EXECUTE" --&gt;
   *    &lt;!--#call COMMAND --&gt;
   *
   * Note that &lt;!--#include ...> directive supports three path
   *specifications:
   *
   * &lt;!--#include virtual="path" --&gt;  Path is relative to web server root
   * &lt;!--#include abspath="path" --&gt;  Path is absolute or relative to the
   *                                  web server working dir
   * &lt;!--#include file="path" --&gt;,    Path is relative to current document
   * &lt;!--#include "path" --&gt;
   *
   * The include directive may be used to include the contents of a file or
   * the result of running a CGI script.
   *
   * The exec directive is used to execute
   * a command on a server, and show command's output. Example:
   *
   * &lt;!--#exec "ls -l" --&gt;
   *
   * The call directive is a way to invoke a C handler from the HTML page.
   * On each occurence of &lt;!--#call COMMAND OPTIONAL_PARAMS> directive,
   * Mongoose calls a registered event handler with MG_EV_SSI_CALL event,
   * and event parameter will point to the COMMAND OPTIONAL_PARAMS string.
   * An event handler can output any text, for example by calling
   * `mg_printf()`. This is a flexible way of generating a web page on
   * server side by calling a C event handler. Example:
   *
   * &lt;!--#call foo --&gt; ... &lt;!--#call bar --&gt;
   *
   * In the event handler:
   *    case MG_EV_SSI_CALL: {
   *      const char *param = (const char *) ev_data;
   *      if (strcmp(param, "foo") == 0) {
   *        mg_printf(c, "hello from foo");
   *      } else if (strcmp(param, "bar") == 0) {
   *        mg_printf(c, "hello from bar");
   *      }
   *      break;
   *    }
   */
  const char *ssi_pattern;

  /* IP ACL. By default, NULL, meaning all IPs are allowed to connect */
  const char *ip_acl;

#if MG_ENABLE_HTTP_URL_REWRITES
  /* URL rewrites.
   *
   * Comma-separated list of `uri_pattern=url_file_or_directory_path` rewrites.
   * When HTTP request is received, Mongoose constructs a file name from the
   * requested URI by combining `document_root` and the URI. However, if the
   * rewrite option is used and `uri_pattern` matches requested URI, then
   * `document_root` is ignored. Instead, `url_file_or_directory_path` is used,
   * which should be a full path name or a path relative to the web server's
   * current working directory. It can also be an URI (http:// or https://)
   * in which case mongoose will behave as a reverse proxy for that destination.
   *
   * Note that `uri_pattern`, as all Mongoose patterns, is a prefix pattern.
   *
   * If uri_pattern starts with `@` symbol, then Mongoose compares it with the
   * HOST header of the request. If they are equal, Mongoose sets document root
   * to `file_or_directory_path`, implementing virtual hosts support.
   * Example: `@foo.com=/document/root/for/foo.com`
   *
   * If `uri_pattern` starts with `%` symbol, then Mongoose compares it with
   * the listening port. If they match, then Mongoose issues a 301 redirect.
   * For example, to redirect all HTTP requests to the
   * HTTPS port, do `%80=https://my.site.com`. Note that the request URI is
   * automatically appended to the redirect location.
   */
  const char *url_rewrites;
#endif

  /* DAV document root. If NULL, DAV requests are going to fail. */
  const char *dav_document_root;

  /*
   * DAV passwords file. If NULL, DAV requests are going to fail.
   * If passwords file is set to "-", then DAV auth is disabled.
   */
  const char *dav_auth_file;

  /* Glob pattern for the files to hide. */
  const char *hidden_file_pattern;

  /* Set to non-NULL to enable CGI, e.g. **.cgi$|**.php$" */
  const char *cgi_file_pattern;

  /* If not NULL, ignore CGI script hashbang and use this interpreter */
  const char *cgi_interpreter;

  /*
   * Comma-separated list of Content-Type overrides for path suffixes, e.g.
   * ".txt=text/plain; charset=utf-8,.c=text/plain"
   */
  const char *custom_mime_types;

  /*
   * Extra HTTP headers to add to each server response.
   * Example: to enable CORS, set this to "Access-Control-Allow-Origin: *".
   */
  const char *extra_headers;
};

/*
 * Serves given HTTP request according to the `options`.
 *
 * Example code snippet:
 *
 * ```c
 * static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   struct http_message *hm = (struct http_message *) ev_data;
 *   struct mg_serve_http_opts opts = { .document_root = "/var/www" };  // C99
 *
 *   switch (ev) {
 *     case MG_EV_HTTP_REQUEST:
 *       mg_serve_http(nc, hm, opts);
 *       break;
 *     default:
 *       break;
 *   }
 * }
 * ```
 */
void mg_serve_http(struct mg_connection *nc, struct http_message *hm,
                   struct mg_serve_http_opts opts);

/*
 * Serves a specific file with a given MIME type and optional extra headers.
 *
 * Example code snippet:
 *
 * ```c
 * static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   switch (ev) {
 *     case MG_EV_HTTP_REQUEST: {
 *       struct http_message *hm = (struct http_message *) ev_data;
 *       mg_http_serve_file(nc, hm, "file.txt",
 *                          mg_mk_str("text/plain"), mg_mk_str(""));
 *       break;
 *     }
 *     ...
 *   }
 * }
 * ```
 */
void mg_http_serve_file(struct mg_connection *nc, struct http_message *hm,
                        const char *path, const struct mg_str mime_type,
                        const struct mg_str extra_headers);

#if MG_ENABLE_HTTP_STREAMING_MULTIPART

/* Callback prototype for `mg_file_upload_handler()`. */
typedef struct mg_str (*mg_fu_fname_fn)(struct mg_connection *nc,
                                        struct mg_str fname);

/*
 * File upload handler.
 * This handler can be used to implement file uploads with minimum code.
 * This handler will process MG_EV_HTTP_PART_* events and store file data into
 * a local file.
 * `local_name_fn` will be invoked with whatever name was provided by the client
 * and will expect the name of the local file to open. A return value of NULL
 * will abort file upload (client will get a "403 Forbidden" response). If
 * non-null, the returned string must be heap-allocated and will be freed by
 * the caller.
 * Exception: it is ok to return the same string verbatim.
 *
 * Example:
 *
 * ```c
 * struct mg_str upload_fname(struct mg_connection *nc, struct mg_str fname) {
 *   // Just return the same filename. Do not actually do this except in test!
 *   // fname is user-controlled and needs to be sanitized.
 *   return fname;
 * }
 * void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   switch (ev) {
 *     ...
 *     case MG_EV_HTTP_PART_BEGIN:
 *     case MG_EV_HTTP_PART_DATA:
 *     case MG_EV_HTTP_PART_END:
 *       mg_file_upload_handler(nc, ev, ev_data, upload_fname);
 *       break;
 *   }
 * }
 * ```
 */
void mg_file_upload_handler(struct mg_connection *nc, int ev, void *ev_data,
                            mg_fu_fname_fn local_name_fn
                                MG_UD_ARG(void *user_data));
#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */
#endif /* MG_ENABLE_FILESYSTEM */

/*
 * Registers a callback for a specified http endpoint
 * Note: if callback is registered it is called instead of the
 * callback provided in mg_bind
 *
 * Example code snippet:
 *
 * ```c
 * static void handle_hello1(struct mg_connection *nc, int ev, void *ev_data) {
 *   (void) ev; (void) ev_data;
 *   mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello1]");
 *  nc->flags |= MG_F_SEND_AND_CLOSE;
 * }
 *
 * static void handle_hello2(struct mg_connection *nc, int ev, void *ev_data) {
 *  (void) ev; (void) ev_data;
 *   mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello2]");
 *  nc->flags |= MG_F_SEND_AND_CLOSE;
 * }
 *
 * void init() {
 *   nc = mg_bind(&mgr, local_addr, cb1);
 *   mg_register_http_endpoint(nc, "/hello1", handle_hello1);
 *   mg_register_http_endpoint(nc, "/hello1/hello2", handle_hello2);
 * }
 * ```
 */
void mg_register_http_endpoint(struct mg_connection *nc, const char *uri_path,
                               MG_CB(mg_event_handler_t handler,
                                     void *user_data));

struct mg_http_endpoint_opts {
  void *user_data;
  /* Authorization domain (realm) */
  const char *auth_domain;
  const char *auth_file;
};

void mg_register_http_endpoint_opt(struct mg_connection *nc,
                                   const char *uri_path,
                                   mg_event_handler_t handler,
                                   struct mg_http_endpoint_opts opts);

/*
 * Authenticates a HTTP request against an opened password file.
 * Returns 1 if authenticated, 0 otherwise.
 */
int mg_http_check_digest_auth(struct http_message *hm, const char *auth_domain,
                              FILE *fp);

/*
 * Authenticates given response params against an opened password file.
 * Returns 1 if authenticated, 0 otherwise.
 *
 * It's used by mg_http_check_digest_auth().
 */
int mg_check_digest_auth(struct mg_str method, struct mg_str uri,
                         struct mg_str username, struct mg_str cnonce,
                         struct mg_str response, struct mg_str qop,
                         struct mg_str nc, struct mg_str nonce,
                         struct mg_str auth_domain, FILE *fp);

/*
 * Sends buffer `buf` of size `len` to the client using chunked HTTP encoding.
 * This function sends the buffer size as hex number + newline first, then
 * the buffer itself, then the newline. For example,
 * `mg_send_http_chunk(nc, "foo", 3)` will append the `3\r\nfoo\r\n` string
 * to the `nc->send_mbuf` output IO buffer.
 *
 * NOTE: The HTTP header "Transfer-Encoding: chunked" should be sent prior to
 * using this function.
 *
 * NOTE: do not forget to send an empty chunk at the end of the response,
 * to tell the client that everything was sent. Example:
 *
 * ```
 *   mg_printf_http_chunk(nc, "%s", "my response!");
 *   mg_send_http_chunk(nc, "", 0); // Tell the client we're finished
 * ```
 */
void mg_send_http_chunk(struct mg_connection *nc, const char *buf, size_t len);

/*
 * Sends a printf-formatted HTTP chunk.
 * Functionality is similar to `mg_send_http_chunk()`.
 */
void mg_printf_http_chunk(struct mg_connection *nc, const char *fmt, ...);

/*
 * Sends the response status line.
 * If `extra_headers` is not NULL, then `extra_headers` are also sent
 * after the response line. `extra_headers` must NOT end end with new line.
 * Example:
 *
 *      mg_send_response_line(nc, 200, "Access-Control-Allow-Origin: *");
 *
 * Will result in:
 *
 *      HTTP/1.1 200 OK\r\n
 *      Access-Control-Allow-Origin: *\r\n
 */
void mg_send_response_line(struct mg_connection *nc, int status_code,
                           const char *extra_headers);

/*
 * Sends an error response. If reason is NULL, the message will be inferred
 * from the error code (if supported).
 */
void mg_http_send_error(struct mg_connection *nc, int code, const char *reason);

/*
 * Sends a redirect response.
 * `status_code` should be either 301 or 302 and `location` point to the
 * new location.
 * If `extra_headers` is not empty, then `extra_headers` are also sent
 * after the response line. `extra_headers` must NOT end end with new line.
 *
 * Example:
 *
 *      mg_http_send_redirect(nc, 302, mg_mk_str("/login"), mg_mk_str(NULL));
 */
void mg_http_send_redirect(struct mg_connection *nc, int status_code,
                           const struct mg_str location,
                           const struct mg_str extra_headers);

/*
 * Sends the response line and headers.
 * This function sends the response line with the `status_code`, and
 * automatically
 * sends one header: either "Content-Length" or "Transfer-Encoding".
 * If `content_length` is negative, then "Transfer-Encoding: chunked" header
 * is sent, otherwise, "Content-Length" header is sent.
 *
 * NOTE: If `Transfer-Encoding` is `chunked`, then message body must be sent
 * using `mg_send_http_chunk()` or `mg_printf_http_chunk()` functions.
 * Otherwise, `mg_send()` or `mg_printf()` must be used.
 * Extra headers could be set through `extra_headers`. Note `extra_headers`
 * must NOT be terminated by a new line.
 */
void mg_send_head(struct mg_connection *n, int status_code,
                  int64_t content_length, const char *extra_headers);

/*
 * Sends a printf-formatted HTTP chunk, escaping HTML tags.
 */
void mg_printf_html_escape(struct mg_connection *nc, const char *fmt, ...);

#if MG_ENABLE_HTTP_URL_REWRITES
/*
 * Proxies a given request to a given upstream http server. The path prefix
 * in `mount` will be stripped of the path requested to the upstream server,
 * e.g. if mount is /api and upstream is http://localhost:8001/foo
 * then an incoming request to /api/bar will cause a request to
 * http://localhost:8001/foo/bar
 *
 * EXPERIMENTAL API. Please use http_serve_http + url_rewrites if a static
 * mapping is good enough.
 */
void mg_http_reverse_proxy(struct mg_connection *nc,
                           const struct http_message *hm, struct mg_str mount,
                           struct mg_str upstream);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_HTTP */

#endif /* CS_MONGOOSE_SRC_HTTP_SERVER_H_ */
