#include <stdio.h>
#include <string.h>
#include "../../../slre/slre.h"
#include "mongoose.h"

static const char *initial_url = "http://test.mosquitto.org";
static const char *regex = "href=\"((https?://)[^\\s/'\"<>]+/?[^\\s'\"<>]*)";
const int max_depth = 2;

struct userdata {
  char *url;
  int depth;
};

void crawl_page(struct mg_mgr *mgr, const char *url, size_t url_len, int depth);
void handle_reply(struct mg_connection *nc, struct http_message *hm);

static void event_handler(struct mg_connection *nc, int event, void *data) {
  struct http_message *hm = (struct http_message *) data;
  int connect_status;

  switch (event) {
    case MG_EV_CONNECT:
      connect_status = *(int *) data;
      if (connect_status != 0) {
        printf("Error while loading page: %s, error: %s\n",
               ((struct userdata *) nc->user_data)->url,
               strerror(connect_status));
      }
      break;
    case MG_EV_CLOSE:
      free(((struct userdata *) nc->user_data)->url);
      free(nc->user_data);
      break;
    case MG_EV_HTTP_REPLY:
      handle_reply(nc, hm);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    default:
      break;
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr, NULL);
  crawl_page(&mgr, initial_url, ~0, 0);

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);

  return 0;
}

void crawl_page(struct mg_mgr *mgr, const char *url, size_t url_len,
                int depth) {
  struct mg_connection *nc;
  struct userdata *data = malloc(sizeof(struct userdata));

  if (url_len == (size_t) ~0) {
    url_len = strlen(url);
  }

  data->url = strncpy(malloc(url_len + 1), url, url_len);
  data->url[url_len] = '\0';
  data->depth = depth;

  nc = mg_connect_http(mgr, event_handler, url, NULL, NULL);
  if (nc != NULL) {
    nc->user_data = data;
  } else {
    printf("Error connecting to [%s]\n", url);
    free(data);
  }
}

void handle_reply(struct mg_connection *nc, struct http_message *hm) {
  struct userdata *ud = (struct userdata *) nc->user_data;
  const char *body = hm->body.p;

  int offset, max_matches = 2, cursor = 0, str_len = strlen(body);
  struct slre_cap caps[max_matches];

  printf("Loaded url: %s at depth %d\n", ud->url, ud->depth);
  if (ud->depth == max_depth) {
    return;
  }

  while (cursor < str_len &&
         (offset = slre_match(regex, body + cursor, str_len - cursor, caps,
                              max_matches, SLRE_IGNORE_CASE)) > 0) {
    crawl_page(nc->mgr, caps[0].ptr, caps[0].len, ud->depth + 1);
    cursor += offset;
  }
}
