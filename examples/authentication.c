#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "mongoose.h"

/*
 * Cookie based authentication
 * taken from http://en.wikipedia.org/wiki/HTTP_cookie#Authentication
 *
 * 1. The user inserts his or her username and password in the text fields
 *    of a login page and sends them to the server;
 * 2. The server receives the username and password and checks them; if
 *    correct, it sends back a page confirming that login has been successful
 *    together with a cookie containing a random session ID that coincides with
 *    a session stored in a database. This cookie is usually made valid for
 *    only the current browser session, however it may also be set to expire at
 *    a future date. The random session ID is then provided on future visits
 *    and provides a way for the server to uniquely identify the browser and
 *    confirm that the browser already has an authenticated user.
 * 3. Every time the user requests a page from the server, the browser
 *    automatically sends the cookie back to the server; the server compares
 *    the cookie with the stored ones; if a match is found, the server knows
 *    which user has requested that page.
 */

static void
login_page(struct mg_connection *conn,
		const struct mg_request_info *ri, void *data)
{
	char		*name, *pass, uri[100];
	const char	*cookie;

	name = mg_get_var(conn, "name");
	pass = mg_get_var(conn, "pass");
	cookie = mg_get_header(conn, "Cookie");

	/*
	 * Here user name and password must be checked against some
	 * database - this is step 2 from the algorithm described above.
	 * This is an example, so hardcode name and password to be
	 * admin/admin, and if this is so, set "allow=yes" cookie and
	 * redirect back to the page where we have been redirected to login.
	 */
	if (name != NULL && pass != NULL &&
	    strcmp(name, "admin") == 0 && strcmp(pass, "admin") == 0) {
		if (cookie == NULL || sscanf(cookie, "uri=%99s", uri) != 1)
			(void) strcpy(uri, "/");
		/* Set allow=yes cookie, which is expected by authorize() */
		mg_printf(conn, "HTTP/1.1 301 Moved Permanently\r\n"
		    "Location: %s\r\n"
		    "Set-Cookie: allow=yes;\r\n\r\n", uri);
	} else {
		/* Print login page */
		mg_printf(conn, "HTTP/1.1 200 OK\r\n"
		    "content-Type: text/html\r\n\r\n"
		    "Please login (enter admin/admin to pass)<br>"
		    "<form method=post>"
		    "Name: <input type=text name=name></input><br/>"
		    "Password: <input type=password name=pass></input><br/>"
		    "<input type=submit value=Login></input>"
		    "</form>");
	}

	if (name != NULL)
		mg_free(name);
	if (pass != NULL)
		mg_free(pass);
}

static void
authorize(struct mg_connection *conn,
		const struct mg_request_info *ri, void *data)
{
	const char	*cookie, *domain;

	cookie = mg_get_header(conn, "Cookie");

	if (!strcmp(ri->uri, "/login")) {
		/* Always authorize accesses to the login page */
		mg_authorize(conn);
	} else if (cookie != NULL && strstr(cookie, "allow=yes") != NULL) {
		/* Valid cookie is present, authorize */
		mg_authorize(conn);
	} else {
		/* Not authorized. Redirect to the login page */
		mg_printf(conn, "HTTP/1.1 301 Moved Permanently\r\n"
		    "Set-Cookie: uri=%s;\r\n"
		    "Location: /login\r\n\r\n", ri->uri);
	}
}


int
main(int argc, char *argv[])
{
	struct mg_context *ctx;

	ctx = mg_start();
	mg_set_option(ctx, "ports", "8080");
	mg_set_auth_callback(ctx, "/*", &authorize, NULL);
	mg_set_uri_callback(ctx, "/login", &login_page, NULL);

	for (;;)
		sleep(1);
}
