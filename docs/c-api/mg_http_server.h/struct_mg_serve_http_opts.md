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
---

This structure defines how `mg_serve_http()` works.
Best practice is to set only required settings, and leave the rest as NULL. 

