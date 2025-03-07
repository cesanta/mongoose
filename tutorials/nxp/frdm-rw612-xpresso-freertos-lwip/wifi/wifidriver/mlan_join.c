/** @file mlan_join.c
 *
 *  @brief  This file provides Functions implementing wlan infrastructure
 *  and adhoc join routines IOCTL handlers as well as command preparation
 *  and response routines for sending adhoc start, adhoc join, and
 *  association commands to the firmware.
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    10/30/2008: initial version
******************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
#if CONFIG_WPS2
#include "wifi_nxp_wps.h"
#endif
/********************************************************
                Local Constants
********************************************************/

/********************************************************
                Local Variables
********************************************************/

/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/
/**
 *  @brief Append a generic IE as a pass through TLV to a TLV buffer.
 *
 *  This function is called from the network join command prep. routine.
 *    If the IE buffer has been setup by the application, this routine appends
 *    the buffer as a pass through TLV type to the request.
 *
 *  @param priv     A pointer to mlan_private structure
 *  @param ppbuffer pointer to command buffer pointer
 *
 *  @return         bytes added to the buffer
 */
static int wlan_cmd_append_generic_ie(mlan_private *priv, t_u8 **ppbuffer)
{
    int ret_len = 0;
    MrvlIEtypesHeader_t ie_header;

    ENTER();

    /* Null Checks */
    if (ppbuffer == MNULL)
    {
        LEAVE();
        return 0;
    }
    if (*ppbuffer == MNULL)
    {
        LEAVE();
        return 0;
    }

    /*
     * If there is a generic ie buffer setup, append it to the return
     *   parameter buffer pointer.
     */
    if (priv->gen_ie_buf_len)
    {
        PRINTM(MINFO, "append generic IE %d to %p\n", priv->gen_ie_buf_len, *ppbuffer);

        /* Wrap the generic IE buffer with a pass through TLV type */
        ie_header.type = wlan_cpu_to_le16(TLV_TYPE_PASSTHROUGH);
        ie_header.len  = wlan_cpu_to_le16(priv->gen_ie_buf_len);
        (void)__memcpy(priv->adapter, *ppbuffer, &ie_header, sizeof(ie_header));

        /* Increment the return size and the return buffer pointer param */
        *ppbuffer += sizeof(ie_header);
        ret_len += sizeof(ie_header);

        /* Copy the generic IE buffer to the output buffer, advance pointer */
        (void)__memcpy(priv->adapter, *ppbuffer, priv->gen_ie_buf, priv->gen_ie_buf_len);

        /* Increment the return size and the return buffer pointer param */
        *ppbuffer += priv->gen_ie_buf_len;
        ret_len += priv->gen_ie_buf_len;

        /* Reset the generic IE buffer */
        priv->gen_ie_buf_len = 0;
    }

    /* return the length appended to the buffer */
    LEAVE();
    return ret_len;
}


/**
 *  @brief This function finds out the common rates between rate1 and rate2.
 *
 *  It will fill common rates in rate1 as output if found.
 *
 *  NOTE: Setting the MSB of the basic rates needs to be taken
 *   care of, either before or after calling this function
 *
 *  @param pmpriv      A pointer to mlan_private structure
 *  @param rate1       the buffer which keeps input and output
 *  @param rate1_size  the size of rate1 buffer
 *  @param rate2       the buffer which keeps rate2
 *  @param rate2_size  the size of rate2 buffer.
 *
 *  @return            MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_get_common_rates(
    IN mlan_private *pmpriv, IN t_u8 *rate1, IN t_u32 rate1_size, IN t_u8 *rate2, IN t_u32 rate2_size)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
#if !CONFIG_MEM_POOLS
    mlan_callbacks *pcb = (mlan_callbacks *)&pmpriv->adapter->callbacks;
#endif
    t_u8 *ptr = rate1;
    t_u8 *tmp = MNULL;
    t_u32 i, j;

    ENTER();

#if !CONFIG_MEM_POOLS
    ret = pcb->moal_malloc(pmpriv->adapter->pmoal_handle, rate1_size, MLAN_MEM_DEF, &tmp);
    if (ret != MLAN_STATUS_SUCCESS || (tmp == MNULL))
#else
    tmp = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
    if (tmp == MNULL)
#endif
    {
        PRINTM(MERROR, "Failed to allocate buffer\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    (void)__memcpy(pmpriv->adapter, tmp, rate1, rate1_size);
    (void)__memset(pmpriv->adapter, rate1, 0, rate1_size);

    for (i = 0; rate2[i] && i < rate2_size; i++)
    {
        for (j = 0; tmp[j] && j < rate1_size; j++)
        {
            /* Check common rate, excluding the bit for basic rate */
            if ((rate2[i] & 0x7FU) == (tmp[j] & 0x7FU))
            {
                *rate1++ = tmp[j];
                break;
            }
        }
    }

    HEXDUMP("rate1 (AP) Rates", tmp, rate1_size);
    HEXDUMP("rate2 (Card) Rates", rate2, rate2_size);
    HEXDUMP("Common Rates", ptr, rate1 - ptr);
    PRINTM(MINFO, "Tx DataRate is set to 0x%X\n", pmpriv->data_rate);

    if (!pmpriv->is_data_rate_auto)
    {
        while (*ptr != 0U)
        {
            if ((*ptr & 0x7fU) == pmpriv->data_rate)
            {
                ret = MLAN_STATUS_SUCCESS;
                goto done;
            }
            ptr++;
        }
        PRINTM(MMSG,
               "Previously set fixed data rate %#x is not "
               "compatible with the network\n",
               pmpriv->data_rate);

        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    ret = MLAN_STATUS_SUCCESS;
done:
    if (tmp != MNULL)
    {
#if !CONFIG_MEM_POOLS
        (void)pcb->moal_mfree(pmpriv->adapter->pmoal_handle, tmp);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, tmp);
#endif
    }

    LEAVE();
    return ret;
}

