#include "modbus.h"
#include "event.h"
#include "util.h"
#include "log.h"

#define MG_MODBUS_MIN_PAYLOAD_LEN 12
#define MG_MODBUS_HEADER_LEN 7

static void mg_modbus_send_mbap(struct mg_connection *c, uint16_t txid,
                                uint16_t pdu_len, uint8_t unit_id) {
  uint8_t hdr[7];
  memset(hdr, 0, sizeof(hdr));
  MG_STORE_BE16(&hdr[0], txid & 0xFFFFU);
  MG_STORE_BE16(&hdr[4], (pdu_len + 1) & 0xFFFFU);
  hdr[6] = unit_id;
  mg_send(c, hdr, sizeof(hdr));
}

static void mg_modbus_send_response(struct mg_connection *c,
                                    struct mg_modbus_req *req,
                                    uint16_t txid, uint8_t unit_id) {
  uint8_t buf[5];
  uint8_t out;
  uint16_t reg;
  uint16_t i, nbytes;

  memset(buf, 0, sizeof(buf));
  if (req == NULL) return;
  if (req->error != MG_MODBUS_ERR_NONE) {
    mg_modbus_send_mbap(c, txid, 2, unit_id);
    buf[0] = (uint8_t) (req->func | 0x80);
    buf[1] = req->error;
    mg_send(c, buf, 2);
    return;
  }
  switch (req->func) {
    case MG_MODBUS_FUNC_READ_COILS:
    case MG_MODBUS_FUNC_READ_DISCRETE_INPUTS:
      if (req->u.bits == NULL) return;
      nbytes = (uint16_t) ((req->len + 7) / 8);
      mg_modbus_send_mbap(c, txid, (uint16_t) (nbytes + 2), unit_id);
      buf[0] = req->func;
      buf[1] = (uint8_t) nbytes;
      mg_send(c, buf, 2);
      memset(buf, 0, sizeof(buf));
      for (i = 0; i < req->len; i++) {
        if (i % 8 == 0) out = 0;
        if (req->u.bits[i]) out |= (uint8_t) (1 << (i % 8));
        if ((i % 8) == 7 || i + 1 == req->len) {
          mg_send(c, &out, 1);
        }
      }
      break;
    case MG_MODBUS_FUNC_READ_HOLDING_REGISTERS:
    case MG_MODBUS_FUNC_READ_INPUT_REGISTERS:
      if (req->u.regs == NULL) return;
      nbytes = (uint16_t) (req->len * 2);
      mg_modbus_send_mbap(c, txid, (uint16_t) (nbytes + 2), unit_id);
      buf[0] = req->func;
      buf[1] = (uint8_t) nbytes;
      mg_send(c, buf, 2);
      for (i = 0; i < req->len; i++) {
        MG_STORE_BE16(&reg, req->u.regs[i] & 0xFFFFU);
        mg_send(c, &reg, 2);
      }
      break;
    case MG_MODBUS_FUNC_WRITE_SINGLE_COIL:
      if (req->len != 1 || req->u.bits == NULL) return;
      mg_modbus_send_mbap(c, txid, 5, unit_id);
      buf[0] = req->func;
      MG_STORE_BE16(&buf[1], req->addr & 0xFFFFu);
      MG_STORE_BE16(&buf[3], req->u.bits[0] ? 0xFF00U : 0x0000U);
      mg_send(c, buf, 5);
      break;
    case MG_MODBUS_FUNC_WRITE_SINGLE_REGISTER:
      if (req->len != 1 || req->u.regs == NULL) return;
      mg_modbus_send_mbap(c, txid, 5, unit_id);
      buf[0] = req->func;
      MG_STORE_BE16(&buf[1], req->addr  & 0xFFFFU);
      MG_STORE_BE16(&buf[3], req->u.regs[0]  & 0xFFFFU);
      mg_send(c, buf, 5);
      break;
    case MG_MODBUS_FUNC_WRITE_MULTIPLE_COILS:
    case MG_MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS:
      mg_modbus_send_mbap(c, txid, 5, unit_id);
      buf[0] = req->func;
      MG_STORE_BE16(&buf[1], req->addr & 0xFFFFU);
      MG_STORE_BE16(&buf[3], req->len & 0xFFFFU);
      mg_send(c, buf, 5);
      break;
    default:
      break;
  }
}

