// Unit test for the mongoose web server.
// g++ -W -Wall -pedantic -g unit_test.c -lssl && ./a.out
// cl unit_test.c /MD

#ifndef _WIN32
#define NS_ENABLE_IPV6
#define NS_ENABLE_SSL
#endif
#define MONGOOSE_POST_SIZE_LIMIT 999

// USE_* definitions must be made before #include "mongoose.c" !
#include "../mongoose.c"

#define FAIL(str, line) do {                    \
  printf("Fail on line %d: [%s]\n", line, str); \
  return str;                                   \
} while (0)

#define ASSERT(expr) do {             \
  static_num_tests++;               \
  if (!(expr)) FAIL(#expr, __LINE__); \
} while (0)

#define RUN_TEST(test) do { const char *msg = test(); \
  if (msg) return msg; } while (0)

#define HTTP_PORT "45772"
#define LISTENING_ADDR "127.0.0.1:" HTTP_PORT

static int static_num_tests = 0;

#if 0
// Connects to host:port, and sends formatted request to it. Returns
// malloc-ed reply and reply length, or NULL on error. Reply contains
// everything including headers, not just the message body.
static char *wget(const char *host, int port, int *len, const char *fmt, ...) {
  char buf[2000], *reply = NULL;
  int request_len, reply_size = 0, n, sock = -1;
  struct sockaddr_in sin;
  struct hostent *he = NULL;
  va_list ap;

  if (host != NULL &&
      (he = gethostbyname(host)) != NULL &&
      (sock = socket(PF_INET, SOCK_STREAM, 0)) != -1) {
    sin.sin_family = AF_INET;
    sin.sin_port = htons((uint16_t) port);
    sin.sin_addr = * (struct in_addr *) he->h_addr_list[0];
    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == 0) {

      // Format and send the request.
      va_start(ap, fmt);
      request_len = vsnprintf(buf, sizeof(buf), fmt, ap);
      va_end(ap);
      while (request_len > 0 && (n = send(sock, buf, request_len, 0)) > 0) {
        request_len -= n;
      }
      if (request_len == 0) {
        *len = 0;
        while ((n = recv(sock, buf, sizeof(buf), 0)) > 0) {
          if (*len + n > reply_size) {
            // Leak possible
            reply = (char *) realloc(reply, reply_size + sizeof(buf));
            reply_size += sizeof(buf);
          }
          if (reply != NULL) {
            memcpy(reply + *len, buf, n);
            *len += n;
          }
        }
      }
      closesocket(sock);
    }
  }

  return reply;
}
#endif

static char *read_file(const char *path, int *size) {
  FILE *fp;
  struct stat st;
  char *data = NULL;
  if ((fp = fopen(path, "rb")) != NULL && !fstat(fileno(fp), &st)) {
    *size = (int) st.st_size;
    data = (char *) malloc(*size);
    fread(data, 1, *size, fp);
    fclose(fp);
  }
  return data;
}

