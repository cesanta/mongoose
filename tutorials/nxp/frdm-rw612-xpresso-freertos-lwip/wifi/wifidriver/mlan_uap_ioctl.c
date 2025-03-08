/** @file mlan_uap_ioctl.c
 *
 *  @brief  This file provides handling of AP mode ioctls
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    02/05/2009: initial version
********************************************************/

#if CONFIG_NXP_WIFI_SOFTAP_SUPPORT
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>


#if defined(WAPI_AP) || defined(HOST_AUTHENTICATOR) || (CONFIG_HOSTAPD)
/**
 *  @brief Set encrypt key
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_uap_sec_ioctl_set_encrypt_key(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_sec_cfg *sec = MNULL;

    ENTER();
    sec = (mlan_ds_sec_cfg *)pioctl_req->pbuf;
    if (pioctl_req->action != MLAN_ACT_SET)
    {
        pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
#ifdef KEY_PARAM_SET_V2
    if (!sec->param.encrypt_key.key_remove && !sec->param.encrypt_key.key_len)
    {
#else
    if (!sec->param.encrypt_key.key_len)
    {
#endif /* KEY_PARAM_SET_V2 */
        PRINTM(MCMND, "Skip set key with key_len = 0\n");
        LEAVE();
        return ret;
    }
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_KEY_MATERIAL, HostCmd_ACT_GEN_SET, KEY_INFO_ENABLED,
                           (t_void *)pioctl_req, &sec->param.encrypt_key);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;
    LEAVE();
    return ret;
}
#endif


#if CONFIG_ECSA
/**
 *  @brief Handle channel switch
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_uap_bss_ioctl_action_chan_switch(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action     = 0;

    ENTER();

    cmd_action = HostCmd_ACT_GEN_SET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, cmd_action, 0, (t_void *)pioctl_req, MNULL);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}
#endif

/**
 *  @brief MLAN uap ioctl handler
 *
 *  @param adapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_ops_uap_ioctl(t_void *adapter, pmlan_ioctl_req pioctl_req)
{
    pmlan_adapter pmadapter = (pmlan_adapter)adapter;
    mlan_status status      = MLAN_STATUS_SUCCESS;
#if CONFIG_ECSA
    mlan_ds_misc_cfg *misc = MNULL;
    mlan_ds_bss *bss       = MNULL;
#endif
#if defined(WAPI_AP) || defined(HOST_AUTHENTICATOR) || (CONFIG_HOSTAPD)
    mlan_ds_sec_cfg *sec = MNULL;
#endif
    mlan_ds_rate *rate = MNULL;

    ENTER();

    switch (pioctl_req->req_id)
    {
#if defined(WAPI_AP) || defined(HOST_AUTHENTICATOR) || (CONFIG_HOSTAPD)
        case MLAN_IOCTL_SEC_CFG:
            sec = (mlan_ds_sec_cfg *)pioctl_req->pbuf;
            if (sec->sub_command == MLAN_OID_SEC_CFG_ENCRYPT_KEY)
                status = wlan_uap_sec_ioctl_set_encrypt_key(pmadapter, pioctl_req);
            break;
#endif /* WAPI_AP || HOST_AUTHENTICATOR */
        case MLAN_IOCTL_RATE:
            rate = (mlan_ds_rate *)(void *)pioctl_req->pbuf;
            if (rate->sub_command == MLAN_OID_RATE_CFG)
            {
                status = wlan_rate_ioctl_cfg(pmadapter, pioctl_req);
            }
            break;
#if CONFIG_ECSA
        case MLAN_IOCTL_MISC_CFG:
            misc = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
            if (misc->sub_command == MLAN_OID_MISC_OPER_CLASS)
                status = wlan_misc_ioctl_oper_class(pmadapter, pioctl_req);
            if (misc->sub_command == MLAN_OID_MISC_OPER_CLASS_CHECK)
                status = wlan_misc_ioctl_operclass_validation(pmadapter, pioctl_req);
            break;
        case MLAN_IOCTL_BSS:
            bss = (mlan_ds_bss *)pioctl_req->pbuf;
            if (bss->sub_command == MLAN_OID_ACTION_CHAN_SWITCH)
                status = wlan_uap_bss_ioctl_action_chan_switch(pmadapter, pioctl_req);
            break;
#endif
#if CONFIG_11AX
        case MLAN_IOCTL_11AX_CFG:
            status = wlan_11ax_cfg_ioctl(pmadapter, pioctl_req);
            break;
#endif
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            break;
    }
    LEAVE();
    return status;
}
#endif /* CONFIG_NXP_WIFI_SOFTAP_SUPPORT */
