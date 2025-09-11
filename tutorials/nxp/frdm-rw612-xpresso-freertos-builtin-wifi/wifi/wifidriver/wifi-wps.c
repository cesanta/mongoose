/** @file wifi-wps.c
 *
 *  @brief This file provides WPS IE and parser.
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <mlan_api.h>

#define SC_Device_Password_ID 0x1012

const t_u8 wps_oui[4] = {0x00, 0x50, 0xf2, 0x04};

typedef MLAN_PACK_START struct
{
    t_u16 Type;
    t_u16 Length;
} MLAN_PACK_END MrvlIEParamSet_t;

/**
 *  @brief  Parsing Device Password ID
 *
 *  @param message      A pointer to buffer for parsing
 *  @param size         Length of buffer
 *  @return             Device Password ID
 */
static t_u16 wps_parser(t_u8 *message, size_t size)
{
    t_u16 device_password_id = 0xffff;
    MrvlIEParamSet_t *ptlv;
    t_u8 *plast_byte, *data;
    t_u16 len;

    /* Beginning from Version, skip IE_ID/Length/SC_OUI field */
    ptlv       = (MrvlIEParamSet_t *)(message + 4);
    data       = (t_u8 *)ptlv;
    plast_byte = (t_u8 *)(message + (t_u8)size);

    while ((void *)ptlv < (void *)plast_byte)
    {
		/* Barriers are normally not required but do ensure the code is
		 * completely within the specified behaviour for the architecture. */
        __asm volatile ( "dsb" ::: "memory" );
        __asm volatile ( "isb" );

		ptlv->Type   = mlan_ntohs(ptlv->Type);
        ptlv->Length = mlan_ntohs(ptlv->Length);

        switch (ptlv->Type)
        {
            case SC_Device_Password_ID:
                wifi_d("SC_Device_Password_ID :: ");
                memcpy(&device_password_id, data, sizeof(t_u16));
                device_password_id = mlan_ntohs(device_password_id);
                wifi_d("device_password_id = 0x%x", device_password_id);
                break;
            default:
                break;
        }

        len = ptlv->Length + sizeof(MrvlIEParamSet_t);

        ptlv->Type   = mlan_htons(ptlv->Type);
        ptlv->Length = mlan_htons(ptlv->Length);

        ptlv = (MrvlIEParamSet_t *)((t_u8 *)ptlv + len);

        data = (t_u8 *)ptlv;
        data += sizeof(MrvlIEParamSet_t);
    } /* while */

    return device_password_id;
}

void check_for_wps_ie(
    const t_u8 *poui, t_u8 oui_type, bool *wps_IE_exist, t_u16 *wps_session, void *element_data, unsigned element_len)
{
    if (!memcmp(poui, &wps_oui, sizeof(wps_oui) - 1U) && oui_type == wps_oui[3])
    {
        /* WPS IE is present in probe response. */
        wifi_d("WPS IE :: %x:%x:%x:%x", poui[0], poui[1], poui[2], oui_type);

        *wps_IE_exist = true;
        *wps_session  = wps_parser(element_data, element_len);
    }
    else if (!(*wps_IE_exist))
    {
        *wps_IE_exist = false;
        *wps_session  = 0xffff;
    }
}
