/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_SNTP_H_
#define CS_MONGOOSE_SRC_SNTP_H_

#if MG_ENABLE_SNTP

#define MG_SNTP_EVENT_BASE 500

/*
 * Received reply from time server. Event handler parameter contains
 * pointer to mg_sntp_message structure
 */
#define MG_SNTP_REPLY (MG_SNTP_EVENT_BASE + 1)

/* Received malformed SNTP packet */
#define MG_SNTP_MALFORMED_REPLY (MG_SNTP_EVENT_BASE + 2)

/* Failed to get time from server (timeout etc) */
#define MG_SNTP_FAILED (MG_SNTP_EVENT_BASE + 3)

struct mg_sntp_message {
  /* if server sends this flags, user should not send requests to it */
  int kiss_of_death;
  /* usual mg_time */
  double time;
};

/* Establishes connection to given sntp server */
struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr,
                                      MG_CB(mg_event_handler_t event_handler,
                                            void *user_data),
                                      const char *sntp_server_name);

/* Sends time request to given connection */
void mg_sntp_send_request(struct mg_connection *c);

/*
 * Helper function
 * Establishes connection to time server, tries to send request
 * repeats sending SNTP_ATTEMPTS times every SNTP_TIMEOUT sec
 * (if needed)
 * See sntp_client example
 */
struct mg_connection *mg_sntp_get_time(struct mg_mgr *mgr,
                                       mg_event_handler_t event_handler,
                                       const char *sntp_server_name);

#endif

#endif /* CS_MONGOOSE_SRC_SNTP_H_ */
