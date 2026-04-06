#include "mongoose.h"

#define DEFAULT_URL "http://localhost:9000/"
#define DEFAULT_CA_PEM "ca.pem"
#define DEFAULT_TIMEOUT_MS 1500
#define DEFAULT_MAX_CONNS 5

struct page {
  struct page *next;
  struct mg_str url;   // Page URL
  struct mg_str body;  // Page body
  int status;          // Page state or HTTP status
  int loading;         // Number of failed load attempts
};
enum { PAGE_PENDING = 0, PAGE_FAILED = -1, PAGE_LOADING = -2 };

struct link {
  struct link *next;
  struct mg_str url;  // Link URL
  struct page *page;  // Page where the URL was found
  int status;         // 0 - unverified, 1 - ok, 2 - broken
};
enum { LINK_UNVERIFIED = 0, LINK_OK = 1, LINK_BROKEN = 2 };

struct conndata {     // Connection-specific data stored in c->data
  uint64_t expire;    // Connect expiration time in milliseconds
  struct page *page;  // Page being fetched on this connection
};

struct crawler {
  struct mg_str url;     // URL to crawl
  struct mg_mgr mgr;     // Event manager
  struct mg_str ca;      // CA PEM file
  uint64_t timeout_ms;   // Connect timeout in milliseconds
  struct link *ignores;  // Ignored links
  struct link *links;    // List of links
  struct page *pages;    // List of pages
  int max_pages;         // Max pages to crawl
  int num_pages;         // Pages crawled
  int exit_on_broken;    // Exit immediately when a broken link is found
};

static void ev_handler(struct mg_connection *c, int ev, void *ev_data);

static void add_page(struct crawler *crawler, const char *url) {
  struct page *page = calloc(1, sizeof(*page));
  page->url = mg_strdup(mg_str(url));
  page->next = crawler->pages;
  crawler->pages = page;
  crawler->num_pages++;
  // MG_INFO(("Added page %.*s", page->url.len, page->url.buf));
}

static void add_link(struct link **head, struct mg_str url, struct page *page) {
  struct link *link = calloc(1, sizeof(*link));
  link->url = mg_strdup(url);
  link->page = page;
  link->next = *head;
  *head = link;
}

static int count_links(struct link *head, int status) {
  int count = 0;
  while (head != NULL) {
    if (head->status == status) count++;
    head = head->next;
  }
  return count;
}

static struct link *find_link(struct link *head, struct mg_str url) {
  while (head != NULL) {
    if (mg_strcasecmp(head->url, url) == 0) return head;
    head = head->next;
  }
  return NULL;
}

static struct page *find_page(struct page *head, struct mg_str url) {
  while (head != NULL) {
    if (mg_strcasecmp(head->url, url) == 0) return head;
    head = head->next;
  }
  return NULL;
}

static void process_link(struct crawler *crawler, struct page *page,
                         struct mg_str url) {
  if (mg_match(url, mg_str("#://#"), NULL)) {
    // MG_INFO(("  REMOTE %.*s", url.len, url.buf));
  } else {
    char buf[8192], *p;
    if (url.len > 0 && url.buf[0] == '/') {
      // Absolute path, prepend scheme and host from website root
      struct mg_str caps[10] = {0};
      if (!mg_match(page->url, mg_str("#://*/#"), caps) &&
          !mg_match(page->url, mg_str("#://*"), caps)) {
        MG_ERROR(("BAD page URL: %.*s", page->url.len, page->url.buf));
        abort();
      }
      mg_snprintf(buf, sizeof(buf), "%.*s://%.*s%.*s", caps[0].len, caps[0].buf,
                  caps[1].len, caps[1].buf, url.len, url.buf);
    } else {
      // Relative path, prepend page URL
      mg_snprintf(buf, sizeof(buf), "%.*s%.*s", page->url.len, page->url.buf,
                  url.len, url.buf);
    }
    if (find_link(crawler->links, mg_str(buf)) == NULL) {
      add_link(&crawler->links, mg_str(buf), page);
    }
    if ((p = strrchr(buf, '#')) != NULL) *p = '\0';  // Strip fragment
    if (find_page(crawler->pages, mg_str(buf)) == NULL &&
        (crawler->max_pages == 0 || crawler->num_pages < crawler->max_pages)) {
      add_page(crawler, buf);
    }
  }
}

