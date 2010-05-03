/*
 * This file is an example of how to embed web-server functionality
 * into existing application.
 * Compilation line (from Mongoose sources root directory):
 * cc mongoose.c examples/example.c -I. -lpthread -o example
 */

#ifdef _WIN32
#include <winsock.h>
#define	snprintf			_snprintf

#ifndef _WIN32_WCE
#ifdef _MSC_VER /* pragmas not valid on MinGW */
#endif /* _MSC_VER */
#define ALIAS_URI "/my_c"
#define ALIAS_DIR "c:\\"

#else /* _WIN32_WCE */
/* Windows CE-specific definitions */
#pragma comment(lib,"ws2")
//#include "compat_wince.h"
#define ALIAS_URI "/my_root"
#define ALIAS_DIR "\\"
#endif /* _WIN32_WCE */

#else
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#define ALIAS_URI "/my_etc"
#define ALIAS_DIR "/etc/"
#endif

#ifndef _WIN32_WCE /* Some ANSI #includes are not available on Windows CE */
#include <time.h>
#include <errno.h>
#include <signal.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "mongoose.h"

/*
 * This callback function is attached to the "/" and "/abc.html" URIs,
 * thus is acting as "index.html" file. It shows a bunch of links
 * to other URIs, and allows to change the value of program's
 * internal variable. The pointer to that variable is passed to the
 * callback function as arg->user_data.
 */
static void
show_index(struct mg_connection *conn,
		const struct mg_request_info *request_info,
		void *user_data)
{
	char		*value;
	const char	*host;

	/* Change the value of integer variable */
	value = mg_get_var(conn, "name1");
	if (value != NULL) {
		* (int *) user_data = atoi(value);
		mg_free(value);

		/*
		 * Suggested by Luke Dunstan. When POST is used,
		 * send 303 code to force the browser to re-request the
		 * page using GET method. This prevents the possibility of
		 * the user accidentally resubmitting the form when using
		 * Refresh or Back commands in the browser.
		 */
		if (!strcmp(request_info->request_method, "POST")) {
			(void) mg_printf(conn, "HTTP/1.1 303 See Other\r\n"
				"Location: %s\r\n\r\n", request_info->uri);
			return;
		}
	}

	mg_printf(conn, "%s",
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
		"<html><body><h1>Welcome to embedded example of Mongoose");
	mg_printf(conn, " v. %s </h1><ul>", mg_version());

	mg_printf(conn, "<li><code>REQUEST_METHOD: %s "
	    "REQUEST_URI: \"%s\" QUERY_STRING: \"%s\""
	    " REMOTE_ADDR: %lx REMOTE_USER: \"(null)\"</code><hr>",
	    request_info->request_method, request_info->uri,
	    request_info->query_string ? request_info->query_string : "(null)",
	    request_info->remote_ip);
	mg_printf(conn, "<li>Internal int variable value: <b>%d</b>",
			* (int *) user_data);

	mg_printf(conn, "%s",
		"<form method=\"GET\">Enter new value: "
		"<input type=\"text\" name=\"name1\"/>"
		"<input type=\"submit\" "
		"value=\"set new value using GET method\"></form>");
	mg_printf(conn, "%s",
		"<form method=\"POST\">Enter new value: "
		"<input type=\"text\" name=\"name1\"/>"
		"<input type=\"submit\" "
		"value=\"set new value using POST method\"></form>");
		
	mg_printf(conn, "%s",
		"<hr><li><a href=\"/secret\">"
		"Protected page</a> (guest:guest)<hr>"
		"<li><a href=\"/huge\">Output lots of data</a><hr>"
		"<li><a href=\"" ALIAS_URI "/\">Aliased "
		ALIAS_DIR " directory</a><hr>");
	mg_printf(conn, "%s",
		"<li><a href=\"/Makefile\">Regular file (Makefile)</a><hr>"
		"<li><a href=\"/ssi_test.shtml\">SSI file "
			"(ssi_test.shtml)</a><hr>"
		"<li><a href=\"/users/joe/\">Wildcard URI example</a><hr>"
		"<li><a href=\"/not-existent/\">Custom 404 handler</a><hr>");

	host = mg_get_header(conn, "Host");
	mg_printf(conn, "<li>'Host' header value: [%s]<hr>",
	    host ? host : "NOT SET");

	mg_printf(conn, "<li>Upload file example. "
	    "<form method=\"post\" enctype=\"multipart/form-data\" "
	    "action=\"/post\"><input type=\"file\" name=\"file\">"
	    "<input type=\"submit\"></form>");

	mg_printf(conn, "%s", "</body></html>");
}

/*
 * This callback is attached to the URI "/post"
 * It uploads file from a client to the server. This is the demostration
 * of how to use POST method to send lots of data from the client.
 * The uploaded file is saved into "uploaded.txt".
 * This function is called many times during single request. To keep the
 * state (how many bytes we have received, opened file etc), we allocate
 * a "struct state" structure for every new connection.
 */
