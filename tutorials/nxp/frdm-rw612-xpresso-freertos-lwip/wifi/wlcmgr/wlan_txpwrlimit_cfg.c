/** @file wlan_txpwrlimit_cfg.c
 *
 *  @brief  This file provides WLAN World Wide Safe Mode Tx Power Limit APIs.
 *
 *  Copyright 2008-2021, 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <wlan.h>
#include <wifi.h>
#ifdef RW610
#include "fsl_ocotp.h"
#endif

#ifdef WIFI_BT_TX_PWR_LIMITS
#include WIFI_BT_TX_PWR_LIMITS
#else
#error "Region tx power config not defined"
#endif

#if defined(RW610) && (CONFIG_COMPRESS_TX_PWTBL || ((CONFIG_COMPRESS_RU_TX_PWTBL) && (CONFIG_11AX)))
typedef struct _rg_power_info
{
    t_u8 *rg_power_table;
    t_u16 rg_len;
} rg_power_info;
#endif

#if defined(RW610) && ((CONFIG_COMPRESS_RU_TX_PWTBL) && (CONFIG_11AX))
typedef struct _ru_power_cfg
{
    t_u16 region_code;
    rg_power_info power_info;
} ru_power_cfg;

/* All type boards ru txpwr data is same, */
ru_power_cfg ru_power_cfg_rw610[] = {
    {0x00, .power_info = {(t_u8 *)rutxpowerlimit_cfg_set_WW, sizeof(rutxpowerlimit_cfg_set_WW)}},
    {0x10, .power_info = {(t_u8 *)rutxpowerlimit_cfg_set_FCC, sizeof(rutxpowerlimit_cfg_set_FCC)}},
    {0x30, .power_info = {(t_u8 *)rutxpowerlimit_cfg_set_EU, sizeof(rutxpowerlimit_cfg_set_EU)}},
    {0x50, .power_info = {(t_u8 *)rutxpowerlimit_cfg_set_CN, sizeof(rutxpowerlimit_cfg_set_CN)}},
    {0xFF, .power_info = {(t_u8 *)rutxpowerlimit_cfg_set_JP, sizeof(rutxpowerlimit_cfg_set_JP)}},
};

int wlan_set_ru_power_cfg(t_u16 region_code)
{
    int i  = 0;
    int rv = -WM_FAIL;

    for (i = 0; i < sizeof(ru_power_cfg_rw610) / sizeof(ru_power_cfg); i++)
    {
        if (region_code == ru_power_cfg_rw610[i].region_code)
        {
            rv = wlan_set_11ax_rutxpowerlimit(ru_power_cfg_rw610[i].power_info.rg_power_table,
                                              ru_power_cfg_rw610[i].power_info.rg_len);

            return rv;
        }
    }

    /* Set default world wide ru txpwr if ru_power_cfg_rw610 does not have a corresponding region_code temporarily */
    wlcm_d("power_info of region_code %d not available, use default world wide ru txpwr by default.", region_code);

    rv = wlan_set_11ax_rutxpowerlimit(ru_power_cfg_rw610[0].power_info.rg_power_table,
                                      ru_power_cfg_rw610[0].power_info.rg_len);

    return rv;
}
#endif

#if defined(RW610) && (CONFIG_COMPRESS_TX_PWTBL)
#define MAX_SOC_OTP_LINE 64
#define OTP_PKG_TAG      0x15D
#define PKG_TYPE_MAX     3

typedef struct _rg_power_cfg
{
    t_u16 region_code;
    rg_power_info power_info[PKG_TYPE_MAX];
} rg_power_cfg;

/* For CSP board, we didn't get tx_power_table data, so use bga data temporary
 * And maybe no BGA or QFN data for avaliable region, use other type data
 */