#if 0
/**
 *  @brief Create the intersection of the rates supported by a target BSS and
 *         our pmadapter settings for use in an assoc/join command.
 *
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param pbss_desc        BSS Descriptor whose rates are used in the setup
 *  @param pout_rates       Output: Octet array of rates common between the BSS
 *                          and the pmadapter supported rates settings
 *  @param pout_rates_size  Output: Number of rates/octets set in pout_rates
 *
 *  @return                 MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_setup_rates_from_bssdesc(IN mlan_private *pmpriv,
                                                 IN BSSDescriptor_t *pbss_desc,
                                                 OUT t_u8 *pout_rates,
                                                 OUT t_u32 *pout_rates_size)
{
    t_u8 card_rates[WLAN_SUPPORTED_RATES] = {0x0};
    t_u32 card_rates_size                 = 0;
    ENTER();

    (void)__memset(pmadapter, card_rates, 0x00, WLAN_SUPPORTED_RATES);
    /* Copy AP supported rates */
    (void)__memcpy(pmpriv->adapter, pout_rates, pbss_desc->supported_rates, WLAN_SUPPORTED_RATES);
    /* Get the STA supported rates */
    card_rates_size = wlan_get_active_data_rates(pmpriv, pmpriv->bss_mode, pmpriv->config_bands, card_rates);
    /* Get the common rates between AP and STA supported rates */
    if (wlan_get_common_rates(pmpriv, pout_rates, WLAN_SUPPORTED_RATES, card_rates, card_rates_size) !=
        MLAN_STATUS_SUCCESS)
    {
        *pout_rates_size = 0;
        PRINTM(MERROR, "wlan_get_common_rates failed\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    *pout_rates_size = MIN(wlan_strlen((char *)pout_rates), WLAN_SUPPORTED_RATES);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief Create the intersection of the rates supported by a target BSS and
 *         our pmadapter settings for use in an assoc/join command.
 *
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param pbss_desc        BSS Descriptor whose rates are used in the setup
 *  @param pout_rates       Output: Octet array of rates common between the BSS
 *                          and the pmadapter supported rates settings
 *  @param pout_rates_size  Output: Number of rates/octets set in pout_rates
 *
 *  @return                 MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_setup_rates_from_bssdesc(mlan_private *pmpriv,
                                          BSSDescriptor_t *pbss_desc,
                                          t_u8 *pout_rates,
                                          t_u32 *pout_rates_size)
{
    t_u8 card_rates[WLAN_SUPPORTED_RATES] = {0};
    t_u32 card_rates_size                 = 0;
    ENTER();
    /* Copy AP supported rates */
    (void)__memcpy(pmpriv->adapter, pout_rates, pbss_desc->supported_rates, WLAN_SUPPORTED_RATES);

    if ((pmpriv->adapter->region_code == COUNTRY_CODE_JP_40 || pmpriv->adapter->region_code == COUNTRY_CODE_JP_FF) &&
        (pbss_desc->phy_param_set.ds_param_set.current_chan == 14))
    {
        /* Special Case: For Japan, 11G rates on CH14 are not allowed*/
        card_rates_size = wlan_get_supported_rates(pmpriv, pmpriv->bss_mode, BAND_B, card_rates);
    }
    else
    {
        /* Get the STA supported rates */
        card_rates_size = wlan_get_supported_rates(pmpriv, pmpriv->bss_mode, pmpriv->config_bands, card_rates);
    }
    /* Get the common rates between AP and STA supported rates */
    if (wlan_get_common_rates(pmpriv, pout_rates, WLAN_SUPPORTED_RATES, card_rates, card_rates_size))
    {
        *pout_rates_size = 0;
        PRINTM(MERROR, "wlan_get_common_rates failed\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    *pout_rates_size = MIN(wlan_strlen((char *)pout_rates), WLAN_SUPPORTED_RATES);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


/**
 *  @brief This function updates RSN IE in the association request.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *
 *  @param ptlv_rsn_ie       A pointer to rsn_ie TLV
 */
static int wlan_update_rsn_ie(mlan_private *pmpriv,
                              MrvlIEtypes_RsnParamSet_t *ptlv_rsn_ie,
                              t_u16 *rsn_ie_len,
                              t_u8 *akm_type)
{
    t_u16 *prsn_cap;
    t_u8 *ptr;
    t_u8 *pairwise_cipher_count_ptr;
    t_u8 *group_mgmt_cipher_suite_ptr = MNULL;
    t_u8 *pmkid_list_ptr              = MNULL;
    t_u8 *end_ptr;
    t_u16 pmf_mask              = 0x00;
    t_u16 pairwise_cipher_count = 0;
    t_u16 akm_suite_count       = 0;
    t_u16 pmkid_count           = 0;
    t_u8 i;

#define PREFERENCE_TKIP 1
    /* Cipher Perference Order:
       (5) CIPHER_SYITE_TYPE_GCMP_256 = 9
       (4) CIPHER_SYITE_TYPE_GCMP_128 = 8
       (3) CIPHER_SYITE_TYPE_CCMP_256 = 10
       (2) CIPHER_SYITE_TYPE_CCMP_128 = 4
       (1) CIPHER_SYITE_TYPE_TKIP     = 2
       (0) Skip
    */
    t_u8 preference_selected;
    t_u8 cipher_selected_id;
#if CONFIG_WPA_SUPP
    t_u8 cipher_preference[11] = {0, 0, 1, 0, 2, 0, 0, 0, 4, 5, 3};
#else
    t_u8 cipher_preference[5] = {0, 0, 1, 0, 2};
#endif
    t_u8 oui[4] = {0x00, 0x0f, 0xac, 0x00};

    /* AKM Perference Order:
       (13) AKM_SUITE_TYPE_SAE_EXT_KEY       = 24
       (12) AKM_SUITE_TYPE_1X_SUITEB_FT_SHA384  = 13
       (11) AKM_SUITE_TYPE_1X_SUITEB_SHA384  = 12
       (10) AKM_SUITE_TYPE_1X_SUITEB_SHA256  = 11
       (9) AKM_SUITE_TYPE_1X_FT  = 3
       (8) AKM_SUITE_TYPE_1X_SHA256  = 5
       (7) AKM_SUITE_TYPE_1X         = 1
       (6) AKM_SUITE_TYPE_FT_SAE     = 9   //Not supported in esupp
       (5) AKM_SUITE_TYPE_SAE        = 8
       (4) AKM_SUITE_TYPE_OWE        = 18
       (3) AKM_SUITE_TYPE_FT_PSK     = 4   //Not supported in esupp
       (2) AKM_SUITE_TYPE_PSK_SHA256 = 6
       (1) AKM_SUITE_TYPE_PSK        = 2
       (0) Skip
    */
    t_u8 akm_type_selected;
    t_u8 akm_type_id        = 0;
#if CONFIG_11R
    t_u8 akm_preference[25] = {0, 7, 1, 9, 3, 8, 2, 0, 5, 6, 0, 10, 11, 12, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 13};
#else
    t_u8 akm_preference[25] = {0, 7, 1, 0, 0, 8, 2, 0, 5, 0, 0, 10, 11, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 13};
#endif

    int ap_mfpc = 0, ap_mfpr = 0, ret = MLAN_STATUS_SUCCESS;

    pmf_mask = (((pmpriv->pmfcfg.mfpc << MFPC_BIT) | (pmpriv->pmfcfg.mfpr << MFPR_BIT)) | (~PMF_MASK));
    /* prsn_cap = prsn_ie->rsn_ie + 2 bytes version + 4 bytes
     * group_cipher_suite + 2 bytes pairwise_cipher_count +
     * pairwise_cipher_count * PAIRWISE_CIPHER_SUITE_LEN + 2 bytes
     * akm_suite_count + akm_suite_count * AKM_SUITE_LEN
     */
    end_ptr = ptlv_rsn_ie->rsn_ie + *rsn_ie_len;

    ptr = ptlv_rsn_ie->rsn_ie + sizeof(t_u16) + 4 * sizeof(t_u8);

    pairwise_cipher_count_ptr = ptr;
    pairwise_cipher_count     = wlan_le16_to_cpu(*(t_u16 *)ptr);
    ptr += sizeof(t_u16);

    preference_selected = 0;
    cipher_selected_id  = 0;
    for (i = 0; i < pairwise_cipher_count; i++)
    {
        if ((ptr[3] < sizeof(cipher_preference)) && (cipher_preference[ptr[3]] > preference_selected))
        {
            preference_selected = cipher_preference[ptr[3]];
            cipher_selected_id  = ptr[3];
        }
        ptr += PAIRWISE_CIPHER_SUITE_LEN;
    }

    if (preference_selected == 0)
    {
        PRINTM(MERROR, "RSNE: PAIRWISE_CIPHER not supported\n");
        return MLAN_STATUS_FAILURE;
    }
    if ((preference_selected == PREFERENCE_TKIP) && ((*akm_type == AssocAgentAuth_Wpa3Sae)
#if CONFIG_DRIVER_OWE
                                                     || (*akm_type == AssocAgentAuth_Owe)))
    {
#else
                                                         ))
    {
#endif
        PRINTM(MERROR, "RSNE: PAIRWISE_CIPHER TKIP not allowed for AKM %s\n",
               (*akm_type == AssocAgentAuth_Wpa3Sae) ? "SAE" : "OWE");
        return MLAN_STATUS_FAILURE;
    }
    if ((preference_selected == PREFERENCE_TKIP) && (*akm_type == AssocAgentAuth_Auto))
    {
        *akm_type = AssocAgentAuth_Open;
    }
    /* Process AKM
     * Preference order for AssocAgentAuth_Auto:
     *  FT Authentication using SAE 00-0F-AC:9  (not supported in embedded supplicant)
     *  SAE Authentication 00-0F-AC:8
     *  OWE Authentication 00-0F-AC:18
     *  FT Authentication using PSK 00-0F-AC:4  (not supported in embedded supplicant)
     *  PSK using SHA-256 00-0F-AC:6
     *  PSK 00-0F-AC:2
     */
    ptr = ptlv_rsn_ie->rsn_ie + sizeof(t_u16) + 4 * sizeof(t_u8) + sizeof(t_u16) +
          pairwise_cipher_count * PAIRWISE_CIPHER_SUITE_LEN;
    akm_suite_count = wlan_le16_to_cpu(*(t_u16 *)ptr);
    ptr += sizeof(t_u16); // move pointer to AKM suite

    akm_type_selected = 0;

    if (*akm_type == AssocAgentAuth_Auto)
    {
        // find the best one
        for (i = 0; i < akm_suite_count; i++)
        {
            if ((ptr[3] < sizeof(akm_preference)) && (akm_preference[ptr[3]] > akm_type_selected))
            {
                akm_type_selected = akm_preference[ptr[3]];
                akm_type_id       = ptr[3];
            }
            ptr += AKM_SUITE_LEN;
        }
        if (akm_type_selected)
        {
            if (akm_type_id == 6)
            {
                *akm_type = AssocAgentAuth_Open;
            }
            else if (akm_type_id == 1)
            {
                *akm_type = AssocAgentAuth_Open;
            }
            else if (akm_type_id == 3)
            {
                *akm_type = AssocAgentAuth_Open;
            }
            else if (akm_type_id == 5)
            {
                *akm_type = AssocAgentAuth_Open;
            }
            else if (akm_type_id == 11)
            {
                *akm_type = AssocAgentAuth_Open;
            }
            else if (akm_type_id == 12)
            {
                *akm_type = AssocAgentAuth_Open;
            }
            else if (akm_type_id == 13)
            {
                *akm_type = AssocAgentAuth_Open;
            }
            else if (akm_type_id == 2)
            {
                *akm_type = AssocAgentAuth_Open;
            }
#if CONFIG_11R
            else if (akm_type_id == 4)
            {
                *akm_type = AssocAgentAuth_FastBss;
            }
#endif
#if CONFIG_DRIVER_OWE
            else if (akm_type_id == 18)
            {
                *akm_type = AssocAgentAuth_Owe;
            }
#endif
            else if (akm_type_id == 8 || akm_type_id == 24)
            {
                *akm_type = AssocAgentAuth_Wpa3Sae;
            }
#if CONFIG_11R
            else if (akm_type_id == 9)
            {
                *akm_type = AssocAgentAuth_FastBss_SAE;
            }
#endif
        }
    }
    else
    {
        // find the matched AKM

        for (i = 0; i < akm_suite_count; i++)
        {
            if (ptr[3] < sizeof(akm_preference))
            {
#if CONFIG_11R
                if ((*akm_type == AssocAgentAuth_FastBss_Skip) && (ptr[3] == 13))
                {
                    break;
                }
                else
#endif
                    if ((*akm_type == AssocAgentAuth_Open) && (ptr[3] == 12))
                {
                    break;
                }
                else if ((*akm_type == AssocAgentAuth_Open) && (ptr[3] == 11))
                {
                    break;
                }
#if CONFIG_11R
                else if ((*akm_type == AssocAgentAuth_FastBss_Skip) && (ptr[3] == 3))
                {
                    break;
                }
#endif
                else if ((*akm_type == AssocAgentAuth_Open) && (ptr[3] == 5))
                {
                    break;
                }
                else if ((*akm_type == AssocAgentAuth_Open) && (ptr[3] == 1))
                {
                    break;
                }
#if CONFIG_11R
                else if ((*akm_type == AssocAgentAuth_FastBss_Skip) && (ptr[3] == 9))
                {
                    break;
                }
#endif
                else if ((*akm_type == AssocAgentAuth_Wpa3Sae) && (ptr[3] == 8 || ptr[3] == 24))
                {
                    break;
                }
#if CONFIG_DRIVER_OWE
                else if ((*akm_type == AssocAgentAuth_Owe) && (ptr[3] == 18))
                {
                    break;
                }
#endif
#if CONFIG_11R
                else if ((*akm_type == AssocAgentAuth_FastBss_Skip) && (ptr[3] == 4))
                {
                    break;
                }
#endif
                if ((*akm_type == AssocAgentAuth_Open) && (ptr[3] == 6))
                {
                    break;
                }
                else if ((*akm_type == AssocAgentAuth_Open) && (ptr[3] == 2))
                {
                    break;
                }
            }
            ptr += AKM_SUITE_LEN;
        }
        if (i == akm_suite_count)
        {
            akm_type_selected = 0; // not found
        }
        else
        {
            akm_type_selected = akm_preference[ptr[3]];
            akm_type_id       = ptr[3];
        }
    }
    if (akm_type_selected == 0)
    {
        PRINTM(MERROR, "RSNE: AKM Suite not found for authtype %d\n", *akm_type);
        return MLAN_STATUS_FAILURE;
    }
    /* Process RSNCAP */
    ptr = ptlv_rsn_ie->rsn_ie + sizeof(t_u16) + 4 * sizeof(t_u8) + sizeof(t_u16) +
          pairwise_cipher_count * PAIRWISE_CIPHER_SUITE_LEN + sizeof(t_u16) + akm_suite_count * AKM_SUITE_LEN;
    prsn_cap = (t_u16 *)ptr;

    ap_mfpc = ((*prsn_cap & (0x1 << MFPC_BIT)) == (0x1 << MFPC_BIT));
    ap_mfpr = ((*prsn_cap & (0x1 << MFPR_BIT)) == (0x1 << MFPR_BIT));

    if ((!ap_mfpc && !ap_mfpr && pmpriv->pmfcfg.mfpr) || ((!ap_mfpc) && ap_mfpr) ||
        (ap_mfpc && ap_mfpr && (!pmpriv->pmfcfg.mfpc)))
    {
        PRINTM(MERROR, "RSNE: Mismatch in PMF config of STA and AP, can't associate to AP\n");
        return MLAN_STATUS_FAILURE;
    }
    *prsn_cap |= PMF_MASK;
    *prsn_cap &= pmf_mask;

    // PMKID
    ptr += sizeof(t_u16);
    if (end_ptr >= (ptr + 2))
    {
        pmkid_count = wlan_le16_to_cpu(*(t_u16 *)ptr);
        ptr += sizeof(t_u16);
        pmkid_list_ptr = ptr;
        ptr += pmkid_count * PMKID_LEN;
    }
    // Group Mgmt Cipher Suite
    if ((end_ptr >= (ptr + 4)) && (pmf_mask & PMF_MASK))
    {
        group_mgmt_cipher_suite_ptr = ptr;
    }
    /* Compose new RSNE */
    // pairwiase
    ptr           = pairwise_cipher_count_ptr;
    *(t_u16 *)ptr = wlan_cpu_to_le16(1);
    ptr += sizeof(t_u16);
    oui[3]        = cipher_selected_id;
    *(t_u32 *)ptr = *(t_u32 *)oui;
    ptr += PAIRWISE_CIPHER_SUITE_LEN;
    // akm
    *(t_u16 *)ptr = wlan_cpu_to_le16(1);
    ptr += sizeof(t_u16);
    oui[3]        = akm_type_id;
    *(t_u32 *)ptr = *(t_u32 *)oui;
    ptr += AKM_SUITE_LEN;
    // RSNCAP
    *(t_u16 *)ptr = wlan_cpu_to_le16(*prsn_cap);
    ptr += sizeof(t_u16);
    // PMKID list
    if (pmkid_list_ptr || group_mgmt_cipher_suite_ptr)
    {
        // Add PMKID
        *(t_u16 *)ptr = wlan_cpu_to_le16(pmkid_count);
        ptr += sizeof(t_u16);
        if (pmkid_count)
        {
            (void)memmove(ptr, (t_u8 *)pmkid_list_ptr, (end_ptr - ptr));
            ptr += pmkid_count * PMKID_LEN;
        }
        if (group_mgmt_cipher_suite_ptr)
        {
            // Add Group Mgmt Cipher Suite
            (void)memmove(ptr, (t_u8 *)group_mgmt_cipher_suite_ptr, (end_ptr - ptr));
            ptr += GROUP_MGMT_CIPHER_SUITE_LEN;
        }
    }
    *rsn_ie_len = ptr - ptlv_rsn_ie->rsn_ie;
    return ret;
}

#if CONFIG_11R
/**
 *  @brief This function is to find FT AKM in RSN.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *
 *  @param rsn_ie       A pointer to rsn_ie
 *
 *  @return             1 when FT AKM is in RSN, otherwise 0
 */
t_u8 wlan_ft_akm_is_used(mlan_private *pmpriv, t_u8 *rsn_ie)
{
    t_u8 *temp;
    t_u16 count;
    t_u16 pairwise_cipher_count  = 0;
    t_u16 akm_suite_count        = 0;
    t_u8 found                   = 0;
    t_u8 rsn_ft_1x_oui[4]        = {0x00, 0x0f, 0xac, 0x03};
    t_u8 rsn_ft_psk_oui[4]       = {0x00, 0x0f, 0xac, 0x04};
    t_u8 rsn_ft_sae_oui[4]       = {0x00, 0x0f, 0xac, 0x09};
    t_u8 rsn_ft_1x_sha384_oui[4] = {0x00, 0x0f, 0xac, 0x0d};

    ENTER();

    if (rsn_ie == MNULL)
    {
        goto done;
    }
    else
    {
        /* Do nothing */
    }

    if (rsn_ie[0] != (t_u8)RSN_IE)
    {
        goto done;
    }
    else
    {
        /* Do nothing */
    }

    /*  4 bytes header + 2 bytes version + 4 bytes group_cipher_suite +
     *  2 bytes pairwise_cipher_count + pairwise_cipher_count *
     * PAIRWISE_CIPHER_SUITE_LEN (4) + 2 bytes akm_suite_count +
     * akm_suite_count * AKM_SUITE_LEN (4)
     */
    count                 = *(t_u16 *)(void *)(rsn_ie + 4 + 2 + 4 * (int)sizeof(t_u8));
    pairwise_cipher_count = wlan_le16_to_cpu(count);
    count                 = *(t_u16 *)(void *)(rsn_ie + 4 + 2 + 4 * (int)sizeof(t_u8) + (int)sizeof(t_u16) +
                               (int)pairwise_cipher_count * 4);
    akm_suite_count       = (t_u16)wlan_le16_to_cpu(count);
    temp                  = (t_u8 *)(rsn_ie + 4 + sizeof(t_u16) + 4 * (int)sizeof(t_u8) + (int)sizeof(t_u16) +
                    (int)pairwise_cipher_count * 4 + sizeof(t_u16));

    while (akm_suite_count > 0U)
    {
        if (__memcmp(pmpriv->adapter, temp, rsn_ft_1x_oui, sizeof(rsn_ft_1x_oui)) == 0 ||
            __memcmp(pmpriv->adapter, temp, rsn_ft_psk_oui, sizeof(rsn_ft_psk_oui)) == 0 ||
            __memcmp(pmpriv->adapter, temp, rsn_ft_sae_oui, sizeof(rsn_ft_sae_oui)) == 0 ||
            __memcmp(pmpriv->adapter, temp, rsn_ft_1x_sha384_oui, sizeof(rsn_ft_1x_sha384_oui)) == 0)
        {
            found = 1;
            break;
        }
        temp += 4;
        akm_suite_count--;
    }

done:
    LEAVE();
    return found;
}

/**
 *  @brief Append  IE as a pass through TLV to a TLV buffer.
 *
 *  This routine appends IE as a pass through TLV type to the request.
 *
 *  @param priv     A pointer to mlan_private structure
 *  @param ie       A pointer to IE buffer
 *  @param ppbuffer pointer to command buffer pointer
 *
 *  @return         bytes added to the buffer
 */
static int wlan_cmd_append_pass_through_ie(mlan_private *priv, IEEEtypes_Generic_t *ie, t_u8 **ppbuffer)
{
    int ret_len = 0;
    MrvlIEtypesHeader_t ie_header;

    ENTER();

    /* Null Checks */
    if (ppbuffer == MNULL)
    {
        LEAVE();
        return 0;
    }
    if (*ppbuffer == MNULL)
    {
        LEAVE();
        return 0;
    }
    if (ie->ieee_hdr.len)
    {
        PRINTM(MINFO, "append generic IE %d to %p\n", ie->ieee_hdr.len, *ppbuffer);

        /* Wrap the generic IE buffer with a pass through TLV type */
        ie_header.type = wlan_cpu_to_le16(TLV_TYPE_PASSTHROUGH);
        ie_header.len  = wlan_cpu_to_le16(ie->ieee_hdr.len + sizeof(IEEEtypes_Header_t));
        __memcpy(priv->adapter, *ppbuffer, &ie_header, sizeof(ie_header));

        /* Increment the return size and the return buffer pointer param
         */
        *ppbuffer += sizeof(ie_header);
        ret_len += sizeof(ie_header);

        /* Copy the generic IE buffer to the output buffer, advance
         * pointer */
        __memcpy(priv->adapter, *ppbuffer, ie, ie->ieee_hdr.len + sizeof(IEEEtypes_Header_t));

        /* Increment the return size and the return buffer pointer param
         */
        *ppbuffer += ie->ieee_hdr.len + sizeof(IEEEtypes_Header_t);
        ret_len += ie->ieee_hdr.len + sizeof(IEEEtypes_Header_t);
    }
    /* return the length appended to the buffer */
    LEAVE();
    return ret_len;
}
#endif

/********************************************************
                Global Functions
********************************************************/
/**
 *  @brief This function prepares command of association.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf    A pointer cast of BSSDescriptor_t from the
 *                        scan table to assoc
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_cmd_802_11_associate(IN mlan_private *pmpriv, IN HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf)
{
    mlan_status ret                    = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter            = pmpriv->adapter;
    HostCmd_DS_802_11_ASSOCIATE *passo = &cmd->params.associate;
    BSSDescriptor_t *pbss_desc;
    MrvlIEtypes_SsIdParamSet_t *pssid_tlv;
    MrvlIEtypes_PhyParamSet_t *pphy_tlv;
    MrvlIEtypes_SsParamSet_t *pss_tlv;
    MrvlIEtypes_RatesParamSet_t *prates_tlv;
    MrvlIEtypes_AuthType_t *pauth_tlv      = MNULL;
    MrvlIEtypes_RsnParamSet_t *prsn_ie_tlv = MNULL;
    MrvlIEtypes_ChanListParamSet_t *pchan_tlv;
    WLAN_802_11_RATES rates = {0x00};
    t_u32 rates_size;
    t_u16 tmp_cap;
    t_u8 *pos, *auth_pos = NULL;
    t_u8 akm_type = 0;
#if CONFIG_11R
    t_u8 ft_akm = 0;
#endif
    MrvlIEtypes_PrevBssid_t *prev_bssid_tlv = MNULL;
    t_u8 zero_mac[MLAN_MAC_ADDR_LENGTH]     = {0};
#if CONFIG_DRIVER_MBO
    t_u8 oper_class = 1;
#endif

#if CONFIG_HOST_MLME
    MrvlIEtypes_HostMlme_t *host_mlme_tlv = MNULL;
#endif

    ENTER();

    (void)__memset(pmadapter, &rates, 0x00, sizeof(WLAN_802_11_RATES));
    pbss_desc = (BSSDescriptor_t *)pdata_buf;
    pos       = (t_u8 *)&cmd->params;

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_ASSOCIATE);

    /* Save so we know which BSS Desc to use in the response handler */
    pmpriv->pattempted_bss_desc = pbss_desc;
#if CONFIG_HOST_MLME
    (void)__memcpy(pmadapter, &pmpriv->curr_bss_params.attemp_bssid, pbss_desc->mac_address, MLAN_MAC_ADDR_LENGTH);
    pmpriv->assoc_req_size = 0;
#endif

    (void)__memcpy(pmadapter, passo->peer_sta_addr, pbss_desc->mac_address, sizeof(passo->peer_sta_addr));

    /* fixme: Look at this value carefully later. The listen interval is given to AP during
     * this assoc. The listen_interval set later during IEEE PS should not (?) exceed this
     */
    /* Set the listen interval */
    passo->listen_interval = wlan_cpu_to_le16(pmpriv->listen_interval);
    /* Set the beacon period */
    passo->beacon_period = wlan_cpu_to_le16(pbss_desc->beacon_period);

    pos                    = (t_u8 *)cmd + S_DS_GEN + sizeof(HostCmd_DS_802_11_ASSOCIATE);
    pssid_tlv              = (MrvlIEtypes_SsIdParamSet_t *)(void *)pos;
    pssid_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_SSID);
    pssid_tlv->header.len  = (t_u16)pbss_desc->ssid.ssid_len;
    (void)__memcpy(pmadapter, pssid_tlv->ssid, pbss_desc->ssid.ssid, pssid_tlv->header.len);
    pos += sizeof(pssid_tlv->header) + pssid_tlv->header.len;
    pssid_tlv->header.len = wlan_cpu_to_le16(pssid_tlv->header.len);

    pphy_tlv              = (MrvlIEtypes_PhyParamSet_t *)(void *)pos;
    pphy_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_PHY_DS);
    pphy_tlv->header.len  = sizeof(pphy_tlv->fh_ds.ds_param_set);
    (void)__memcpy(pmadapter, &pphy_tlv->fh_ds.ds_param_set, &pbss_desc->phy_param_set.ds_param_set.current_chan,
                   sizeof(pphy_tlv->fh_ds.ds_param_set));
    pos += sizeof(pphy_tlv->header) + pphy_tlv->header.len;
    pphy_tlv->header.len = wlan_cpu_to_le16(pphy_tlv->header.len);

    pss_tlv              = (MrvlIEtypes_SsParamSet_t *)(void *)pos;
    pss_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_CF);
    pss_tlv->header.len  = sizeof(pss_tlv->cf_ibss.cf_param_set);
    pos += sizeof(pss_tlv->header) + pss_tlv->header.len;
    pss_tlv->header.len = wlan_cpu_to_le16(pss_tlv->header.len);

    /* Get the common rates supported between the driver and the BSS Desc */
    if (wlan_setup_rates_from_bssdesc(pmpriv, pbss_desc, rates, &rates_size) != MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    /* Save the data rates into Current BSS state structure */
    pmpriv->curr_bss_params.num_of_rates = rates_size;
    (void)__memcpy(pmadapter, &pmpriv->curr_bss_params.data_rates, rates, rates_size);

    /* Setup the Rates TLV in the association command */
    prates_tlv              = (MrvlIEtypes_RatesParamSet_t *)(void *)pos;
    prates_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_RATES);
    prates_tlv->header.len  = wlan_cpu_to_le16((t_u16)rates_size);
    (void)__memcpy(pmadapter, prates_tlv->rates, rates, rates_size);
    pos += sizeof(prates_tlv->header) + rates_size;
    PRINTM(MINFO, "ASSOC_CMD: Rates size = %d\n", rates_size);

    /* Add the Authentication type to be used for Auth frames if needed */
    if ((pmpriv->sec_info.authentication_mode != MLAN_AUTH_MODE_AUTO)
#if CONFIG_DRIVER_OWE
        || (
#if CONFIG_HOST_MLME
            !pmpriv->curr_bss_params.host_mlme &&
#endif
            (pbss_desc->owe_transition_mode == OWE_TRANS_MODE_OWE))
#endif
    )
    {
        pauth_tlv              = (MrvlIEtypes_AuthType_t *)(void *)pos;
        pauth_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AUTH_TYPE);
        pauth_tlv->header.len  = sizeof(pauth_tlv->auth_type);
        if (pmpriv->sec_info.authentication_mode == MLAN_AUTH_MODE_SAE)
        {
            pauth_tlv->auth_type = wlan_cpu_to_le16((t_u16)AssocAgentAuth_Wpa3Sae);
        }
#if CONFIG_11R
        else if (pmpriv->sec_info.authentication_mode == MLAN_AUTH_MODE_FT)
        {
#if CONFIG_HOST_MLME
            if (pmpriv->curr_bss_params.host_mlme)
            {
                pauth_tlv->auth_type = wlan_cpu_to_le16(AssocAgentAuth_FastBss_Skip);
            }
            else
#endif
            {
                pauth_tlv->auth_type = wlan_cpu_to_le16(AssocAgentAuth_FastBss);
            }
        }
#endif
#if CONFIG_DRIVER_OWE
        else if (
#if CONFIG_HOST_MLME
            !pmpriv->curr_bss_params.host_mlme &&
#endif
            ((pbss_desc->owe_transition_mode == OWE_TRANS_MODE_OWE) ||
             (pmpriv->sec_info.authentication_mode == MLAN_AUTH_MODE_OWE)))
        {
            pauth_tlv->auth_type = wlan_cpu_to_le16((t_u16)AssocAgentAuth_Owe);
        }
#endif
        else if (pmpriv->sec_info.wep_status == Wlan802_11WEPEnabled)
        {
            pauth_tlv->auth_type = wlan_cpu_to_le16((t_u16)pmpriv->sec_info.authentication_mode);
        }
        else
        {
            pauth_tlv->auth_type = wlan_cpu_to_le16(MLAN_AUTH_MODE_OPEN);
        }
        pos += sizeof(pauth_tlv->header) + pauth_tlv->header.len;
        pauth_tlv->header.len = wlan_cpu_to_le16(pauth_tlv->header.len);
    }

    if (IS_SUPPORT_MULTI_BANDS(pmadapter) && (pbss_desc->bss_band & pmpriv->config_bands) &&
        !(ISSUPP_11NENABLED(pmadapter->fw_cap_info) && (!pbss_desc->disable_11n) &&
          (pmpriv->config_bands & BAND_GN || pmpriv->config_bands & BAND_AN) && (pbss_desc->pht_cap != MNULL)))
    {
        /* Append a channel TLV for the channel the attempted AP was found on */
        pchan_tlv              = (MrvlIEtypes_ChanListParamSet_t *)(void *)pos;
        pchan_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_CHANLIST);
        pchan_tlv->header.len  = wlan_cpu_to_le16(sizeof(ChanScanParamSet_t));

        (void)__memset(pmadapter, pchan_tlv->chan_scan_param, 0x00, sizeof(ChanScanParamSet_t));
        pchan_tlv->chan_scan_param[0].chan_number = (pbss_desc->phy_param_set.ds_param_set.current_chan);
        PRINTM(MINFO, "Assoc: TLV Chan = %d\n", pchan_tlv->chan_scan_param[0].chan_number);

        pchan_tlv->chan_scan_param[0].radio_type = wlan_band_to_radio_type((t_u8)pbss_desc->bss_band);

        PRINTM(MINFO, "Assoc: TLV Band = %d\n", pchan_tlv->chan_scan_param[0].radio_type);
        pos += sizeof(pchan_tlv->header) + sizeof(ChanScanParamSet_t);
    }

