---
title: "struct http_message"
decl_name: "struct http_message"
symbol_kind: "struct"
signature: |
  struct http_message {
    struct mg_str message; /* Whole message: request line + headers + body */
  
    /* HTTP Request line (or HTTP response line) */
    struct mg_str method; /* "GET" */
    struct mg_str uri;    /* "/my_file.html" */
    struct mg_str proto;  /* "HTTP/1.1" -- for both request and response */
  
    /* For responses, code and response status message are set */
    int resp_code;
    struct mg_str resp_status_msg;
  
    /*
     * Query-string part of the URI. For example, for HTTP request
     *    GET /foo/bar?param1=val1&param2=val2
     *    |    uri    |     query_string     |
     *
     * Note that question mark character doesn't belong neither to the uri,
     * nor to the query_string
     */
    struct mg_str query_string;
  
    /* Headers */
    struct mg_str header_names[MG_MAX_HTTP_HEADERS];
    struct mg_str header_values[MG_MAX_HTTP_HEADERS];
  
    /* Message body */
    struct mg_str body; /* Zero-length for requests with no body */
  };
---

HTTP message 