static const char *test_parse_http_message() {
  struct mg_connection ri;
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
  char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nbaz\r\n\r\n";
  char req5[] = "GET / HTTP/1.1\r\n\r\n";
  char req6[] = "G";
  char req7[] = " blah ";
  char req8[] = " HTTP/1.1 200 OK \n\n";
  char req9[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";

  ASSERT(get_request_len("\r\n", 3) == -1);
  ASSERT(get_request_len("\r\n", 2) == 0);
  ASSERT(get_request_len("GET", 3) == 0);
  ASSERT(get_request_len("\n\n", 2) == 2);
  ASSERT(get_request_len("\n\r\n", 3) == 3);
  ASSERT(get_request_len("\xdd\xdd", 2) == 0);
  ASSERT(get_request_len("\xdd\x03", 2) == -1);
  ASSERT(get_request_len(req3, sizeof(req3) - 1) == 0);
  ASSERT(get_request_len(req6, sizeof(req6) - 1) == 0);
  ASSERT(get_request_len(req7, sizeof(req7) - 1) == 0);

  ASSERT(parse_http_message(req9, sizeof(req9) - 1, &ri) == sizeof(req9) - 1);
  ASSERT(ri.num_headers == 1);

  ASSERT(parse_http_message(req1, sizeof(req1) - 1, &ri) == sizeof(req1) - 1);
  ASSERT(strcmp(ri.http_version, "1.1") == 0);
  ASSERT(ri.num_headers == 0);

  ASSERT(parse_http_message(req2, sizeof(req2) - 1, &ri) == -1);
  ASSERT(parse_http_message(req6, 0, &ri) == -1);
  ASSERT(parse_http_message(req8, sizeof(req8) - 1, &ri) == sizeof(req8) - 1);

  // TODO(lsm): Fix this. Header value may span multiple lines.
  ASSERT(parse_http_message(req4, sizeof(req4) - 1, &ri) == sizeof(req4) - 1);
  ASSERT(strcmp(ri.http_version, "1.1") == 0);
  ASSERT(ri.num_headers == 3);
  ASSERT(strcmp(ri.http_headers[0].name, "A") == 0);
  ASSERT(strcmp(ri.http_headers[0].value, "foo bar") == 0);
  ASSERT(strcmp(ri.http_headers[1].name, "B") == 0);
  ASSERT(strcmp(ri.http_headers[1].value, "bar") == 0);
  ASSERT(strcmp(ri.http_headers[2].name, "baz\r\n\r") == 0);
  ASSERT(strcmp(ri.http_headers[2].value, "") == 0);

  ASSERT(parse_http_message(req5, sizeof(req5) - 1, &ri) == sizeof(req5) - 1);
  ASSERT(strcmp(ri.request_method, "GET") == 0);
  ASSERT(strcmp(ri.http_version, "1.1") == 0);

  return NULL;
}

static const char *test_should_keep_alive(void) {
  struct mg_connection conn;
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "GET / HTTP/1.0\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";
  char req4[] = "GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";

  memset(&conn, 0, sizeof(conn));
  ASSERT(parse_http_message(req1, sizeof(req1) - 1, &conn) == sizeof(req1) - 1);
  ASSERT(should_keep_alive(&conn) != 0);

  parse_http_message(req2, sizeof(req2) - 1, &conn);
  ASSERT(should_keep_alive(&conn) == 0);

  parse_http_message(req3, sizeof(req3) - 1, &conn);
  ASSERT(should_keep_alive(&conn) == 0);

  parse_http_message(req4, sizeof(req4) - 1, &conn);
  ASSERT(should_keep_alive(&conn) != 0);

  return NULL;
}

static const char *test_match_prefix(void) {
  ASSERT(match_prefix("/api", 4, "/api") == 4);
  ASSERT(match_prefix("/a/", 3, "/a/b/c") == 3);
  ASSERT(match_prefix("/a/", 3, "/ab/c") == -1);
  ASSERT(match_prefix("/*/", 3, "/ab/c") == 4);
  ASSERT(match_prefix("**", 2, "/a/b/c") == 6);
  ASSERT(match_prefix("/*", 2, "/a/b/c") == 2);
  ASSERT(match_prefix("*/*", 3, "/a/b/c") == 2);
  ASSERT(match_prefix("**/", 3, "/a/b/c") == 5);
  ASSERT(match_prefix("**.foo|**.bar", 13, "a.bar") == 5);
  ASSERT(match_prefix("a|b|cd", 6, "cdef") == 2);
  ASSERT(match_prefix("a|b|c?", 6, "cdef") == 2);
  ASSERT(match_prefix("a|?|cd", 6, "cdef") == 1);
  ASSERT(match_prefix("/a/**.cgi", 9, "/foo/bar/x.cgi") == -1);
  ASSERT(match_prefix("/a/**.cgi", 9, "/a/bar/x.cgi") == 12);
  ASSERT(match_prefix("**/", 3, "/a/b/c") == 5);
  ASSERT(match_prefix("**/$", 4, "/a/b/c") == -1);
  ASSERT(match_prefix("**/$", 4, "/a/b/") == 5);
  ASSERT(match_prefix("$", 1, "") == 0);
  ASSERT(match_prefix("$", 1, "x") == -1);
  ASSERT(match_prefix("*$", 2, "x") == 1);
  ASSERT(match_prefix("/$", 2, "/") == 1);
  ASSERT(match_prefix("**/$", 4, "/a/b/c") == -1);
  ASSERT(match_prefix("**/$", 4, "/a/b/") == 5);
  ASSERT(match_prefix("*", 1, "/hello/") == 0);
  ASSERT(match_prefix("**.a$|**.b$", 11, "/a/b.b/") == -1);
  ASSERT(match_prefix("**.a$|**.b$", 11, "/a/b.b") == 6);
  ASSERT(match_prefix("**.a$|**.b$", 11, "/a/B.A") == 6);
  ASSERT(match_prefix("**o$", 4, "HELLO") == 5);
  return NULL;
}

static const char *test_remove_double_dots() {
  struct { char before[20], after[20]; } data[] = {
    {"////a", "/a"},
    {"/.....", "/."},
    {"/......", "/"},
    {"...", "..."},
    {"/...///", "/./"},
    {"/a...///", "/a.../"},
    {"/.x", "/.x"},
    {"/\\", "/"},
    {"/a\\", "/a\\"},
    {"/a\\\\...", "/a\\."},
  };
  size_t i;

  for (i = 0; i < ARRAY_SIZE(data); i++) {
    remove_double_dots_and_double_slashes(data[i].before);
    ASSERT(strcmp(data[i].before, data[i].after) == 0);
  }

  return NULL;
}

static const char *test_get_var(void) {
  static const char *post[] = {
    "a=1&&b=2&d&=&c=3%20&e=",
    "q=&st=2012%2F11%2F13+17%3A05&et=&team_id=",
    NULL
  };
  char buf[20];

  ASSERT(get_var(post[0], strlen(post[0]), "a", buf, sizeof(buf)) == 1);
  ASSERT(buf[0] == '1' && buf[1] == '\0');
  ASSERT(get_var(post[0], strlen(post[0]), "b", buf, sizeof(buf)) == 1);
  ASSERT(buf[0] == '2' && buf[1] == '\0');
  ASSERT(get_var(post[0], strlen(post[0]), "c", buf, sizeof(buf)) == 2);
  ASSERT(buf[0] == '3' && buf[1] == ' ' && buf[2] == '\0');
  ASSERT(get_var(post[0], strlen(post[0]), "e", buf, sizeof(buf)) == 0);
  ASSERT(buf[0] == '\0');

  ASSERT(get_var(post[0], strlen(post[0]), "d", buf, sizeof(buf)) == -1);
  ASSERT(get_var(post[0], strlen(post[0]), "c", buf, 2) == -2);

  ASSERT(get_var(post[0], strlen(post[0]), "x", NULL, 10) == -2);
  ASSERT(get_var(post[0], strlen(post[0]), "x", buf, 0) == -2);
  ASSERT(get_var(post[1], strlen(post[1]), "st", buf, 16) == -2);
  ASSERT(get_var(post[1], strlen(post[1]), "st", buf, 17) == 16);
  return NULL;
}

static const char *test_url_decode(void) {
  char buf[100];

  ASSERT(mg_url_decode("foo", 3, buf, 3, 0) == -1);  // No space for \0
  ASSERT(mg_url_decode("foo", 3, buf, 4, 0) == 3);
  ASSERT(strcmp(buf, "foo") == 0);

  ASSERT(mg_url_decode("a+", 2, buf, sizeof(buf), 0) == 2);
  ASSERT(strcmp(buf, "a+") == 0);

  ASSERT(mg_url_decode("a+", 2, buf, sizeof(buf), 1) == 2);
  ASSERT(strcmp(buf, "a ") == 0);

  ASSERT(mg_url_decode("%61", 1, buf, sizeof(buf), 1) == 1);
  ASSERT(strcmp(buf, "%") == 0);

  ASSERT(mg_url_decode("%61", 2, buf, sizeof(buf), 1) == 2);
  ASSERT(strcmp(buf, "%6") == 0);

  ASSERT(mg_url_decode("%61", 3, buf, sizeof(buf), 1) == 1);
  ASSERT(strcmp(buf, "a") == 0);
  return NULL;
}

static const char *test_to64(void) {
  ASSERT(to64("0") == 0);
  ASSERT(to64("") == 0);
  ASSERT(to64("123") == 123);
  ASSERT(to64("-34") == -34);
  ASSERT(to64("3566626116") == 3566626116);
  return NULL;
}

static const char *test_base64_encode(void) {
  const char *in[] = {"a", "ab", "abc", "abcd", NULL};
  const char *out[] = {"YQ==", "YWI=", "YWJj", "YWJjZA=="};
  char buf[100];
  int i;

  for (i = 0; in[i] != NULL; i++) {
    base64_encode((unsigned char *) in[i], strlen(in[i]), buf);
    ASSERT(!strcmp(buf, out[i]));
  }

  return NULL;
}

static const char *test_mg_parse_header(void) {
  const char *str = "xx=1 kl yy, ert=234 kl=123, "
    "ii=\"12\\\"34\" zz='aa bb',tt=2,gf=\"xx d=1234";
  char buf[10];
  ASSERT(mg_parse_header(str, "yy", buf, sizeof(buf)) == 0);
  ASSERT(mg_parse_header(str, "ert", buf, sizeof(buf)) == 3);
  ASSERT(strcmp(buf, "234") == 0);
  ASSERT(mg_parse_header(str, "ert", buf, 2) == 0);
  ASSERT(mg_parse_header(str, "ert", buf, 3) == 0);
  ASSERT(mg_parse_header(str, "ert", buf, 4) == 3);
  ASSERT(mg_parse_header(str, "gf", buf, sizeof(buf)) == 0);
  ASSERT(mg_parse_header(str, "zz", buf, sizeof(buf)) == 5);
  ASSERT(strcmp(buf, "aa bb") == 0);
  ASSERT(mg_parse_header(str, "d", buf, sizeof(buf)) == 4);
  ASSERT(strcmp(buf, "1234") == 0);
  buf[0] = 'x';
  ASSERT(mg_parse_header(str, "MMM", buf, sizeof(buf)) == 0);
  ASSERT(buf[0] == '\0');
  ASSERT(mg_parse_header(str, "kl", buf, sizeof(buf)) == 3);
  ASSERT(strcmp(buf, "123") == 0);
  ASSERT(mg_parse_header(str, "xx", buf, sizeof(buf)) == 1);
  ASSERT(strcmp(buf, "1") == 0);
  ASSERT(mg_parse_header(str, "ii", buf, sizeof(buf)) == 5);
  ASSERT(strcmp(buf, "12\"34") == 0);
  ASSERT(mg_parse_header(str, "tt", buf, sizeof(buf)) == 1);
  ASSERT(strcmp(buf, "2") == 0);
  return NULL;
}

static const char *test_next_option(void) {
  const char *p, *list = "x/8,/y**=1;2k,z";
  struct vec a, b;
  int i;

  ASSERT(next_option(NULL, &a, &b) == NULL);
  for (i = 0, p = list; (p = next_option(p, &a, &b)) != NULL; i++) {
    ASSERT(i != 0 || (a.ptr == list && a.len == 3 && b.len == 0));
    ASSERT(i != 1 || (a.ptr == list + 4 && a.len == 4 && b.ptr == list + 9 &&
                      b.len == 4));

    ASSERT(i != 2 || (a.ptr == list + 14 && a.len == 1 && b.len == 0));
  }
  return NULL;
}

static int cb1(struct mg_connection *conn) {
  int result = MG_REQUEST_NOT_PROCESSED;
  if (!strcmp(conn->uri, "/cb1")) {
    mg_printf(conn, "%s %s %s",
              conn->server_param == NULL ? "?" : (char *) conn->server_param,
              conn->connection_param == NULL ? "?" : "!", conn->remote_ip);
    result = MG_REQUEST_PROCESSED;
  }
  return result;
}

static int error_handler(struct mg_connection *conn) {
  mg_printf(conn, "HTTP/1.0 404 NF\r\n\r\nERR: %d", conn->status_code);
  return MG_ERROR_PROCESSED;
}

static int ts1(struct mg_connection *conn) {
  if (conn->status_code == MG_CONNECT_SUCCESS) {
    mg_printf(conn, "%s", "GET /cb1 HTTP/1.0\r\n\r\n");
    return MG_CLIENT_CONTINUE;
  } else if (conn->status_code == MG_DOWNLOAD_SUCCESS) {
    sprintf((char *) conn->connection_param, "%.*s",
            (int) conn->content_len, conn->content);
  }
  return MG_CLIENT_CLOSE;
}

static int ts2(struct mg_connection *conn) {
  if (conn->status_code == MG_CONNECT_SUCCESS) {
    mg_printf(conn, "%s", "GET /non_exist HTTP/1.0\r\n\r\n");
    return MG_CLIENT_CONTINUE;
  } else if (conn->status_code == MG_DOWNLOAD_SUCCESS) {
    sprintf((char *) conn->connection_param, "%s %.*s",
            conn->uri, (int) conn->content_len, conn->content);
  }
  return MG_CLIENT_CLOSE;
}

static const char *test_server(void) {
  char buf1[100] = "", buf2[100] = "";
  struct mg_server *server = mg_create_server((void *) "foo");

  ASSERT(server != NULL);
  ASSERT(mg_set_option(server, "listening_port", LISTENING_ADDR) == NULL);
  ASSERT(mg_set_option(server, "document_root", ".") == NULL);
  mg_set_request_handler(server, cb1);
  mg_set_http_error_handler(server, error_handler);

  ASSERT(mg_connect(server, "127.0.0.1", atoi(HTTP_PORT),  0, ts1, buf1) == 1);
  ASSERT(mg_connect(server, "127.0.0.1", atoi(HTTP_PORT), 0, ts2, buf2) == 1);

  { int i; for (i = 0; i < 50; i++) mg_poll_server(server, 1); }
  ASSERT(strcmp(buf1, "foo ? 127.0.0.1") == 0);
  ASSERT(strcmp(buf2, "404 ERR: 404") == 0);

  ASSERT(strcmp(static_config_options[URL_REWRITES * 2], "url_rewrites") == 0);
  mg_destroy_server(&server);
  ASSERT(server == NULL);
  return NULL;
}

// This http client sends two requests using one connection
static int cb2(struct mg_connection *conn) {
  const char *file1 = "mongoose.h", *file2 = "mongoose.c", *file_name = file2;
  char *buf = (char *) conn->connection_param, *file_data;
  int file_len, ret_val = 1;

  switch (conn->status_code) {
    case MG_CONNECT_SUCCESS:
      mg_printf(conn, "GET /%s HTTP/1.1\r\n\r\n", file1);
      strcat(buf, "a");
      ret_val = 0;
      break;
    case MG_CONNECT_FAILURE: strcat(buf, "b"); break;
    case MG_DOWNLOAD_FAILURE: strcat(buf, "c"); break;
    case MG_DOWNLOAD_SUCCESS:
      if (!strcmp(buf, "a")) {
        // Send second request
        mg_printf(conn, "GET /%s HTTP/1.1\r\n\r\n", file2);
        file_name = file1;
      }
      if ((file_data = read_file(file_name, &file_len)) != NULL) {
        if ((size_t) file_len == conn->content_len &&
            memcmp(file_data, conn->content, file_len) == 0) {
          strcat(buf, "d");
        } else {
          strcat(buf, "f");
        }
        free(file_data);
      }
      ret_val = !strcmp(buf, "ad") ? 0 : 1;
      break;
    default: strcat(buf, "e"); break;
  }

  return ret_val;
}

static int cb4h(struct mg_connection *conn) {
  int result = MG_REQUEST_NOT_PROCESSED;
  if (!strcmp(conn->uri, "/x")) {
    mg_send_data(conn, ":-)", 3);
    result = MG_REQUEST_PROCESSED;
  }
  return result;
}

static int cb4(struct mg_connection *conn) {
  if (conn->status_code == MG_CONNECT_SUCCESS) {
    mg_printf(conn, "%s", "POST /x HTTP/1.0\r\nContent-Length: 999999\r\n\r\n");
    return MG_CLIENT_CONTINUE;
  } else {
    sprintf((char *) conn->connection_param, "%.*s",
            (int) conn->content_len, conn->content);
  }
  return MG_CLIENT_CLOSE;
}

static int cb3(struct mg_connection *conn) {
  sprintf((char *) conn->connection_param, "%d", conn->status_code);
  return MG_CLIENT_CLOSE;
}

static const char *test_mg_connect(void) {
  char buf2[40] = "", buf3[40] = "", buf4[40] = "";
  struct mg_server *server = mg_create_server(NULL);

  ASSERT(mg_set_option(server, "listening_port", LISTENING_ADDR) == NULL);
  ASSERT(mg_set_option(server, "document_root", ".") == NULL);
  mg_set_request_handler(server, cb4h);
  ASSERT(mg_connect(server, "", 0, 0, NULL, NULL) == 0);
  ASSERT(mg_connect(server, "127.0.0.1", atoi(HTTP_PORT), 0, cb2, buf2) == 1);
  ASSERT(mg_connect(server, "127.0.0.1", 29, 0, cb3, buf3) == 1);
  ASSERT(mg_connect(server, "127.0.0.1", atoi(HTTP_PORT), 0, cb4, buf4) == 1);

  { int i; for (i = 0; i < 50; i++) mg_poll_server(server, 1); }

  printf("buf2: [%s]\n", buf2);
  //ASSERT(strcmp(buf2, "add") == 0);
  ASSERT(strcmp(buf3, "1") == 0);
  ASSERT(strcmp(buf4, "500 Server Error\nPOST size > 999") == 0);
  mg_destroy_server(&server);

  return NULL;
}

#define DISP "Content-Disposition: form/data; "
#define CRLF "\r\n"
#define BOUNDARY "--xyz"
static const char *test_parse_multipart(void) {
  char a[100], b[100];
  const char *p;
  static const char f1[] = BOUNDARY CRLF DISP "name=f1" CRLF CRLF
    "some_content" CRLF BOUNDARY CRLF
    BOUNDARY CRLF DISP "name=f2; filename=\"foo bar.txt\"" CRLF CRLF
    "another_content" CRLF BOUNDARY CRLF
    "--" CRLF;
  int n, n2, len, f1_len = sizeof(f1) - 1;

  ASSERT(mg_parse_multipart("", 0, a, sizeof(a), b, sizeof(b), &p, &len) == 0);
  ASSERT(mg_parse_multipart("a", 1, a, sizeof(a), b, sizeof(b), &p, &len) == 0);
  ASSERT((n = mg_parse_multipart(f1, f1_len, a, sizeof(a),
                                 b, sizeof(b), &p, &len)) > 0);
  ASSERT(len == 12);
  ASSERT(memcmp(p, "some_content", len) == 0);
  ASSERT(strcmp(a, "f1") == 0);
  ASSERT(b[0] == '\0');

  ASSERT((n2 = mg_parse_multipart(f1 + n, f1_len - n, a, sizeof(a),
                                  b, sizeof(b), &p, &len)) > 0);
  ASSERT(len == 15);
  ASSERT(memcmp(p, "another_content", len) == 0);
  ASSERT(strcmp(a, "f2") == 0);
  ASSERT(strcmp(b, "foo bar.txt") == 0);

  ASSERT((n2 = mg_parse_multipart(f1 + n + n2, f1_len - (n + n2), a, sizeof(a),
                                  b, sizeof(b), &p, &len)) == 0);

  return NULL;
}

static int us1(struct mg_connection *conn) {
  static const char *file_name = "mongoose.h";
  char *cp = (char *) conn->connection_param;
  int file_size;

  if (conn->status_code == MG_CONNECT_SUCCESS) {
    mg_printf(conn, "GET /%s HTTP/1.0\r\n\r\n", cp);
    return 0;
  } else if (conn->status_code == MG_DOWNLOAD_SUCCESS) {
    char *file_data = read_file(file_name, &file_size);
    sprintf(cp, "%d %s", (size_t) file_size == conn->content_len &&
            memcmp(file_data, conn->content, file_size) == 0 ? 1 : 0,
            conn->query_string == NULL ? "?" : conn->query_string);
    free(file_data);
  } else if (conn->status_code == MG_CONNECT_FAILURE) {
    sprintf(cp, "%s", "cf");
  } else {
    sprintf(cp, "%s", "df");
  }
  return 1;
}

#ifdef MONGOOSE_USE_SSL
static const char *test_ssl(void) {
  static const char *ssl_cert = "examples/ssl_cert.pem";
  char buf1[100] = "mongoose.h";
  struct mg_server *server = mg_create_server(NULL);

  ASSERT(mg_set_option(server, "listening_port", LISTENING_ADDR) == NULL);
  ASSERT(mg_set_option(server, "document_root", ".") == NULL);
  ASSERT(mg_set_option(server, "ssl_certificate", ssl_cert) == NULL);

  ASSERT(mg_connect(server, "127.0.0.1", atoi(HTTP_PORT), 1, us1, buf1) == 1);

  { int i; for (i = 0; i < 50; i++) mg_poll_server(server, 1); }
  ASSERT(strcmp(buf1, "1 ?") == 0);
  mg_destroy_server(&server);
  return NULL;
}
#endif

static const char *test_mg_set_option(void) {
  struct mg_server *server = mg_create_server(NULL);
  ASSERT(mg_set_option(server, "listening_port", "0") == NULL);
  ASSERT(mg_get_option(server, "listening_port")[0] != '\0');
  mg_destroy_server(&server);
  return NULL;
}

static const char *test_rewrites(void) {
  char buf1[100] = "xx";
  struct mg_server *server = mg_create_server(NULL);

  ASSERT(mg_set_option(server, "listening_port", "0") == NULL);
  ASSERT(mg_set_option(server, "document_root", ".") == NULL);
  ASSERT(mg_set_option(server, "url_rewrites", "/xx=./mongoose.h") == NULL);

  ASSERT(mg_connect(server, "127.0.0.1",
                    atoi(mg_get_option(server, "listening_port")),
                    0, us1, buf1) == 1);

  { int i; for (i = 0; i < 50; i++) mg_poll_server(server, 1); }
  //printf("[%s]\n", buf1);
  ASSERT(strcmp(buf1, "1 ?") == 0);
  mg_destroy_server(&server);
  return NULL;
}

static const char *run_all_tests(void) {
  RUN_TEST(test_should_keep_alive);
  RUN_TEST(test_match_prefix);
  RUN_TEST(test_remove_double_dots);
  RUN_TEST(test_parse_http_message);
  RUN_TEST(test_to64);
  RUN_TEST(test_url_decode);
  RUN_TEST(test_base64_encode);
  RUN_TEST(test_mg_parse_header);
  RUN_TEST(test_get_var);
  RUN_TEST(test_next_option);
  RUN_TEST(test_parse_multipart);
  RUN_TEST(test_mg_set_option);
  //RUN_TEST(test_server);
  //RUN_TEST(test_mg_connect);
  //RUN_TEST(test_rewrites);
#ifdef MONGOOSE_USE_SSL
  RUN_TEST(test_ssl);
#endif
  return NULL;
}

int __cdecl main(void) {
  const char *fail_msg = run_all_tests();
  printf("%s, tests run: %d\n", fail_msg ? "FAIL" : "PASS", static_num_tests);
  return fail_msg == NULL ? EXIT_SUCCESS : EXIT_FAILURE;
}
