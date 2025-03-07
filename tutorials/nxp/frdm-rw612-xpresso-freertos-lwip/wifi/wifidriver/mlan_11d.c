/** @file mlan_11d.c
 *
 *  @brief  This file provides functions for 802.11D
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
                Local Variables
********************************************************/

/** Region code mapping */
typedef struct _region_code_mapping
{
    /** Region */
    t_u8 region[COUNTRY_CODE_LEN];
    /** Code */
    t_u8 code;
} region_code_mapping_t;

/** Region code mapping table */
static const region_code_mapping_t region_code_mapping[] = {
    {"WW ", 0x00}, /* World Wide Safe */
    {"US ", 0x10}, /* US FCC */
    {"CA ", 0x20}, /* IC Canada */
    {"SG ", 0x10}, /* Singapore */
    {"EU ", 0x30}, /* ETSI */
    {"AU ", 0x30}, /* Australia */
    {"KR ", 0x30}, /* Republic Of Korea */
    {"FR ", 0x32}, /* France */
    {"JP ", 0xFF}, /* Japan */
    {"CN ", 0x50}, /* China */
};

#ifdef STA_SUPPORT
/** Default Tx power */
#define TX_PWR_DEFAULT 10

/** Universal region code */
#define UNIVERSAL_REGION_CODE 0xff

#define EU_REGION_CODE 0x30

/* Following two structures define the supported channels */
/** Channels for 802.11b/g */
static const chan_freq_power_t channel_freq_power_UN_BG[] = {
    {1, 2412, TX_PWR_DEFAULT},  {2, 2417, TX_PWR_DEFAULT},  {3, 2422, TX_PWR_DEFAULT},  {4, 2427, TX_PWR_DEFAULT},
    {5, 2432, TX_PWR_DEFAULT},  {6, 2437, TX_PWR_DEFAULT},  {7, 2442, TX_PWR_DEFAULT},  {8, 2447, TX_PWR_DEFAULT},
    {9, 2452, TX_PWR_DEFAULT},  {10, 2457, TX_PWR_DEFAULT}, {11, 2462, TX_PWR_DEFAULT}, {12, 2467, TX_PWR_DEFAULT},
    {13, 2472, TX_PWR_DEFAULT}, {14, 2484, TX_PWR_DEFAULT}};

#if CONFIG_5GHz_SUPPORT
/** Channels for 802.11a/j */
static chan_freq_power_t channel_freq_power_UN_AJ[] = {
    {8, 5040, TX_PWR_DEFAULT},   {12, 5060, TX_PWR_DEFAULT},  {16, 5080, TX_PWR_DEFAULT},  {34, 5170, TX_PWR_DEFAULT},
    {38, 5190, TX_PWR_DEFAULT},  {42, 5210, TX_PWR_DEFAULT},  {46, 5230, TX_PWR_DEFAULT},  {36, 5180, TX_PWR_DEFAULT},
    {40, 5200, TX_PWR_DEFAULT},  {44, 5220, TX_PWR_DEFAULT},  {48, 5240, TX_PWR_DEFAULT},  {52, 5260, TX_PWR_DEFAULT},
    {56, 5280, TX_PWR_DEFAULT},  {60, 5300, TX_PWR_DEFAULT},  {64, 5320, TX_PWR_DEFAULT},  {100, 5500, TX_PWR_DEFAULT},
    {104, 5520, TX_PWR_DEFAULT}, {108, 5540, TX_PWR_DEFAULT}, {112, 5560, TX_PWR_DEFAULT}, {116, 5580, TX_PWR_DEFAULT},
    {120, 5600, TX_PWR_DEFAULT}, {124, 5620, TX_PWR_DEFAULT}, {128, 5640, TX_PWR_DEFAULT}, {132, 5660, TX_PWR_DEFAULT},
    {136, 5680, TX_PWR_DEFAULT}, {140, 5700, TX_PWR_DEFAULT}, {144, 5720, TX_PWR_DEFAULT}, {149, 5745, TX_PWR_DEFAULT},
    {153, 5765, TX_PWR_DEFAULT}, {157, 5785, TX_PWR_DEFAULT}, {161, 5805, TX_PWR_DEFAULT},
#if CONFIG_UNII4_BAND_SUPPORT
    {165, 5825, TX_PWR_DEFAULT}, {169, 5845, TX_PWR_DEFAULT}, {173, 5865, TX_PWR_DEFAULT}, {177, 5885, TX_PWR_DEFAULT},
#else
    {165, 5825, TX_PWR_DEFAULT}
#endif
    /*  {240, 4920, TX_PWR_DEFAULT},
        {244, 4940, TX_PWR_DEFAULT},
        {248, 4960, TX_PWR_DEFAULT},
        {252, 4980, TX_PWR_DEFAULT},
    channels for 11J JP 10M channel gap */
};
#endif /* CONFIG_5GHz_SUPPORT */
#endif /* STA_SUPPORT */
/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/

/**
 *  @brief This function converts region string to region code
 *
 *  @param region_string    Region string
 *
 *  @return                 Region code
 */
t_u8 region_string_2_region_code(t_u8 *region_string)
{
    t_u8 i;

    ENTER();

    for (i = 0; i < ARRAY_SIZE(region_code_mapping); i++)
    {
        if (memcmp(region_string, region_code_mapping[i].region, COUNTRY_CODE_LEN - 1) == 0)
        {
            LEAVE();
            return region_code_mapping[i].code;
        }
    }

    if (wlan_is_etsi_country(NULL, region_string))
    {
        LEAVE();
        return EU_REGION_CODE;
    }

    /* Default is WW */
    LEAVE();
    return region_code_mapping[0].code;
}

/**
 *  @brief This function converts region string to integer code
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param region       Region string
 *  @param code         [output] Region code
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_region_2_code(pmlan_adapter pmadapter, t_u8 *region, OUT t_u8 *code)
{
    t_u8 i;
    t_u8 size = sizeof(region_code_mapping) / sizeof(region_code_mapping_t);

    ENTER();

    if (wlan_is_etsi_country(pmadapter, region))
    {
        *code = EU_REGION_CODE;
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }

    /* Look for code in mapping table */
    for (i = 0; i < size; i++)
    {
        if (!__memcmp(pmadapter, region_code_mapping[i].region, region, COUNTRY_CODE_LEN))
        {
            *code = region_code_mapping[i].code;
            LEAVE();
            return MLAN_STATUS_SUCCESS;
        }
    }

    LEAVE();
    return MLAN_STATUS_FAILURE;
}

/**
 *  @brief This function converts country index to country string
 *
 *  @param code         Country index
 *
 *  @return             Country string
 */
const uint8_t *wlan_11d_country_index_2_string(int country)
{
    if (country < 1 || country > 9)
    {
        return ((const uint8_t *)region_code_mapping[0].region);
    }
    return ((const uint8_t *)region_code_mapping[country - 1].region);
}

