/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 *
 * This example demonstrates how to implement cookie authentication
 * and session management using Mongoose.
 */

#include <inttypes.h>
#include <stdlib.h>

#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

/* This is the name of the cookie carrying the session ID. */
#define SESSION_COOKIE_NAME "mgs"
/* In our example sessions are destroyed after 30 seconds of inactivity. */
#define SESSION_TTL 30.0
#define SESSION_CHECK_INTERVAL 5.0

/* Session information structure. */
struct session {
  /* Session ID. Must be unique and hard to guess. */
  uint64_t id;
  /*
   * Time when the session was created and time of last activity.
   * Used to clean up stale sessions.
   */
  double created;
  double last_used; /* Time when the session was last active. */

  /* User name this session is associated with. */
  char *user;
  /* Some state associated with user's session. */
  int lucky_number;
};

/*
 * This example uses a simple in-memory storage for just 10 sessions.
 * A real-world implementation would use persistent storage of some sort.
 */
#define NUM_SESSIONS 10
struct session s_sessions[NUM_SESSIONS];

/*
 * Password check function.
 * In our example all users have password "password".
 */
static int check_pass(const char *user, const char *pass) {
  (void) user;
  return (strcmp(pass, "password") == 0);
}

/*
 * Parses the session cookie and returns a pointer to the session struct
 * or NULL if not found.
 */
static struct session *get_session(struct http_message *hm) {
  struct mg_str *cookie_header = mg_get_http_header(hm, "cookie");
  if (cookie_header == NULL) return NULL;
  char ssid[21];
  if (!mg_http_parse_header(cookie_header, SESSION_COOKIE_NAME, ssid,
                            sizeof(ssid))) {
    return NULL;
  }
  uint64_t sid = strtoull(ssid, NULL, 16);
  for (int i = 0; i < NUM_SESSIONS; i++) {
    if (s_sessions[i].id == sid) {
      s_sessions[i].last_used = mg_time();
      return &s_sessions[i];
    }
  }
  return NULL;
}

/*
 * Destroys the session state.
 */
static void destroy_session(struct session *s) {
  free(s->user);
  memset(s, 0, sizeof(*s));
}

/*
 * Creates a new session for the user.
 */
static struct session *create_session(const char *user,
                                      const struct http_message *hm) {
  /* Find first available slot or use the oldest one. */
  struct session *s = NULL;
  struct session *oldest_s = s_sessions;
  for (int i = 0; i < NUM_SESSIONS; i++) {
    if (s_sessions[i].id == 0) {
      s = &s_sessions[i];
      break;
    }
    if (s_sessions[i].last_used < oldest_s->last_used) {
      oldest_s = &s_sessions[i];
    }
  }
  if (s == NULL) {
    destroy_session(oldest_s);
    printf("Evicted %" INT64_X_FMT "/%s\n", oldest_s->id, oldest_s->user);
    s = oldest_s;
  }
  /* Initialize new session. */
  s->created = s->last_used = mg_time();
  s->user = strdup(user);
  s->lucky_number = rand();
  /* Create an ID by putting various volatiles into a pot and stirring. */
  cs_sha1_ctx ctx;
  cs_sha1_init(&ctx);
  cs_sha1_update(&ctx, (const unsigned char *) hm->message.p, hm->message.len);
  cs_sha1_update(&ctx, (const unsigned char *) s, sizeof(*s));
  unsigned char digest[20];
  cs_sha1_final(digest, &ctx);
  s->id = *((uint64_t *) digest);
  return s;
}

/*
 * If requested via GET, serves the login page.
 * If requested via POST (form submission), checks password and logs user in.
 */