#if CONFIG_WPA_SUPP
#if CONFIG_WPA_SUPP_WPS
    if (pmpriv->wps.session_enable == MFALSE)
    {
#endif /* CONFIG_WPA_SUPP_WPS */
#elif (CONFIG_WPS2)
    if (wlan_get_prov_session() != PROV_WPS_SESSION_ATTEMPT)
    {
#endif
        /* fixme: The above 'if' is used instead of below 'if' for now since
           WPS module is mlan integrated yet. Fix after it is done.
        */
        /* if (!pmpriv->wps.session_enable) { */
        if ((pmpriv->sec_info.wpa_enabled || pmpriv->sec_info.wpa2_enabled)
#if CONFIG_DRIVER_OWE
            || (pbss_desc->owe_transition_mode == OWE_TRANS_MODE_OWE) ||
            (pmpriv->sec_info.authentication_mode == MLAN_AUTH_MODE_OWE)
#endif
        )
        {
            if ((pauth_tlv == MNULL) && (pmpriv->sec_info.authentication_mode == MLAN_AUTH_MODE_AUTO))
            {
                auth_pos = pos;
                pos += sizeof(MrvlIEtypes_AuthType_t);
            }

            prsn_ie_tlv              = (MrvlIEtypes_RsnParamSet_t *)(void *)pos;
            prsn_ie_tlv->header.type = (t_u16)pmpriv->wpa_ie[0]; /* WPA_IE
                                                                    or
                                                                    RSN_IE
                                                                  */
            prsn_ie_tlv->header.type = prsn_ie_tlv->header.type & 0x00FF;
            prsn_ie_tlv->header.type = wlan_cpu_to_le16(prsn_ie_tlv->header.type);
            prsn_ie_tlv->header.len  = (t_u16)pmpriv->wpa_ie[1];
            prsn_ie_tlv->header.len  = prsn_ie_tlv->header.len & 0x00FF;
            if (prsn_ie_tlv->header.len <= (sizeof(pmpriv->wpa_ie) - 2))
            {
                (void)__memcpy(pmadapter, prsn_ie_tlv->rsn_ie, &pmpriv->wpa_ie[2], prsn_ie_tlv->header.len);
                if (pmpriv->sec_info.wpa2_enabled
#if CONFIG_WPA_SUPP_DPP
                    && pmpriv->is_dpp_connect == MFALSE
#endif
                )
                {
                    akm_type             = pauth_tlv ? wlan_le16_to_cpu(pauth_tlv->auth_type) : AssocAgentAuth_Auto;
                    t_u16 rsn_ie_tlv_len = prsn_ie_tlv->header.len;
                    ret = (mlan_status)wlan_update_rsn_ie(pmpriv, prsn_ie_tlv, &rsn_ie_tlv_len, &akm_type);
                    prsn_ie_tlv->header.len = rsn_ie_tlv_len;
#if CONFIG_11R
                    /** parse rsn ie to find whether ft akm is used*/
                    ft_akm = wlan_ft_akm_is_used(pmpriv, (t_u8 *)prsn_ie_tlv);
#endif
                }
            }
            else
            {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            HEXDUMP("ASSOC_CMD: RSN IE", (t_u8 *)prsn_ie_tlv, sizeof(prsn_ie_tlv->header) + prsn_ie_tlv->header.len);
            pos += sizeof(prsn_ie_tlv->header) + prsn_ie_tlv->header.len;
            prsn_ie_tlv->header.len = wlan_cpu_to_le16(prsn_ie_tlv->header.len);

#if CONFIG_11R
            if ((akm_type == AssocAgentAuth_FastBss) && (pmpriv->sec_info.is_ft == false))
            {
                akm_type = AssocAgentAuth_Open;
            }
            if ((akm_type == AssocAgentAuth_FastBss_SAE) && (pmpriv->sec_info.is_ft == false))
            {
                akm_type = AssocAgentAuth_Wpa3Sae;
            }
#endif
#if CONFIG_DRIVER_OWE
            if ((akm_type == AssocAgentAuth_Owe) && (pmpriv->curr_bss_params.host_mlme))
            {
                akm_type = AssocAgentAuth_Open;
            }
#endif
            if ((pauth_tlv == MNULL) && (pmpriv->sec_info.authentication_mode == MLAN_AUTH_MODE_AUTO))
            {
                pauth_tlv              = (MrvlIEtypes_AuthType_t *)auth_pos;
                pauth_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AUTH_TYPE);
                pauth_tlv->header.len  = sizeof(pauth_tlv->auth_type);
                pauth_tlv->auth_type   = wlan_cpu_to_le16(akm_type);

                pauth_tlv->header.len = wlan_cpu_to_le16(pauth_tlv->header.len);
            }
        }
#if CONFIG_WPA_SUPP
#if CONFIG_WPA_SUPP_WPS
    }
#endif /* CONFIG_WPA_SUPP_WPS */
#elif (CONFIG_WPS2)
    }
