---
title: "struct mg_serve_http_opts"
decl_name: "struct mg_serve_http_opts"
symbol_kind: "struct"
signature: |
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
  
    /* SSI files pattern. If not set, "**.shtml$|**.shtm$" is used. */
    const char *ssi_pattern;
  
    /* IP ACL. By default, NULL, meaning all IPs are allowed to connect */
    const char *ip_acl;
  
    /* URL rewrites.
     *
     * Comma-separated list of `uri_pattern=file_or_directory_path` rewrites.
     * When HTTP request is received, Mongoose constructs a file name from the
     * requested URI by combining `document_root` and the URI. However, if the
     * rewrite option is used and `uri_pattern` matches requested URI, then
     * `document_root` is ignored. Instead, `file_or_directory_path` is used,
     * which should be a full path name or a path relative to the web server's
     * current working directory. Note that `uri_pattern`, as all Mongoose
     * patterns, is a prefix pattern.
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
---

This structure defines how `mg_serve_http()` works.
Best practice is to set only required settings, and leave the rest as NULL. 

