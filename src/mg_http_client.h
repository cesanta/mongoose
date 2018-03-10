/*
 * === Client API reference
 */

#ifndef CS_MONGOOSE_SRC_HTTP_CLIENT_H_
#define CS_MONGOOSE_SRC_HTTP_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Helper function that creates an outbound HTTP connection.
 *
 * `url` is the URL to fetch. It must be properly URL-encoded, e.g. have
 * no spaces, etc. By default, `mg_connect_http()` sends the Connection and
 * Host headers. `extra_headers` is an extra HTTP header to send, e.g.
 * `"User-Agent: my-app\r\n"`.
 * If `post_data` is NULL, then a GET request is created. Otherwise, a POST
 * request is created with the specified POST data. Note that if the data being
 * posted is a form submission, the `Content-Type` header should be set
 * accordingly (see example below).
 *
 * Examples:
 *
 * ```c
 *   nc1 = mg_connect_http(mgr, ev_handler_1, "http://www.google.com", NULL,
 *                         NULL);
 *   nc2 = mg_connect_http(mgr, ev_handler_1, "https://github.com", NULL, NULL);
 *   nc3 = mg_connect_http(
 *       mgr, ev_handler_1, "my_server:8000/form_submit/",
 *       "Content-Type: application/x-www-form-urlencoded\r\n",
 *       "var_1=value_1&var_2=value_2");
 * ```
 */
struct mg_connection *mg_connect_http(
    struct mg_mgr *mgr,
    MG_CB(mg_event_handler_t event_handler, void *user_data), const char *url,
    const char *extra_headers, const char *post_data);

/*
 * Helper function that creates an outbound HTTP connection.
 *
 * Mostly identical to mg_connect_http, but allows you to provide extra
 *parameters
 * (for example, SSL parameters)
 */
struct mg_connection *mg_connect_http_opt(
    struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data),
    struct mg_connect_opts opts, const char *url, const char *extra_headers,
    const char *post_data);

/* Creates digest authentication header for a client request. */
int mg_http_create_digest_auth_header(char *buf, size_t buf_len,
                                      const char *method, const char *uri,
                                      const char *auth_domain, const char *user,
                                      const char *passwd, const char *nonce);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_HTTP_CLIENT_H_ */
