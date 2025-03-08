/** @file dns-server.c
 *
 *  @brief This file provides the DNS Server
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** dns-server.c: The DNS Server
 */
#include <string.h>

#include <osa.h>
#include <wm_net.h>
#include <dhcp-server.h>
#include <wlan.h>

#include "dhcp-bootp.h"
#include "dns.h"
#include "dhcp-priv.h"

static struct dns_server_data dnss;
static int (*dhcp_dns_server_handler)(char *msg, int len, struct sockaddr_in *fromaddr);
extern struct dhcp_server_data dhcps;

/* take a domain name and convert it into a DNS QNAME format, i.e.
 * foo.rats.com. -> 03 66 6f 6f 04 72 61 74 73 03 63 6f 6d 00
 *
 * The size of the QNAME will be one byte longer than the original string.
 */
static void format_qname(char *domain_name, char *dns_qname)
{
    int i   = 0, len;
    char *s = domain_name;
    char *d = dns_qname + 1;

    len = (int)strlen(s);
    s += len - 1;
    d += len - 1;

    while (s >= domain_name)
    {
        if (*s != '.')
        {
            *d = *s;
            i++;
        }
        else
        {
            *d = (char)i;
            i  = 0;
        }
        s--;
        d--;
    }
    dns_qname[0] = (char)i;
}

static unsigned int make_answer_rr(char *base, char *query, char *dst)
{
    struct dns_question *q;
    struct dns_rr *rr = (struct dns_rr *)(void *)dst;
    char *query_start = query;

    rr->name_ptr = htons(((uint16_t)(query - base) | 0xC000U));

    /* skip past the qname (label) field */
    do
    {
        if (*query > 0U)
        {
            query += *query + 1;
        }
    } while (*query > 0U);
    query++;

    q = (struct dns_question *)(void *)query;
    query += sizeof(struct dns_question);

    rr->type     = q->type;
    rr->class    = q->class;
    rr->ttl      = htonl(60U * 60U * 1U); /* 1 hour */
    rr->rdlength = htons(4);
    rr->rd       = dhcps.my_ip;

    return (unsigned int)(query - query_start);
}

static char *parse_questions(unsigned int num_questions, uint8_t *pos, int *found)
{
    uint8_t *base = pos;
    int i;

    pos += sizeof(struct dns_header);

    for (; num_questions > 0U; num_questions--)
    {
        if (!*found)
        {
            for (i = 0; i < dnss.count_qnames; i++)
            {
                *found =
                    (int)(!strncmp(dnss.list_qnames[i].qname, (char *)pos, (size_t)(base + SERVER_BUFFER_SIZE - pos)));
                if (*found != 0)
                {
                    break;
                }
            }
        }
        do
        {
            if (*pos > 0U)
            {
                pos += *pos + 1U;
            }
            if (pos >= base + SERVER_BUFFER_SIZE)
            {
                return NULL;
            }
        } while (*pos > 0U);
        pos += 1U + sizeof(struct dns_question);
    }
    return (char *)pos;
}

#define ERROR_REFUSED 5
static int process_dns_message(char *msg, int len, struct sockaddr_in *fromaddr)
{
    struct dns_header *hdr;
    char *pos;
    char *outp = msg + len;
    int found  = 0, nq, i;

    if (len < sizeof(struct dns_header))
    {
        dhcp_e("DNS request is not complete, hence ignoring it");
        return -WM_E_DHCPD_DNS_IGNORE;
    }

    hdr = (struct dns_header *)(void *)msg;

    dhcp_d("DNS transaction id: 0x%x", htons(hdr->id));

    if (hdr->flags.fields.qr != 0U)
    {
        dhcp_e("ignoring this dns message (not a query)");
        return -WM_E_DHCPD_DNS_IGNORE;
    }

    nq = (int)ntohs(hdr->num_questions);
    dhcp_d("we were asked %d questions", nq);

    if (nq <= 0)
    {
        dhcp_e("ignoring this dns msg (not a query or 0 questions)");
        return -WM_E_DHCPD_DNS_IGNORE;
    }

    outp = parse_questions((unsigned int)nq, (uint8_t *)msg, &found);
    if (found && outp != NULL)
    {
        pos = msg + sizeof(struct dns_header);
        for (i = 0; i < nq; i++)
        {
            if (outp + sizeof(struct dns_rr) >= msg + SERVER_BUFFER_SIZE)
            {
                dhcp_d("no room for more answers, refusing");
                break;
            }
            pos += make_answer_rr(msg, pos, outp);
            outp += sizeof(struct dns_rr);
        }
        hdr->flags.fields.qr    = 1;
        hdr->flags.fields.aa    = 1;
        hdr->flags.fields.rcode = 0;
        hdr->flags.num          = htons(hdr->flags.num);
        hdr->answer_rrs         = htons((uint16_t)i);
        /* the response consists of:
         * - 1 x DNS header
         * - num_questions x query fields from the message we're parsing
         * - num_answers x answer fields that we've appended
         */
        return SEND_RESPONSE(dnss.dnssock, (struct sockaddr *)(void *)fromaddr, msg, outp - msg);
    }

    /* make the header represent a response */
    hdr->flags.fields.qr     = 1;
    hdr->flags.fields.opcode = 0;
    /* Errors are never authoritative (unless they are
       NXDOMAINS, which this is not) */
    hdr->flags.fields.aa    = 0;
    hdr->flags.fields.tc    = 0;
    hdr->flags.fields.rd    = 1;
    hdr->flags.fields.ra    = 0;
    hdr->flags.fields.rcode = ERROR_REFUSED;
    hdr->flags.num          = htons(hdr->flags.num);
    /* number of entries in questions section */
    hdr->num_questions  = htons(0x01);
    hdr->answer_rrs     = 0; /* number of resource records in answer section */
    hdr->authority_rrs  = 0;
    hdr->additional_rrs = 0;
    (void)SEND_RESPONSE(dnss.dnssock, (struct sockaddr *)(void *)fromaddr, msg, outp - msg);

    return -WM_E_DHCPD_DNS_IGNORE;
}