rg_power_cfg rg_power_cfg_rw610[] = {
    {0x00, .power_info[RW610_PACKAGE_TYPE_QFN] = {(t_u8 *)rg_rw610_WW, sizeof(rg_rw610_WW)},
     .power_info[RW610_PACKAGE_TYPE_CSP] = {(t_u8 *)rg_rw610_WW, sizeof(rg_rw610_WW)},
     .power_info[RW610_PACKAGE_TYPE_BGA] = {(t_u8 *)rg_rw610_WW, sizeof(rg_rw610_WW)}},
    {0x10, .power_info[RW610_PACKAGE_TYPE_QFN] = {(t_u8 *)rg_rw610_qfn, sizeof(rg_rw610_qfn)},
     .power_info[RW610_PACKAGE_TYPE_CSP] = {(t_u8 *)rg_rw610_csp, sizeof(rg_rw610_csp)},
     .power_info[RW610_PACKAGE_TYPE_BGA] = {(t_u8 *)rg_rw610_bga, sizeof(rg_rw610_bga)}},
    {0x30, .power_info[RW610_PACKAGE_TYPE_QFN] = {(t_u8 *)rg_rw610_EU, sizeof(rg_rw610_EU)},
     .power_info[RW610_PACKAGE_TYPE_CSP] = {(t_u8 *)rg_rw610_EU, sizeof(rg_rw610_EU)},
     .power_info[RW610_PACKAGE_TYPE_BGA] = {(t_u8 *)rg_rw610_EU, sizeof(rg_rw610_EU)}},
    {0x50, .power_info[RW610_PACKAGE_TYPE_QFN] = {(t_u8 *)rg_rw610_CN, sizeof(rg_rw610_CN)},
     .power_info[RW610_PACKAGE_TYPE_CSP] = {(t_u8 *)rg_rw610_CN, sizeof(rg_rw610_CN)},
     .power_info[RW610_PACKAGE_TYPE_BGA] = {(t_u8 *)rg_rw610_CN, sizeof(rg_rw610_CN)}},
    {0xFF, .power_info[RW610_PACKAGE_TYPE_QFN] = {(t_u8 *)rg_rw610_JP, sizeof(rg_rw610_JP)},
     .power_info[RW610_PACKAGE_TYPE_CSP] = {(t_u8 *)rg_rw610_JP, sizeof(rg_rw610_JP)},
     .power_info[RW610_PACKAGE_TYPE_BGA] = {(t_u8 *)rg_rw610_JP, sizeof(rg_rw610_JP)}},
};

int wlan_set_rg_power_cfg(t_u16 region_code)
{
    int i               = 0;
    uint32_t board_type = 0;
    int rv              = WM_SUCCESS;

    board_type = wifi_get_board_type();
    if (RW610_PACKAGE_TYPE_QFN == board_type)
    {
        (void)PRINTF("PKG_TYPE: QFN\r\n");
        (void)PRINTF("Set QFN tx power table data \r\n");
    }
    else if (RW610_PACKAGE_TYPE_BGA == board_type)
    {
        (void)PRINTF("PKG_TYPE: BGA\r\n");
        (void)PRINTF("Set BGA tx power table data \r\n");
    }
    else if (RW610_PACKAGE_TYPE_CSP == board_type)
    {
        (void)PRINTF("PKG_TYPE: CSP\r\n");
        (void)PRINTF("Set CSP tx power table data \r\n");
    }
    else
    {
        board_type = RW610_PACKAGE_TYPE_BGA;
        (void)PRINTF("PKG_TYPE: UNKNOWN\r\n");
        (void)PRINTF("Set BGA tx power table data \r\n");
        (void)PRINTF("Can't get board type, we use bga data default \r\n");
    }

    for (i = 0; i < sizeof(rg_power_cfg_rw610) / sizeof(rg_power_cfg); i++)
    {
        if (region_code == rg_power_cfg_rw610[i].region_code)
        {
            rv = wlan_set_region_power_cfg(rg_power_cfg_rw610[i].power_info[board_type].rg_power_table,
                                           rg_power_cfg_rw610[i].power_info[board_type].rg_len);

            return rv;
        }
    }

    /* Set default world wide power table if rg_power_cfg_rw610 does not have a corresponding region_code temporarily */
    wlcm_d("power_info of region_code %d not available, use default world wide power table by default.", region_code);

    rv = wlan_set_region_power_cfg(rg_power_cfg_rw610[0].power_info[board_type].rg_power_table,
                                   rg_power_cfg_rw610[0].power_info[board_type].rg_len);

    return rv;
}
#elif (CONFIG_COMPRESS_TX_PWTBL)
typedef struct _rg_power_cfg
{
    t_u16 region_code;
    t_u8 *rg_power_table;
    t_u16 rg_len;
} rg_power_cfg;

rg_power_cfg rg_power_cfg_FC[] = {
    {
        0x00,
        (t_u8 *)rg_table_fc,
        sizeof(rg_table_fc),
    },
};

int wlan_set_rg_power_cfg(t_u16 region_code)
{
    int i  = 0;
    int rv = WM_SUCCESS;

    for (i = 0; i < sizeof(rg_power_cfg_FC) / sizeof(rg_power_cfg); i++)
    {
        if (region_code == rg_power_cfg_FC[i].region_code)
        {
            rv = wlan_set_region_power_cfg(rg_power_cfg_FC[i].rg_power_table, rg_power_cfg_FC[i].rg_len);
            if (rv != WM_SUCCESS)
                (void)PRINTF("Unable to set compressed TX power table configuration\r\n");
            return rv;
        }
    }

    return -WM_FAIL;
}