// Scan page body for id="frag" or name="frag" using mg_match scanning loops
static bool find_fragment_in_body(struct mg_str body, struct mg_str frag) {
  struct mg_str b = body;
  struct mg_str caps[10] = {0};
  while (mg_match(b, mg_str("#id=\"#\"#"), caps) ||
         mg_match(b, mg_str("#name=\"#\"#"), caps)) {
    size_t advance = caps[2].buf - b.buf;
    b.buf += advance, b.len -= advance;
    if (mg_strcasecmp(caps[1], frag) == 0) return true;
  }
  return false;
}

static void verify_links(struct crawler *crawler) {
  for (struct link *link = crawler->links; link != NULL; link = link->next) {
    if (link->status > 0) continue;
    // Split URL into page URL part and fragment part
    struct mg_str url = link->url, frag = {NULL, 0};
    for (size_t i = 0; i < link->url.len; i++) {
      if (link->url.buf[i] == '#') {
        url.len = i;
        frag.buf = link->url.buf + i + 1;
        frag.len = link->url.len - i - 1;
        break;
      }
    }
    struct page *page = find_page(crawler->pages, url);
    if (page == NULL || page->status == PAGE_PENDING ||
        page->status == PAGE_LOADING)
      continue;  // not yet crawled or external
    if (page->status != 200) {
      link->status = LINK_BROKEN;  // page fetch failed -> broken
    } else if (page->body.buf == NULL) {
      MG_ERROR(("Page body is NULL for %.*s", page->url.len, page->url.buf));
      abort();  // This should never happen, but just in case
    } else if (frag.len == 0) {
      link->status = LINK_OK;  // no fragment, page exists -> ok
    } else if (find_fragment_in_body(page->body, frag)) {
      link->status = LINK_OK;  // fragment anchor found in body -> ok
    } else {
      link->status = LINK_BROKEN;  // fragment anchor missing -> broken
    }
    if (link->status == LINK_BROKEN) {
      MG_INFO(("BROKEN %.*s on page %.*s %lu", link->url.len, link->url.buf,
               link->page->url.len, link->page->url.buf, page->body.len));
      // mg_file_write(&mg_fs_posix, "/tmp/x", link->page->body.buf,
      //               link->page->body.len);
      if (crawler->exit_on_broken) exit(1);
    }
  }
}

static bool should_parse_body(struct mg_str root, struct mg_str url) {
  struct mg_str prefix = mg_str_n(url.buf, root.len);
  if (url.len < root.len) return false;
  if (mg_strcasecmp(root, prefix) == 0) return true;
  return false;
}

static void save_body(struct crawler *crawler, struct page *page,
                      struct mg_str body) {
  page->body = mg_strdup(body);
  if (should_parse_body(crawler->url, page->url)) {
    struct mg_str caps[10];
    struct mg_str b = page->body;                        // Re-init
    while (mg_match(b, mg_str("#href=\"#\"#"), caps)) {  // Process href=...
      process_link(crawler, page, caps[1]);
      size_t advance = caps[2].buf - b.buf;
      b.buf += advance, b.len -= advance;
    }
    b = page->body;                                     // Re-init
    while (mg_match(b, mg_str("#src=\"#\"#"), caps)) {  // Process src=...
      process_link(crawler, page, caps[1]);
      size_t advance = caps[2].buf - b.buf;
      b.buf += advance, b.len -= advance;
    }
  }
  verify_links(crawler);
}

