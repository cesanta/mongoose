/** @file dns.h
 *
 *  @brief DNS Header
 *
 *  Copyright 2008-2020 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __DNS_H__
#define __DNS_H__

PACK_START struct dns_header
{
    uint16_t id;
    PACK_START union
    {
        PACK_START struct
        {
            uint16_t rcode : 4, /* response code */
                cd : 1,         /* checking disabled RFC-2535 */
                ad : 1,         /* authentic data RFC-2535 */
                z : 1,          /* zero */
                ra : 1,         /* recursion available */
                rd : 1,         /* recursion desired */
                tc : 1,         /* truncated */
                aa : 1,         /* authoritative answer */
                opcode : 4,     /* (should be 0 for normal DNS messages) */
                qr : 1;         /* query/response */
        } fields PACK_END;
        uint16_t num;
    } flags PACK_END;
    uint16_t num_questions;
    uint16_t answer_rrs;
    uint16_t authority_rrs;
    uint16_t additional_rrs;
} PACK_END;

PACK_START struct dns_question
{
    /* query name (label) field */
    uint16_t type;
    uint16_t class;
} PACK_END;

PACK_START struct dns_rr
{
    uint16_t name_ptr; /* pointer to name */
    uint16_t type;     /* resource type */
    uint16_t class;    /* resource class */
    uint32_t ttl;      /* time to live */
    uint16_t rdlength; /* resource data length */
    uint32_t rd;       /* resource data: we only provide a
               4-byte data response (an IP
               address) but this is actually a
               field of length rdlength */
} PACK_END;

struct dns_qname
{
    char qname[MAX_QNAME_SIZE + 1];
};

struct dns_server_data
{
    int count_qnames;
    int dnssock;
    struct sockaddr_in dnsaddr; /* dns server address */
    struct dns_qname *list_qnames;
};

int dns_server_init(void *intrfc_handle);
void dns_process_packet(void);
uint32_t dns_get_nameserver(void);
int dns_get_maxsock(fd_set *rfds);
void dns_free_allocations(void);
#endif /* __DNS_H__ */