#endif

#if CONFIG_COMPRESS_TX_PWTBL || ((CONFIG_COMPRESS_RU_TX_PWTBL) && (CONFIG_11AX))
int wlan_set_wwsm_txpwrlimit()
{
    int rv = WM_SUCCESS;
#ifdef WLAN_REGION_CODE
    rv = wlan_set_country_code(WLAN_REGION_CODE);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set country code\r\n");
        return -WM_FAIL;
    }
#endif
#ifdef RW610
    unsigned int region_code = 0;
#endif
#ifdef WLAN_REGION_CODE
    rv = wlan_set_country_code(WLAN_REGION_CODE);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set country code\r\n");
        return -WM_FAIL;
    }
#endif
#ifdef RW610
    ARG_UNUSED(tx_pwrlimit_2g_cfg);
    ARG_UNUSED(chanlist_2g_cfg);
#if CONFIG_5GHz_SUPPORT
    ARG_UNUSED(tx_pwrlimit_5g_cfg);
    ARG_UNUSED(chanlist_5g_cfg);
#endif
#endif
#ifndef RW610
    rv = wlan_set_chanlist(&chanlist_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G chanlist configuration\r\n");
        return -WM_FAIL;
    }
#if CONFIG_5GHz_SUPPORT
    rv = wlan_set_chanlist(&chanlist_5g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 5G chanlist configuration\r\n");
        return -WM_FAIL;
    }
#endif
#endif
#ifdef RW610
    wlan_get_region_code(&region_code);
#if CONFIG_COMPRESS_TX_PWTBL
    rv = wlan_set_rg_power_cfg(region_code);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set compressed TX power table configuration\r\n");
        return -WM_FAIL;
    }
#endif

#if (CONFIG_COMPRESS_RU_TX_PWTBL) && (CONFIG_11AX)
    rv = wlan_set_ru_power_cfg(region_code);
    if (rv != WM_SUCCESS)
    {
        return -WM_FAIL;
    }
#endif

#else
    rv = wlan_set_region_power_cfg(rg_table_fc, rg_table_fc_len);
    if (rv != WM_SUCCESS)
    {
        return -WM_FAIL;
    }
#endif /* RW610 */

#ifndef RW610
#if CONFIG_11AX
#if CONFIG_COMPRESS_RU_TX_PWTBL
    rv = wlan_set_11ax_rutxpowerlimit(rutxpowerlimit_cfg_set, sizeof(rutxpowerlimit_cfg_set));
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#else
    rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_2g_cfg_set);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#if CONFIG_5GHz_SUPPORT
    else
    {
        rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_5g_cfg_set);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to set 5G RU TX PWR Limit configuration\r\n");
            return -WM_FAIL;
        }
    }
#endif
#endif /* CONFIG_COMPRESS_RU_TX_PWTBL */
#endif /* CONFIG_11AX */
#endif /* RW610 */

    return rv;
}
#else
int wlan_set_wwsm_txpwrlimit(void)
{
    int rv = WM_SUCCESS;
#ifdef WLAN_REGION_CODE
    rv     = wlan_set_country_code(WLAN_REGION_CODE);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set country code\r\n");
        return -WM_FAIL;
    }
#endif
    rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_2g_cfg, &tx_pwrlimit_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#if CONFIG_5GHz_SUPPORT
    rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_5g_cfg, &tx_pwrlimit_5g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 5G TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#endif

#ifndef RW610
#if CONFIG_11AX
#if CONFIG_COMPRESS_RU_TX_PWTBL
    rv = wlan_set_11ax_rutxpowerlimit(rutxpowerlimit_cfg_set, sizeof(rutxpowerlimit_cfg_set));
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#else
    rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_2g_cfg_set);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#if CONFIG_5GHz_SUPPORT
    else
    {
        rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_5g_cfg_set);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to set 5G RU TX PWR Limit configuration\r\n");
            return -WM_FAIL;
        }
    }
#endif
#endif /* CONFIG_COMPRESS_RU_TX_PWTBL */
#endif /* CONFIG_11AX */
#endif /* RW610 */

    return rv;
}
#endif /* CONFIG_COMPRESS_TX_PWTBL */

#ifndef RW610
const char *wlan_get_wlan_region_code(void)
{
#ifdef WLAN_REGION_CODE
    return WLAN_REGION_CODE;
#else
#error "Please define WLAN_REGION_CODE in Region tx power config file"
#endif
}
#endif