#endif

    if ((pauth_tlv != MNULL) && (pauth_tlv->auth_type == wlan_cpu_to_le16(AssocAgentAuth_Wpa3Sae)))
    {
        if ((pbss_desc->prsnx_ie != MNULL) && pbss_desc->prsnx_ie->ieee_hdr.len &&
            (pbss_desc->prsnx_ie->data[0] & (0x1 << SAE_H2E_BIT)))
        {
            MrvlIEtypes_SAE_PWE_Mode_t *psae_pwe_mode_tlv;

            /* Setup the sae mode TLV in the association command */
            psae_pwe_mode_tlv              = (MrvlIEtypes_SAE_PWE_Mode_t *)(void *)pos;
            psae_pwe_mode_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_WPA3_SAE_PWE_DERIVATION_MODE);
            psae_pwe_mode_tlv->header.len  = sizeof(psae_pwe_mode_tlv->pwe);
            psae_pwe_mode_tlv->pwe[0]      = pbss_desc->prsnx_ie->data[0];
            pos += sizeof(psae_pwe_mode_tlv->header) + sizeof(psae_pwe_mode_tlv->pwe);
        }
    }

    if (ISSUPP_11NENABLED(pmadapter->fw_cap_info) && (!pbss_desc->disable_11n) &&
        (pmpriv->config_bands & BAND_GN || pmpriv->config_bands & BAND_AN) && wmsdk_is_11N_enabled() &&
        (!pmpriv->sec_info.is_wpa_tkip))
    {
        (void)wlan_cmd_append_11n_tlv(pmpriv, pbss_desc, &pos);
    }
    else if (pmpriv->hotspot_cfg & HOTSPOT_ENABLED)
    {
        wlan_add_ext_capa_info_ie(pmpriv, pbss_desc, &pos);
    }
    else
    {
        /* Do nothing */
    }