void dhcp_enable_dns_server(char **domain_names)
{
    if (dhcp_dns_server_handler != NULL || dnss.list_qnames != NULL)
    {
        return;
    }

    int i;
    /* To reduce footprint impact, dns server support is kept optional */
    dhcp_dns_server_handler = process_dns_message;
    if (domain_names != NULL)
    {
        while (domain_names[dnss.count_qnames] != NULL)
        {
            dnss.count_qnames++;
        }
#if !CONFIG_MEM_POOLS
        dnss.list_qnames = OSA_MemoryAllocate(dnss.count_qnames * sizeof(struct dns_qname));
#else
        /*TODO:This function is not called from anywhere .
         * Please make sure to assign correct memory pool
         * whenever this function will be used. */

        dnss.list_qnames = OSA_MemoryPoolAllocate(buf_1280_MemoryPool);
#endif
        for (i = 0; i < dnss.count_qnames; i++)
        {
            (void)memset(dnss.list_qnames[i].qname, 0, sizeof(struct dns_qname));
            format_qname(domain_names[i], dnss.list_qnames[i].qname);
        }
    }
}

int dns_server_init(void *intrfc_handle)
{
    if (dhcp_dns_server_handler == NULL)
    {
        return WM_SUCCESS;
    }

    dnss.dnsaddr.sin_family      = AF_INET;
    dnss.dnsaddr.sin_addr.s_addr = INADDR_ANY;
    dnss.dnsaddr.sin_port        = htons(NAMESERVER_PORT);
    dnss.dnssock                 = dhcp_create_and_bind_udp_socket(&dnss.dnsaddr, intrfc_handle);
    if (dnss.dnssock < 0)
    {
        return -WM_E_DHCPD_SOCKET;
    }

    return WM_SUCCESS;
}

void dns_process_packet(void)
{
    if (dhcp_dns_server_handler == NULL)
    {
        return;
    }

    struct sockaddr_in caddr;
    socklen_t flen = sizeof(caddr);
    int len;
    len = recvfrom(dnss.dnssock, dhcps.msg, sizeof(dhcps.msg), 0, (struct sockaddr *)(void *)&caddr, &flen);
    if (len > 0 && len < SERVER_BUFFER_SIZE)
    {
        dhcp_d("recved msg on dns sock len: %d", len);
        (void)dhcp_dns_server_handler(dhcps.msg, len, &caddr);
    }
}

uint32_t dns_get_nameserver(void)
{
    if (dhcp_dns_server_handler != NULL)
    {
        return dhcps.my_ip;
    }
    return 0;
}

int dns_get_maxsock(fd_set *rfds)
{
    if (dhcp_dns_server_handler == NULL)
    {
        return dhcps.sock;
    }

    int max_sock;
    FD_SET(dnss.dnssock, rfds);
    max_sock = (dhcps.sock > dnss.dnssock ? dhcps.sock : dnss.dnssock);
    return max_sock;
}

void dns_free_allocations(void)
{
    if (dhcp_dns_server_handler == NULL)
    {
        return;
    }

    if (dnss.list_qnames != NULL)
    {
        dnss.count_qnames = 0;
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(dnss.list_qnames);
#else
        OSA_MemoryPoolFree(buf_1280_MemoryPool, dnss.list_qnames);
#endif
        dnss.list_qnames = NULL;
    }
    if (dnss.dnssock != -1)
    {
        if (net_close(dnss.dnssock) != 0)
        {
            dhcp_w("Failed to close dns socket: %d", net_get_sock_error(dnss.dnssock));
        }
        dnss.dnssock = -1;
    }
    dhcp_dns_server_handler = NULL;
}