/**
 *  @brief This function converts integer code to region string
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param code         Region code
 *
 *  @return             Region string
 */
const t_u8 *wlan_11d_code_2_region(pmlan_adapter pmadapter, t_u8 code)
{
    t_u8 i;
    t_u8 size = sizeof(region_code_mapping) / sizeof(region_code_mapping_t);

    ENTER();

    /* Look for code in mapping table */
    for (i = 0; i < size; i++)
    {
        if (region_code_mapping[i].code == code)
        {
            LEAVE();
            return ((const t_u8 *)region_code_mapping[i].region);
        }
    }

    LEAVE();
    /* Default is WW */
    return ((const t_u8 *)region_code_mapping[0].region);
}

#ifdef STA_SUPPORT
/**
 *  @brief This function Checks if channel txpwr is learned from AP/IBSS
 *
 *  @param pmadapter            A pointer to mlan_adapter structure
 *  @param band                 Band number
 *  @param chan                 Channel number
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *
 *  @return                     MTRUE or MFALSE
 */
static t_u8 wlan_11d_channel_known(pmlan_adapter pmadapter,
                                   t_u16 band,
                                   t_u8 chan,
                                   parsed_region_chan_11d_t *parsed_region_chan)
{
    chan_power_11d_t *pchan_pwr = parsed_region_chan->chan_pwr;
    t_u8 no_of_chan             = parsed_region_chan->no_of_chan;
    t_u8 i                      = 0;
    t_u8 ret                    = MFALSE;
    mlan_private *pmpriv;

    ENTER();

    HEXDUMP("11D: parsed_region_chan", (t_u8 *)pchan_pwr, sizeof(chan_power_11d_t) * no_of_chan);

    /* Search channel */
    for (i = 0; i < no_of_chan; i++)
    {
        if (chan == pchan_pwr[i].chan && band == pchan_pwr[i].band)
        {
            PRINTM(MINFO, "11D: Found channel:%d (band:%d)\n", chan, band);
            ret = MTRUE;

            if ((band & BAND_A) != 0U)
            {
                /* If chan is a DFS channel, we need to see an AP on it */
                pmpriv = wlan_get_priv(pmadapter, MLAN_BSS_ROLE_STA);
                if (pmpriv != MNULL && wlan_11h_radar_detect_required(pmpriv, chan))
                {
                    PRINTM(MINFO, "11H: DFS channel %d, and ap_seen=%d\n", chan, pchan_pwr[i].ap_seen);
                    ret = pchan_pwr[i].ap_seen;
                }
            }

            LEAVE();
            return ret;
        }
    }

    PRINTM(MINFO, "11D: Could not find channel:%d (band:%d)\n", chan, band);
    LEAVE();
    return ret;
}

/**
 *  @brief This function generates parsed_region_chan from Domain Info
 *           learned from AP/IBSS
 *
 *  @param pmadapter            Pointer to mlan_adapter structure
 *  @param region_chan          Pointer to region_chan_t
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *
 *  @return                     N/A
 */
static t_void wlan_11d_generate_parsed_region_chan(pmlan_adapter pmadapter,
                                                   region_chan_t *region_chan,
                                                   parsed_region_chan_11d_t *parsed_region_chan)
{
    const chan_freq_power_t *cfp;
    t_u8 i;

    ENTER();

    /* Region channel must be provided */
    if (region_chan == MNULL)
    {
        PRINTM(MWARN, "11D: region_chan is MNULL\n");
        LEAVE();
        return;
    }

    /* Get channel-frequency-power trio */
    cfp = region_chan->pcfp;
    if (cfp == MNULL)
    {
        PRINTM(MWARN, "11D: cfp equal MNULL\n");
        LEAVE();
        return;
    }

    /* Set channel, band and power */
    for (i = 0; i < region_chan->num_cfp; i++)
    {
        parsed_region_chan->chan_pwr[i].chan = (t_u8)cfp->channel;
        parsed_region_chan->chan_pwr[i].band = (t_u8)region_chan->band;
        parsed_region_chan->chan_pwr[i].pwr  = (t_u8)cfp->max_tx_power;
        PRINTM(MINFO, "11D: Chan[%d] Band[%d] Pwr[%d]\n", parsed_region_chan->chan_pwr[i].chan,
               parsed_region_chan->chan_pwr[i].band, parsed_region_chan->chan_pwr[i].pwr);
        cfp++;
    }
    parsed_region_chan->no_of_chan = region_chan->num_cfp;

    PRINTM(MINFO, "11D: no_of_chan[%d]\n", parsed_region_chan->no_of_chan);

    LEAVE();
    return;
}

