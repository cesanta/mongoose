#pragma once

#include "net.h"
#include "str.h"

// MQTT packet type codes. The received command is in mg_mqtt_message.cmd.
#define MQTT_CMD_CONNECT 1
#define MQTT_CMD_CONNACK 2
#define MQTT_CMD_PUBLISH 3
#define MQTT_CMD_PUBACK 4
#define MQTT_CMD_PUBREC 5
#define MQTT_CMD_PUBREL 6
#define MQTT_CMD_PUBCOMP 7
#define MQTT_CMD_SUBSCRIBE 8
#define MQTT_CMD_SUBACK 9
#define MQTT_CMD_UNSUBSCRIBE 10
#define MQTT_CMD_UNSUBACK 11
#define MQTT_CMD_PINGREQ 12
#define MQTT_CMD_PINGRESP 13
#define MQTT_CMD_DISCONNECT 14
#define MQTT_CMD_AUTH 15

// MQTT5 property IDs. Used in mg_mqtt_prop.id.
#define MQTT_PROP_PAYLOAD_FORMAT_INDICATOR 0x01
#define MQTT_PROP_MESSAGE_EXPIRY_INTERVAL 0x02
#define MQTT_PROP_CONTENT_TYPE 0x03
#define MQTT_PROP_RESPONSE_TOPIC 0x08
#define MQTT_PROP_CORRELATION_DATA 0x09
#define MQTT_PROP_SUBSCRIPTION_IDENTIFIER 0x0B
#define MQTT_PROP_SESSION_EXPIRY_INTERVAL 0x11
#define MQTT_PROP_ASSIGNED_CLIENT_IDENTIFIER 0x12
#define MQTT_PROP_SERVER_KEEP_ALIVE 0x13
#define MQTT_PROP_AUTHENTICATION_METHOD 0x15
#define MQTT_PROP_AUTHENTICATION_DATA 0x16
#define MQTT_PROP_REQUEST_PROBLEM_INFORMATION 0x17
#define MQTT_PROP_WILL_DELAY_INTERVAL 0x18
#define MQTT_PROP_REQUEST_RESPONSE_INFORMATION 0x19
#define MQTT_PROP_RESPONSE_INFORMATION 0x1A
#define MQTT_PROP_SERVER_REFERENCE 0x1C
#define MQTT_PROP_REASON_STRING 0x1F
#define MQTT_PROP_RECEIVE_MAXIMUM 0x21
#define MQTT_PROP_TOPIC_ALIAS_MAXIMUM 0x22
#define MQTT_PROP_TOPIC_ALIAS 0x23
#define MQTT_PROP_MAXIMUM_QOS 0x24
#define MQTT_PROP_RETAIN_AVAILABLE 0x25
#define MQTT_PROP_USER_PROPERTY 0x26
#define MQTT_PROP_MAXIMUM_PACKET_SIZE 0x27
#define MQTT_PROP_WILDCARD_SUBSCRIPTION_AVAILABLE 0x28
#define MQTT_PROP_SUBSCRIPTION_IDENTIFIER_AVAILABLE 0x29
#define MQTT_PROP_SHARED_SUBSCRIPTION_AVAILABLE 0x2A

// Property value types used internally to encode/decode MQTT5 properties.
enum {
  MQTT_PROP_TYPE_BYTE,
  MQTT_PROP_TYPE_STRING,
  MQTT_PROP_TYPE_STRING_PAIR,
  MQTT_PROP_TYPE_BINARY_DATA,
  MQTT_PROP_TYPE_VARIABLE_INT,
  MQTT_PROP_TYPE_INT,
  MQTT_PROP_TYPE_SHORT
};

// Return codes from mg_mqtt_parse().
enum { MQTT_OK, MQTT_INCOMPLETE, MQTT_MALFORMED };

// A single MQTT5 property. Use in mg_mqtt_opts.props / mg_mqtt_opts.will_props
// arrays, and iterate received properties with mg_mqtt_next_prop().
struct mg_mqtt_prop {
  uint8_t id;         // Property ID, one of MQTT_PROP_*
  uint32_t iv;        // Integer value for BYTE, SHORT, INT, VARIABLE_INT types
  struct mg_str key;  // Property key string; only set for USER_PROPERTY pairs
  struct mg_str val;  // String/binary value; set for STRING, BINARY_DATA, and USER_PROPERTY
};

// Options passed to mg_mqtt_connect(), mg_mqtt_pub(), mg_mqtt_sub(),
// mg_mqtt_unsub(), and mg_mqtt_disconnect(). Zero-initialise and set only
// the fields relevant to the operation being called.
struct mg_mqtt_opts {
  struct mg_str user;               // CONNECT: username; empty = omit
  struct mg_str pass;               // CONNECT: password; empty = omit
  struct mg_str client_id;          // CONNECT: client ID; empty = auto-generate
  struct mg_str topic;              // CONNECT: will topic; PUBLISH/SUBSCRIBE/UNSUBSCRIBE: topic
  struct mg_str message;            // CONNECT: will payload; PUBLISH: message payload
  uint8_t qos;                      // QoS level: 0, 1, or 2
  uint8_t version;                  // MQTT version: 4 (3.1.1) or 5; 0 defaults to 4
  uint16_t keepalive;               // CONNECT: keepalive interval in seconds; 0 = no timeout
  uint16_t retransmit_id;           // PUBLISH: packet ID for retransmit; init to 0 for new message
  bool retain;                      // PUBLISH/CONNECT will: set the retain flag
  bool clean;                       // CONNECT: start a clean session
  struct mg_mqtt_prop *props;       // MQTT5: properties array; NULL if none
  size_t num_props;                 // MQTT5: number of entries in props
  struct mg_mqtt_prop *will_props;  // MQTT5 CONNECT: will properties; NULL if none
  size_t num_will_props;            // MQTT5: number of entries in will_props
};