static void fail_page(struct page *page) {
  if (page->status != PAGE_LOADING) return;
  page->loading++;
  if (page->loading >= 3) {
    page->status = PAGE_FAILED;
    MG_INFO(("FAILED %.*s", page->url.len, page->url.buf));
  } else {
    page->status = PAGE_PENDING;
  }
}

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  struct crawler *crawler = (struct crawler *) c->fn_data;
  struct conndata *cd = (struct conndata *) c->data;

  if (ev == MG_EV_OPEN) {
    // Connection created. Store connect expiration time in c->data
    cd->expire = mg_millis() + crawler->timeout_ms;
  } else if (ev == MG_EV_POLL) {
    if (mg_millis() > cd->expire) {
      mg_error(c, "Connect timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    // Connected to server. Extract host name from URL
    struct mg_str host = mg_url_host(cd->page->url.buf);
    if (c->is_tls) {
      struct mg_tls_opts opts = {.ca = crawler->ca, .name = host};
      mg_tls_init(c, &opts);
    }
    mg_printf(c,
              "%s %s HTTP/1.0\r\n"
              "Host: %.*s\r\n"
              "Content-Type: octet-stream\r\n"
              "\r\n",
              "GET", mg_url_uri(cd->page->url.buf), host.len, host.buf);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    cd->page->status = atoi(hm->uri.buf);
    if (cd->page->status == 200) {
      save_body(crawler, cd->page, hm->body);
    } else {
      MG_ERROR(("HTTP error %d for page %.*s", cd->page->status,
                cd->page->url.len, cd->page->url.buf));
      fail_page(cd->page);
    }
    c->is_closing = 1;
  } else if (ev == MG_EV_CLOSE) {
    fail_page(cd->page);
  } else if (ev == MG_EV_ERROR) {
    fail_page(cd->page);
    MG_ERROR(("%s, page: %.*s", ev_data, cd->page->url.len, cd->page->url.buf));
  }
}

static bool schedule_pages(struct crawler *crawler) {
  unsigned num_conns = 0;
  bool busy = false;
  for (struct page *page = crawler->pages; page != NULL; page = page->next) {
    if (page->status == PAGE_LOADING) {
      num_conns++;
      busy = true;
      continue;
    }
    if (page->status != PAGE_PENDING) continue;
    busy = true;
    if (num_conns >= DEFAULT_MAX_CONNS) continue;
    struct mg_connection *c =
        mg_http_connect(&crawler->mgr, page->url.buf, ev_handler, crawler);
    if (c == NULL) {
      page->loading++;
      if (page->loading >= 3) {
        page->status = PAGE_FAILED;
        MG_INFO(("FAILED %.*s", page->url.len, page->url.buf));
      }
      continue;
    }
    num_conns++;
    page->status = PAGE_LOADING;
    struct conndata *cd = (struct conndata *) c->data;
    cd->page = page;
  }
  return busy;
}

int main(int argc, char *argv[]) {
  struct mg_str ca = mg_str(DEFAULT_CA_PEM);
  struct crawler crawler = {
      .url = mg_str(DEFAULT_URL),
      .timeout_ms = DEFAULT_TIMEOUT_MS,
  };

  // Parse command-line flags
  for (int i = 1; i + 1 < argc; i++) {
    if (strcmp(argv[i], "-ca") == 0) {
      ca = mg_str(argv[++i]);
    } else if (strcmp(argv[i], "-url") == 0) {
      crawler.url = mg_str(argv[++i]);
    } else if (strcmp(argv[i], "-exit") == 0) {
      crawler.exit_on_broken = 1;
    } else if (strcmp(argv[i], "-max") == 0) {
      crawler.max_pages = atoi(argv[++i]);
    } else {
      fprintf(stderr,
              "Usage: %s OPTIONS\n"
              "  -exit        - exit on broken link immeditely\n"
              "  -ca   PEM    - TLS CA PEM file path, default: %s\n"
              "  -url  URL    - URL to fetch, default: %s\n"
              "  -max  NUMBER - maximum pages to crawl, default: 0 (all)\n",
              argv[0], DEFAULT_CA_PEM, DEFAULT_URL);
      exit(EXIT_FAILURE);
    }
  }
  crawler.ca = mg_file_read(&mg_fs_posix, ca.buf);

  mg_mgr_init(&crawler.mgr);
  mg_log_set(MG_LL_INFO);

  add_page(&crawler, crawler.url.buf);
  while (schedule_pages(&crawler)) {
    mg_mgr_poll(&crawler.mgr, 50);
  }
  mg_mgr_free(&crawler.mgr);
  MG_INFO(("Exiting, %d pages, %d broken links, %d OK links, %d pending links",
           crawler.num_pages, count_links(crawler.links, LINK_BROKEN),
           count_links(crawler.links, LINK_OK), count_links(crawler.links, 0)));
  return 0;
}
