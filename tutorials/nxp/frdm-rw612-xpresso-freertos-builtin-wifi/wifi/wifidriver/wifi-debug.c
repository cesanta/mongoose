/** @file wifi-debug.c
 *
 *  @brief This file provides WIFI debug APIs.
 *
 *  Copyright 2008-2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <osa.h>

#include <wifi-debug.h>

#if CONFIG_WIFI_EXTRA_DEBUG
static const char *const status_table[] = {
    [0]  = "Successful",
    [1]  = "Unspecified failure",
    [10] = "Information field does not support all requested capabilities.",
    [11] = "Could not confirm existing assiciation. Denied reassociation.",
    [12] = "Denied Association. Reason beyond scope of Standard.",
    [13] = "Auth Algo not supported by responding station.",
    [14] = "Transaction sequence number of received authentication frame is out of expected sequence.",
    [15] = "Challenge failure. Rejected Auth.",
    [16] = "Timeout waiting for next frame in sequence. Rejected Auth.",
    [17] = "Failure at AP to to handle additional associated STA. Rejected Assoc.",
    [18] = "Requesting STA does not support all BSSBasicRateSet data rated. Denied Assoc.",
    [19] = "Requesting STA does not support short preamble option. Denied Assoc.",
    [20] = "Requesting STA does not support PBCC modulation option. Denied Assoc.",
    [21] = "Requesting STA does not support channel agility option. Denied Assoc.",
    [22] = "Spectrum Management capability needed. Rejected Assoc.",
    [23] = "Power Capability element info is unacceptable. Rejected Assoc.",
    [24] = "Supported Channels element info is unacceptable. Rejected Assoc.",
    [25] = "Requesting STA does not support short slot time option. Rejected Assoc.",
    [26] = "Requesting STA does not support ER-PBCC modulation option. Rejected Assoc.",
    [27] = "Requesting STA does not support HT features. Denied Assoc.",
    [28] = "Unreachable R0KH",
    [29] = "AP needs PCO transition support. Requesting STA does not support it. Denied Assoc.",
    [30] = "Try again later. Temporarily Rejected Assoc.",
    [31] = "Violation of policy for Robust Management frame",
    [32] = "Unspecified, QoS related failure",
    [33] = "Insufficient bandwidth with QAP to handle another QSTA. Denied Assoc.",
    [34] = "Poor channel conditions resulting in Assoic Denied.",
    [35] = "Requesting STA does not support QoS facility. Denied Assoc (with QBSS).",
    [37] = "The request has been declined",
    [38] = "The failed. One or more parameters have invalid values",
    [39] =
        "The TS has not been created because the request cannot be "
        "honored. However, a suggested Tspec is provided so that the "
        "initiating QSTA may attempt to send another TS with the suggested "
        "changes to the TSpec",
    [40] = "Information Element is Invalid",
    [41] = "Invalid Group Cipher",
    [42] = "Invalid Pairwise Cipher",
    [43] = "Invalid AKMP",
    [44] = "RSN IE version unsupported ",
    [45] = "RSN IE Capabilities are invalid ",
    [46] = "Security policy does not allows rejected Cipher suite",
    [47] =
        "The TS has not been created. However, the HC may be capable "
        "of creating a TS, in response to a request, after the time "
        "indicated in the TS Delay element",
    [48] = "By policy BSS not allow Direct link",
    [49] = "QBSS does not include Destination STA",
    [50] = "The destination STA is not a QSTA",
    [51] = "Listen Interval is too large. Assoc Denied",
    [52] = "Fast BSS Transition Action Frame Count Invalid",
    [53] = "PMKID Invalid",
    [54] = "MDIE Invalid",
    [55] = "FTIE Invalid",
};

const char *get_status_str(uint16_t status)
{
    if (status < sizeof(status_table) / sizeof(status_table[0]) && status_table[status])
        return status_table[status];
    return "<unknown>";
}
#endif /* CONFIG_WIFI_EXTRA_DEBUG */

void wifi_show_assoc_fail_reason(int status)
{
#if CONFIG_WIFI_EXTRA_DEBUG
    (void)PRINTF("[wifi] Assoc Status: %s\n\r", get_status_str(status));

    switch (status)
    {
        case 13:
            (void)PRINTF("Assoc failed: Network not found.\n\r");
            break;
        case 18:
            (void)PRINTF("(11n disabled ?)\n\r");
            break;
    }
#endif /* CONFIG_WIFI_EXTRA_DEBUG */
}

#ifdef DUMP_PACKET_MAC
void dump_mac_addr(const char *msg, unsigned char *addr)
{
    if (msg)
        (void)PRINTF("%s: ", msg);
    else
        (void)PRINTF("mac: ", msg);

    int i;
    for (i = 0; i < MLAN_MAC_ADDR_LENGTH; i++)
    {
        (void)PRINTF("%x", addr[i]);
        if (i != (MLAN_MAC_ADDR_LENGTH - 1))
            (void)PRINTF(":");
    }

    (void)PRINTF("\n\r");
}
#endif /* DUMP_PACKET_MAC */



