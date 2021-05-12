void mg_connect_resolved(struct mg_connection *);

#if MG_ARCH == MG_ARCH_FREERTOS_TCP
static inline void *mg_calloc(int cnt, size_t size) {
  void *p = pvPortMalloc(cnt * size);
  if (p != NULL) memset(p, 0, size);
  return p;
}
#define calloc(a, b) mg_calloc((a), (b))
#define malloc(a) pvPortMalloc(a)
#define free(a) vPortFree(a)
#endif
