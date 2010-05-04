/*
 * This file is part of the Mongoose project, http://code.google.com/p/mongoose
 * It implements an online chat server. For more details,
 * see the documentation on project page.
 * To start the server,
 *  a) type "make" in the directory where this file lives
 *  b) point your browser to http://127.0.0.1:8081
 *
 * NOTE(lsm): this file follows Google style, not BSD style as the rest of
 * Mongoose code.
 *
 * $Id: chat.c 513 2010-05-03 11:06:08Z valenok $
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "mongoose.h"

#define MAX_USER_LEN  20
#define MAX_MESSAGE_LEN  100
#define MAX_MESSAGES 5
#define MAX_SESSIONS 2

static const char *login_url = "/login.html";
static const char *authorize_url = "/authorize";
static const char *web_root = "./html";
static const char *http_ports = "8081,8082s";
static const char *ssl_certificate = "ssl_cert.pem";

static const char *ajax_reply_start =
  "HTTP/1.1 200 OK\r\n"
  "Cache: no-cache\r\n"
  "Content-Type: application/x-javascript\r\n"
  "\r\n";

// Describes single message sent to a chat. If user is empty (0 length),
// the message is then originated from the server itself.
struct message {
  long id;
  char user[MAX_USER_LEN];
  char text[MAX_MESSAGE_LEN];
  time_t utc_timestamp;
};

// Describes web session.
struct session {
  char session_id[33];
  char authenticated_user[MAX_USER_LEN];
  time_t expiration_timestamp_utc;
};

static struct message messages[MAX_MESSAGES];  // Ringbuffer for messages
static struct session sessions[MAX_SESSIONS];  // Current sessions
static long last_message_id;

// Protects messages, sessions, last_message_id
static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

// Get a get of messages with IDs greater than last_id and transform them
// into a JSON string. Return that string to the caller. The string is
// dynamically allocated, caller must free it. If there are no messages,
// NULL is returned.
static char *messages_to_json(long last_id) {
  const struct message *message;
  int max_msgs, len;
  char buf[sizeof(messages)];  // Large enough to hold all messages

  // Read-lock the ringbuffer. Loop over all messages, making a JSON string.
  pthread_rwlock_rdlock(&rwlock);
  len = 0;
  max_msgs = sizeof(messages) / sizeof(messages[0]);
  // If client is too far behind, return all messages.
  if (last_message_id - last_id > max_msgs) {
    last_id = last_message_id - max_msgs;
  }
  for (; last_id < last_message_id; last_id++) {
    message = &messages[last_id % max_msgs];
    if (message->utc_timestamp == 0) {
      break;
    }
    // buf is allocated on stack and hopefully is large enough to hold all
    // messages (it may be too small if the ringbuffer is full and all
    // messages are large. in this case asserts will trigger).
    len += snprintf(buf + len, sizeof(buf) - len,
        "{user: '%s', text: '%s', timestamp: %lu, id: %lu},",
        message->user, message->text, message->utc_timestamp, message->id);
    assert(len > 0);
    assert((size_t) len < sizeof(buf));
  }
  pthread_rwlock_unlock(&rwlock);

  return len == 0 ? NULL : strdup(buf);
}

// If "callback" param is present in query string, this is JSONP call.
// Return 1 in this case, or 0 if "callback" is not specified.
// Wrap an output in Javascript function call.
static int handle_jsonp(struct mg_connection *conn,
    const struct mg_request_info *request_info) {
  char cb[64];

  mg_get_qsvar(request_info, "callback", cb, sizeof(cb));
  if (cb[0] != '\0') {
    mg_printf(conn, "%s(", cb);
  }
 
  return cb[0] == '\0' ? 0 : 1;
}

// A handler for the /ajax/get_messages endpoint.
// Return a list of messages with ID greater than requested.
static void ajax_get_messages(struct mg_connection *conn,
    const struct mg_request_info *request_info) {
  char last_id[32], *json;
  int is_jsonp;

  mg_printf(conn, "%s", ajax_reply_start);
  is_jsonp = handle_jsonp(conn, request_info);

  mg_get_qsvar(request_info, "last_id", last_id, sizeof(last_id));
  if ((json = messages_to_json(strtoul(last_id, NULL, 10))) != NULL) {
    mg_printf(conn, "[%s]", json);
    free(json);
  }

  if (is_jsonp) {
    mg_printf(conn, "%s", ")");
  }
}

// A handler for the /ajax/send_message endpoint.
static void ajax_send_message(struct mg_connection *conn,
    const struct mg_request_info *request_info) {
  struct message *message;
  char text[sizeof(message->text) - 1];
  int is_jsonp;

  mg_printf(conn, "%s", ajax_reply_start);
  is_jsonp = handle_jsonp(conn, request_info);

  (void) mg_get_qsvar(request_info, "text", text, sizeof(text));
  if (text[0] != '\0') {
    // We have a message to store. Write-lock the ringbuffer,
    // grab the next message and copy data into it.
    pthread_rwlock_wrlock(&rwlock);
    message = &messages[last_message_id %
      (sizeof(messages) / sizeof(messages[0]))];
    // TODO(lsm): JSON-encode all text strings
    strncpy(message->text, text, sizeof(text));
    strncpy(message->user, "joe", sizeof(message->user));
    message->utc_timestamp = time(0);
    message->id = last_message_id++;
    pthread_rwlock_unlock(&rwlock);
  }

  mg_printf(conn, "%s", text[0] == '\0' ? "false" : "true");

  if (is_jsonp) {
    mg_printf(conn, "%s", ")");
  }
}

// Redirect user to the login form. In the cookie, store the original URL
// we came from, so that after the authorization we could redirect back.
static void redirect_to_login(struct mg_connection *conn,
    const struct mg_request_info *request_info) {
  mg_printf(conn, "HTTP/1.1 302 Found\r\n"
      "Set-Cookie: original_url=%s\r\n"
      "Location: %s\r\n\r\n", request_info->uri, login_url);
}

// Return 1 if username/password is allowed, 0 otherwise.
static int check_password(const char *user, const char *password) {
  // In production environment we should ask an authentication system
  // to authenticate the user.
  // Here however we do trivial check: if username == password, allow.
  return (strcmp(user, password) == 0 ? 1 : 0);
}

// A handler for the /authorize endpoint.
// Login page form sends user name and password to this endpoint.
static void authorize(struct mg_connection *conn,
    const struct mg_request_info *request_info) {
  char user[20], password[20], original_url[200];

  // Fetch user name and password.
  mg_get_qsvar(request_info, "user", user, sizeof(user));
  mg_get_qsvar(request_info, "password", password, sizeof(password));
  mg_get_cookie(conn, "original_url", original_url, sizeof(original_url));

  if (user[0] && password[0] && check_password(user, password)) {
    // Authentication success:
    //   1. create new session
    //   2. set session ID token in the cookie
    //   3. remove original_url from the cookie - not needed anymore
    //   4. redirect client back to the original URL
    // TODO(lsm): implement sessions.
    mg_printf(conn, "HTTP/1.1 302 Found\r\n"
        "Set-Cookie: sid=1234; max-age=2h; http-only\r\n"  // Set session ID
        "Set-Cookie: original_url=/; max_age=0\r\n"  // Delete original_url
        "Location: %s\r\n\r\n", original_url[0] == '\0' ? "/" : original_url);
  } else {
    // Authentication failure, redirect to login again.
    redirect_to_login(conn, request_info);
  }
}

// Return 1 if request is authorized, 0 otherwise.
static int is_authorized(const struct mg_connection *conn,
    const struct mg_request_info *request_info) {
  // TODO(lsm): implement this part: fetch session ID from the cookie.
  return 0;
}

// Return 1 if authorization is required for requested URL, 0 otherwise.
static int must_authorize(const struct mg_request_info *request_info) {
  return (strcmp(request_info->uri, login_url) != 0 &&
      strcmp(request_info->uri, authorize_url) != 0);
}

static enum mg_error_t process_request(struct mg_connection *conn,
    const struct mg_request_info *request_info) {
  enum mg_error_t processed = MG_SUCCESS;

  if (must_authorize(request_info) &&
      !is_authorized(conn, request_info)) {
    // If user is not authorized, redirect to the login form.
    redirect_to_login(conn, request_info);
  } else if (strcmp(request_info->uri, authorize_url) == 0) {
    authorize(conn, request_info);
  } else if (strcmp(request_info->uri, "/ajax/get_messages") == 0) {
    ajax_get_messages(conn, request_info);
  } else if (strcmp(request_info->uri, "/ajax/send_message") == 0) {
    ajax_send_message(conn, request_info);
  } else {
    // No suitable handler found, mark as not processed. Mongoose will
    // try to serve the request.
    processed = MG_ERROR;
  }

  return processed;
}

int main(int argc, char *argv[]) {
  struct mg_context	*ctx;

  ctx = mg_start();

  mg_set_option(ctx, "root", web_root);
  mg_set_option(ctx, "ssl_cert", ssl_certificate);  // Must be set before ports
  mg_set_option(ctx, "ports", http_ports);
  mg_set_option(ctx, "dir_list", "no");   // Disable directory listing

  mg_set_callback(ctx, MG_EVENT_NEW_REQUEST, &process_request);

  printf("Chat server started on ports %s, press enter to quit.\n", http_ports);
  getchar();
  mg_stop(ctx);
  printf("%s\n", "Chat server stopped.");

  return EXIT_SUCCESS;
}

// vim:ts=2:sw=2:et