#if CONFIG_11AC
    if (ISSUPP_11ACENABLED(pmadapter->fw_cap_info) && (!pbss_desc->disable_11n) &&
        wlan_11ac_bandconfig_allowed(pmpriv, pbss_desc->bss_band))
    {
        (void)wlan_cmd_append_11ac_tlv(pmpriv, pbss_desc, &pos);
    }
#endif

#if CONFIG_11AX
    if ((IS_FW_SUPPORT_11AX(pmadapter)) && (!pbss_desc->disable_11n) &&
        wlan_11ax_bandconfig_allowed(pmpriv, pbss_desc->bss_band))
        wlan_cmd_append_11ax_tlv(pmpriv, pbss_desc, &pos);
#endif

    (void)wlan_wmm_process_association_req(pmpriv, &pos, &pbss_desc->wmm_ie, pbss_desc->pht_cap);

#if CONFIG_11R
#if CONFIG_WPA_SUPP
    if (pmpriv->md_ie_len != 0U)
#else
    if (ft_akm == 1U)
#endif
    {
        wlan_cmd_append_pass_through_ie(pmpriv, (IEEEtypes_Generic_t *)(void *)pmpriv->md_ie, &pos);
    }
    else
    {
        /* Do nothing */
    }
#endif

#if CONFIG_HOST_MLME
    if (pmpriv->curr_bss_params.host_mlme)
    {
        host_mlme_tlv              = (MrvlIEtypes_HostMlme_t *)pos;
        host_mlme_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_HOST_MLME);
        host_mlme_tlv->header.len  = wlan_cpu_to_le16(sizeof(host_mlme_tlv->host_mlme));
        host_mlme_tlv->host_mlme   = MTRUE;
        pos += sizeof(host_mlme_tlv->header) + host_mlme_tlv->header.len;
    }
