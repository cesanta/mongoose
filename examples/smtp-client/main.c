#include "mongoose.h"

static const char *server = "tcp://smtp.gmail.com:587";
static const char *user = "aaa@domain.com";  // Change this! Your mail account
static const char *pass = "xxxxxxxxxxxxxx";  // Change this! Your mail password
static const char *to = "bbb@domain.com";    // Change this! Destination email

static const char *from = "My Mail Sender";
static const char *subj = "Test email from Mongoose library!";
static const char *mesg = "Hi!\nThis is a test message.\nBye.";

static bool s_quit;

enum { EHLO, STARTTLS, STARTTLS_WAIT, AUTH, FROM, TO, DATA, BODY, QUIT, END };

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  uint8_t *state = (uint8_t *) c->data;
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_READ) {
    if (c->recv.len > 0 && c->recv.buf[c->recv.len - 1] == '\n') {
      MG_INFO(("<-- %.*s", (int) c->recv.len - 2, c->recv.buf));
      c->recv.len = 0;
      if (*state == EHLO) {
        mg_printf(c, "EHLO myname\r\n");
        *state = STARTTLS;
      } else if (*state == STARTTLS) {
        mg_printf(c, "STARTTLS\r\n");
        *state = STARTTLS_WAIT;
      } else if (*state == STARTTLS_WAIT) {
        struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                   .name = mg_url_host(server)};
        mg_tls_init(c, &opts);
        *state = AUTH;
      } else if (*state == AUTH) {
        char a[100], b[300] = "";
        size_t n = mg_snprintf(a, sizeof(a), "%c%s%c%s", 0, user, 0, pass);
        mg_base64_encode((uint8_t *) a, n, b, sizeof(b));
        mg_printf(c, "AUTH PLAIN %s\r\n", b);
        *state = FROM;
      } else if (*state == FROM) {
        mg_printf(c, "MAIL FROM: <%s>\r\n", user);
        *state = TO;
      } else if (*state == TO) {
        mg_printf(c, "RCPT TO: <%s>\r\n", to);
        *state = DATA;
      } else if (*state == DATA) {
        mg_printf(c, "DATA\r\n");
        *state = BODY;
      } else if (*state == BODY) {
        mg_printf(c, "From: %s <%s>\r\n", from, user);  // Mail header
        mg_printf(c, "Subject: %s\r\n", subj);          // Mail header
        mg_printf(c, "\r\n");                           // End of headers
        mg_printf(c, "%s\r\n", mesg);                   // Mail body
        mg_printf(c, ".\r\n");                          // End of body
        *state = QUIT;
      } else if (*state == QUIT) {
        mg_printf(c, "QUIT\r\n");
        *state = END;
      } else {
        c->is_draining = 1;
        MG_INFO(("end"));
      }
      MG_INFO(("--> %.*s", (int) c->send.len - 2, c->send.buf));
    }
  } else if (ev == MG_EV_CLOSE) {
    s_quit = true;
  } else if (ev == MG_EV_TLS_HS) {
    MG_INFO(("TLS handshake done! Sending EHLO again"));
    mg_printf(c, "EHLO myname\r\n");
  }
  (void) fn_data, (void) ev_data;
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);
  mg_connect(&mgr, server, fn, NULL);
  while (s_quit == false) mg_mgr_poll(&mgr, 1000);
  return 0;
}
