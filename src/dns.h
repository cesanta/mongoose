#pragma once

#include "net.h"
#include "str.h"

// DNS record types, used in mg_dns_rr.atype and mg_mdns_query().
#define MG_DNS_RTYPE_A 1     // IPv4 address (4 bytes)
#define MG_DNS_RTYPE_PTR 12  // Domain name pointer, used for reverse lookups and service discovery
#define MG_DNS_RTYPE_TXT 16  // Arbitrary text, used by DNS-SD for service metadata
#define MG_DNS_RTYPE_AAAA 28 // IPv6 address (16 bytes)
#define MG_DNS_RTYPE_SRV 33  // Service location: priority, weight, port, target hostname

// Mongoose sends DNS queries that contain only one question:
// either A (IPv4) or AAAA (IPv6) address lookup.
// Therefore, we expect zero or one answer.
// If `resolved` is true, then `addr` contains resolved IPv4 or IPV6 address.
struct mg_dns_message {
  uint16_t txnid;       // Transaction ID
  bool resolved;        // Resolve successful, addr is set
  struct mg_addr addr;  // Resolved address
  char name[256];       // Host name
};

// DNS wire-format header (RFC 1035). All fields are in network byte order;
// use mg_ntohs() to read them.
struct mg_dns_header {
  uint16_t txnid;           // Transaction ID, matched between query and response
  uint16_t flags;           // Flags: bit15=QR (0=query,1=response), bit10=AA, bit8=RD
  uint16_t num_questions;   // Number of question records following the header
  uint16_t num_answers;     // Number of answer records in the response
  uint16_t num_authority_prs;  // Number of authority records
  uint16_t num_other_prs;      // Number of additional records
};

// Parsed DNS resource record fields, extracted from the wire format.
struct mg_dns_rr {
  uint16_t nlen;    // Byte length of the encoded name in the wire format
  uint16_t atype;   // Record type: MG_DNS_RTYPE_A=1, AAAA=28, PTR=12, SRV=33, TXT=16
  uint16_t aclass;  // Record class: 1=INET; mDNS queries: bit15=QU (unicast request)
  uint16_t alen;    // Byte length of the RDATA section (e.g. 4 for A, 16 for AAAA)
};

// DNS-SD service description. Used by the user to describe a service to advertise.
struct mg_dnssd_record {
  struct mg_str srvcproto;  // Service and protocol label, e.g. "_http._tcp"
  struct mg_str txt;        // TXT record contents, verbatim
  uint16_t port;            // Port number for the SRV record
};

// ev_data for MG_EV_MDNS_REQ. The handler must set is_resp=true and fill r
// (and optionally respname/addr) to send a response; leave is_resp=false to ignore.
struct mg_mdns_req {
  struct mg_dns_rr *rr;         // Parsed resource record from the incoming query
  struct mg_dnssd_record *r;    // User-supplied service record to include in the response
  struct mg_str reqname;        // Queried hostname, without the .local suffix
  struct mg_str respname;       // Hostname to use in response; defaults to fn_data if empty
  struct mg_addr *addr;         // IP address for A record; uses local interface if NULL
  bool is_listing;  // True if this is a service-discovery listing (_services._dns-sd._udp)
  bool is_resp;     // Set to true in the handler to trigger a response
  bool is_unicast;  // True if the client requested a unicast (QU) response
};

// ev_data for MG_EV_MDNS_RESP, carrying address/data from a mDNS response
struct mg_mdns_resp {
  struct mg_dns_rr *rr;  // Resource record from the response (1st in chain)
  struct mg_str name;    // Resolved hostname, without the .local suffix
  struct mg_addr addr;   // Resolved IP address
  struct mg_dnssd_record sd; // Service Discovery data
};

// Parses a DNS query or response from buf/len into dm. Returns true on success.
// On a response, sets dm->resolved=true and fills dm->addr if an A/AAAA record
// is found. Always fills dm->txnid and dm->name. Rejects packets larger than 512 bytes.
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm);

// Parses one DNS record at offset ofs in buf/len into rr. Set is_question=true
// for question records (no RDATA), false for answer records. Returns the number
// of bytes consumed (use to advance ofs), or 0 on error.
size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *rr);

// Creates an mDNS listener on UDP multicast 224.0.0.251:5353. Registers it as
// the manager's mDNS resolver (mgr->mdns). Fires MG_EV_MDNS_REQ for incoming
// queries and MG_EV_MDNS_RESP for incoming responses. Returns NULL on error.
struct mg_connection *mg_mdns_listen(struct mg_mgr *mgr, mg_event_handler_t fn,
                                     void *fn_data);

// Sends an mDNS query on connection c (from mg_mdns_listen()) for name, with
// record type rtype (e.g. MG_DNS_RTYPE_A). Returns false on send error.
bool mg_mdns_query(struct mg_connection *c, const char *name, unsigned int rtype);


// Private API, do not expose or call in the user app
void mg_resolve(struct mg_connection *, const char *url);
void mg_resolve_cancel(struct mg_connection *);