#endif
    if (memcmp(&pmpriv->curr_bss_params.prev_bssid, zero_mac, MLAN_MAC_ADDR_LENGTH))
    {
        prev_bssid_tlv              = (MrvlIEtypes_PrevBssid_t *)pos;
        prev_bssid_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_PREV_BSSID);
        prev_bssid_tlv->header.len  = wlan_cpu_to_le16(MLAN_MAC_ADDR_LENGTH);
        __memcpy(pmadapter, prev_bssid_tlv->prev_bssid, &pmpriv->curr_bss_params.prev_bssid, MLAN_MAC_ADDR_LENGTH);
        PRINTM(MCMND, "ASSOCIATE: PREV_BSSID = " MACSTR "\n", MAC2STR(pmpriv->curr_bss_params.prev_bssid));
        pos += sizeof(prev_bssid_tlv->header) + MLAN_MAC_ADDR_LENGTH;
    }

#if CONFIG_DRIVER_MBO
    wlan_get_curr_global_oper_class(pmpriv, pbss_desc->phy_param_set.ds_param_set.current_chan, BW_20MHZ, &oper_class);
    wlan_add_supported_oper_class_ie(pmpriv, &pos, oper_class);
#endif

    /* fixme: Currently not required */

    wlan_cmd_append_generic_ie(pmpriv, &pos);


    cmd->size = wlan_cpu_to_le16((t_u16)(pos - (t_u8 *)passo) + S_DS_GEN);

    /* Set the Capability info at last */
    (void)__memcpy(pmadapter, &tmp_cap, &pbss_desc->cap_info, sizeof(passo->cap_info));

    if (pmpriv->config_bands == BAND_B)
    {
        SHORT_SLOT_TIME_DISABLED(tmp_cap);
    }

    /* set SpectrumMgmt(BIT8) and RadioMeasurement(BIT12) if 11K is enabled
     */