/**
 *  @brief This function generates domain_info from parsed_region_chan
 *
 *  @param pmadapter            Pointer to mlan_adapter structure
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *
 *  @return                     MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_11d_generate_domain_info(pmlan_adapter pmadapter, parsed_region_chan_11d_t *parsed_region_chan)
{
    t_u8 no_of_sub_band    = 0;
    t_u8 no_of_chan        = parsed_region_chan->no_of_chan;
    t_u8 no_of_parsed_chan = 0;
    t_u8 first_chan = 0, next_chan = 0, max_pwr = 0;
    t_u8 i, flag = MFALSE;
    wlan_802_11d_domain_reg_t *domain_info = &pmadapter->domain_reg;

    ENTER();

    /* Should be only place that clear domain_reg (besides init) */
    (void)__memset(pmadapter, domain_info, 0, sizeof(wlan_802_11d_domain_reg_t));

    /* Set country code */
    (void)__memcpy(pmadapter, domain_info->country_code,
                   wlan_11d_code_2_region(pmadapter, (t_u8)pmadapter->region_code), COUNTRY_CODE_LEN);

    PRINTM(MINFO, "11D: Number of channel = %d\n", no_of_chan);
    HEXDUMP("11D: parsed_region_chan", (t_u8 *)parsed_region_chan, sizeof(parsed_region_chan_11d_t));

    /* Set channel and power */
    for (i = 0; i < no_of_chan; i++)
    {
        if (!flag)
        {
            flag      = MTRUE;
            next_chan = first_chan = parsed_region_chan->chan_pwr[i].chan;
            max_pwr                = parsed_region_chan->chan_pwr[i].pwr;
            no_of_parsed_chan      = 1;
            continue;
        }

        if (parsed_region_chan->chan_pwr[i].chan == next_chan + 1U && parsed_region_chan->chan_pwr[i].pwr == max_pwr)
        {
            next_chan++;
            no_of_parsed_chan++;
        }
        else
        {
            domain_info->sub_band[no_of_sub_band].first_chan = first_chan;
            domain_info->sub_band[no_of_sub_band].no_of_chan = no_of_parsed_chan;
            domain_info->sub_band[no_of_sub_band].max_tx_pwr = max_pwr;
            no_of_sub_band++;
            no_of_parsed_chan = 1;
            next_chan = first_chan = parsed_region_chan->chan_pwr[i].chan;
            max_pwr                = parsed_region_chan->chan_pwr[i].pwr;
        }
    }

    if (flag != 0U)
    {
        domain_info->sub_band[no_of_sub_band].first_chan = first_chan;
        domain_info->sub_band[no_of_sub_band].no_of_chan = no_of_parsed_chan;
        domain_info->sub_band[no_of_sub_band].max_tx_pwr = max_pwr;
        no_of_sub_band++;
    }
    domain_info->no_of_sub_band = no_of_sub_band;

    PRINTM(MINFO, "11D: Number of sub-band =0x%x\n", domain_info->no_of_sub_band);
    HEXDUMP("11D: domain_info", (t_u8 *)domain_info,
            COUNTRY_CODE_LEN + 1 + sizeof(IEEEtypes_SubbandSet_t) * no_of_sub_band);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function updates the channel power table with the channel
 *            present in BSSDescriptor.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc    A pointer to BSSDescriptor_t
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_11d_update_chan_pwr_table(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc)
{
    mlan_adapter *pmadapter                      = pmpriv->adapter;
    parsed_region_chan_11d_t *parsed_region_chan = &pmadapter->parsed_region_chan;
    t_u16 i;
    t_u8 tx_power = 0;
    t_u8 chan;

    ENTER();

    chan = pbss_desc->phy_param_set.ds_param_set.current_chan;

    tx_power = wlan_get_txpwr_of_chan_from_cfp(pmpriv, chan);

    if (!tx_power)
    {
        PRINTM(MMSG, "11D: Invalid channel\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    /* Check whether the channel already exists in channel power table of
       parsed region */
    for (i = 0; ((i < parsed_region_chan->no_of_chan) && (i < MAX_NO_OF_CHAN)); i++)
    {
        if (parsed_region_chan->chan_pwr[i].chan == chan && parsed_region_chan->chan_pwr[i].band == pbss_desc->bss_band)
        {
            /* Channel already exists, use minimum of existing and tx_power */
            parsed_region_chan->chan_pwr[i].pwr     = MIN(parsed_region_chan->chan_pwr[i].pwr, tx_power);
            parsed_region_chan->chan_pwr[i].ap_seen = MTRUE;
            break;
        }
    }

    if (i == parsed_region_chan->no_of_chan && i < MAX_NO_OF_CHAN)
    {
        /* Channel not found. Update the channel in the channel-power table */
        parsed_region_chan->chan_pwr[i].chan    = chan;
        parsed_region_chan->chan_pwr[i].band    = (t_u8)pbss_desc->bss_band;
        parsed_region_chan->chan_pwr[i].pwr     = tx_power;
        parsed_region_chan->chan_pwr[i].ap_seen = MTRUE;
        parsed_region_chan->no_of_chan++;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function finds the no_of_chan-th chan after the first_chan
 *
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param band       Band
 *  @param first_chan First channel number
 *  @param no_of_chan Number of channels
 *  @param chan       Pointer to the returned no_of_chan-th chan number
 *
 *  @return           MTRUE or MFALSE
 */
static t_u8 wlan_11d_get_chan(pmlan_adapter pmadapter, t_u16 band, t_u8 first_chan, t_u8 no_of_chan, t_u8 *chan)
{
    const chan_freq_power_t *cfp = MNULL;
    t_u8 i;
    t_u8 cfp_no = 0;

    ENTER();
    if ((band & (BAND_B | BAND_G | BAND_GN)) != 0U)
    {
        cfp    = (const chan_freq_power_t *)channel_freq_power_UN_BG;
        cfp_no = sizeof(channel_freq_power_UN_BG) / sizeof(chan_freq_power_t);
    }
#if CONFIG_5GHz_SUPPORT
    else if ((band & (BAND_A | BAND_AN)) != 0U)
    {
        cfp    = channel_freq_power_UN_AJ;
        cfp_no = sizeof(channel_freq_power_UN_AJ) / sizeof(chan_freq_power_t);
    }
#endif /* CONFIG_5GHz_SUPPORT */
    else
    {
        PRINTM(MERROR, "11D: Wrong Band[%d]\n", band);
        LEAVE();
        return MFALSE;
    }
    /* Locate the first_chan */
    for (i = 0; i < cfp_no; i++)
    {
        if ((cfp + i)->channel == first_chan)
        {
            PRINTM(MINFO, "11D: first_chan found\n");
            break;
        }
    }

    if (i < cfp_no)
    {
        /* Check if beyond the boundary */
        if (i + no_of_chan < cfp_no)
        {
            /* Get first_chan + no_of_chan */
            *chan = (t_u8)(cfp + i + no_of_chan)->channel;
            LEAVE();
            return MTRUE;
        }
    }

    LEAVE();
    return MFALSE;
}

/**
 *  @brief This function processes the country info present in BSSDescriptor.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc     A pointer to BSSDescriptor_t
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_11d_process_country_info(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    parsed_region_chan_11d_t region_chan;
    parsed_region_chan_11d_t *parsed_region_chan = &pmadapter->parsed_region_chan;
    t_u16 i, j, num_chan_added = 0;

    ENTER();

    (void)__memset(pmadapter, &region_chan, 0, sizeof(parsed_region_chan_11d_t));

    /* Parse 11D country info */
    if (wlan_11d_parse_domain_info(pmadapter, &pbss_desc->country_info, pbss_desc->bss_band, &region_chan) !=
        MLAN_STATUS_SUCCESS)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    if (parsed_region_chan->no_of_chan != 0U)
    {
        /*
         * Check if the channel number already exists in the
         * chan-power table of parsed_region_chan
         */
        for (i = 0; (i < region_chan.no_of_chan && i < MAX_NO_OF_CHAN); i++)
        {
            for (j = 0; (j < parsed_region_chan->no_of_chan && j < MAX_NO_OF_CHAN); j++)
            {
                /*
                 * Channel already exists, update the tx power with new tx
                 * power, since country IE is valid here.
                 */
                if (region_chan.chan_pwr[i].chan == parsed_region_chan->chan_pwr[j].chan &&
                    region_chan.chan_pwr[i].band == parsed_region_chan->chan_pwr[j].band)
                {
                    parsed_region_chan->chan_pwr[j].pwr = region_chan.chan_pwr[i].pwr;
                    break;
                }
            }

            if (j == parsed_region_chan->no_of_chan && j < MAX_NO_OF_CHAN)
            {
                if (parsed_region_chan->no_of_chan + num_chan_added >= MAX_NO_OF_CHAN)
                {
                    break;
                }
                /*
                 * Channel does not exist in the channel power table,
                 * update this new chan and tx_power to the channel power table
                 */
                parsed_region_chan->chan_pwr[parsed_region_chan->no_of_chan + num_chan_added].chan =
                    region_chan.chan_pwr[i].chan;
                parsed_region_chan->chan_pwr[parsed_region_chan->no_of_chan + num_chan_added].band =
                    region_chan.chan_pwr[i].band;
                parsed_region_chan->chan_pwr[parsed_region_chan->no_of_chan + num_chan_added].pwr =
                    region_chan.chan_pwr[i].pwr;
                parsed_region_chan->chan_pwr[parsed_region_chan->no_of_chan + num_chan_added].ap_seen = MFALSE;
                num_chan_added++;
            }
        }
        parsed_region_chan->no_of_chan += (t_u8)num_chan_added;
        if (parsed_region_chan->no_of_chan > MAX_NO_OF_CHAN)
        {
            parsed_region_chan->no_of_chan = MAX_NO_OF_CHAN;
        }
    }
    else
    {
        /* Parsed region is empty, copy the first one */
        (void)__memcpy(pmadapter, parsed_region_chan, &region_chan, sizeof(parsed_region_chan_11d_t));
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This helper function copies chan_power_11d_t element
 *
 *  @param chan_dst   Pointer to destination of chan_power
 *  @param chan_src   Pointer to source of chan_power
 *
 *  @return           N/A
 */
static t_void wlan_11d_copy_chan_power(chan_power_11d_t *chan_dst, chan_power_11d_t *chan_src)
{
    ENTER();

    chan_dst->chan    = chan_src->chan;
    chan_dst->band    = chan_src->band;
    chan_dst->pwr     = chan_src->pwr;
    chan_dst->ap_seen = chan_src->ap_seen;

    LEAVE();
    return;
}

/**
 *  @brief This function sorts parsed_region_chan in ascending
 *  channel number.
 *
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *
 *  @return                     N/A
 */
static t_void wlan_11d_sort_parsed_region_chan(parsed_region_chan_11d_t *parsed_region_chan)
{
    t_u8 i, j;
    chan_power_11d_t temp;
    chan_power_11d_t *pchan_power = parsed_region_chan->chan_pwr;

    ENTER();

    PRINTM(MINFO, "11D: Number of channel = %d\n", parsed_region_chan->no_of_chan);

    // Use insertion sort method
    for (i = 1; i < parsed_region_chan->no_of_chan; i++)
    {
        wlan_11d_copy_chan_power(&temp, pchan_power + i);
        for (j = i; j > 0 && (pchan_power + j - 1)->chan > temp.chan; j--)
        {
            wlan_11d_copy_chan_power(pchan_power + j, pchan_power + j - 1);
        }
        wlan_11d_copy_chan_power(pchan_power + j, &temp);
    }

    HEXDUMP("11D: parsed_region_chan", (t_u8 *)parsed_region_chan, sizeof(parsed_region_chan_11d_t));

    LEAVE();
    return;
}
#endif /* STA_SUPPORT */

/**
 *  @brief This function sends domain info to FW
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_11d_send_domain_info(mlan_private *pmpriv, t_void *pioctl_buf, t_bool is_op_special_set)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    /* Send cmd to FW to set domain info */
    if (is_op_special_set)
    {
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11D_DOMAIN_INFO, HostCmd_ACT_SPC_SET, 0, (t_void *)pioctl_buf,
                               MNULL);
    }
    else
    {
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11D_DOMAIN_INFO, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_buf,
                               MNULL);
    }
    if (ret != MLAN_STATUS_SUCCESS)
    {
        PRINTM(MERROR, "11D: Failed to download domain Info\n");
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function overwrites domain_info
 *
 *  @param pmadapter        Pointer to mlan_adapter structure
 *  @param band             Intended operating band
 *  @param country_code     Intended country code
 *  @param num_sub_band     Count of tuples in list below
 *  @param sub_band_list    List of sub_band tuples
 *
 *  @return                 MLAN_STATUS_SUCCESS
 */
mlan_status wlan_11d_set_domain_info(mlan_private *pmpriv,
                                     t_u16 band,
                                     const t_u8 country_code[COUNTRY_CODE_LEN],
                                     t_u8 num_sub_band,
                                     IEEEtypes_SubbandSet_t *sub_band_list)
{
    mlan_adapter *pmadapter            = pmpriv->adapter;
    wlan_802_11d_domain_reg_t *pdomain = &pmadapter->domain_reg;
    mlan_status ret                    = MLAN_STATUS_SUCCESS;

    ENTER();

    (void)__memset(pmadapter, pdomain, 0, sizeof(wlan_802_11d_domain_reg_t));
    (void)__memcpy(pmadapter, pdomain->country_code, country_code, COUNTRY_CODE_LEN);
    pdomain->band           = band;
    pdomain->no_of_sub_band = num_sub_band;
    (void)__memcpy(pmadapter, pdomain->sub_band, sub_band_list,
                   MIN(MRVDRV_MAX_SUBBAND_802_11D, num_sub_band) * sizeof(IEEEtypes_SubbandSet_t));

    LEAVE();
    return ret;
}

/********************************************************
                Global functions
********************************************************/

/**
 *  @brief This function gets if priv is a station (STA)
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *
 *  @return             MTRUE or MFALSE
 */
t_bool wlan_is_station(mlan_private *pmpriv)
{
    ENTER();
    LEAVE();
    return (GET_BSS_ROLE(pmpriv) == MLAN_BSS_ROLE_STA) ? MTRUE : MFALSE;
}

/**
 *  @brief This function gets if 11D is enabled
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *
 *  @return             MTRUE or MFALSE
 */
t_bool wlan_11d_support_is_enabled(mlan_private *pmpriv)
{
    ENTER();
    LEAVE();
    return (pmpriv->state_11d.user_enable_11d_support == ENABLE_11D) ? MTRUE : MFALSE;
}

/**
 *  @brief This function gets if 11D is enabled
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *
 *  @return             MTRUE or MFALSE
 */
t_bool wlan_11d_is_enabled(mlan_private *pmpriv)
{
    ENTER();
    LEAVE();
    return (pmpriv->state_11d.enable_11d == ENABLE_11D) ? MTRUE : MFALSE;
}

static wlan_11d_fn_t wlan_11d_fn = {
#ifdef STA_SUPPORT
    .wlan_11d_prepare_dnld_domain_info_cmd_p = wlan_11d_prepare_dnld_domain_info_cmd,
    .wlan_11d_create_dnld_countryinfo_p      = wlan_11d_create_dnld_countryinfo,
    .wlan_11d_parse_dnld_countryinfo_p       = wlan_11d_parse_dnld_countryinfo,
#endif
};

static void *wlan_11d_enable_support = (wlan_11d_fn_t *)&wlan_11d_fn;

/**
 *  @brief This function sets 11D support
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
t_u16 wlan_enable_11d_support(mlan_private *pmpriv)
{
    ENTER();
    LEAVE();

    pmpriv->state_11d.user_enable_11d_support = ENABLE_11D;

    pmpriv->support_11d = wlan_11d_enable_support;

    return (t_u16)MLAN_STATUS_SUCCESS;
}

static wlan_11d_apis_t wlan_11d_apis = {
    .wlan_11d_cfg_ioctl_p              = wlan_11d_cfg_ioctl,
    .wlan_11d_cfg_domain_info_p        = wlan_11d_cfg_domain_info,
    .wlan_cmd_802_11d_domain_info_p    = wlan_cmd_802_11d_domain_info,
#if UAP_SUPPORT
    .wlan_11d_handle_uap_domain_info_p = wlan_11d_handle_uap_domain_info,
#endif
};

static void *wlan_11d_support_apis = (wlan_11d_apis_t *)&wlan_11d_apis;

/**
 *  @brief This function enables 11D driver APIs
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
t_u16 wlan_11d_support_APIs(mlan_private *pmpriv)
{
    ENTER();
    LEAVE();

    pmpriv->support_11d_APIs = wlan_11d_support_apis;

    return (t_u16)MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Initialize interface variable for 11D
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *
 *  @return             N/A
 */
t_void wlan_11d_priv_init(mlan_private *pmpriv)
{
    wlan_802_11d_state_t *state = &pmpriv->state_11d;

    ENTER();

    /* Start in disabled mode */
    state->enable_11d = DISABLE_11D;
    if (!pmpriv->adapter->init_para.cfg_11d)
    {
        state->user_enable_11d = DEFAULT_11D_STATE;
    }
    else
    {
        state->user_enable_11d =
            (pmpriv->adapter->init_para.cfg_11d == MLAN_INIT_PARA_DISABLED) ? DISABLE_11D : ENABLE_11D;
    }

    LEAVE();
    return;
}

/**
 *  @brief Initialize device variable for 11D
 *
 *  @param pmadapter    Pointer to mlan_adapter structure
 *
 *  @return             N/A
 */
t_void wlan_11d_init(mlan_adapter *pmadapter)
{
    ENTER();

#ifdef STA_SUPPORT
    (void)__memset(pmadapter, &(pmadapter->parsed_region_chan), 0, sizeof(parsed_region_chan_11d_t));
    (void)__memset(pmadapter, &(pmadapter->universal_channel), 0, sizeof(region_chan_t));
#endif
    (void)__memset(pmadapter, &(pmadapter->domain_reg), 0, sizeof(wlan_802_11d_domain_reg_t));

    LEAVE();
    return;
}


/**
 *  @brief This function implements command CMD_802_11D_DOMAIN_INFO
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd         A pointer to HostCmd_DS_COMMAND structure of
 *                        command buffer
 *  @param cmd_action   Command action
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_802_11d_domain_info(mlan_private *pmpriv, HostCmd_DS_COMMAND *pcmd, t_u16 cmd_action)
{
    mlan_adapter *pmadapter                      = pmpriv->adapter;
    HostCmd_DS_802_11D_DOMAIN_INFO *pdomain_info = &pcmd->params.domain_info;
    MrvlIEtypes_DomainParamSet_t *domain         = &pdomain_info->domain;
    t_u8 no_of_sub_band                          = pmadapter->domain_reg.no_of_sub_band;

    ENTER();

    PRINTM(MINFO, "11D: number of sub-band=0x%x\n", no_of_sub_band);

    pcmd->command        = wlan_cpu_to_le16(HostCmd_CMD_802_11D_DOMAIN_INFO);
    pdomain_info->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_GET)
    {
        /* Dump domain info */
        pcmd->size = wlan_cpu_to_le16(sizeof(pdomain_info->action) + S_DS_GEN);
        HEXDUMP("11D: 802_11D_DOMAIN_INFO", (t_u8 *)pcmd, wlan_le16_to_cpu(pcmd->size));
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }

    /* Set domain info fields */
    domain->header.type = wlan_cpu_to_le16(TLV_TYPE_DOMAIN);
    (void)__memcpy(pmadapter, domain->country_code, pmadapter->domain_reg.country_code, sizeof(domain->country_code));

    domain->header.len = (t_u16)(((no_of_sub_band * sizeof(IEEEtypes_SubbandSet_t)) + sizeof(domain->country_code)));

    if (no_of_sub_band != 0U)
    {
        (void)__memcpy(pmadapter, domain->sub_band, pmadapter->domain_reg.sub_band,
                       MIN(MRVDRV_MAX_SUBBAND_802_11D, no_of_sub_band) * sizeof(IEEEtypes_SubbandSet_t));

        pcmd->size = wlan_cpu_to_le16(sizeof(pdomain_info->action) + domain->header.len + sizeof(MrvlIEtypesHeader_t) +
                                      S_DS_GEN);
    }
    else
    {
        pcmd->size = wlan_cpu_to_le16(sizeof(pdomain_info->action) + S_DS_GEN);
    }
    domain->header.len = wlan_cpu_to_le16(domain->header.len);

    HEXDUMP("11D: 802_11D_DOMAIN_INFO", (t_u8 *)pcmd, wlan_le16_to_cpu(pcmd->size));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


#ifdef STA_SUPPORT

/** Region code mapping */
typedef struct _region_code_t
{
    /** Region */
    t_u8 region[COUNTRY_CODE_LEN];
} region_code_t;

/**
 *  @brief This function check cfg80211 special region code.
 *
 *  @param region_string         Region string
 *
 *  @return     MTRUE/MFALSE
 */
t_u8 is_special_region_code(t_u8 *region_string)
{
    t_u8 i;
    region_code_t special_region_code[] = {{"00 "}, {"99 "}, {"98 "}, {"97 "}};

    for (i = 0; i < COUNTRY_CODE_LEN && region_string[i]; i++)
        region_string[i] = toupper(region_string[i]);

    for (i = 0; i < ARRAY_SIZE(special_region_code); i++)
    {
        if (!memcmp(region_string, special_region_code[i].region, COUNTRY_CODE_LEN))
        {
            PRINTM(MINFO, "special region code=%s\n", region_string);
            return MTRUE;
        }
    }
    return MFALSE;
}

/**
 *  @brief This function parses country information for region channel
 *
 *  @param pmadapter            Pointer to mlan_adapter structure
 *  @param country_info         Country information
 *  @param band                 Chan band
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *
 *  @return                     MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_parse_domain_info(pmlan_adapter pmadapter,
                                       IEEEtypes_CountryInfoFullSet_t *country_info,
                                       t_u16 band,
                                       parsed_region_chan_11d_t *parsed_region_chan)
{
    t_u8 no_of_sub_band, no_of_chan;
    t_u8 last_chan, first_chan, cur_chan = 0;
    t_u8 idx = 0;
    t_u8 j, i;

    ENTER();

    /*
     * Validation Rules:
     *    1. Valid Region Code
     *    2. First Chan increment
     *    3. Channel range no overlap
     *    4. Channel is valid?
     *    5. Channel is supported by Region?
     *    6. Others
     */

    HEXDUMP("country_info", (t_u8 *)country_info, 30);
    /* Step 1: Check region_code */
    if (!(*(country_info->country_code)) || (country_info->len <= COUNTRY_CODE_LEN))
    {
        /* No region info or wrong region info: treat as no 11D info */
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    no_of_sub_band = (t_u8)((country_info->len - COUNTRY_CODE_LEN) / sizeof(IEEEtypes_SubbandSet_t));

    last_chan = 0;
    for (j = 0; j < no_of_sub_band; j++)
    {
        if (country_info->sub_band[j].first_chan <= last_chan)
        {
            /* Step2&3: Check First Chan Num increment and no overlap */
            PRINTM(MINFO, "11D: Chan[%d>%d] Overlap\n", country_info->sub_band[j].first_chan, last_chan);
            continue;
        }

        first_chan = country_info->sub_band[j].first_chan;
        no_of_chan = country_info->sub_band[j].no_of_chan;

        i = 0;
        while (idx < MAX_NO_OF_CHAN && i < no_of_chan)
        {
            /* Step 4 : Channel is supported? */
            if (wlan_11d_get_chan(pmadapter, band, first_chan, i, &cur_chan) == MFALSE)
            {
                /* Chan is not found in UN table */
                PRINTM(MWARN, "11D: channel is not supported: %d\n", i);
                break;
            }

            last_chan = cur_chan;

            /* Step 5: We don't need to check if cur_chan is supported by mrvl
               in region */
            parsed_region_chan->chan_pwr[idx].chan = cur_chan;
            parsed_region_chan->chan_pwr[idx].band = (t_u8)band;
            parsed_region_chan->chan_pwr[idx].pwr  = country_info->sub_band[j].max_tx_pwr;
            idx++;
            i++;
        }

        /* Step 6: Add other checking if any */
    }

    parsed_region_chan->no_of_chan = idx;

    PRINTM(MINFO, "11D: number of channel=0x%x\n", parsed_region_chan->no_of_chan);
    HEXDUMP("11D: parsed_region_chan", (t_u8 *)parsed_region_chan->chan_pwr, sizeof(chan_power_11d_t) * idx);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function converts channel to frequency
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param chan         Channel number
 *  @param band         Band
 *
 *  @return             Channel frequency
 */
t_u32 wlan_11d_chan_2_freq(pmlan_adapter pmadapter, t_u8 chan, t_u16 band)
{
    const chan_freq_power_t *cf;
    t_u16 cnt;
    t_u16 i;
    t_u32 freq = 0;

    ENTER();

#if CONFIG_5GHz_SUPPORT
    /* Get channel-frequency-power trios */
    if ((band & (BAND_A | BAND_AN)) != 0)
    {
        cf  = channel_freq_power_UN_AJ;
        cnt = sizeof(channel_freq_power_UN_AJ) / sizeof(chan_freq_power_t);
    }
    else
    {
#endif /* CONFIG_5GHz_SUPPORT */
        cf  = (const chan_freq_power_t *)channel_freq_power_UN_BG;
        cnt = sizeof(channel_freq_power_UN_BG) / sizeof(chan_freq_power_t);
#if CONFIG_5GHz_SUPPORT
    }
#endif /* CONFIG_5GHz_SUPPORT */

    /* Locate channel and return corresponding frequency */
    for (i = 0; i < cnt; i++)
    {
        if (chan == cf[i].channel)
        {
            freq = cf[i].freq;
        }
    }

    LEAVE();
    return freq;
}

/**
 *  @brief This function setups scan channels
 *
 *  @param pmpriv       Pointer to mlan_private structure
 *  @param band         Band
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_11d_set_universaltable(mlan_private *pmpriv, t_u16 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 size              = (t_u16)sizeof(chan_freq_power_t);
    t_u16 i                 = 0;

    ENTER();

    (void)__memset(pmadapter, pmadapter->universal_channel, 0, sizeof(pmadapter->universal_channel));

    if ((band & (BAND_B | BAND_G | BAND_GN)) != 0U)
    /* If band B, G or N */
    {
        /* Set channel-frequency-power */
        pmadapter->universal_channel[i].num_cfp = (t_u8)(sizeof(channel_freq_power_UN_BG) / size);
        PRINTM(MINFO, "11D: BG-band num_cfp=%d\n", pmadapter->universal_channel[i].num_cfp);

        pmadapter->universal_channel[i].pcfp  = (const chan_freq_power_t *)channel_freq_power_UN_BG;
        pmadapter->universal_channel[i].valid = MTRUE;

        /* Set region code */
        pmadapter->universal_channel[i].region = UNIVERSAL_REGION_CODE;

        /* Set band */
        if ((band & BAND_GN) != 0U)
        {
            pmadapter->universal_channel[i].band = BAND_G;
        }
        else
        {
            pmadapter->universal_channel[i].band = (band & BAND_G) ? BAND_G : BAND_B;
        }
        i++;
    }

#if CONFIG_5GHz_SUPPORT
#if CONFIG_11AC
    if ((band & (BAND_A | BAND_AN | BAND_AAC)) != 0U)
    {
#else
    if ((band & (BAND_A | BAND_AN)) != 0U)
    {
#endif
        /* If band A */

        /* Set channel-frequency-power */
        pmadapter->universal_channel[i].num_cfp = (t_u8)(sizeof(channel_freq_power_UN_AJ) / size);
        PRINTM(MINFO, "11D: AJ-band num_cfp=%d\n", pmadapter->universal_channel[i].num_cfp);

        pmadapter->universal_channel[i].pcfp = channel_freq_power_UN_AJ;

        pmadapter->universal_channel[i].valid = MTRUE;

        /* Set region code */
        pmadapter->universal_channel[i].region = UNIVERSAL_REGION_CODE;

        /* Set band */
        pmadapter->universal_channel[i].band = BAND_A;
    }
#endif /* CONFIG_5GHz_SUPPORT */

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function calculates the scan type for channels
 *
 *  @param pmadapter            A pointer to mlan_adapter structure
 *  @param band                 Band number
 *  @param chan                 Chan number
 *  @param parsed_region_chan   Pointer to parsed_region_chan_11d_t
 *
 *  @return                     PASSIVE if chan is unknown; ACTIVE if chan is known
 */
mlan_scan_type wlan_11d_get_scan_type(mlan_private *pmpriv,
                                      t_u16 band,
                                      t_u8 chan,
                                      parsed_region_chan_11d_t *parsed_region_chan)
{
    mlan_adapter *pmadapter = pmpriv->adapter;

    mlan_scan_type scan_type = MLAN_SCAN_TYPE_PASSIVE;

    ENTER();

    if (wlan_11d_channel_known(pmadapter, band, chan, parsed_region_chan) != 0U)
    {
        /* Channel found */
        PRINTM(MINFO, "11D: Channel found and doing Active Scan\n");
        scan_type = MLAN_SCAN_TYPE_ACTIVE;
    }
    else
    {
        PRINTM(MINFO, "11D: Channel not found and doing Passive Scan\n");
    }

    LEAVE();
    return scan_type;
}

/**
 *  @brief This function clears the parsed region table, if 11D is enabled
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_clear_parsedtable(mlan_private *pmpriv)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    mlan_status ret         = MLAN_STATUS_SUCCESS;

    ENTER();

    if (wlan_11d_is_enabled(pmpriv))
    {
        (void)__memset(pmadapter, &(pmadapter->parsed_region_chan), 0, sizeof(parsed_region_chan_11d_t));
    }
    else
    {
        ret = MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function generates 11D info from user specified regioncode
 *         and download to FW
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param band         Band to create
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_create_dnld_countryinfo(mlan_private *pmpriv, t_u16 band)
{
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    region_chan_t *region_chan;
    parsed_region_chan_11d_t parsed_region_chan;
    t_u8 j;

    ENTER();

    /* Only valid if 11D is enabled */
    if (wlan_11d_is_enabled(pmpriv))
    {
        PRINTM(MINFO, "11D: Band[%d]\n", band);

        /* Update parsed_region_chan; download domain info to FW */

        /* Find region channel */
        for (j = 0; j < MAX_REGION_CHANNEL_NUM; j++)
        {
            bool continue_loop = MFALSE;
            region_chan        = &pmadapter->region_channel[j];

            PRINTM(MINFO, "11D: [%d] region_chan->Band[%d]\n", j, region_chan->band);

            if (region_chan == MNULL || !region_chan->valid || region_chan->pcfp == MNULL)
            {
                continue;
            }
            switch (region_chan->band)
            {
#if CONFIG_5GHz_SUPPORT
                case BAND_A:
                    switch (band)
                    {
                        case BAND_A:
                        case BAND_AN:
                        case BAND_A | BAND_AN:
#if CONFIG_11AC
                        case BAND_A | BAND_AN | BAND_AAC:
#endif
#if CONFIG_11AX
                        case BAND_A | BAND_AN | BAND_AAC | BAND_AAX:
#endif
                            break;
                        default:
                            continue_loop = MTRUE;
                            break;
                    }
                    break;
#endif /* CONFIG_5GHz_SUPPORT */
                case BAND_B:
                case BAND_G:
                    switch (band)
                    {
                        case BAND_B:
                        case BAND_G:
                        case BAND_G | BAND_B:
                        case BAND_GN:
                        case BAND_G | BAND_GN:
                        case BAND_B | BAND_G | BAND_GN:
#if CONFIG_11AC
                        case BAND_B | BAND_G | BAND_GN | BAND_GAC:
#endif
#if CONFIG_11AX
                        case BAND_B | BAND_G | BAND_GN | BAND_GAC | BAND_GAX:
#endif
                            break;
                        default:
                            continue_loop = MTRUE;
                            break;
                    }
                    break;
                default:
                    continue_loop = MTRUE;
                    break;
            }

            if (continue_loop == MTRUE)
            {
                continue;
            }
            else
            {
                break;
            }
        }

        /* Check if region channel found */
        if (j >= MAX_REGION_CHANNEL_NUM)
        {
            PRINTM(MERROR, "11D: region_chan not found. Band[%d]\n", band);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }

        /* Generate parsed region channel info from region channel */
        (void)__memset(pmadapter, &parsed_region_chan, 0, sizeof(parsed_region_chan_11d_t));
        wlan_11d_generate_parsed_region_chan(pmadapter, region_chan, &parsed_region_chan);

        /* Generate domain info from parsed region channel info */
        (void)wlan_11d_generate_domain_info(pmadapter, &parsed_region_chan);

        /* Set domain info */
        ret = wlan_11d_send_domain_info(pmpriv, MNULL, MFALSE);
        if (ret != MLAN_STATUS_SUCCESS)
        {
            PRINTM(MERROR, "11D: Error sending domain info to FW\n");
        }
    }

    LEAVE();
    return ret;
}

void wlan_filter_domain_channel(mlan_private *pmpriv,
                                parsed_region_chan_11d_t *origin_region_chan,
                                parsed_region_chan_11d_t *filtered_region_chan)
{
    t_u32 i;

    for (i = 0; (i < origin_region_chan->no_of_chan) && (i < MAX_NO_OF_CHAN); i++)
    {
        if(MTRUE == wlan_check_channel_by_region_table(pmpriv, origin_region_chan->chan_pwr[i].chan))
        {
            (void)__memcpy(pmpriv->adapter, &filtered_region_chan->chan_pwr[filtered_region_chan->no_of_chan],
                           &origin_region_chan->chan_pwr[i], sizeof(chan_power_11d_t));
            filtered_region_chan->no_of_chan++;
        }
    }
}
/**
 *  @brief This function parses country info from AP and
 *           download country info to FW
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc     A pointer to BSS descriptor
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_parse_dnld_countryinfo(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc)
{
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    parsed_region_chan_11d_t region_chan;
    parsed_region_chan_11d_t bssdesc_region_chan;
    parsed_region_chan_11d_t filtered_region_chan;
    t_u32 i, j;

    ENTER();

    /* Only valid if 11D is enabled */
    if (wlan_11d_is_enabled(pmpriv))
    {
        (void)__memset(pmadapter, &region_chan, 0, sizeof(parsed_region_chan_11d_t));
        (void)__memset(pmadapter, &bssdesc_region_chan, 0, sizeof(parsed_region_chan_11d_t));
        (void)__memset(pmadapter, &filtered_region_chan, 0, sizeof(parsed_region_chan_11d_t));

        (void)__memcpy(pmadapter, &region_chan, &pmadapter->parsed_region_chan, sizeof(parsed_region_chan_11d_t));

        if (pbss_desc != MNULL)
        {
            /** Country code */
            t_u8 country_code[COUNTRY_CODE_LEN];
            country_code[0] = pbss_desc->country_info.country_code[0];
            country_code[1] = pbss_desc->country_info.country_code[1];
            country_code[2] = ' ';

            if (is_special_region_code(country_code))
            {
                PRINTM(MINFO, "Skip special region code in CountryIE");
                LEAVE();
                return MLAN_STATUS_SUCCESS;
            }
            /* Parse domain info if available */
            ret = wlan_11d_parse_domain_info(pmadapter, &pbss_desc->country_info, pbss_desc->bss_band,
                                             &bssdesc_region_chan);
            if (ret == MLAN_STATUS_SUCCESS)
            {
                /* Update the channel-power table */
                for (i = 0; ((i < bssdesc_region_chan.no_of_chan) && (i < MAX_NO_OF_CHAN)); i++)
                {
                    for (j = 0; ((j < region_chan.no_of_chan) && (j < MAX_NO_OF_CHAN)); j++)
                    {
                        /*
                         * Channel already exists, use minimum of existing
                         * tx power and tx_power received from
                         * country info of the current AP
                         */
                        if (region_chan.chan_pwr[i].chan == bssdesc_region_chan.chan_pwr[j].chan &&
                            region_chan.chan_pwr[i].band == bssdesc_region_chan.chan_pwr[j].band)
                        {
                            region_chan.chan_pwr[j].pwr =
                                MIN(region_chan.chan_pwr[j].pwr, bssdesc_region_chan.chan_pwr[i].pwr);
                            break;
                        }
                    }
                }
            }
        }

        /* Filter out channel list of current region code, then generate domain info */
        (void)wlan_filter_domain_channel(pmpriv, &region_chan, &filtered_region_chan);

        /* Generate domain info */
        (void)wlan_11d_generate_domain_info(pmadapter, &filtered_region_chan);

        /* Set domain info */
        ret = wlan_11d_send_domain_info(pmpriv, MNULL, MFALSE);
        if (ret != MLAN_STATUS_SUCCESS)
        {
            PRINTM(MERROR, "11D: Error sending domain info to FW\n");
        }
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function prepares domain info from scan table and
 *         downloads the domain info command to the FW.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_prepare_dnld_domain_info_cmd(mlan_private *pmpriv)
{
    mlan_status ret                               = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter                       = pmpriv->adapter;
    IEEEtypes_CountryInfoFullSet_t *pcountry_full = MNULL;
    t_u32 idx;

    ENTER();

    /* Only valid if 11D is enabled */
    if (wlan_11d_is_enabled(pmpriv) && pmadapter->num_in_scan_table != 0U)
    {
        for (idx = 0; idx < pmadapter->num_in_scan_table; idx++)
        {
            pcountry_full = &pmadapter->pscan_table[idx].country_info;

            ret = wlan_11d_update_chan_pwr_table(pmpriv, &pmadapter->pscan_table[idx]);

            if (*(pcountry_full->country_code) != 0U && (pcountry_full->len > COUNTRY_CODE_LEN))
            {
                /* Country info found in the BSS Descriptor */
                ret = wlan_11d_process_country_info(pmpriv, &pmadapter->pscan_table[idx]);
            }
        }

        /* Sort parsed_region_chan in ascending channel number */
        wlan_11d_sort_parsed_region_chan(&pmadapter->parsed_region_chan);
#if 0
        /* Check if connected */
        if (pmpriv->media_connected == MTRUE)
        {
            ret = wlan_11d_parse_dnld_countryinfo(pmpriv, &pmpriv->curr_bss_params.bss_descriptor);
        }
        else
        {
            ret = wlan_11d_parse_dnld_countryinfo(pmpriv, MNULL);
        }
#endif
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function sets up domain_reg and downloads CMD to FW
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   Pointer to the IOCTL request buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_cfg_domain_info(IN pmlan_adapter pmadapter, IN mlan_ioctl_req *pioctl_req)
{
    mlan_status ret                      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv                 = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_11d_domain_info *domain_info = MNULL;
    mlan_ds_11d_cfg *cfg_11d             = MNULL;
    t_u8 region_code                     = 0;

    ENTER();

    cfg_11d     = (mlan_ds_11d_cfg *)(void *)pioctl_req->pbuf;
    domain_info = &cfg_11d->param.domain_info;

#ifdef OTP_CHANINFO
    if ((pmadapter->otp_region != MNULL) && (pmadapter->otp_region->force_reg != 0U))
    {
        (void)PRINTF("ForceRegionRule is set in the on-chip OTP memory\r\n");
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
#endif

    /* Update region code and table based on country code */
    if (wlan_11d_region_2_code(pmadapter, domain_info->country_code, &region_code) == MLAN_STATUS_SUCCESS)
    {
        pmadapter->region_code = region_code;
        ret                    = wlan_set_regiontable(pmpriv, region_code, pmadapter->fw_bands);
        if (ret != MLAN_STATUS_SUCCESS)
        {
            goto done;
        }
    }

    (void)wlan_11d_set_domain_info(pmpriv, domain_info->band, domain_info->country_code, domain_info->no_of_sub_band,
                                   (IEEEtypes_SubbandSet_t *)(void *)domain_info->sub_band);
    ret = wlan_11d_send_domain_info(pmpriv, pioctl_req, MFALSE);

    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

done:
    LEAVE();
    return ret;
}
#endif /* STA_SUPPORT */

#if UAP_SUPPORT
/**
 *  @brief This function handles domain info data from UAP interface.
 *         Checks conditions, sets up domain_reg, then downloads CMD.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param band         Band interface is operating on
 *  @param domain_tlv   Pointer to domain_info tlv
 *  @param pioctl_buf   Pointer to the IOCTL buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_11d_handle_uap_domain_info(mlan_private *pmpriv, t_u16 band, t_u8 *domain_tlv, t_void *pioctl_buf)
{
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    MrvlIEtypes_DomainParamSet_t *pdomain_tlv;
    t_u8 num_sub_band = 0;
    t_u8 region_code  = 0;

    ENTER();

    pdomain_tlv = (MrvlIEtypes_DomainParamSet_t *)(void *)domain_tlv;

    // update region code & table based on country string
    if (wlan_11d_region_2_code(pmadapter, pdomain_tlv->country_code, &region_code) == MLAN_STATUS_SUCCESS)
    {
        pmadapter->region_code = region_code;
        ret                    = wlan_set_regiontable(pmpriv, region_code, pmadapter->fw_bands);
    }

    num_sub_band = (t_u8)((pdomain_tlv->header.len - COUNTRY_CODE_LEN) / sizeof(IEEEtypes_SubbandSet_t));

    // TODO: don't just clobber pmadapter->domain_reg.
    // Add some checking or merging between STA & UAP domain_info

    (void)wlan_11d_set_domain_info(pmpriv, band, pdomain_tlv->country_code, num_sub_band, pdomain_tlv->sub_band);

    /* wmsdk: We do not yet have mechanism in wlan_prepare_cmd() to
       separate uao and sta commands. Hence we have to call uap cmd send
       function here manually */
    /* ret = wlan_11d_send_domain_info(pmpriv, pioctl_buf); */
    int rv = wifi_uap_prepare_and_send_cmd(pmpriv, HostCmd_CMD_802_11D_DOMAIN_INFO, HostCmd_ACT_GEN_SET, 0,
                                           (t_void *)pioctl_buf, MNULL, MLAN_BSS_TYPE_UAP, NULL);
    if (rv != 0)
    {
        wuap_w("Unable to send uap domain info");
        ret = MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}
#endif