// Received MQTT message. Passed as ev_data for MG_EV_MQTT_MSG (PUBLISH),
// MG_EV_MQTT_CMD (all packets), and MG_EV_MQTT_OPEN (CONNACK).
// topic, data, and dgram are zero-copy slices into the receive buffer; they
// are only valid until the next mg_mgr_poll() call.
struct mg_mqtt_message {
  struct mg_str topic;  // PUBLISH: message topic; slice into dgram
  struct mg_str data;   // PUBLISH: message payload; slice into dgram
  struct mg_str dgram;  // Complete raw MQTT packet, including fixed header
  uint16_t id;          // Packet ID; set for QoS>0 PUBLISH, PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, UNSUBACK
  uint8_t cmd;          // Packet type, one of MQTT_CMD_*
  uint8_t qos;          // QoS level extracted from the PUBLISH fixed header
  uint8_t ack;          // CONNACK return code; 0 = success
  size_t props_start;   // MQTT5: byte offset from dgram.buf to start of properties
  size_t props_size;    // MQTT5: byte length of the properties section
};

// Opens a TCP connection to url and immediately sends a CONNECT packet.
// opts may be NULL to use defaults (MQTT 3.1.1, no auth, auto client ID).
// Fires MG_EV_MQTT_OPEN when CONNACK is received (ev_data: uint8_t* return code, 0=success).
// Fires MG_EV_MQTT_MSG for incoming PUBLISH packets (ev_data: struct mg_mqtt_message *).
// Fires MG_EV_MQTT_CMD for every received MQTT packet (ev_data: struct mg_mqtt_message *).
// Returns NULL on error.
struct mg_connection *mg_mqtt_connect(struct mg_mgr *, const char *url,
                                      const struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data);

// Creates an MQTT server listener on url.
// Fires MG_EV_MQTT_CMD for each complete packet received (ev_data: struct mg_mqtt_message *).
// Returns NULL on error.
struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data);

// Sends a CONNECT packet on an already-open connection.
// Called automatically by mg_mqtt_connect(); exposed for advanced use such
// as re-authenticating on an existing connection.
void mg_mqtt_login(struct mg_connection *c, const struct mg_mqtt_opts *opts);

// Sends a PUBLISH packet. opts.topic and opts.message carry the topic and
// payload. For QoS 0 returns 0; for QoS 1/2 returns the assigned packet ID.
// To retransmit, set opts.retransmit_id to the ID from the previous call;
// use 0 for a new message.
uint16_t mg_mqtt_pub(struct mg_connection *c, const struct mg_mqtt_opts *opts);

// Sends a SUBSCRIBE packet for opts.topic at opts.qos.
void mg_mqtt_sub(struct mg_connection *, const struct mg_mqtt_opts *opts);

// Sends an UNSUBSCRIBE packet for opts.topic.
void mg_mqtt_unsub(struct mg_connection *c, const struct mg_mqtt_opts *opts);

// Parses a raw MQTT packet in buf/len. version must be 4 (MQTT 3.1.1) or 5.
// On success, fills *m; topic, data, and dgram slice into buf (zero-copy).
// Returns MQTT_OK, MQTT_INCOMPLETE (need more data), or MQTT_MALFORMED.
int mg_mqtt_parse(const uint8_t *, size_t, uint8_t, struct mg_mqtt_message *);

// Low-level helper: sends the MQTT fixed header (command byte and
// variable-length remaining-length field). Call before writing the PDU body.
// cmd: MQTT_CMD_*; flags: low 4 bits of the first byte; len: remaining length.
void mg_mqtt_send_header(struct mg_connection *, uint8_t cmd, uint8_t flags,
                         uint32_t len);

// Sends a PINGREQ packet.
void mg_mqtt_ping(struct mg_connection *);

// Sends a PINGRESP packet.
void mg_mqtt_pong(struct mg_connection *);

// Sends a DISCONNECT packet. For MQTT5, opts.props can carry reason
// properties. opts may be NULL for a plain disconnect with no properties.
void mg_mqtt_disconnect(struct mg_connection *, const struct mg_mqtt_opts *);

// Iterates MQTT5 properties in a received message. Start with ofs=0; pass
// the returned value back on each successive call. Returns 0 when done.
// Fills *prop with each property; key and val are zero-copy slices into msg->dgram.
size_t mg_mqtt_next_prop(struct mg_mqtt_message *, struct mg_mqtt_prop *,
                         size_t ofs);