static void login_handler(struct mg_connection *nc, int ev, void *p) {
  struct http_message *hm = (struct http_message *) p;
  if (mg_vcmp(&hm->method, "POST") != 0) {
    /* Serve login.html */
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  } else {
    /* Perform password check. */
    char user[50], pass[50];
    int ul = mg_get_http_var(&hm->body, "user", user, sizeof(user));
    int pl = mg_get_http_var(&hm->body, "pass", pass, sizeof(pass));
    if (ul > 0 && pl > 0) {
      if (check_pass(user, pass)) {
        struct session *s = create_session(user, hm);
        char shead[100];
        snprintf(shead, sizeof(shead),
                 "Set-Cookie: %s=%" INT64_X_FMT "; path=/", SESSION_COOKIE_NAME,
                 s->id);
        mg_http_send_redirect(nc, 302, mg_mk_str("/"), mg_mk_str(shead));
        fprintf(stderr, "%s logged in, sid %" INT64_X_FMT "\n", s->user, s->id);
      } else {
        mg_printf(nc, "HTTP/1.0 403 Unauthorized\r\n\r\nWrong password.\r\n");
      }
    } else {
      mg_printf(nc, "HTTP/1.0 400 Bad Request\r\n\r\nuser, pass required.\r\n");
    }
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
  (void) ev;
}

/*
 * Logs the user out.
 * Removes cookie and any associated session state.
 */
static void logout_handler(struct mg_connection *nc, int ev, void *p) {
  struct http_message *hm = (struct http_message *) p;
  char shead[100];
  snprintf(shead, sizeof(shead), "Set-Cookie: %s=", SESSION_COOKIE_NAME);
  mg_http_send_redirect(nc, 302, mg_mk_str("/"), mg_mk_str(shead));
  struct session *s = get_session(hm);
  if (s != NULL) {
    fprintf(stderr, "%s logged out, session %" INT64_X_FMT " destroyed\n",
            s->user, s->id);
    destroy_session(s);
  }
  nc->flags |= MG_F_SEND_AND_CLOSE;
  (void) ev;
}

/* Cleans up sessions that have been idle for too long. */
void check_sessions(void) {
  double threshold = mg_time() - SESSION_TTL;
  for (int i = 0; i < NUM_SESSIONS; i++) {
    struct session *s = &s_sessions[i];
    if (s->id != 0 && s->last_used < threshold) {
      fprintf(stderr, "Session %" INT64_X_FMT " (%s) closed due to idleness.\n",
              s->id, s->user);
      destroy_session(s);
    }
  }
}

/* Main event handler. */
static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  switch (ev) {
    case MG_EV_HTTP_REQUEST: {
      struct http_message *hm = (struct http_message *) p;
      struct session *s = get_session(hm);
      /* Ask the user to log in if they did not present a valid cookie. */
      if (s == NULL) {
        mg_http_send_redirect(nc, 302, mg_mk_str("/login.html"),
                              mg_mk_str(NULL));
        nc->flags |= MG_F_SEND_AND_CLOSE;
        break;
      }
      /*
       * Serve the page that was requested.
       * Save session in user_data for use by SSI calls.
       */
      fprintf(stderr, "%s (sid %" INT64_X_FMT ") requested %.*s\n", s->user,
              s->id, (int) hm->uri.len, hm->uri.p);
      nc->user_data = s;
      mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
      break;
    }
    case MG_EV_SSI_CALL: {
      /* Expand variables in a page by using session data. */
      const char *var = (const char *) p;
      const struct session *s = (const struct session *) nc->user_data;
      if (strcmp(var, "user") == 0) {
        mg_printf_html_escape(nc, "%s", s->user);
      } else if (strcmp(var, "lucky_number") == 0) {
        mg_printf_html_escape(nc, "%d", s->lucky_number);
      }
      break;
    }
    case MG_EV_TIMER: {
      /* Perform session maintenance. */
      check_sessions();
      mg_set_timer(nc, mg_time() + SESSION_CHECK_INTERVAL);
      break;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  srand(mg_time());

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, ev_handler);

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";
  mg_register_http_endpoint(nc, "/login.html", login_handler);
  mg_register_http_endpoint(nc, "/logout", logout_handler);
  mg_set_timer(nc, mg_time() + SESSION_CHECK_INTERVAL);

  printf("Starting web server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