static void handle_pdu(struct mg_connection *c, uint8_t *buf, size_t len) {
  uint16_t max_len, val, txid, addr, quantity;
  uint8_t func, unit_id, byte_count, packed;
  int i;
  size_t pdu_len = len - MG_MODBUS_HEADER_LEN, len_check;
  struct mg_modbus_req mr;

  if (len < MG_MODBUS_MIN_PAYLOAD_LEN) return;
  memset(&mr, 0, sizeof(mr));
  txid = MG_LOAD_BE16(&buf[0]);
  unit_id = buf[6];
  func = buf[7];
  addr = MG_LOAD_BE16(&buf[8]);
  mr.func = func;
  mr.addr = addr;
  mr.error = MG_MODBUS_ERR_NONE;
  mr.len = 0;
  mr.u.bits = NULL;
  switch (func) {
    case MG_MODBUS_FUNC_READ_COILS:
    case MG_MODBUS_FUNC_READ_DISCRETE_INPUTS:
      if (pdu_len != 5) goto modbus_illegal_value;
      quantity = MG_LOAD_BE16(&buf[10]);
      max_len = 0x07D0;  // 2000 bits
      if (quantity == 0 || quantity > max_len) goto modbus_illegal_value;
      mr.len = quantity;
      mr.u.bits = (bool *) mg_calloc((size_t) mr.len, sizeof(bool));
      if (mr.u.bits == NULL) goto modbus_oom;
      break;
    case MG_MODBUS_FUNC_READ_HOLDING_REGISTERS:
    case MG_MODBUS_FUNC_READ_INPUT_REGISTERS:
      if (pdu_len != 5) goto modbus_illegal_value;
      quantity = MG_LOAD_BE16(&buf[10]);
      max_len = 0x007D;  // 125 registers
      if (quantity == 0 || quantity > max_len) goto modbus_illegal_value;
      mr.len = quantity;
      mr.u.regs = (uint16_t *) mg_calloc((size_t) mr.len, sizeof(uint16_t));
      if (mr.u.regs == NULL) goto modbus_oom;
      break;
    case MG_MODBUS_FUNC_WRITE_SINGLE_COIL:
      if (pdu_len != 5) goto modbus_illegal_value;
      mr.len = 1;
      mr.u.bits = (bool *) mg_calloc(1, sizeof(bool));
      if (mr.u.bits == NULL) goto modbus_oom;
      val = MG_LOAD_BE16(&buf[10]);
      if (val != 0x0000 && val != 0xFF00) goto modbus_illegal_value;
      mr.u.bits[0] = (val == 0xFF00);
      break;
    case MG_MODBUS_FUNC_WRITE_SINGLE_REGISTER:
      if (pdu_len != 5) goto modbus_illegal_value;
      mr.len = 1;
      mr.u.regs = (uint16_t *) mg_calloc(1, sizeof(uint16_t));
      if (mr.u.regs == NULL) goto modbus_oom;
      mr.u.regs[0] = MG_LOAD_BE16(&buf[10]);
      break;
    case MG_MODBUS_FUNC_WRITE_MULTIPLE_COILS: {
      quantity = MG_LOAD_BE16(&buf[10]);
      if (quantity == 0 || quantity > 0x07B0) goto modbus_illegal_value;
      if (pdu_len < 6) goto modbus_illegal_value;
      byte_count = buf[12];
      if (byte_count != (uint8_t) ((quantity + 7) / 8)) goto modbus_illegal_value;
      len_check = (size_t) (6 + byte_count);
      if (len_check != pdu_len) goto modbus_illegal_value;
      mr.len = quantity;
      mr.u.bits = (bool *) mg_calloc((size_t) mr.len, sizeof(bool));
      if (mr.u.bits == NULL) goto modbus_oom;
      for (i = 0; i < mr.len; i++) {
        packed = buf[13 + (i / 8)];
        mr.u.bits[i] = ((packed >> (i % 8)) & 1) != 0;
      }
      break;
    }
    case MG_MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS: {
      quantity = MG_LOAD_BE16(&buf[10]);
      if (quantity == 0 || quantity > 0x007B) goto modbus_illegal_value;
      if (pdu_len < 6) goto modbus_illegal_value;
      byte_count = buf[12];
      if (byte_count != (uint8_t) (quantity * 2)) goto modbus_illegal_value;
      len_check = (size_t) (6 + quantity * 2);
      if (len_check != pdu_len) goto modbus_illegal_value;
      mr.len = quantity;
      mr.u.regs = (uint16_t *) mg_calloc((size_t) mr.len, sizeof(uint16_t));
      if (mr.u.regs == NULL) goto modbus_oom;
      for (i = 0; i < mr.len; i++) {
        mr.u.regs[i] = MG_LOAD_BE16(&buf[13 + i * 2]);
      }
      break;
    }
    default:
      MG_ERROR(("Unsupported modbus function"));
      mr.error = MG_MODBUS_ERR_ILLEGAL_FUNCTION;
      mg_modbus_send_response(c, &mr, txid, unit_id);
      return;
  }
  mg_call(c, MG_EV_MODBUS_REQ, &mr);
  goto modbus_exit;

modbus_illegal_value:
  MG_ERROR(("Invalid data"));
  mr.error = MG_MODBUS_ERR_ILLEGAL_VALUE;
  goto modbus_exit;
modbus_oom:
  MG_ERROR(("OOM"));
  mr.error = MG_MODBUS_ERR_DEVICE_FAILURE;
modbus_exit:
  mg_modbus_send_response(c, &mr, txid, unit_id);
  if (mr.u.bits != NULL) mg_free(mr.u.bits);
}

static void modbus_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_READ) {
    while (c->recv.len >= MG_MODBUS_MIN_PAYLOAD_LEN) {
      uint16_t len = MG_LOAD_BE16(&c->recv.buf[4]);  // PDU length
      uint16_t proto_id = MG_LOAD_BE16(&c->recv.buf[2]);
      if (len < 6 || proto_id != 0) {
        mg_error(c, "invalid pdu");
        break;
      } else if (c->recv.len < len + 6U) {
        break;  // Partial frame, buffer more
      } else {
        handle_pdu(c, c->recv.buf, len + 6);  // Parse PDU and call user
        mg_iobuf_del(&c->recv, 0, len + 6);   // Delete received PDU
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
