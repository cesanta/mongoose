#if MG_NET_IF == MG_NET_IF_SIMPLELINK && !defined(MG_SIMPLELINK_NO_OSI)

#include "mg_task.h"

#include <oslib/osi.h>

enum mg_q_msg_type {
  MG_Q_MSG_CB,
};
struct mg_q_msg {
  enum mg_q_msg_type type;
  void (*cb)(struct mg_mgr *mgr, void *arg);
  void *arg;
};
static OsiMsgQ_t s_mg_q;
static void mg_task(void *arg);

bool mg_start_task(int priority, int stack_size, mg_init_cb mg_init) {
  if (osi_MsgQCreate(&s_mg_q, "MG", sizeof(struct mg_q_msg), 16) != OSI_OK) {
    return false;
  }
  if (osi_TaskCreate(mg_task, (const signed char *) "MG", stack_size,
                     (void *) mg_init, priority, NULL) != OSI_OK) {
    return false;
  }
  return true;
}

static void mg_task(void *arg) {
  struct mg_mgr mgr;
  mg_init_cb mg_init = (mg_init_cb) arg;
  mg_mgr_init(&mgr, NULL);
  mg_init(&mgr);
  while (1) {
    struct mg_q_msg msg;
    mg_mgr_poll(&mgr, 1);
    if (osi_MsgQRead(&s_mg_q, &msg, 1) != OSI_OK) continue;
    switch (msg.type) {
      case MG_Q_MSG_CB: {
        msg.cb(&mgr, msg.arg);
      }
    }
  }
}

void mg_run_in_task(void (*cb)(struct mg_mgr *mgr, void *arg), void *cb_arg) {
  struct mg_q_msg msg = {MG_Q_MSG_CB, cb, cb_arg};
  osi_MsgQWrite(&s_mg_q, &msg, OSI_NO_WAIT);
}

#endif /* MG_NET_IF == MG_NET_IF_SIMPLELINK && !defined(MG_SIMPLELINK_NO_OSI) \
          */