#if CONFIG_11K
        if (pmpriv->enable_host_11k == (t_u8)1U)
    {
        SPECTRUM_MGMT_ENABLED(tmp_cap);
        RADIO_MEASUREMENT_ENABLED(tmp_cap);
    }
    else
#endif
    {
        RADIO_MEASUREMENT_DISABLED(tmp_cap);
    }

    tmp_cap &= CAPINFO_MASK;
    PRINTM(MINFO, "ASSOC_CMD: tmp_cap=%4X CAPINFO_MASK=%4lX\n", tmp_cap, CAPINFO_MASK);
    tmp_cap = wlan_cpu_to_le16(tmp_cap);
    (void)__memcpy(pmadapter, &passo->cap_info, &tmp_cap, sizeof(passo->cap_info));

done:

    LEAVE();
    return ret;
}

/**
 *  @brief Association firmware command response handler
 *
 *   The response buffer for the association command has the following
 *      memory layout.
 *
 *   For cases where an association response was not received (indicated
 *      by the CapInfo and AId field):
 *
 *     .------------------------------------------------------------.
 *     |  Header(4 * sizeof(t_u16)):  Standard command response hdr |
 *     .------------------------------------------------------------.
 *     |  cap_info/Error Return(t_u16):                             |
 *     |           0xFFFF(-1): Internal error for association       |
 *     |           0xFFFE(-2): Authentication unhandled message     |
 *     |           0xFFFD(-3): Authentication refused               |
 *     |           0xFFFC(-4): Timeout waiting for AP response      |
 *     |           0xFFFB(-5): Internal error for authentication    |
 *     .------------------------------------------------------------.
 *     |  status_code(t_u16):                                       |
 *     |        If cap_info is -1:                                  |
 *     |           An internal firmware failure prevented the       |
 *     |           command from being processed. The status code    |
 *     |           is 6 if associate response parameter invlaid,    |
 *     |           1 otherwise.                                     |
 *     |                                                            |
 *     |        If cap_info is -2:                                  |
 *     |           An authentication frame was received but was     |
 *     |           not handled by the firmware. IEEE Status code    |
 *     |           for the failure is returned.                     |
 *     |                                                            |
 *     |        If cap_info is -3:                                  |
 *     |           An authentication frame was received and the     |
 *     |           status_code is the IEEE Status reported in the   |
 *     |           response.                                        |
 *     |                                                            |
 *     |        If cap_info is -4:                                  |
 *     |           (1) Association response timeout                 |
 *     |           (2) Authentication response timeout              |
 *     |                                                            |
 *     |        If cap_info is -5:                                  |
 *     |           An internal firmware failure prevented the       |
 *     |           command from being processed. The status code    |
 *     |           is 6 if authentication parameter invlaid,        |
 *     |           1 otherwise.                                     |
 *     .------------------------------------------------------------.
 *     |  a_id(t_u16): 0xFFFF                                       |
 *     .------------------------------------------------------------.
 *
 *
 *   For cases where an association response was received, the IEEE
 *     standard association response frame is returned:
 *
 *     .------------------------------------------------------------.
 *     |  Header(4 * sizeof(t_u16)):  Standard command response hdr |
 *     .------------------------------------------------------------.
 *     |  cap_info(t_u16): IEEE Capability                          |
 *     .------------------------------------------------------------.
 *     |  status_code(t_u16): IEEE Status Code                      |
 *     .------------------------------------------------------------.
 *     |  a_id(t_u16): IEEE Association ID                          |
 *     .------------------------------------------------------------.
 *     |  IEEE IEs(variable): Any received IEs comprising the       |
 *     |                      remaining portion of a received       |
 *     |                      association response frame.           |
 *     .------------------------------------------------------------.
 *
 *  For simplistic handling, the status_code field can be used to determine
 *    an association success (0) or failure (non-zero).
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ret_802_11_associate(IN mlan_private *pmpriv, IN HostCmd_DS_COMMAND *resp, IN t_void *pioctl_buf)
{
    mlan_status ret            = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req *pioctl_req = (mlan_ioctl_req *)pioctl_buf;
    IEEEtypes_AssocRsp_t *passoc_rsp;
    BSSDescriptor_t *pbss_desc;
    /* t_u8 enable_data = MTRUE; */
    /* t_u8 event_buf[100]; */
    /* mlan_event *pevent = (mlan_event *) event_buf; */
    t_u8 cur_mac[MLAN_MAC_ADDR_LENGTH] = {0x0};
#if CONFIG_WMM
    t_u8 media_connected = pmpriv->media_connected;
#endif
    /* mlan_adapter *pmadapter = pmpriv->adapter; */

#if CONFIG_HOST_MLME
    IEEEtypes_MgmtHdr_t *hdr;
    t_u16 assoc_rsp_size = 0;
#endif

    ENTER();

#if CONFIG_HOST_MLME
    if (pmpriv->curr_bss_params.host_mlme)
    {
        hdr = (IEEEtypes_MgmtHdr_t *)&resp->params;
        assoc_rsp_size = resp->size - S_DS_GEN;
        if ((assoc_rsp_size >= (sizeof(IEEEtypes_MgmtHdr_t)
             + sizeof(IEEEtypes_AssocRsp_t))) && !__memcmp(pmpriv->adapter, hdr->BssId,
             pmpriv->pattempted_bss_desc->mac_address, MLAN_MAC_ADDR_LENGTH))
        {
            passoc_rsp = (IEEEtypes_AssocRsp_t *)((t_u8 *)(&resp->params) + sizeof(IEEEtypes_MgmtHdr_t));
        }
        else
        {
            passoc_rsp = (IEEEtypes_AssocRsp_t *)&resp->params;
        }
    }
    else
#endif

        passoc_rsp = (IEEEtypes_AssocRsp_t *)(void *)&resp->params;
    passoc_rsp->status_code = wlan_le16_to_cpu(passoc_rsp->status_code);
    if (pmpriv->media_connected == MTRUE)
    {
        (void)__memcpy(pmpriv->adapter, cur_mac, pmpriv->curr_bss_params.bss_descriptor.mac_address,
                       MLAN_MAC_ADDR_LENGTH);
    }

    /* fixme: Enable this when req. We may not need to save the
       resp. buffer at all */

    /* fixme: This is not needed as of now. */
    if (passoc_rsp->status_code)
    {
        if (pmpriv->media_connected == MTRUE)
        {
            if (pmpriv->port_ctrl_mode == MTRUE)
                pmpriv->port_open = pmpriv->prior_port_status;
            if (!__memcmp(pmpriv->adapter, cur_mac, pmpriv->pattempted_bss_desc->mac_address, MLAN_MAC_ADDR_LENGTH))
                wlan_reset_connect_state(pmpriv, MTRUE);
        }

        wifi_d(
            "ASSOC_RESP: Association Failed, "
            "status code = %d, a_id = 0x%x",
            wlan_le16_to_cpu(passoc_rsp->status_code), wlan_le16_to_cpu(passoc_rsp->a_id));

        return MLAN_STATUS_FAILURE;
    }

    /* Send a Media Connected event, according to the Spec */
    pmpriv->media_connected = MTRUE;

