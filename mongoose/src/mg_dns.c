static __thread struct mg_resolve_ctx *s_resolve = NULL;

void mg_resolve_async(struct mg_mgr *mgr, const char *name,
                     mg_resolve_cb_t cb, void *data) {
  if (s_resolve == NULL) {
    s_resolve = calloc(1, sizeof(*s_resolve));
    s_resolve->mgr = mgr;
    s_resolve->cb = cb;
    s_resolve->data = data;
    s_resolve->name = strdup(name);
    // ... DNS resolution logic
  }
}