static void
show_post(struct mg_connection *conn,
		const struct mg_request_info *request_info,
		void *user_data)
{
	const char	*path = "uploaded.txt";
	FILE		*fp;

	mg_printf(conn, "HTTP/1.0 200 OK\nContent-Type: text/plain\n\n");

	/*
	 * Open a file and write POST data into it. We do not do any URL
	 * decoding here. File will contain form-urlencoded stuff.
	 */
	if ((fp = fopen(path, "wb+")) == NULL) {
		(void) fprintf(stderr, "Error opening %s: %s\n",
		    path, strerror(errno));
	} else if (fwrite(request_info->post_data,
	    request_info->post_data_len, 1, fp) != 1) {
		(void) fprintf(stderr, "Error writing to %s: %s\n",
		    path, strerror(errno));
	} else {
		/* Write was successful */
		(void) fclose(fp);
	}
}

/*
 * This callback function is attached to the "/secret" URI.
 * It shows simple text message, but in order to be shown, user must
 * authorized himself against the passwords file "passfile".
 */
static void
show_secret(struct mg_connection *conn,
		const struct mg_request_info *request_info,
		void *user_data)
{
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "%s", "Content-Type: text/html\r\n\r\n");
	mg_printf(conn, "%s", "<html><body>");
	mg_printf(conn, "%s", "<p>This is a protected page</body></html>");
}

/*
 * This callback function is attached to the "/huge" URI.
 * It outputs binary data to the client.
 * The number of bytes already sent is stored directly in the arg->state.
 */
static void
show_huge(struct mg_connection *conn,
		const struct mg_request_info *request_info,
		void *user_data)
{
	int		i;
	const char	*line = "Hello, this is a line of text";

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "%s", "Content-Type: text/plain\r\n\r\n");

	for (i = 0; i < 1024 * 1024; i++)
		mg_printf(conn, "%s\n", line);
}

/*
 * This callback function is used to show how to handle 404 error
 */
static void
show_404(struct mg_connection *conn,
		const struct mg_request_info *request_info,
		void *user_data)
{
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "%s", "Content-Type: text/plain\r\n\r\n");
	mg_printf(conn, "%s", "Oops. File not found! ");
	mg_printf(conn, "%s", "This is a custom error handler.");
}

/*
 * This callback function is attached to the wildcard URI "/users/.*"
 * It shows a greeting message and an actual URI requested by the user.
 */
static void
show_users(struct mg_connection *conn,
		const struct mg_request_info *request_info,
		void *user_data)
{
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "%s", "Content-Type: text/html\r\n\r\n");
	mg_printf(conn, "%s", "<html><body>");
	mg_printf(conn, "%s", "<h1>Hi. This is a wildcard uri handler"
	    "for the URI /users/*/ </h1>");
	mg_printf(conn, "<h2>URI: %s</h2></body></html>", request_info->uri);
}

/*
 * Make sure we have ho zombies from CGIs
 */
static void
signal_handler(int sig_num)
{
	switch (sig_num) {
#ifndef _WIN32
	case SIGCHLD:
		while (waitpid(-1, &sig_num, WNOHANG) > 0) ;
		break;
#endif /* !_WIN32 */
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	int			data = 1234567;
	struct mg_context	*ctx;
	
	/* Get rid of warnings */
	argc = argc;
	argv = argv;

#ifndef _WIN32
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, &signal_handler);
#endif /* !_WIN32 */

	/*
	 * Initialize SHTTPD context.
	 * Attach folder c:\ to the URL /my_c  (for windows), and
	 * /etc/ to URL /my_etc (for UNIX). These are Apache-like aliases.
	 * Set WWW root to current directory.
	 * Start listening on ports 8080 and 8081
	 */
	ctx = mg_start();
	mg_set_option(ctx, "ssl_cert", "ssl_cert.pem");
	mg_set_option(ctx, "aliases", ALIAS_URI "=" ALIAS_DIR);
	mg_set_option(ctx, "ports", "8080,8081s");

	/* Register an index page under two URIs */
	mg_set_uri_callback(ctx, "/", &show_index, (void *) &data);
	mg_set_uri_callback(ctx, "/abc.html", &show_index, (void *) &data);

	/* Register a callback on wildcard URI */
	mg_set_uri_callback(ctx, "/users/*/", &show_users, NULL);

	/* Show how to use password protection */
	mg_set_uri_callback(ctx, "/secret", &show_secret, NULL);
	mg_set_option(ctx, "protect", "/secret=passfile");

	/* Show how to use stateful big data transfer */
	mg_set_uri_callback(ctx, "/huge", &show_huge, NULL);

	/* Register URI for file upload */
	mg_set_uri_callback(ctx, "/post", &show_post, NULL);

	mg_set_error_callback(ctx, 404, show_404, NULL);

	/* Wait until user presses 'enter' on console */
	(void) getchar();
	mg_stop(ctx);

	return (EXIT_SUCCESS);
}