#if CONFIG_WMM_UAPSD
    pmpriv->adapter->pps_uapsd_mode = MFALSE;
    pmpriv->adapter->tx_lock_flag   = MFALSE;
#endif

    /* Set the attempted BSSID Index to current */
    pbss_desc = pmpriv->pattempted_bss_desc;

    PRINTM(MINFO, "ASSOC_RESP: %s\n", pbss_desc->ssid.ssid);

    /* Make a copy of current BSSID descriptor */
    (void)__memcpy(pmpriv->adapter, &pmpriv->curr_bss_params.bss_descriptor, pbss_desc, sizeof(BSSDescriptor_t));

    /* Update curr_bss_params */
    pmpriv->curr_bss_params.bss_descriptor.channel = pbss_desc->phy_param_set.ds_param_set.current_chan;

    pmpriv->curr_bss_params.band = (t_u8)pbss_desc->bss_band;


    if (pbss_desc->wmm_ie.vend_hdr.element_id == WMM_IE)
    {
        pmpriv->curr_bss_params.wmm_enabled = MTRUE;
    }
    else
    {
        pmpriv->curr_bss_params.wmm_enabled = MFALSE;
    }

    if ((pmpriv->wmm_required ||
         ((pbss_desc->pht_cap != MNULL) && (pbss_desc->pht_cap->ieee_hdr.element_id == HT_CAPABILITY))) &&
        pmpriv->curr_bss_params.wmm_enabled)
    {
        pmpriv->wmm_enabled = MTRUE;
    }
    else
    {
        pmpriv->wmm_enabled = MFALSE;
    }

    /* fixme: Enable if req */
#if CONFIG_WMM_UAPSD
    pmpriv->curr_bss_params.wmm_uapsd_enabled = MFALSE;

    if (pmpriv->wmm_enabled == MTRUE)
    {
        pmpriv->curr_bss_params.wmm_uapsd_enabled = pbss_desc->wmm_ie.qos_info.qos_uapsd;
    }
#endif
    if (pmpriv->sec_info.wpa_enabled || pmpriv->sec_info.wpa2_enabled)
    {
        pmpriv->wpa_is_gtk_set = MFALSE;
    }


    /* Reset SNR/NF/RSSI values */
    pmpriv->data_rssi_last = 0;
    pmpriv->data_nf_last   = 0;
    pmpriv->data_rssi_avg  = 0;
    pmpriv->data_nf_avg    = 0;
    pmpriv->bcn_rssi_last  = 0;
    pmpriv->bcn_nf_last    = 0;
    pmpriv->bcn_rssi_avg   = 0;
    pmpriv->bcn_nf_avg     = 0;
    pmpriv->rxpd_rate      = 0;
#ifdef SD8801
    pmpriv->rxpd_htinfo = 0;
#else
    pmpriv->rxpd_rate_info = 0;
#endif
    if (pbss_desc->pht_cap != MNULL)
    {
        if ((GETHT_MAXAMSDU(pbss_desc->pht_cap->ht_cap.ht_cap_info)) != 0U)
        {
            pmpriv->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
        }
        else
        {
            pmpriv->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
        }
    }


#if CONFIG_WMM
    /* Add the ra_list here for infra mode as there will be only 1 ra always */
    if (media_connected == MTRUE)
    {
        if (0 == wlan_ralist_update_enh(pmpriv, cur_mac, pmpriv->curr_bss_params.bss_descriptor.mac_address))
        {
            wlan_ralist_add_enh(pmpriv, pmpriv->curr_bss_params.bss_descriptor.mac_address);
        }

        wlan_11n_cleanup_reorder_tbl(pmpriv);
        wlan_11n_deleteall_txbastream_tbl(pmpriv);
    }
    else
    {
        wlan_ralist_add_enh(pmpriv, pmpriv->curr_bss_params.bss_descriptor.mac_address);
    }
#endif


    if (!pmpriv->sec_info.wpa_enabled && !pmpriv->sec_info.wpa2_enabled && !pmpriv->sec_info.ewpa_enabled &&
        !pmpriv->sec_info.wapi_enabled)
    {
        /* We are in Open/WEP mode, open port immediately */
        if (pmpriv->port_ctrl_mode == MTRUE)
        {
            pmpriv->port_open = MTRUE;
            PRINTM(MINFO, "ASSOC_RESP: port_status = OPEN\n");
        }
    }

    if (pmpriv->sec_info.wpa_enabled || pmpriv->sec_info.wpa2_enabled || pmpriv->sec_info.ewpa_enabled ||
        pmpriv->sec_info.wapi_enabled)
    {
        pmpriv->scan_block = MTRUE;
    }

    /* done: */
    /* Need to indicate IOCTL complete */
    if (pioctl_req != MNULL)
    {
            pioctl_req->status_code = MLAN_ERROR_NO_ERROR;
    }

    LEAVE();
    return ret;
}


/**
 *  @brief Associated to a specific BSS discovered in a scan
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
 *  @param pbss_desc     A pointer to the BSS descriptor to associate with.
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_associate(IN mlan_private *pmpriv, IN t_void *pioctl_buf, IN BSSDescriptor_t *pbss_desc)
{
    mlan_status ret                          = MLAN_STATUS_SUCCESS;
    t_u8 current_bssid[MLAN_MAC_ADDR_LENGTH] = {0x0};
    pmlan_ioctl_req pioctl_req               = (mlan_ioctl_req *)pioctl_buf;

    ENTER();

    /* Return error if the pmadapter or table entry is not marked as infra */
    if ((pmpriv->bss_mode != MLAN_BSS_MODE_INFRA) || (pbss_desc->bss_mode != MLAN_BSS_MODE_INFRA))
    {
        if (pioctl_req != MNULL)
        {
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
        }
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    (void)__memcpy(pmpriv->adapter, &current_bssid, &pmpriv->curr_bss_params.bss_descriptor.mac_address,
                   sizeof(current_bssid));

    /* Clear any past association response stored for application retrieval */
    /* fixme: enable this later if assoc rsp buffer is needed */
    /* pmpriv->assoc_rsp_size = 0; */

    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_ASSOCIATE, HostCmd_ACT_GEN_SET, 0, pioctl_buf, pbss_desc);

    LEAVE();
    return ret;
}


/**
 *  @brief Convert band to radio type used in channel TLV
 *
 *  @param band		Band enumeration to convert to a channel TLV radio type
 *
 *  @return		Radio type designator for use in a channel TLV
 */
t_u8 wlan_band_to_radio_type(IN t_u8 band)
{
    t_u8 ret_radio_type;

    ENTER();

    switch (band)
    {
        case BAND_A:
        case BAND_AN:
        case BAND_A | BAND_AN:
            ret_radio_type = HostCmd_SCAN_RADIO_TYPE_A;
            break;
        case BAND_B:
        case BAND_G:
        case BAND_B | BAND_G:
        default:
            ret_radio_type = HostCmd_SCAN_RADIO_TYPE_BG;
            break;
    }

    LEAVE();
    return ret_radio_type;
}

bool wlan_use_non_default_ht_vht_cap(IN BSSDescriptor_t *pbss_desc)
{
    /* If connect to 11ax or non-brcm AP, still use default HT/VHT cap */
    if (
#if CONFIG_11AX
        pbss_desc->phe_cap ||
#endif
        (!pbss_desc->brcm_ie_exist && !pbss_desc->epigram_ie_exist))
        return false;

    /* In HT cap, check if "Transmit Null Data Packet" is set to 0,
     * In VHT cap, check if "Number of Sounding Dimensions" is set to 3,
     * If both are true, do not use default HT/VHT cap */
    if ((pbss_desc->pht_cap && (((pbss_desc->ht_cap_saved.ht_cap.tx_bf_cap >> 4) & 0x1) == 0x0))
#if CONFIG_11AC
        && (!pbss_desc->pvht_cap ||
            (pbss_desc->pvht_cap && (GET_VHTCAP_NUMSNDDM(pbss_desc->vht_cap_saved.vht_cap.vht_cap_info) == 0x2)))
#endif
    )
    {
        return true;
    }

    return false;
}
