#include "modbus.h"
#include "event.h"
#include "util.h"

static void handle_pdu(struct mg_connection *c, uint8_t *buf, size_t len) {
  uint16_t addr = MG_LOAD_BE16(&buf[8]);
  uint16_t quantity = MG_LOAD_BE16(&buf[10]);
  struct mg_modbus_cmd mc = {buf[7], MG_MODBUS_ERROR_NONE, addr, 0, quantity};

  // IMPLEMENT THIS. Avoid using stack. If we need to read many values,
  // resize c->send and point mc.values array there.

  (void) c, (void) buf, (void) len;
}

#define MG_MODBUS_MIN_PAYLOAD_LEN 12
static void modbus_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  // MBAP header (7) +  function code FC(1) + ADDR(2) + QUANTITY(2)
  if (ev == MG_EV_READ) {
    while (c->recv.len >= MG_MODBUS_MIN_PAYLOAD_LEN) {
      uint16_t len = MG_LOAD_BE16(&c->recv.buf[4]);  // PDU length
      if (len < 6) {
        mg_error(c, "invalid pdu");
        break;
      } else if (c->recv.len < len + 6U) {
        break;  // Partial frame, buffer more
      } else {
        handle_pdu(c, c->recv.buf, len + 6);  // Parse PDU and call user
        mg_iobuf_del(&c->recv, 0, len + 6U);  // Delete received PDU
      }
    }
  }
  (void) ev_data;
}

struct mg_connection *mg_modbus_listen(struct mg_mgr *mgr, const char *url,
                                       mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_listen(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = modbus_ev_handler;
  return c;
}
