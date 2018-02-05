#include "mg_net_if.h"
#include "mg_internal.h"
#include "mg_net_if_socket.h"

extern const struct mg_iface_vtable mg_default_iface_vtable;

const struct mg_iface_vtable *mg_ifaces[] = {
    &mg_default_iface_vtable,
};

int mg_num_ifaces = (int) (sizeof(mg_ifaces) / sizeof(mg_ifaces[0]));

struct mg_iface *mg_if_create_iface(const struct mg_iface_vtable *vtable,
                                    struct mg_mgr *mgr) {
  struct mg_iface *iface = (struct mg_iface *) MG_CALLOC(1, sizeof(*iface));
  iface->mgr = mgr;
  iface->data = NULL;
  iface->vtable = vtable;
  return iface;
}

struct mg_iface *mg_find_iface(struct mg_mgr *mgr,
                               const struct mg_iface_vtable *vtable,
                               struct mg_iface *from) {
  int i = 0;
  if (from != NULL) {
    for (i = 0; i < mgr->num_ifaces; i++) {
      if (mgr->ifaces[i] == from) {
        i++;
        break;
      }
    }
  }

  for (; i < mgr->num_ifaces; i++) {
    if (mgr->ifaces[i]->vtable == vtable) {
      return mgr->ifaces[i];
    }
  }
  return NULL;
}
