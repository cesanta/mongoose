/** @file mlan_cfp.c
 *
 *  @brief  This file provides WLAN client mode channel, frequency and power related code
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*************************************************************
Change Log:
    04/16/2009: initial version
************************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
    Local Variables
********************************************************/

/** 100mW */
#define WLAN_TX_PWR_DEFAULT 20
/** 100mW */
#define WLAN_TX_PWR_US_DEFAULT 20
/** 100mW */
#define WLAN_TX_PWR_JP_BG_DEFAULT 20
/** 200mW */
#define WLAN_TX_PWR_JP_A_DEFAULT 23
/** 100mW */
#define WLAN_TX_PWR_FR_100MW 20
/** 10mW */
#define WLAN_TX_PWR_FR_10MW 10
/** 100mW */
#define WLAN_TX_PWR_EMEA_DEFAULT 20
/** 200mW */
#define WLAN_TX_PWR_200MW 23
/** 2000mW */
#define WLAN_TX_PWR_CN_2000MW 33
#if defined(RW610) || defined(IW610)
/** 22dBm */
#define WLAN_TX_PWR_WW_DEFAULT 22
#else
/** 8dBm */
#define WLAN_TX_PWR_WW_DEFAULT 8
#endif
/** Region code mapping */
typedef struct _country_code_mapping
{
    /** Region */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Code for B/G CFP table */
    t_u8 cfp_code_bg;
    /** Code for A CFP table */
    t_u8 cfp_code_a;
} country_code_mapping_t;

static const country_code_mapping_t country_code_mapping[] = {
    {"WW", 0x00, 0x00}, /* World Wide Safe */
    {"US", 0x10, 0x10}, /* US FCC */
    {"CA", 0x10, 0x20}, /* IC Canada */
    {"SG", 0x10, 0x10}, /* Singapore */
    {"EU", 0x30, 0x30}, /* ETSI */
    {"AU", 0x30, 0x30}, /* Australia */
    {"KR", 0x30, 0x30}, /* Republic Of Korea */
    {"FR", 0x32, 0x32}, /* France */
    {"JP", 0xFF, 0x40}, /* Japan */
    {"CN", 0x30, 0x50}, /* China */
};

#define COUNTRY_ID_US 0
#define COUNTRY_ID_JP 1
#define COUNTRY_ID_CN 2
#define COUNTRY_ID_EU 3

typedef struct _oper_bw_chan
{
    /*non-global operating class*/
    t_u8 oper_class;
    /*global operating class*/
    t_u8 global_oper_class;
    /*bandwidth 0-20M 1-40M 2-80M 3-160M*/
    t_u8 bandwidth;
    /*channel list*/
    t_u8 channel_list[13];
} oper_bw_chan;

/** oper class table for US*/
static oper_bw_chan oper_bw_chan_us[] = {
    /** non-Global oper class, global oper class, bandwidth, channel list*/
    {1, 115, 0, {36, 40, 44, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {2, 118, 0, {52, 56, 60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {3, 124, 0, {149, 153, 157, 161, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {4, 121, 0, {100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 0}},
    {5, 125, 0, {149, 153, 157, 161, 165, 0, 0, 0, 0, 0, 0, 0, 0}},
    {12, 81, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 0}},
    {22, 116, 1, {36, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {23, 119, 1, {52, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {24, 122, 1, {100, 108, 116, 124, 132, 140, 0, 0, 0, 0, 0, 0, 0}},
    {25, 126, 1, {149, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {26, 126, 1, {149, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {27, 117, 1, {40, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {28, 120, 1, {56, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {29, 123, 1, {104, 112, 120, 128, 136, 144, 0, 0, 0, 0, 0, 0, 0}},
    {30, 127, 1, {153, 161, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {31, 127, 1, {153, 161, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {32, 83, 1, {1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0}},
    {33, 84, 1, {5, 6, 7, 8, 9, 10, 11, 0, 0, 0, 0, 0, 0}},
#if CONFIG_11AC
    {128, 128, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
    {129, 129, 3, {50, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {130, 130, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
#endif
};

/** oper class table for EU*/
static oper_bw_chan oper_bw_chan_eu[] = {
    /** non-global oper class,global oper class, bandwidth, channel list*/
    {1, 115, 0, {36, 40, 44, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {2, 118, 0, {52, 56, 60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {3, 121, 0, {100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 0, 0}},
    {4, 81, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}},
    {5, 116, 1, {36, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {6, 119, 1, {52, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {7, 122, 1, {100, 108, 116, 124, 132, 0, 0, 0, 0, 0, 0, 0, 0}},
    {8, 117, 1, {40, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {9, 120, 1, {56, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {10, 123, 1, {104, 112, 120, 128, 136, 0, 0, 0, 0, 0, 0, 0, 0}},
    {11, 83, 1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0}},
    {12, 84, 1, {5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 0, 0, 0}},
    {17, 125, 0, {149, 153, 157, 161, 165, 169, 0, 0, 0, 0, 0, 0, 0}},
#if CONFIG_11AC
    {128, 128, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
    {129, 129, 3, {50, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {130, 130, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
#endif
};

/** oper class table for Japan*/
static oper_bw_chan oper_bw_chan_jp[] = {
    /** non-Global oper class,global oper class, bandwidth, channel list*/
    {1, 115, 0, {34, 38, 42, 46, 36, 40, 44, 48, 0, 0, 0, 0, 0}},
    {30, 81, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}},
    {31, 82, 0, {14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {32, 118, 0, {52, 56, 60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {33, 118, 0, {52, 56, 60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {34, 121, 0, {100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 0, 0}},
    {35, 121, 0, {100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 0, 0}},
    {36, 116, 1, {36, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {37, 119, 1, {52, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {38, 119, 1, {52, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {39, 122, 1, {100, 108, 116, 124, 132, 0, 0, 0, 0, 0, 0, 0, 0}},
    {40, 122, 1, {100, 108, 116, 124, 132, 0, 0, 0, 0, 0, 0, 0, 0}},
    {41, 117, 1, {40, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {42, 120, 1, {56, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {43, 120, 1, {56, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {44, 123, 1, {104, 112, 120, 128, 136, 0, 0, 0, 0, 0, 0, 0, 0}},
    {45, 123, 1, {104, 112, 120, 128, 136, 0, 0, 0, 0, 0, 0, 0, 0}},
    {56, 83, 1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0}},
    {57, 84, 1, {5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 0, 0, 0}},
    {58, 121, 0, {100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 0, 0}},
#if CONFIG_11AC
    {128, 128, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
    {129, 129, 3, {50, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {130, 130, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
#endif
};

/** oper class table for China*/
static oper_bw_chan oper_bw_chan_cn[] = {
    /** non-Global oper class,global oper class, bandwidth, channel list*/
    {1, 115, 0, {36, 40, 44, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {2, 118, 0, {52, 56, 60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {3, 125, 0, {149, 153, 157, 161, 165, 0, 0, 0, 0, 0, 0, 0, 0}},
    {4, 116, 1, {36, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {5, 119, 1, {52, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {6, 126, 1, {149, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {7, 81, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}},
    {8, 83, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0}},
    {9, 84, 1, {5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 0, 0, 0}},
#if CONFIG_11AC
    {128, 128, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
    {129, 129, 3, {50, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {130, 130, 2, {42, 58, 106, 122, 138, 155, 0, 0, 0, 0, 0, 0, 0}},
#endif
};

/** Country code for ETSI */
static t_u8 eu_country_code_table[][COUNTRY_CODE_LEN] = {
    "AL", "AD", "AT", "AU", "BY", "BE", "BA", "BG", "HR", "CY", "CZ", "DK", "EE", "FI", "FR", "MK",
    "DE", "GR", "HU", "IS", "IE", "IT", "KR", "LV", "LI", "LT", "LU", "MT", "MD", "MC", "ME", "NL",
    "NO", "PL", "RO", "RU", "SM", "RS", "SI", "SK", "ES", "SE", "CH", "TR", "UA", "UK", "GB", "NZ"};

/**
 * The structure for Channel-Frequency-Power table
 */
typedef struct _cfp_table
{
    /** Region or Code */
    t_u8 code;
    /** Frequency/Power */
    const chan_freq_power_t *cfp;
    /** No of CFP flag */
    int cfp_no;
} cfp_table_t;

/* Format { Channel, Frequency (MHz), MaxTxPower } */
/** Band: 'B/G', Region: USA FCC/Canada IC */
static const chan_freq_power_t channel_freq_power_US_BG[] = {
    {1, 2412, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {2, 2417, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {3, 2422, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {4, 2427, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {5, 2432, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {6, 2437, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {7, 2442, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {8, 2447, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {9, 2452, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {10, 2457, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {11, 2462, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}};

/** Band: 'B/G', Region: Europe ETSI/China */
static const chan_freq_power_t channel_freq_power_EU_BG[] = {
    {1, 2412, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},  {2, 2417, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {3, 2422, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},  {4, 2427, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {5, 2432, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},  {6, 2437, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {7, 2442, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},  {8, 2447, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {9, 2452, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},  {10, 2457, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {11, 2462, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE}, {12, 2467, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {13, 2472, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE}};

/** Band: 'B/G', Region: France */
static const chan_freq_power_t channel_freq_power_FR_BG[] = {
    {1, 2412, WLAN_TX_PWR_FR_100MW, (bool)MFALSE}, {2, 2417, WLAN_TX_PWR_FR_100MW, (bool)MFALSE},
    {3, 2422, WLAN_TX_PWR_FR_100MW, (bool)MFALSE}, {4, 2427, WLAN_TX_PWR_FR_100MW, (bool)MFALSE},
    {5, 2432, WLAN_TX_PWR_FR_100MW, (bool)MFALSE}, {6, 2437, WLAN_TX_PWR_FR_100MW, (bool)MFALSE},
    {7, 2442, WLAN_TX_PWR_FR_100MW, (bool)MFALSE}, {8, 2447, WLAN_TX_PWR_FR_100MW, (bool)MFALSE},
    {9, 2452, WLAN_TX_PWR_FR_100MW, (bool)MFALSE}, {10, 2457, WLAN_TX_PWR_FR_10MW, (bool)MFALSE},
    {11, 2462, WLAN_TX_PWR_FR_10MW, (bool)MFALSE}, {12, 2467, WLAN_TX_PWR_FR_10MW, (bool)MFALSE},
    {13, 2472, WLAN_TX_PWR_FR_10MW, (bool)MFALSE}};


/** Band: 'B/G', Region: Japan */
static const chan_freq_power_t channel_freq_power_JPN40_BG[] = {{14, 2484, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE}};


/** Band : 'B/G', Region: Special */
static const chan_freq_power_t channel_freq_power_SPECIAL_BG[] = {
    {1, 2412, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},  {2, 2417, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},
    {3, 2422, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},  {4, 2427, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},
    {5, 2432, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},  {6, 2437, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},
    {7, 2442, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},  {8, 2447, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},
    {9, 2452, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},  {10, 2457, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},
    {11, 2462, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE}, {12, 2467, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE},
    {13, 2472, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE}, {14, 2484, WLAN_TX_PWR_JP_BG_DEFAULT, (bool)MFALSE}};

/** Band : 'B/G', Region: World Wide Safe */
static chan_freq_power_t channel_freq_power_WW_BG[] = {
    {1, 2412, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},  {2, 2417, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {3, 2422, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},  {4, 2427, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {5, 2432, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},  {6, 2437, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {7, 2442, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},  {8, 2447, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {9, 2452, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},  {10, 2457, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {11, 2462, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {12, 2467, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {13, 2472, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE}};

/** Band : 'B/G', Region: Custom - Place holder for Max 14 channels (As defined in WWSM)*/
static chan_freq_power_t channel_freq_power_Custom_BG[] = {
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}};

static uint8_t rf_radio_modes_group[] = {
    0,  /* set the radio in power down mode */
    /*1,   sets the radio in 5GHz band, 2X2 mode(path A+B) */
    3,  /* sets the radio in 5GHz band, 1X1 mode(path A) */
   /* 4,   sets the radio in 5GHz band, 1X1 mode(path B) */
   /* 9,   sets the radio in 2.4GHz band, 2X2 mode(path A+B) */
    11, /* sets the radio in 2.4GHz band, 1X1 mode(path A) */
   /* 14, sets the radio in 2.4GHz band, 1X1 mode(path B)*/
};

/**
 * The 2.4GHz CFP tables
 */
static cfp_table_t cfp_table_BG[] = {
    {
        0x00, /* World Wide Safe */
        (chan_freq_power_t *)channel_freq_power_WW_BG,
        (int)(sizeof(channel_freq_power_WW_BG) / sizeof(chan_freq_power_t)),
    },
    {
        0x10, /* US FCC */
        (const chan_freq_power_t *)channel_freq_power_US_BG,
        (int)(sizeof(channel_freq_power_US_BG) / sizeof(chan_freq_power_t)),
    },
    {
        0x20, /* CANADA IC */
        (const chan_freq_power_t *)channel_freq_power_US_BG,
        (int)(sizeof(channel_freq_power_US_BG) / sizeof(chan_freq_power_t)),
    },
    {
        0x30, /* EU */
        (const chan_freq_power_t *)channel_freq_power_EU_BG,
        (int)(sizeof(channel_freq_power_EU_BG) / sizeof(chan_freq_power_t)),
    },
    {
        0x32, /* FRANCE */
        (const chan_freq_power_t *)channel_freq_power_FR_BG,
        (int)(sizeof(channel_freq_power_FR_BG) / sizeof(chan_freq_power_t)),
    },
    {
        0x40, /* JAPAN */
        (const chan_freq_power_t *)channel_freq_power_JPN40_BG,
        (int)(sizeof(channel_freq_power_JPN40_BG) / sizeof(chan_freq_power_t)),
    },
    {
        0x50, /* China */
        (const chan_freq_power_t *)channel_freq_power_EU_BG,
        (int)(sizeof(channel_freq_power_EU_BG) / sizeof(chan_freq_power_t)),
    },
    {
        0xff, /* Special */
        (const chan_freq_power_t *)channel_freq_power_SPECIAL_BG,
        (int)(sizeof(channel_freq_power_SPECIAL_BG) / sizeof(chan_freq_power_t)),
    },
    /* Add new region here */
};

/** Number of the CFP tables for 2.4GHz */
#define MLAN_CFP_TABLE_SIZE_BG (sizeof(cfp_table_BG) / sizeof(cfp_table_t))

#if CONFIG_5GHz_SUPPORT
/* Format { Channel, Frequency (MHz), MaxTxPower, DFS } */
/** Band: 'A', Region: USA FCC, Spain, France */
static const chan_freq_power_t channel_freq_power_A[] = {
    {36, 5180, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},  {40, 5200, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {44, 5220, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},  {48, 5240, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {52, 5260, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},   {56, 5280, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {60, 5300, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},   {64, 5320, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {100, 5500, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {104, 5520, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {108, 5540, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {112, 5560, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {116, 5580, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {120, 5600, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {124, 5620, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {128, 5640, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {132, 5660, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {136, 5680, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {140, 5700, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {144, 5720, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {149, 5745, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {153, 5765, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {157, 5785, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {161, 5805, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {165, 5825, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
#if CONFIG_UNII4_BAND_SUPPORT
    {169, 5845, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {173, 5865, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {177, 5885, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
#endif
};

/** Band: 'A', Region: Canada IC */
static const chan_freq_power_t channel_freq_power_CAN_A[] = {
    {36, 5180, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},  {40, 5200, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {44, 5220, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},  {48, 5240, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {52, 5260, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},   {56, 5280, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {60, 5300, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},   {64, 5320, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {100, 5500, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {104, 5520, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {108, 5540, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {112, 5560, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {116, 5580, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {132, 5660, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {136, 5680, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},  {140, 5700, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {144, 5720, WLAN_TX_PWR_US_DEFAULT, (bool)MTRUE},
    {149, 5745, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {153, 5765, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {157, 5785, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}, {161, 5805, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE},
    {165, 5825, WLAN_TX_PWR_US_DEFAULT, (bool)MFALSE}};

/** Band: 'A', Region: Europe ETSI */
static const chan_freq_power_t channel_freq_power_EU_A[] = {
    {36, 5180, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE}, {40, 5200, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {44, 5220, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE}, {48, 5240, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {52, 5260, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},  {56, 5280, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},
    {60, 5300, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},  {64, 5320, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},
    {100, 5500, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE}, {104, 5520, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},
    {108, 5540, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE}, {112, 5560, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},
    {116, 5580, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE}, {120, 5600, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},
    {124, 5620, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE}, {128, 5640, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},
    {132, 5660, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE}, {136, 5680, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE},
    {140, 5700, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MTRUE}, {149, 5745, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {153, 5765, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE}, {157, 5785, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE},
    {161, 5805, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE}, {165, 5825, WLAN_TX_PWR_EMEA_DEFAULT, (bool)MFALSE}};

/** Band: 'A', Region: Japan */
static const chan_freq_power_t channel_freq_power_JPN_A[] = {
    {36, 5180, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MFALSE}, {40, 5200, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MFALSE},
    {44, 5220, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MFALSE}, {48, 5240, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MFALSE},
    {52, 5260, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},  {56, 5280, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},
    {60, 5300, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},  {64, 5320, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},
    {100, 5500, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE}, {104, 5520, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},
    {108, 5540, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE}, {112, 5560, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},
    {116, 5580, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE}, {120, 5600, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},
    {124, 5620, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE}, {128, 5640, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},
    {132, 5660, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE}, {136, 5680, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE},
    {140, 5700, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE}, {144, 5720, WLAN_TX_PWR_JP_A_DEFAULT, (bool)MTRUE}};

/** Band: 'A', Region: China */
static const chan_freq_power_t channel_freq_power_CN_A[] = {
    {36, 5180, WLAN_TX_PWR_200MW, MFALSE}, {40, 5200, WLAN_TX_PWR_200MW, MFALSE},
    {44, 5220, WLAN_TX_PWR_200MW, MFALSE}, {48, 5240, WLAN_TX_PWR_200MW, MFALSE},
    {52, 5260, WLAN_TX_PWR_200MW, MTRUE},  {56, 5280, WLAN_TX_PWR_200MW, MTRUE},
    {60, 5300, WLAN_TX_PWR_200MW, MTRUE},  {64, 5320, WLAN_TX_PWR_200MW, MTRUE},
    {149, 5745, WLAN_TX_PWR_CN_2000MW, MFALSE}, {153, 5765, WLAN_TX_PWR_CN_2000MW, MFALSE},
    {157, 5785, WLAN_TX_PWR_CN_2000MW, MFALSE}, {161, 5805, WLAN_TX_PWR_CN_2000MW, MFALSE},
    {165, 5825, WLAN_TX_PWR_CN_2000MW, MFALSE}};


/** Band: 'A', Region: World Wide Safe */
static chan_freq_power_t channel_freq_power_WW_A[] = {
    {36, 5180, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {40, 5200, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {44, 5220, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {48, 5240, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {52, 5260, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},  {56, 5280, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {60, 5300, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},  {64, 5320, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {100, 5500, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE}, {104, 5520, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {108, 5540, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE}, {112, 5560, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {116, 5580, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE}, {120, 5600, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {124, 5620, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE}, {128, 5640, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {132, 5660, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE}, {136, 5680, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {140, 5700, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE}, {144, 5720, WLAN_TX_PWR_WW_DEFAULT, (bool)MTRUE},
    {149, 5745, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {153, 5765, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {157, 5785, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {161, 5805, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {165, 5825, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}};

/** Band: 'A', Region: Custom - Place holder for Max 25 channels (As defined in WWSM) */
static chan_freq_power_t channel_freq_power_Custom_A[] = {
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
#if CONFIG_UNII4_BAND_SUPPORT
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE}, {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
    {0, 0, WLAN_TX_PWR_WW_DEFAULT, (bool)MFALSE},
#endif
};


/** Band: 'A', Code: 1, Low band (5150-5250 MHz) channels */
static const chan_freq_power_t channel_freq_power_low_band[] = {
    {36, 5180, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
    {40, 5200, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
    {44, 5220, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
    {48, 5240, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
};

/** Band: 'A', Code: 2, Lower middle band (5250-5350 MHz) channels */
static const chan_freq_power_t channel_freq_power_lower_middle_band[] = {
    {52, 5260, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {56, 5280, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {60, 5300, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {64, 5320, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
};

/** Band: 'A', Code: 3, Upper middle band (5470-5725 MHz) channels */
static const chan_freq_power_t channel_freq_power_upper_middle_band[] = {
    {100, 5500, WLAN_TX_PWR_DEFAULT, (bool)MTRUE}, {104, 5520, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {108, 5540, WLAN_TX_PWR_DEFAULT, (bool)MTRUE}, {112, 5560, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {116, 5580, WLAN_TX_PWR_DEFAULT, (bool)MTRUE}, {120, 5600, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {124, 5620, WLAN_TX_PWR_DEFAULT, (bool)MTRUE}, {128, 5640, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {132, 5660, WLAN_TX_PWR_DEFAULT, (bool)MTRUE}, {136, 5680, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
    {140, 5700, WLAN_TX_PWR_DEFAULT, (bool)MTRUE},
};

/** Band: 'A', Code: 4, High band (5725-5850 MHz) channels */
static const chan_freq_power_t channel_freq_power_high_band[] = {{149, 5745, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
                                                                 {153, 5765, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
                                                                 {157, 5785, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
                                                                 {161, 5805, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
                                                                 {165, 5825, WLAN_TX_PWR_DEFAULT, (bool)MFALSE}};

/** Band: 'A', Code: 5, Low band (5150-5250 MHz) and
 * High band (5725-5850 MHz) channels */
static const chan_freq_power_t channel_freq_power_low_high_band[] = {
    {36, 5180, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},  {40, 5200, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
    {44, 5220, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},  {48, 5240, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
    {149, 5745, WLAN_TX_PWR_DEFAULT, (bool)MFALSE}, {153, 5765, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
    {157, 5785, WLAN_TX_PWR_DEFAULT, (bool)MFALSE}, {161, 5805, WLAN_TX_PWR_DEFAULT, (bool)MFALSE},
    {165, 5825, WLAN_TX_PWR_DEFAULT, (bool)MFALSE}};

/**
 * The 5GHz CFP tables
 */
static cfp_table_t cfp_table_A[] = {
    {
        0x00, /* World Wide Safe*/
        (chan_freq_power_t *)channel_freq_power_WW_A,
        (int)(sizeof(channel_freq_power_WW_A) / sizeof(chan_freq_power_t)),
    },
    {
        0x10, /* US FCC */
        (const chan_freq_power_t *)channel_freq_power_A,
        (int)(sizeof(channel_freq_power_A) / sizeof(chan_freq_power_t)),
    },
    {
        0x20, /* CANADA IC */
        (const chan_freq_power_t *)channel_freq_power_CAN_A,
        (int)(sizeof(channel_freq_power_CAN_A) / sizeof(chan_freq_power_t)),
    },
    {
        0x30, /* EU */
        (const chan_freq_power_t *)channel_freq_power_EU_A,
        (int)(sizeof(channel_freq_power_EU_A) / sizeof(chan_freq_power_t)),
    },
    {
        0x32, /* FRANCE */
        (const chan_freq_power_t *)channel_freq_power_A,
        (int)(sizeof(channel_freq_power_A) / sizeof(chan_freq_power_t)),
    },
    {
        0x40, /* JAPAN */
        (const chan_freq_power_t *)channel_freq_power_JPN_A,
        (int)((int)(sizeof(channel_freq_power_JPN_A) / sizeof(chan_freq_power_t))),
    },
    {
        0x50, /* China */
        (const chan_freq_power_t *)channel_freq_power_CN_A,
        (int)(sizeof(channel_freq_power_CN_A) / sizeof(chan_freq_power_t)),
    },
    {
        0xff, /* Special */
        (const chan_freq_power_t *)channel_freq_power_JPN_A,
        (int)(sizeof(channel_freq_power_JPN_A) / sizeof(chan_freq_power_t)),
    },
    {0x1, /* Low band (5150-5250 MHz) channels */
     (const chan_freq_power_t *)channel_freq_power_low_band,
     (int)(sizeof(channel_freq_power_low_band) / sizeof(chan_freq_power_t))},
    {0x2, /* Lower middle band (5250-5350 MHz) channels */
     (const chan_freq_power_t *)channel_freq_power_lower_middle_band,
     (int)(sizeof(channel_freq_power_lower_middle_band) / sizeof(chan_freq_power_t))},
    {0x3, /* Upper middle band (5470-5725 MHz) channels */
     (const chan_freq_power_t *)channel_freq_power_upper_middle_band,
     (int)(sizeof(channel_freq_power_upper_middle_band) / sizeof(chan_freq_power_t))},
    {0x4, /* High band (5725-5850 MHz) channels */
     (const chan_freq_power_t *)channel_freq_power_high_band,
     (int)(sizeof(channel_freq_power_high_band) / sizeof(chan_freq_power_t))},
    {0x5, /* Low band (5150-5250 MHz) and High band (5725-5850 MHz) channels */
     (const chan_freq_power_t *)channel_freq_power_low_high_band,
     (int)(sizeof(channel_freq_power_low_high_band) / sizeof(chan_freq_power_t))},
    /* Add new region here */
};

/** Number of the CFP tables for 5GHz */
#define MLAN_CFP_TABLE_SIZE_A (sizeof(cfp_table_A) / sizeof(cfp_table_t))

#endif /* CONFIG_5GHz_SUPPORT */

/********************************************************
    Global Variables
********************************************************/
/**
 * The table to keep region code
 */
t_u16 region_code_index[MRVDRV_MAX_REGION_CODE] = {0x00, 0x10, 0x20, 0x30, 0x32, 0x40, 0x41, 0x50, 0xfe, 0xff};


/** The table to keep CFP code for A */
t_u16 cfp_code_index_a[MRVDRV_MAX_CFP_CODE_A] = {0x1, 0x2, 0x3, 0x4, 0x5};

/**
 * The rates supported for ad-hoc B mode
 */
t_u8 AdhocRates_B[B_SUPPORTED_RATES] = {0x82, 0x84, 0x8b, 0x96, 0};

/**
 * The rates supported for ad-hoc G mode
 */
t_u8 AdhocRates_G[G_SUPPORTED_RATES] = {0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0};

/**
 * The rates supported for ad-hoc BG mode
 */
t_u8 AdhocRates_BG[BG_SUPPORTED_RATES] = {0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c, 0};

/**
 * The rates supported in A mode for ad-hoc
 */
t_u8 AdhocRates_A[A_SUPPORTED_RATES] = {0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0};

/**
 * The rates supported in A mode (used for BAND_A)
 */
t_u8 SupportedRates_A[A_SUPPORTED_RATES] = {0x0c, 0x12, 0x18, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0};

#if CONFIG_11N
/**
 * The rates supported by the card
 */
static t_u16 WlanDataRates[WLAN_SUPPORTED_RATES_EXT] = {
    0x02,  0x04, 0x0B, 0x16, 0x00, 0x0C, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6C, 0x90, 0x0D,
    0x1A,  0x27, 0x34, 0x4E, 0x68, 0x75, 0x82, 0x0C, 0x1B, 0x36, 0x51, 0x6C, 0xA2, 0xD8, 0xF3,
    0x10E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#else
/**
 * The rates supported by the card
 */
t_u16 WlanDataRates[WLAN_SUPPORTED_RATES] = {0x02, 0x04, 0x0B, 0x16, 0x00, 0x0C, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60,
                                             0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

/**
 * The rates supported in B mode
 */
t_u8 SupportedRates_B[B_SUPPORTED_RATES] = {0x02, 0x04, 0x0b, 0x16, 0};

/**
 * The rates supported in G mode (BAND_G, BAND_G|BAND_GN)
 */
t_u8 SupportedRates_G[G_SUPPORTED_RATES] = {0x0c, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c, 0};

/**
 * The rates supported in BG mode (BAND_B|BAND_G, BAND_B|BAND_G|BAND_GN)
 */
t_u8 SupportedRates_BG[BG_SUPPORTED_RATES] = {0x02, 0x04, 0x0b, 0x0c, 0x12, 0x16, 0x18,
                                              0x24, 0x30, 0x48, 0x60, 0x6c, 0};

/**
 * The rates supported in N mode
 */
t_u8 SupportedRates_N[N_SUPPORTED_RATES] = {0x02, 0x04, 0};

#if CONFIG_11AX
#define MCS_NUM_AX 12
/**
 * for MCS0/MCS1/MCS3/MCS4 have 4 additional DCM=1 value
 * note: the value in the table is 2 multiplier of the actual rate
 */
static t_u16 ax_mcs_rate_nss1[12][MCS_NUM_AX + 4] = {
    {0x90, 0x48, 0x120, 0x90, 0x1B0, 0x240, 0x120, 0x360, 0x1B0, 0x481, 0x511, 0x5A1, 0x6C1, 0x781, 0x871,
     0x962}, /*SG 160M*/
    {0x88, 0x44, 0x110, 0x88, 0x198, 0x220, 0x110, 0x330, 0x198, 0x440, 0x4C9, 0x551, 0x661, 0x716, 0x7F9,
     0x8DC}, /*MG 160M*/
    {0x7A, 0x3D, 0xF5, 0x7A, 0x16F, 0x1EA, 0xF5, 0x2DF, 0x16F, 0x3D4, 0x44E, 0x4C9, 0x5BE, 0x661, 0x72D,
     0x7F9}, /*LG 160M*/
    {0x48, 0x24, 0x90, 0x48, 0xD8, 0x120, 0x90, 0x1B0, 0xD8, 0x240, 0x288, 0x2D0, 0x360, 0x3C0, 0x438,
     0x4B0}, /*SG 80M*/
    {0x44, 0x22, 0x88, 0x44, 0xCC, 0x110, 0x88, 0x198, 0xCC, 0x220, 0x264, 0x2A8, 0x330, 0x38B, 0x3FC,
     0x46E},                                                                                                  /*MG 80M*/
    {0x3D, 0x1E, 0x7A, 0x3D, 0xB7, 0xF5, 0x7A, 0x16F, 0xB7, 0x1EA, 0x227, 0x264, 0x2DF, 0x330, 0x396, 0x3FC}, /*LG 80M*/
    {0x22, 0x11, 0x44, 0x22, 0x67, 0x89, 0x44, 0xCE, 0x67, 0x113, 0x135, 0x158, 0x19D, 0x1CA, 0x204, 0x23D},  /*SG 40M*/
    {0x20, 0x10, 0x41, 0x20, 0x61, 0x82, 0x41, 0xC3, 0x61, 0x104, 0x124, 0x145, 0x186, 0x1B1, 0x1E7, 0x21D},  /*MG 40M*/
    {0x1D, 0xE, 0x3A, 0x1D, 0x57, 0x75, 0x3A, 0xAF, 0x57, 0xEA, 0x107, 0x124, 0x15F, 0x186, 0x1B6, 0x1E7},    /*LG 40M*/
    {0x11, 0x8, 0x22, 0x11, 0x33, 0x44, 0x22, 0x67, 0x33, 0x89, 0x9A, 0xAC, 0xCE, 0xE5, 0x102, 0x11E},        /*SG 20M*/
    {0x10, 0x8, 0x20, 0x10, 0x30, 0x41, 0x20, 0x61, 0x30, 0x82, 0x92, 0xA2, 0xC3, 0xD8, 0xF3, 0x10E},         /*MG 20M*/
    {0xE, 0x7, 0x1D, 0xE, 0x2B, 0x3A, 0x1D, 0x57, 0x2B, 0x75, 0x83, 0x92, 0xAF, 0xC3, 0xDB, 0xF3}             /*LG 20M*/
};
#endif


/**
 *  @brief This function finds the CFP in
 *  		cfp_table_BG/A based on region/code and band parameter.
 *
 *  @param pmadapter  A pointer to mlan_adapter structure
 *  @param region     The region code
 *  @param band       The band
 *  @param cfp_no     A pointer to CFP number
 *
 *  @return           A pointer to CFP
 */
static const chan_freq_power_t *wlan_get_region_cfp_table(pmlan_adapter pmadapter, t_u8 region, t_u16 band, int *cfp_no)
{
    t_u32 i;
    t_u8 cfp_bg;
#if CONFIG_5GHz_SUPPORT
    t_u8 cfp_a;
#endif /* CONFIG_5GHz_SUPPORT */

    ENTER();

    cfp_bg = region;
#if CONFIG_5GHz_SUPPORT
    cfp_a = region;
#endif /* CONFIG_5GHz_SUPPORT */
    if (region == 0U || region == 0x40)
    {
        /* Invalid region code or Japan case, use CFP code */
        cfp_bg = pmadapter->cfp_code_bg;
#if CONFIG_5GHz_SUPPORT
        cfp_a = pmadapter->cfp_code_a;
#endif /* CONFIG_5GHz_SUPPORT */
    }

    if ((band & (BAND_B | BAND_G | BAND_GN | BAND_GAC)) != 0U)
    {
#ifdef OTP_CHANINFO
        /* Return the FW cfp table for requested region code, if available.
         * If region is not forced and the requested region code is different,
         * simply return the corresponding pre-defined table.
         */
        if ((pmadapter->otp_region != MNULL) && (pmadapter->cfp_otp_bg != MNULL))
        {
            if (pmadapter->otp_region->force_reg != (t_u16)MFALSE ||
                (cfp_bg == (t_u8)pmadapter->otp_region->region_code))
            {
                *cfp_no = (int)pmadapter->tx_power_table_bg_rows;
                LEAVE();
                return pmadapter->cfp_otp_bg;
            }
        }
#endif
        for (i = 0; i < MLAN_CFP_TABLE_SIZE_BG; i++)
        {
            PRINTM(MINFO, "cfp_table_BG[%d].code=%d\n", i, cfp_table_BG[i].code);
            /* Check if region/code matches for BG bands */
            if (cfp_table_BG[i].code == cfp_bg)
            {
                /* Select by band */
                *cfp_no = cfp_table_BG[i].cfp_no;
                LEAVE();
                return cfp_table_BG[i].cfp;
            }
        }
    }

#if CONFIG_5GHz_SUPPORT
    if ((band & (BAND_A | BAND_AN | BAND_AAC)) != 0U)
    {
#ifdef OTP_CHANINFO
        /* Return the FW cfp table for requested region code */
        if ((pmadapter->otp_region != MNULL) && (pmadapter->cfp_otp_a != MNULL))
        {
            if (pmadapter->otp_region->force_reg != (t_u16)MFALSE ||
                (cfp_a == (t_u8)pmadapter->otp_region->region_code))
            {
                *cfp_no = (int)pmadapter->tx_power_table_a_rows;
                LEAVE();
                return pmadapter->cfp_otp_a;
            }
        }
#endif
        for (i = 0; i < MLAN_CFP_TABLE_SIZE_A; i++)
        {
            PRINTM(MINFO, "cfp_table_A[%d].code=%d\n", i, cfp_table_A[i].code);
            /* Check if region/code matches for A bands */
            if (cfp_table_A[i].code == cfp_a)
            {
                /* Select by band */
                *cfp_no = cfp_table_A[i].cfp_no;
                LEAVE();
                return cfp_table_A[i].cfp;
            }
        }
    }
#endif /* CONFIG_5GHz_SUPPORT */

    if (region == 0U)
    {
#if CONFIG_5GHz_SUPPORT
        PRINTM(MERROR, "Error Band[0x%x] or code[BG:%#x, A:%#x]\n", band, cfp_bg, cfp_a);

#else
        PRINTM(MERROR, "Error Band[0x%x] or code[BG:%#x]\n", band, cfp_bg);
#endif /* CONFIG_5GHz_SUPPORT */
    }
    else
    {
        PRINTM(MERROR, "Error Band[0x%x] or region[%#x]\n", band, region);
    }

    LEAVE();
    return MNULL;
}

/********************************************************
    Global Functions
********************************************************/
/**
 *  @brief This function converts region string to integer code
 *
 *  @param pmadapter        A pointer to mlan_adapter structure
 *  @param country_code     Country string
 *  @param cfp_bg           Pointer to buffer
 *  @param cfp_a            Pointer to buffer
 *
 *  @return                 MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_misc_country_2_cfp_table_code(pmlan_adapter pmadapter, t_u8 *country_code, t_u8 *cfp_bg, t_u8 *cfp_a)
{
    t_u8 i;

    ENTER();

    /* Look for code in mapping table */
    for (i = 0; i < NELEMENTS(country_code_mapping); i++)
    {
        if (!__memcmp(pmadapter, country_code_mapping[i].country_code, country_code, COUNTRY_CODE_LEN - 1))
        {
            *cfp_bg = country_code_mapping[i].cfp_code_bg;
            *cfp_a  = country_code_mapping[i].cfp_code_a;
            LEAVE();
            return MLAN_STATUS_SUCCESS;
        }
    }

    LEAVE();
    return MLAN_STATUS_FAILURE;
}

#ifdef SD8801
/**
 *  @brief Use index to get the data rate
 *
 *  @param pmadapter   A pointer to mlan_adapter structure
 *  @param index       The index of data rate
 *  @param ht_info     HT info
 *
 *  @return            Data rate or 0
 */
t_u32 wlan_index_to_data_rate(pmlan_adapter pmadapter, t_u8 index, t_u8 ht_info)
{
#if CONFIG_11N
#define MCS_NUM_SUPP 8
    t_u16 mcs_num_supp              = MCS_NUM_SUPP;
    t_u16 mcs_rate[4][MCS_NUM_SUPP] = {{0x1b, 0x36, 0x51, 0x6c, 0xa2, 0xd8, 0xf3, 0x10e},  /*LG 40M*/
                                       {0x1e, 0x3c, 0x5a, 0x78, 0xb4, 0xf0, 0x10e, 0x12c}, /*SG 40M */
                                       {0x0d, 0x1a, 0x27, 0x34, 0x4e, 0x68, 0x75, 0x82},   /*LG 20M */
                                       {0x0e, 0x1c, 0x2b, 0x39, 0x56, 0x73, 0x82, 0x90}};  /*SG 20M */
#endif
    t_u32 rate = 0;

    ENTER();

#if CONFIG_11N
    if (ht_info & MBIT(0))
    {
        if (index == MLAN_RATE_BITMAP_MCS0)
        {
            if (ht_info & MBIT(2))
                rate = 0x0D; /* MCS 32 SGI rate */
            else
                rate = 0x0C; /* MCS 32 LGI rate */
        }
        else if (index < mcs_num_supp)
        {
            if (ht_info & MBIT(1))
            {
                if (ht_info & MBIT(2))
                    rate = mcs_rate[1][index]; /* SGI, 40M */
                else
                    rate = mcs_rate[0][index]; /* LGI, 40M */
            }
            else
            {
                if (ht_info & MBIT(2))
                    rate = mcs_rate[3][index]; /* SGI, 20M */
                else
                    rate = mcs_rate[2][index]; /* LGI, 20M */
            }
        }
        else
            rate = WlanDataRates[0];
    }
    else
#endif /* ENABLE_802_11N */
    {
#if CONFIG_11N
        /* 11n non HT rates */
        if (index >= WLAN_SUPPORTED_RATES_EXT)
        {
#else
        if (index >= WLAN_SUPPORTED_RATES)
        {
#endif
            index = 0;
        }
        rate = WlanDataRates[index];
    }
    LEAVE();
    return rate;
}
#else
/**
 *  @brief Use index to get the data rate
 *
 *  @param pmadapter        A pointer to mlan_adapter structure
 *  @param index            The index of data rate
 *  @param tx_rate_info     Tx rate info
 *  @param ext_rate_info    Extend tx rate info
 *  @return                 Data rate or 0
 */
t_u32 wlan_index_to_data_rate(pmlan_adapter pmadapter,
                              t_u8 index,
                              t_u8 tx_rate_info
#if CONFIG_11AX
                              ,
                              t_u8 ext_rate_info
#endif
)
{
#if CONFIG_11N
#define MCS_NUM_SUPP 8U
    t_u16 mcs_rate[4][MCS_NUM_SUPP] = {{0x1b, 0x36, 0x51, 0x6c, 0xa2, 0xd8, 0xf3, 0x10e},  /*LG 40M*/
                                       {0x1e, 0x3c, 0x5a, 0x78, 0xb4, 0xf0, 0x10e, 0x12c}, /*SG 40M */
                                       {0x0d, 0x1a, 0x27, 0x34, 0x4e, 0x68, 0x75, 0x82},   /*LG 20M */
                                       {0x0e, 0x1c, 0x2b, 0x39, 0x56, 0x73, 0x82, 0x90}};  /*SG 20M */
#endif

#if CONFIG_11AC
#define MCS_NUM_AC 10
    /* NSS 1. note: the value in the table is 2 multiplier of the actual rate
     *              in other words, it is in the unit of 500 Kbs
     */
    t_u16 ac_mcs_rate_nss1[8][MCS_NUM_AC] = {
        {0x75, 0xEA, 0x15F, 0x1D4, 0x2BE, 0x3A8, 0x41D, 0x492, 0x57C, 0x618},  /* LG 160M*/
        {0x82, 0x104, 0x186, 0x208, 0x30C, 0x410, 0x492, 0x514, 0x618, 0x6C6}, /* SG 160M*/
        {0x3B, 0x75, 0xB0, 0xEA, 0x15F, 0x1D4, 0x20F, 0x249, 0x2BE, 0x30C},    /* LG 80M */
        {0x41, 0x82, 0xC3, 0x104, 0x186, 0x208, 0x249, 0x28A, 0x30C, 0x363},   /* SG 80M */
        {0x1B, 0x36, 0x51, 0x6C, 0xA2, 0xD8, 0xF3, 0x10E, 0x144, 0x168},       /* LG 40M */
        {0x1E, 0x3C, 0x5A, 0x78, 0xB4, 0xF0, 0x10E, 0x12C, 0x168, 0x190},      /* SG 40M */
        {0xD, 0x1A, 0x27, 0x34, 0x4E, 0x68, 0x75, 0x82, 0x9C, 0x00},           /* LG 20M */
        {0xF, 0x1D, 0x2C, 0x3A, 0x57, 0x74, 0x82, 0x91, 0xAE, 0x00},           /* SG 20M */
    };
#endif /* CONFIG_11AC */

    t_u32 rate     = 0;
#if (CONFIG_11AC) || (CONFIG_11AX)
    t_u8 mcs_index = 0;
#endif
#if CONFIG_11AX
    t_u8 he_dcm    = 0;
    t_u8 stbc      = 0;
#endif
    t_u8 bw        = 0;
    t_u8 gi        = 0;
    ENTER();

#if CONFIG_11AC
    if ((tx_rate_info & 0x3U) == (t_u8)MLAN_RATE_FORMAT_VHT)
    {
        /* VHT rate */
        mcs_index = index & 0xFU;

        if (mcs_index > 9U)
        {
            mcs_index = 9U;
        }

        /* 20M: bw=0, 40M: bw=1, 80M: bw=2, 160M: bw=3 */
        bw = (tx_rate_info & 0xCU) >> 2U;
        /* LGI: gi =0, SGI: gi = 1 */
        gi = (tx_rate_info & 0x10U) >> 4U;
        {
            /* NSS = 1 */
            rate = ac_mcs_rate_nss1[2U * (3U - bw) + gi][mcs_index];
        }
    }
    else
#endif
#if CONFIG_11AX
        if ((mlan_rate_format)(tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE)
    {
        /* HE rate */
        mcs_index = index & 0xF;
        he_dcm    = ext_rate_info & MBIT(0);
        if (mcs_index > MCS_NUM_AX - 1)
            mcs_index = MCS_NUM_AX - 1;
        /* 20M: bw=0, 40M: bw=1, 80M: bw=2, 160M: bw=3 */
        bw = (tx_rate_info & (MBIT(3) | MBIT(2))) >> 2;
        /* BIT7:BIT4 0:0= 0.8us,0:1= 0.8us, 1:0=1.6us, 1:1=3.2us or 0.8us */
        gi = (tx_rate_info & MBIT(4)) >> 4 | (tx_rate_info & MBIT(7)) >> 6;
        /* STBC: BIT5 in tx rate info */
        stbc = (tx_rate_info & MBIT(5)) >> 5;
        if (gi > 3)
        {
            PRINTM(MERROR, "Invalid gi value");
            return 0;
        }
        if ((gi == 3) && stbc && he_dcm)
        {
            gi     = 0;
            stbc   = 0;
            he_dcm = 0;
        }
        /* map to gi 0:0.8us,1:1.6us 2:3.2us*/
        if (gi > 0)
            gi = gi - 1;
        switch (mcs_index)
        {
            case 0:
            case 1:
                rate = ax_mcs_rate_nss1[3 * (3 - bw) + gi][mcs_index * 2 + he_dcm];
                break;
            case 2:
                rate = ax_mcs_rate_nss1[3 * (3 - bw) + gi][mcs_index * 2];
                break;
            case 3:
            case 4:
                rate = ax_mcs_rate_nss1[3 * (3 - bw) + gi][mcs_index * 2 - 1 + he_dcm];
                break;
            default:
                rate = ax_mcs_rate_nss1[3 * (3 - bw) + gi][mcs_index + 4];
                break;
        }
    }
    else
#endif
#if CONFIG_11N
        if ((tx_rate_info & 0x3U) == (t_u8)MLAN_RATE_FORMAT_HT)
    {
        /* HT rate */
        /* 20M: bw=0, 40M: bw=1 */
        bw = (tx_rate_info & 0xCU) >> 2U;
        /* LGI: gi =0, SGI: gi = 1 */
        gi = (tx_rate_info & 0x10U) >> 4U;
        if (index == MLAN_RATE_BITMAP_MCS0)
        {
            if (gi == 1U)
            {
                rate = 0x0D; /* MCS 32 SGI rate */
            }
            else
            {
                rate = 0x0C; /* MCS 32 LGI rate */
            }
        }
        else if (index < MCS_NUM_SUPP)
        {
            if (bw <= 1U)
            {
                rate = mcs_rate[2U * (1U - bw) + gi][index];
            }
            else
            {
                rate = WlanDataRates[0];
            }
        }
        else
        {
            rate = WlanDataRates[0];
        }
    }
    else
#endif /* CONFIG_11N */
    {
#if CONFIG_11N
        /* 11n non HT rates */
        if (index >= WLAN_SUPPORTED_RATES_EXT)
        {
#else
        if (index >= WLAN_SUPPORTED_RATES)
        {
#endif
            index = 0;
        }
        rate = WlanDataRates[index];
    }
    LEAVE();
    return rate;
}
#endif


/**
 *  @brief Get active data rates
 *
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param bss_mode         The specified BSS mode (Infra/IBSS)
 *  @param config_bands     The specified band configuration
 *  @param rates            The buf to return the active rates
 *
 *  @return                 The number of Rates
 */
t_u32 wlan_get_active_data_rates(mlan_private *pmpriv,
                                 mlan_bss_mode bss_mode,
                                 t_u16 config_bands,
                                 WLAN_802_11_RATES rates)
{
    t_u32 k;

    ENTER();

    if (pmpriv->media_connected != MTRUE)
    {
        k = wlan_get_supported_rates(pmpriv, bss_mode, config_bands, rates);
    }
    else
    {
        k = wlan_copy_rates(rates, 0, pmpriv->curr_bss_params.data_rates, (int)pmpriv->curr_bss_params.num_of_rates);
    }

    LEAVE();
    return k;
}

#ifdef STA_SUPPORT
/**
 *  @brief This function search through all the regions cfp table to find the channel,
 *            if the channel is found then gets the MIN txpower of the channel
 *            present in all the regions.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param channel      Channel number.
 *
 *  @return             The Tx power
 */
t_u8 wlan_get_txpwr_of_chan_from_cfp(mlan_private *pmpriv, t_u8 channel)
{
    t_u8 i        = 0;
    t_u8 j        = 0;
    t_u8 tx_power = 0;
    t_u32 cfp_no;
    const chan_freq_power_t *cfp = MNULL;
#if CONFIG_5GHz_SUPPORT
    const chan_freq_power_t *cfp_a = MNULL;
    t_u32 cfp_no_a;
#endif /* CONFIG_5GHz_SUPPORT */

    ENTER();

    for (i = 0; i < MLAN_CFP_TABLE_SIZE_BG; i++)
    {
        /* Get CFP */
        cfp    = cfp_table_BG[i].cfp;
        cfp_no = (t_u32)cfp_table_BG[i].cfp_no;
        /* Find matching channel and get Tx power */
        for (j = 0; j < cfp_no; j++)
        {
            if ((cfp + j)->channel == channel)
            {
                if (tx_power != 0U)
                {
                    tx_power = MIN(tx_power, (t_u8)((cfp + j)->max_tx_power));
                }
                else
                {
                    tx_power = (t_u8)(cfp + j)->max_tx_power;
                }
                break;
            }
        }
    }

#if CONFIG_5GHz_SUPPORT
    for (i = 0; i < MLAN_CFP_TABLE_SIZE_A; i++)
    {
        /* Get CFP */
        cfp_a    = cfp_table_A[i].cfp;
        cfp_no_a = (t_u32)cfp_table_A[i].cfp_no;
        for (j = 0; j < cfp_no_a; j++)
        {
            if ((cfp_a + j)->channel == channel)
            {
                if (tx_power != 0U)
                {
                    tx_power = MIN(tx_power, (t_u8)((cfp_a + j)->max_tx_power));
                }
                else
                {
                    tx_power = (t_u8)((cfp_a + j)->max_tx_power);
                }
                break;
            }
        }
    }
#endif /* CONFIG_5GHz_SUPPORT */

    LEAVE();
    return tx_power;
}

/**
 *  @brief Get the channel frequency power info for a specific channel
 *
 *  @param pmadapter            A pointer to mlan_adapter structure
 *  @param band                 It can be BAND_A, BAND_G or BAND_B
 *  @param channel              The channel to search for
 *  @param region_channel       A pointer to region_chan_t structure
 *
 *  @return                     A pointer to chan_freq_power_t structure or MNULL if not found.
 */

const chan_freq_power_t *wlan_get_cfp_by_band_and_channel(pmlan_adapter pmadapter,
                                                          t_u16 band,
                                                          t_u16 channel,
                                                          region_chan_t *region_channel)
{
    region_chan_t *rc;
    const chan_freq_power_t *cfp = MNULL;
    t_u8 i, j;

    ENTER();

    j = 0;
    while (cfp == MNULL && (j < MAX_REGION_CHANNEL_NUM))
    {
        bool continue_loop = MFALSE;
        rc                 = &region_channel[j];

        if (rc->valid == (t_u8)MFALSE || rc->pcfp == MNULL)
        {
            j++;
            continue;
        }
        switch (rc->band)
        {
            case BAND_A:
                switch (band)
                {
                    case BAND_AN:
                    case BAND_A | BAND_AN:
                    case BAND_A | BAND_AN | BAND_AAC:
                    case BAND_A: /* Matching BAND_A */
                        break;

                    default:
                        j++;
                        continue_loop = MTRUE;
                        break;
                }
                break;
            case BAND_B:
            case BAND_G:
                switch (band)
                {
                    case BAND_GN:
                    case BAND_B | BAND_G | BAND_GN:
                    case BAND_G | BAND_GN:
                    case BAND_GN | BAND_GAC:
                    case BAND_B | BAND_G | BAND_GN | BAND_GAC:
                    case BAND_G | BAND_GN | BAND_GAC:
                    case BAND_B | BAND_G:
                    case BAND_B: /* Matching BAND_B/G */
                    case BAND_G:
                        break;
                    default:
                        j++;
                        continue_loop = MTRUE;
                        break;
                }
                break;
            default:
                j++;
                continue_loop = MTRUE;
                break;
        }
        if (continue_loop == MTRUE)
        {
            continue;
        }
        if (channel == FIRST_VALID_CHANNEL)
        {
            cfp = &rc->pcfp[0];
        }
        else
        {
            for (i = 0; i < rc->num_cfp; i++)
            {
                if (rc->pcfp[i].channel == channel)
                {
                    cfp = &rc->pcfp[i];
                    break;
                }
            }
        }
        j++;
    }

    if (cfp == MNULL && channel != 0U)
    {
        PRINTM(MERROR,
               "wlan_get_cfp_by_band_and_channel(): cannot find "
               "cfp by band %d & channel %d\n",
               band, channel);
    }

    LEAVE();
    return cfp;
}

/**
 *  @brief Find the channel frequency power info for a specific channel
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param band         It can be BAND_A, BAND_G or BAND_B
 *  @param channel      The channel to search for
 *
 *  @return             A pointer to chan_freq_power_t structure or MNULL if not found.
 */
const chan_freq_power_t *wlan_find_cfp_by_band_and_channel(mlan_adapter *pmadapter, t_u16 band, t_u16 channel)
{
    const chan_freq_power_t *cfp = MNULL;

    ENTER();

    /* Any station(s) with 11D enabled */
    if (wlan_count_priv_cond(pmadapter, wlan_11d_is_enabled, wlan_is_station) > 0)
    {
        cfp = wlan_get_cfp_by_band_and_channel(pmadapter, band, channel, pmadapter->universal_channel);
    }
    else
    {
        cfp = wlan_get_cfp_by_band_and_channel(pmadapter, band, channel, pmadapter->region_channel);
    }

    LEAVE();
    return cfp;
}

/**
 *  @brief Find the channel frequency power info for a specific frequency
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param band         It can be BAND_A, BAND_G or BAND_B
 *  @param freq         The frequency to search for
 *
 *  @return         Pointer to chan_freq_power_t structure; MNULL if not found
 */
const chan_freq_power_t *wlan_find_cfp_by_band_and_freq(mlan_adapter *pmadapter, t_u16 band, t_u32 freq)
{
    const chan_freq_power_t *cfp = MNULL;
    region_chan_t *rc;
    t_u8 i, j;

    ENTER();

    j = 0;
    while (cfp == MNULL && (j < MAX_REGION_CHANNEL_NUM))
    {
        bool continue_loop = MFALSE;
        rc                 = &pmadapter->region_channel[j];

        /* Any station(s) with 11D enabled */
        if (wlan_count_priv_cond(pmadapter, wlan_11d_is_enabled, wlan_is_station) > 0)
        {
            rc = &pmadapter->universal_channel[j];
        }

        if (rc->valid == 0U || rc->pcfp == MNULL)
        {
            j++;
            continue;
        }
        switch (rc->band)
        {
            case BAND_A:
                switch (band)
                {
                    case BAND_AN:
                    case BAND_A | BAND_AN:
                    case BAND_A | BAND_AN | BAND_AAC:
                    case BAND_A: /* Matching BAND_A */
                        break;
                    default:
                        j++;
                        continue_loop = MTRUE;
                        break;
                }
                break;
            case BAND_B:
            case BAND_G:
                switch (band)
                {
                    case BAND_GN:
                    case BAND_B | BAND_G | BAND_GN:
                    case BAND_G | BAND_GN:
                    case BAND_GN | BAND_GAC:
                    case BAND_B | BAND_G | BAND_GN | BAND_GAC:
                    case BAND_G | BAND_GN | BAND_GAC:
                    case BAND_B | BAND_G:
                    case BAND_B:
                    case BAND_G:
                        break;
                    default:
                        j++;
                        continue_loop = MTRUE;
                        break;
                }
                break;
            default:
                j++;
                continue_loop = MTRUE;
                break;
        }
        if (continue_loop == MTRUE)
        {
            continue;
        }
        for (i = 0; i < rc->num_cfp; i++)
        {
            if (rc->pcfp[i].freq == freq)
            {
                cfp = &rc->pcfp[i];
                break;
            }
        }
        j++;
    }

    if (cfp == MNULL && freq != 0U)
    {
        PRINTM(MERROR,
               "wlan_find_cfp_by_band_and_freq(): cannot find cfp by "
               "band %d & freq %d\n",
               band, freq);
    }

    LEAVE();
    return cfp;
}
#endif /* STA_SUPPORT */

/**
 *  @brief Check if Rate Auto
 *
 *  @param pmpriv               A pointer to mlan_private structure
 *
 *  @return                     MTRUE or MFALSE
 */
bool wlan_is_rate_auto(mlan_private *pmpriv)
{
    t_u32 i;
    int rate_num = 0;

    ENTER();

    for (i = 0; i < NELEMENTS(pmpriv->bitmap_rates); i++)
    {
        if (pmpriv->bitmap_rates[i] != 0U)
        {
            rate_num++;
        }
    }

    LEAVE();
    if (rate_num > 1)
    {
        return MTRUE;
    }
    else
    {
        return MFALSE;
    }
}

/**
 *  @brief Covert Rate Bitmap to Rate index
 *
 *  @param pmadapter    Pointer to mlan_adapter structure
 *  @param rate_bitmap  Pointer to rate bitmap
 *  @param size         Size of the bitmap array
 *
 *  @return             Rate index
 */
int wlan_get_rate_index(pmlan_adapter pmadapter, t_u16 *rate_bitmap, int size)
{
    int i;

    ENTER();

    for (i = 0; i < size * 8; i++)
    {
        if ((rate_bitmap[i / 16] & ((t_u32)1U << ((t_u32)i % 16U))) != 0U)
        {
            LEAVE();
            return i;
        }
    }

    LEAVE();
    return -1;
}

/**
 *  @brief Convert config_bands to B/G/A band
 *
 *  @param config_bands     The specified band configuration
 *
 *  @return                 BAND_B|BAND_G|BAND_A
 */
t_u16 wlan_convert_config_bands(t_u16 config_bands)
{
    t_u16 bands = 0;
    if (config_bands & BAND_B)
        bands |= BAND_B;
    if (config_bands & BAND_G || config_bands & BAND_GN
#ifdef ENABLE_802_11AC
        || config_bands & BAND_GAC
#endif
#ifdef ENABLE_802_11AX
        || config_bands & BAND_GAX
#endif
    )
        bands |= BAND_G;
    if (config_bands & BAND_A || config_bands & BAND_AN
#ifdef ENABLE_802_11AC
        || config_bands & BAND_AAC
#endif
#ifdef ENABLE_802_11AX
        || config_bands & BAND_AAX
#endif
    )
        bands |= BAND_A;
    return bands;
}

/**
 *  @brief Get supported data rates
 *
 *  @param pmpriv           A pointer to mlan_private structure
 *  @param bss_mode         The specified BSS mode (Infra/IBSS)
 *  @param config_bands     The specified band configuration
 *  @param rates            The buf to return the supported rates
 *
 *  @return                 The number of Rates
 */
t_u32 wlan_get_supported_rates(mlan_private *pmpriv,
                               mlan_bss_mode bss_mode,
                               t_u16 config_bands,
                               WLAN_802_11_RATES rates)
{
    t_u32 k     = 0;
    t_u16 bands = 0;

    ENTER();

    bands = wlan_convert_config_bands(config_bands);
    if (bss_mode == MLAN_BSS_MODE_INFRA)
    {
        /* Infra. mode */
        if (bands == BAND_B)
        {
            /* B only */
            k = wlan_copy_rates(rates, k, SupportedRates_B, sizeof(SupportedRates_B));
        }
        else if (bands == BAND_G)
        {
            /* G only */
            k = wlan_copy_rates(rates, k, SupportedRates_G, sizeof(SupportedRates_G));
        }
        else if (bands & (BAND_B | BAND_G))
        {
            /* BG only */
                k = wlan_copy_rates(rates, k, SupportedRates_BG, sizeof(SupportedRates_BG));
        }
        else if (bands & BAND_A)
        {
            /* support A */
            k = wlan_copy_rates(rates, k, SupportedRates_A, sizeof(SupportedRates_A));
        }
    }
    else
    {
        /* Adhoc. mode */
        if (bands == BAND_B)
        {
            /* B only */
            k = wlan_copy_rates(rates, k, AdhocRates_B, sizeof(AdhocRates_B));
        }
        else if (bands == BAND_G)
        {
            /* G only */
            k = wlan_copy_rates(rates, k, AdhocRates_G, sizeof(AdhocRates_G));
        }
        else if (bands & BAND_A)
        {
            /* support A */
            k = wlan_copy_rates(rates, k, AdhocRates_A, sizeof(AdhocRates_A));
        }
        else
        {
            k = wlan_copy_rates(rates, k, AdhocRates_BG, sizeof(AdhocRates_BG));
        }
    }

    LEAVE();
    return k;
}

/**
 *  @brief This function sets region table.
 *
 *  @param pmpriv  A pointer to mlan_private structure
 *  @param region  The region code
 *  @param band    The band
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_set_regiontable(mlan_private *pmpriv, t_u8 region, t_u16 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int i                   = 0;
    const chan_freq_power_t *cfp;
    int cfp_no;

    ENTER();

    (void)__memset(pmadapter, pmadapter->region_channel, 0, sizeof(pmadapter->region_channel));

    if ((band & (BAND_B | BAND_G | BAND_GN)) != 0U)
    {
        cfp = wlan_get_region_cfp_table(pmadapter, region, (BAND_G | BAND_B | BAND_GN), &cfp_no);
        if (cfp != MNULL)
        {
            pmadapter->region_channel[i].num_cfp = (t_u8)cfp_no;
            pmadapter->region_channel[i].pcfp    = cfp;
        }
        else
        {
            PRINTM(MERROR, "wrong region code %#x in Band B-G\n", region);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmadapter->region_channel[i].valid  = (t_u8)MTRUE;
        pmadapter->region_channel[i].region = region;
        if ((band & BAND_GN) != 0U)
        {
            pmadapter->region_channel[i].band = BAND_G;
        }
        else
        {
            pmadapter->region_channel[i].band = (band & BAND_G) != 0U ? BAND_G : BAND_B;
        }
        i++;
    }
#if CONFIG_5GHz_SUPPORT
    if ((band & (BAND_A | BAND_AN | BAND_AAC)) != 0U)
    {
        cfp = wlan_get_region_cfp_table(pmadapter, region, BAND_A, &cfp_no);
        if (cfp != MNULL)
        {
            pmadapter->region_channel[i].num_cfp = (t_u8)cfp_no;
            pmadapter->region_channel[i].pcfp    = cfp;
        }
        else
        {
            PRINTM(MERROR, "wrong region code %#x in Band A\n", region);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmadapter->region_channel[i].valid  = (t_u8)MTRUE;
        pmadapter->region_channel[i].region = region;
        pmadapter->region_channel[i].band   = BAND_A;
    }
#endif /* CONFIG_5GHz_SUPPORT */

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Get if radar detection is enabled or not on a certain channel
 *
 *  @param priv    Private driver information structure
 *  @param chnl Channel to determine radar detection requirements
 *
 *  @return
 *    - MTRUE if radar detection is required
 *    - MFALSE otherwise
 */
t_bool wlan_get_cfp_radar_detect(mlan_private *priv, t_u8 chnl)
{
    t_u8 i, j;
    t_bool required               = MFALSE;
    const chan_freq_power_t *pcfp = MNULL;

    ENTER();

    /* get the cfp table first */
    for (i = 0; i < MAX_REGION_CHANNEL_NUM; i++)
    {
        if (priv->adapter->region_channel[i].band == BAND_A)
        {
            pcfp = priv->adapter->region_channel[i].pcfp;
            break;
        }
    }

    if (pcfp == MNULL)
    {
        /* This means operation in BAND-A is not support, we can just return
           false here, it's harmless */
        goto done;
    }

    /* get the radar detection requirements according to chan num */
    for (j = 0; j < priv->adapter->region_channel[i].num_cfp; j++)
    {
        if (pcfp[j].channel == chnl)
        {
            required = pcfp[j].passive_scan_or_radar_detect;
            break;
        }
    }

done:
    LEAVE();
    return required;
}

/**
 *  @brief Get if scan type is passive or not on a certain channel for b/g band
 *
 *  @param priv    Private driver information structure
 *  @param chnl Channel to determine scan type
 *
 *  @return
 *    - MTRUE if scan type is passive
 *    - MFALSE otherwise
 */

t_bool wlan_bg_scan_type_is_passive(mlan_private *priv, t_u8 chnl)
{
    t_u8 i, j;
    t_bool passive                = MFALSE;
    const chan_freq_power_t *pcfp = MNULL;

    ENTER();

    /* get the cfp table first */
    for (i = 0; i < MAX_REGION_CHANNEL_NUM; i++)
    {
        if ((priv->adapter->region_channel[i].band & (BAND_B | BAND_G)) != 0U)
        {
            pcfp = priv->adapter->region_channel[i].pcfp;
            break;
        }
    }

    if (pcfp == MNULL)
    {
        /* This means operation in BAND-B or BAND_G is not support, we can
           just return false here */
        goto done;
    }

    /* get the bg scan type according to chan num */
    for (j = 0; j < priv->adapter->region_channel[i].num_cfp; j++)
    {
        if (pcfp[j].channel == chnl)
        {
            passive = pcfp[j].passive_scan_or_radar_detect;
            break;
        }
    }

done:
    LEAVE();
    return passive;
}

/**
 * @bried convert ht_info to rate_info
 *
 * @param ht_info	ht_info
 *
 * @return		rate_info
 */
t_u8 wlan_convert_v14_rate_ht_info(t_u8 ht_info)
{
    t_u8 rate_info = 0;
    rate_info      = ht_info & 0x01U;
    /* band */
    rate_info |= (ht_info & MBIT(1)) << 1;
    /* Short GI */
    rate_info |= (ht_info & MBIT(2)) << 1;
    return rate_info;
}

/**
 * @brief Validate if channel is in range of World Wide Safe Mode
 *
 * @param chan_num	Channel Number
 *
 * @return		Valid or Invalid
 */
t_bool wlan_is_channel_valid(t_u8 chan_num)
{
    t_bool valid = MFALSE;
    int i        = 0;
    chan_freq_power_t *cfp_wwsm;
    int cfp_no = 0;

    ENTER();

    cfp_wwsm = (chan_freq_power_t *)channel_freq_power_WW_BG;
    cfp_no   = (int)(sizeof(channel_freq_power_WW_BG) / sizeof(chan_freq_power_t));

    for (i = 0; i < cfp_no; i++)
    {
        /* Channel 0 is invalid */
        if (chan_num == 0U)
        {
            PRINTM(MERROR, "Invalid channel. Channel number can't be %d\r\n", chan_num);
            valid = MFALSE;
            break;
        }

        if (chan_num == cfp_wwsm[i].channel)
        {
            valid = MTRUE;
            break;
        }
    }

#if CONFIG_5GHz_SUPPORT
    if (!valid)
    {
        cfp_wwsm = (chan_freq_power_t *)channel_freq_power_WW_A;
        cfp_no   = (int)(sizeof(channel_freq_power_WW_A) / sizeof(chan_freq_power_t));

        for (i = 0; i < cfp_no; i++)
        {
            /* Channel 0 is invalid */
            if (chan_num == 0U)
            {
                PRINTM(MERROR, "Invalid channel. Channel number can't be %d\r\n", chan_num);
                valid = MFALSE;
                break;
            }

            if (chan_num == cfp_wwsm[i].channel)
            {
                valid = MTRUE;
                break;
            }
        }
    }
#endif

    LEAVE();
    return valid;
}

/**
 * @brief Validate if channel is in range of region table
 *
 * @param priv    Private driver information structure
 * @param chan_num	Channel Number
 *
 * @return		Valid or Invalid
 */
t_bool wlan_check_channel_by_region_table(mlan_private *pmpriv, t_u8 chan_num)
{
    t_bool valid = MFALSE;
    int i        = 0;
    mlan_adapter *pmadapter = pmpriv->adapter;
    const chan_freq_power_t *cfp = pmadapter->region_channel[0].pcfp;
    int cfp_no  = pmadapter->region_channel[0].num_cfp;

    ENTER();

    if(NULL == cfp)
        return MFALSE;

    /* Channel 0 is invalid */
    if (chan_num == 0)
    {
        PRINTM(MERROR, "Invalid channel. Channel number can't be %d\r\n", chan_num);
        valid = MFALSE;
        return valid;
    }

    for (i = 0; i < cfp_no; i++)
    {
        if (chan_num == cfp[i].channel)
        {
            valid = MTRUE;
            break;
        }
    }

#if CONFIG_5GHz_SUPPORT
    if (!valid)
    {
        cfp = pmadapter->region_channel[1].pcfp;
        cfp_no   = pmadapter->region_channel[1].num_cfp;

        if(NULL == cfp)
            return MFALSE;

        for (i = 0; i < cfp_no; i++)
        {
            if (chan_num == cfp[i].channel)
            {
                valid = MTRUE;
                break;
            }
        }
    }
#endif

    LEAVE();
    return valid;
}

/**
 * @brief Validate if radio mode is in range of World Wide Safe Mode
 *
 * @param mode	radio mode
 *
 * @return		Valid or Invalid
 */
t_bool wlan_is_radio_mode_valid(t_u8 mode)
{
    t_bool valid = MFALSE;
    int i        = 0;
    int mode_num;

    ENTER();
    mode_num = (sizeof(rf_radio_modes_group) / sizeof(rf_radio_modes_group[0]));

    for (i = 0; i < mode_num; i++)
    {
        if (mode == rf_radio_modes_group[i])
        {
            valid = MTRUE;
            break;
        }
    }

    if (valid == MFALSE)
    {
        PRINTF("Invalid radio mode. Radio mode can't be %d\r\n", mode);
    }
    LEAVE();
    return valid;
}

/**
 * @brief Validate if channel and its frequency is in range of World Wide Safe Mode
 *
 * @param chan_num	Channel Number
 * @param chan_freq	Channel Frequency
 *
 * @return		Valid or Invalid
 */
t_bool wlan_is_channel_and_freq_valid(mlan_adapter *pmadapter, t_u8 chan_num, t_u16 chan_freq)
{
    t_bool valid = MFALSE;
    int i        = 0;
    const chan_freq_power_t *cfp;
    int cfp_no = 0;

    ENTER();

    cfp = wlan_get_region_cfp_table(pmadapter, pmadapter->region_code, (BAND_G | BAND_B | BAND_GN), &cfp_no);

    for (i = 0; i < cfp_no; i++)
    {
        /* Channel 0 is invalid */
        if (chan_num == 0U)
        {
            PRINTM(MERROR, "Invalid channel. Channel number can't be %d\r\n", chan_num);
            valid = MFALSE;
            break;
        }

        if (chan_num == cfp[i].channel)
        {
            /* Channel Number is valid. Now validate the corresponding frequency as well */
            if (chan_freq == cfp[i].freq)
            {
                valid = MTRUE;
            }
            else
            {
                PRINTM(MERROR, "Channel freq mismatch. Expected %d, Configured %d\r\n", cfp[i].freq, chan_freq);
                valid = MFALSE;
            }

            break;
        }
    }

#if CONFIG_5GHz_SUPPORT
    if (!valid)
    {
        cfp = wlan_get_region_cfp_table(pmadapter, pmadapter->region_code, BAND_A, &cfp_no);

        for (i = 0; i < cfp_no; i++)
        {
            /* Channel 0 is invalid */
            if (chan_num == 0U)
            {
                PRINTM(MERROR, "Invalid channel. Channel number can't be %d\r\n", chan_num);
                valid = MFALSE;
                break;
            }

            if (chan_num == cfp[i].channel)
            {
                /* Channel Number is valid. Now validate the corresponding frequency as well */
                if (chan_freq == cfp[i].freq)
                {
                    valid = MTRUE;
                }
                else
                {
                    PRINTM(MERROR, "Channel freq mismatch. Expected %d, Configured %d\r\n", cfp[i].freq,
                           chan_freq);
                    valid = MFALSE;
                }

                break;
            }
        }
    }
#endif

    LEAVE();
    return valid;
}

/**
 * @brief Set Custom CFP Table
 *
 * @param chanlist	Channel List Config
 * @param cfp_no_bg	A pointer to the number of CFP entries for Band BG
 * @param cfp_no_a	A pointer to the number of CFP entries for Band A
 *
 * @return              MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
#if CONFIG_5GHz_SUPPORT
mlan_status wlan_set_custom_cfp_table(wifi_chanlist_t *chanlist, t_u8 *cfp_no_bg, t_u8 *cfp_no_a)
#else
mlan_status wlan_set_custom_cfp_table(wifi_chanlist_t *chanlist, t_u8 *cfp_no_bg)
#endif
{
    t_u8 i      = 0;
    t_u8 idx_bg = 0;
    *cfp_no_bg  = 0;

#if CONFIG_5GHz_SUPPORT
    t_u8 idx_a = 0;
    *cfp_no_a  = 0;
#endif

    ENTER();

    for (i = 0; i < chanlist->num_chans; i++)
    {
        if ((chanlist->chan_info[i].chan_num <= MAX_CHANNELS_BG) &&
            (idx_bg < (sizeof(channel_freq_power_Custom_BG) / sizeof(chan_freq_power_t))))
        {
            channel_freq_power_Custom_BG[idx_bg].channel = chanlist->chan_info[i].chan_num;
            channel_freq_power_Custom_BG[idx_bg].freq    = chanlist->chan_info[i].chan_freq;
            channel_freq_power_Custom_BG[idx_bg].passive_scan_or_radar_detect =
                chanlist->chan_info[i].passive_scan_or_radar_detect;
            idx_bg++;
            *cfp_no_bg = idx_bg;
        }
#if CONFIG_5GHz_SUPPORT
        else if ((chanlist->chan_info[i].chan_num > MAX_CHANNELS_BG) &&
                 (idx_a < (sizeof(channel_freq_power_Custom_A) / sizeof(chan_freq_power_t))))
        {
            channel_freq_power_Custom_A[idx_a].channel = chanlist->chan_info[i].chan_num;
            channel_freq_power_Custom_A[idx_a].freq    = chanlist->chan_info[i].chan_freq;
            channel_freq_power_Custom_A[idx_a].passive_scan_or_radar_detect =
                chanlist->chan_info[i].passive_scan_or_radar_detect;
            idx_a++;
            *cfp_no_a = idx_a;
        }
#endif
        else
        {
#if CONFIG_5GHz_SUPPORT
            PRINTM(MERROR, "Error in configuring custom CFP table. ch %d, idx_bg %d, idx_a %d\r\n",
                   chanlist->chan_info[i].chan_num, idx_bg, idx_a);
#else
            PRINTM(MERROR, "Error in configuring custom CFP table. ch %d, idx_bg %d\r\n", chanlist->chan_info[i].chan_num,
                   idx_bg);
#endif
            return MLAN_STATUS_FAILURE;
        }
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function sets custom region table.
 *
 *  @param pmpriv 	A pointer to mlan_private structure
 *  @param cfp_no_bg	Number of CFP entries for Band BG
 *  @param cfp_no_a	Number of CFP entries for Band A
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
#if CONFIG_5GHz_SUPPORT
void wlan_set_custom_regiontable(mlan_private *pmpriv, t_u8 cfp_no_bg, t_u8 cfp_no_a)
#else
void wlan_set_custom_regiontable(mlan_private *pmpriv, t_u8 cfp_no_bg)
#endif
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int i                   = 0;

    ENTER();

    if (cfp_no_bg != 0U)
    {
        pmadapter->region_channel[i].num_cfp = (t_u8)cfp_no_bg;
        pmadapter->region_channel[i].pcfp    = channel_freq_power_Custom_BG;
    }

    i++;

#if CONFIG_5GHz_SUPPORT
    if (cfp_no_a != 0U)
    {
        pmadapter->region_channel[i].num_cfp = (t_u8)cfp_no_a;
        pmadapter->region_channel[i].pcfp    = channel_freq_power_Custom_A;
    }
#endif

    LEAVE();
}

/**
 *  @brief This function gets the list of configured active channels.
 *
 *  @param pmpriv 	A pointer to mlan_private structure
 *  @param chan_list	A pointer to the channel list
 *  @param num_chans	A pointer to the number of active channels
 *  @param acs_band  ACS band info     0: get 2.4G channel list
 *                                     1: get 5G channel list
 */
void wlan_get_active_channel_list(mlan_private *pmpriv, t_u8 *chan_list, t_u8 *num_chans, t_u16 acs_band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int i                   = 0;
    int j                   = 0;
    const chan_freq_power_t *cfp;
    int cfp_no;

    ENTER();

    *num_chans = 0;

    if (acs_band == 0)
    {
        cfp    = pmadapter->region_channel[i].pcfp;
        cfp_no = pmadapter->region_channel[i].num_cfp;
        for (j = 0; j < cfp_no; j++)
        {
            if ((!(cfp[j].dynamic.flags & NXP_CHANNEL_DISABLED)) && (!cfp[j].passive_scan_or_radar_detect))
            {
                *(chan_list++) = cfp[j].channel;
                *num_chans     = *num_chans + 1;
            }
        }
    }
    if (acs_band == 1)
    {
#if CONFIG_5GHz_SUPPORT
        i      = 1;
        cfp    = pmadapter->region_channel[i].pcfp;
        cfp_no = (int)pmadapter->region_channel[i].num_cfp;

        for (j = 0; j < cfp_no; j++)
        {
            if ((!(cfp[j].dynamic.flags & NXP_CHANNEL_DISABLED)) && (!cfp[j].passive_scan_or_radar_detect))
            {
                *(chan_list++) = (t_u8)cfp[j].channel;
                *num_chans     = *num_chans + 1U;
            }
        }

#endif
    }
}

#ifdef OTP_CHANINFO
/**
 *  @brief	Update CFP tables and power tables from FW
 *
 *  @param priv		Private driver information structure
 *  @param buf		Pointer to the buffer holding TLV data
 *					from 0x242 command response.
 *  @param buf_left	bufsize
 *
 *  @return
 *    None
 */
void wlan_add_fw_cfp_tables(pmlan_private pmpriv, t_u8 *buf, t_u16 buf_left)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    mlan_callbacks *pcb     = (mlan_callbacks *)&pmadapter->callbacks;
    MrvlIEtypesHeader_t *head;
    t_u16 tlv;
    t_u16 tlv_buf_len;
    t_u16 tlv_buf_left;
    t_u16 i;
    int k               = 0, rows, cols;
    t_u16 max_tx_pwr_bg = WLAN_TX_PWR_DEFAULT;
#if CONFIG_5GHz_SUPPORT
    t_u16 max_tx_pwr_a = WLAN_TX_PWR_DEFAULT;
#endif
    t_u8 *tlv_buf;
    t_u8 *data;
    t_u8 *tmp;
    mlan_status ret;

    ENTER();

    if (buf == MNULL)
    {
        PRINTM(MERROR, "CFP table update failed!\n");
        goto out;
    }
    if (pmadapter->otp_region != MNULL)
    {
        wlan_free_fw_cfp_tables(pmadapter);
    }

    pmadapter->tx_power_table_bg_rows = FW_CFP_TABLE_MAX_ROWS_BG;
    pmadapter->tx_power_table_bg_cols = FW_CFP_TABLE_MAX_COLS_BG;
#if CONFIG_5GHz_SUPPORT
    pmadapter->tx_power_table_a_rows = FW_CFP_TABLE_MAX_ROWS_A;
    pmadapter->tx_power_table_a_cols = FW_CFP_TABLE_MAX_COLS_A;
#endif
    tlv_buf      = (t_u8 *)buf;
    tlv_buf_left = buf_left;

    while (tlv_buf_left >= sizeof(*head))
    {
        head        = (MrvlIEtypesHeader_t *)(void *)tlv_buf;
        tlv         = wlan_le16_to_cpu(head->type);
        tlv_buf_len = wlan_le16_to_cpu(head->len);

        if (tlv_buf_left < (sizeof(*head) + tlv_buf_len))
        {
            break;
        }
        data = (t_u8 *)head + sizeof(*head);

        switch (tlv)
        {
            case TLV_TYPE_REGION_INFO:
                /* Skip adding fw region info if it already exists or
                 * if this TLV has no set data
                 */
                if (*data == 0U)
                {
                    break;
                }
                if (pmadapter->otp_region != MNULL)
                {
                    break;
                }

                ret = pcb->moal_malloc(pmadapter->pmoal_handle, sizeof(otp_region_info_t), MLAN_MEM_DEF,
                                       (t_u8 **)(void **)&pmadapter->otp_region);

                if (ret != MLAN_STATUS_SUCCESS || (pmadapter->otp_region == MNULL))
                {
                    PRINTM(MERROR,
                           "Memory allocation for the otp region"
                           " info struct failed!\n");
                    break;
                }
                /* Save region info values from OTP in the otp_region
                 * structure
                 */
                (void)__memcpy(pmadapter, pmadapter->otp_region, data, sizeof(otp_region_info_t));

                data += sizeof(otp_region_info_t);
                /* Get pre-defined cfp tables corresponding to the region code
                 * in OTP
                 */
                for (i = 0; i < MLAN_CFP_TABLE_SIZE_BG; i++)
                {
                    if (cfp_table_BG[i].code == pmadapter->otp_region->region_code)
                    {
                        max_tx_pwr_bg = (cfp_table_BG[i].cfp)->max_tx_power;
                        break;
                    }
                }
#if CONFIG_5GHz_SUPPORT
                for (i = 0; i < MLAN_CFP_TABLE_SIZE_A; i++)
                {
                    if (cfp_table_A[i].code == pmadapter->otp_region->region_code)
                    {
                        max_tx_pwr_a = (cfp_table_A[i].cfp)->max_tx_power;
                        break;
                    }
                }
#endif
                /* Update the region code and the country code in pmadapter */
                pmadapter->region_code                = pmadapter->otp_region->region_code;
                pmadapter->country_code[0]            = pmadapter->otp_region->country_code[0];
                pmadapter->country_code[1]            = pmadapter->otp_region->country_code[1];
                pmadapter->country_code[2]            = (t_u8)'\0';
                pmadapter->domain_reg.country_code[0] = pmadapter->otp_region->country_code[0];
                pmadapter->domain_reg.country_code[1] = pmadapter->otp_region->country_code[1];
                pmadapter->domain_reg.country_code[2] = (t_u8)'\0';
                pmadapter->cfp_code_bg                = pmadapter->otp_region->region_code;
#if CONFIG_5GHz_SUPPORT
                pmadapter->cfp_code_a = pmadapter->otp_region->region_code;
#endif
                break;
            case TLV_TYPE_CHAN_ATTR_CFG:
                /* Skip adding fw cfp tables if they already exist or
                 * if this TLV has no set data
                 */
                if (*data == 0U)
                {
                    break;
                }

#if CONFIG_5GHz_SUPPORT
                if ((pmadapter->cfp_otp_bg != MNULL) || (pmadapter->cfp_otp_a != MNULL))
#else
                if (pmadapter->cfp_otp_bg != MNULL)
#endif
                {
                    break;
                }

                ret = pcb->moal_malloc(pmadapter->pmoal_handle,
                                       pmadapter->tx_power_table_bg_rows * sizeof(chan_freq_power_t), MLAN_MEM_DEF,
                                       (t_u8 **)(void **)&pmadapter->cfp_otp_bg);
                if ((ret != MLAN_STATUS_SUCCESS) || (pmadapter->cfp_otp_bg == MNULL))
                {
                    PRINTM(MERROR,
                           "Memory allocation for storing otp bg"
                           " table data failed!\n");
                    break;
                }
                (void)__memset(pmadapter, pmadapter->cfp_otp_bg, 0, pmadapter->tx_power_table_bg_rows * sizeof(chan_freq_power_t));
                /* Save channel usability flags from OTP data in the fw cfp bg
                 * table and set frequency and max_tx_power values
                 */
                for (i = 0; i < pmadapter->tx_power_table_bg_rows; i++)
                {
                    (pmadapter->cfp_otp_bg + i)->channel = *data;
                    if (*data == 14U)
                    {
                        (pmadapter->cfp_otp_bg + i)->freq = 2484;
                    }
                    else
                    {
                        (pmadapter->cfp_otp_bg + i)->freq = (t_u32)2412U + (t_u32)5U * ((t_u32)*data - (t_u32)1U);
                    }
                    (pmadapter->cfp_otp_bg + i)->max_tx_power = max_tx_pwr_bg;
                    data++;
                    (pmadapter->cfp_otp_bg + i)->dynamic.flags = *data;
                    if ((*data & NXP_CHANNEL_DFS) != 0U)
                    {
                        (pmadapter->cfp_otp_bg + i)->passive_scan_or_radar_detect = MTRUE;
                    }
                    data++;
                }
#if CONFIG_5GHz_SUPPORT
                ret = pcb->moal_malloc(pmadapter->pmoal_handle,
                                       pmadapter->tx_power_table_a_rows * sizeof(chan_freq_power_t), MLAN_MEM_DEF,
                                       (t_u8 **)(void *)&pmadapter->cfp_otp_a);
                if ((ret != MLAN_STATUS_SUCCESS) || (pmadapter->cfp_otp_a == MNULL))
                {
                    PRINTM(MERROR,
                           "Memory allocation for storing otp a"
                           " table data failed!\n");
                    break;
                }
                (void)__memset(pmadapter, pmadapter->cfp_otp_bg, 0, pmadapter->tx_power_table_bg_rows * sizeof(chan_freq_power_t));
                /* Save channel usability flags from OTP data in the fw cfp a
                 * table and set frequency and max_tx_power values
                 */
                for (i = 0; i < pmadapter->tx_power_table_a_rows; i++)
                {
                    (pmadapter->cfp_otp_a + i)->channel = *data;
                    if (*data < 183U)
                    {
                        /* 5GHz channels */
                        (pmadapter->cfp_otp_a + i)->freq = (t_u32)5035U + (t_u32)5U * ((t_u32)*data - (t_u32)7U);
                    }
                    else
                    {
                        /* 4GHz channels */
                        (pmadapter->cfp_otp_a + i)->freq = (t_u32)4915U + (t_u32)5U * ((t_u32)*data - (t_u32)183U);
                    }
                    (pmadapter->cfp_otp_a + i)->max_tx_power = max_tx_pwr_a;
                    data++;
                    (pmadapter->cfp_otp_a + i)->dynamic.flags = *data;
                    if ((*data & NXP_CHANNEL_DFS) != 0U)
                    {
                        (pmadapter->cfp_otp_a + i)->passive_scan_or_radar_detect = MTRUE;
                    }
                    data++;
                }
#endif
                break;
            case TLV_TYPE_POWER_TABLE:
                /* Skip adding fw power tables if this TLV has no data or
                 * if they already exists but force reg rule is set in the otp
                 */
                if (*data == 0U)
                {
                    break;
                }
                if ((pmadapter->otp_region != MNULL) && (pmadapter->otp_region->force_reg != 0U) &&
                    (pmadapter->tx_power_table_bg != MNULL))
                {
                    break;
                }

                /* Save the tlv data in power tables for band BG and A */
                tmp = data;
                i   = 0;
                while ((i < (t_u16)pmadapter->tx_power_table_bg_rows * (t_u16)pmadapter->tx_power_table_bg_cols) &&
                       (i < tlv_buf_len) && (*tmp != 36U))
                {
                    i++;
                    tmp++;
                }
                if (pmadapter->tx_power_table_bg == MNULL)
                {
                    ret = pcb->moal_malloc(pmadapter->pmoal_handle, i, MLAN_MEM_DEF,
                                           (t_u8 **)&pmadapter->tx_power_table_bg);
                    if ((ret != MLAN_STATUS_SUCCESS) || (pmadapter->tx_power_table_bg == MNULL))
                    {
                        PRINTM(MERROR,
                               "Memory allocation for the BG-band"
                               " power table falied!\n");
                        break;
                    }
                }
                if (i > 0U)
                {
                    (void)__memcpy(pmadapter, pmadapter->tx_power_table_bg, data, i);
                }
                pmadapter->tx_power_table_bg_size = i;
                data += i;
#if CONFIG_5GHz_SUPPORT
                i = 0;
                while ((i < (t_u16)pmadapter->tx_power_table_a_rows * (t_u16)pmadapter->tx_power_table_a_cols) &&
                       (i < (tlv_buf_len - pmadapter->tx_power_table_bg_size)))
                {
                    i++;
                }
                if (pmadapter->tx_power_table_a == MNULL)
                {
                    ret = pcb->moal_malloc(pmadapter->pmoal_handle, i, MLAN_MEM_DEF,
                                           (t_u8 **)&pmadapter->tx_power_table_a);
                    if ((ret != MLAN_STATUS_SUCCESS) || (pmadapter->tx_power_table_a == MNULL))
                    {
                        PRINTM(MERROR,
                               "Memory allocation for the A-band"
                               " power table failed!\n");
                        break;
                    }
                }
                if (i > 0U)
                {
                    (void)__memcpy(pmadapter, pmadapter->tx_power_table_a, data, i);
                }
                pmadapter->tx_power_table_a_size = i;
#endif
                break;
            case TLV_TYPE_POWER_TABLE_ATTR:
                pmadapter->tx_power_table_bg_rows = ((power_table_attr_t *)(void *)data)->rows_2g;
                pmadapter->tx_power_table_bg_cols = ((power_table_attr_t *)(void *)data)->cols_2g;
#if CONFIG_5GHz_SUPPORT
                pmadapter->tx_power_table_a_rows = ((power_table_attr_t *)(void *)data)->rows_5g;
                pmadapter->tx_power_table_a_cols = ((power_table_attr_t *)(void *)data)->cols_5g;
#endif
                break;
            default:
                PRINTM(MINFO, "Unexpected TLV \n");
                break;
        }
        tlv_buf += (sizeof(*head) + tlv_buf_len);
        tlv_buf_left -= ((t_u16)sizeof(*head) + tlv_buf_len);
    }
    if ((pmadapter->cfp_otp_bg == MNULL) || (pmadapter->tx_power_table_bg == MNULL))
    {
        goto out;
    }
    /* Set remaining flags for BG */
    rows = (int)pmadapter->tx_power_table_bg_rows;
    cols = (int)pmadapter->tx_power_table_bg_cols;

    for (i = 0; i < (t_u16)rows; i++)
    {
        k = ((int)i * (int)cols) + (int)1U;
        if (((pmadapter->cfp_otp_bg + i)->dynamic.flags & NXP_CHANNEL_DISABLED) != 0U)
        {
            continue;
        }

        if (pmadapter->tx_power_table_bg[k + (int)MOD_CCK] == 0U)
        {
            (pmadapter->cfp_otp_bg + i)->dynamic.flags |= NXP_CHANNEL_NO_CCK;
        }

        if (pmadapter->tx_power_table_bg[k + (int)MOD_OFDM_PSK] == 0U &&
            pmadapter->tx_power_table_bg[k + (int)MOD_OFDM_QAM16] == 0U &&
            pmadapter->tx_power_table_bg[k + (int)MOD_OFDM_QAM64] == 0U)
        {
            (pmadapter->cfp_otp_bg + i)->dynamic.flags |= NXP_CHANNEL_NO_OFDM;
        }
    }
out:
    LEAVE();
}

/**
 *  @brief	This function deallocates otp cfp and power tables memory.
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 */
void wlan_free_fw_cfp_tables(mlan_adapter *pmadapter)
{
    pmlan_callbacks pcb;

    ENTER();

    pcb = &pmadapter->callbacks;
    if (pmadapter->otp_region != NULL)
    {
        (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter->otp_region);
    }
    if (pmadapter->cfp_otp_bg != NULL)
    {
        (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter->cfp_otp_bg);
    }
    if (pmadapter->tx_power_table_bg != NULL)
    {
        (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter->tx_power_table_bg);
    }
    pmadapter->otp_region             = MNULL;
    pmadapter->cfp_otp_bg             = MNULL;
    pmadapter->tx_power_table_bg      = MNULL;
    pmadapter->tx_power_table_bg_size = 0;
#if CONFIG_5GHz_SUPPORT
    if (pmadapter->cfp_otp_a != NULL)
    {
        (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter->cfp_otp_a);
    }
    if (pmadapter->tx_power_table_a != NULL)
    {
        (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter->tx_power_table_a);
    }
    pmadapter->cfp_otp_a             = MNULL;
    pmadapter->tx_power_table_a      = MNULL;
    pmadapter->tx_power_table_a_size = 0;
#endif
    LEAVE();
}
#endif /* OTP_CHANINFO */

t_bool wlan_is_etsi_country(pmlan_adapter pmadapter, t_u8 *country_code)
{
    t_u8 i;
    t_u32 meas_country_code_len = 0;

    ENTER();
    /* Look for code in EU country code table */
    for (i = 0; i < NELEMENTS(eu_country_code_table); i++)
    {
        meas_country_code_len = COUNTRY_CODE_LEN - 1U;
        if (__memcmp(pmadapter, eu_country_code_table[i], country_code, meas_country_code_len) == 0)
        {
            LEAVE();
            return MTRUE;
        }
    }

    LEAVE();
    return MFALSE;
}

static oper_bw_chan *wlan_get_nonglobal_operclass_table(mlan_private *pmpriv, int *arraysize)
{
    t_u8 country_code[][COUNTRY_CODE_LEN] = {"US", "JP", "CN"};
    int country_id                        = 0;
    oper_bw_chan *poper_bw_chan           = MNULL;
    t_u32 meas_country_code_len           = 0;

    ENTER();
    for (country_id = 0; country_id < 3; country_id++)
    {
        meas_country_code_len = COUNTRY_CODE_LEN - 1U;
        if (__memcmp(pmpriv->adapter, pmpriv->adapter->country_code, country_code[country_id], meas_country_code_len) ==
            0)
        {
            break;
        }
    }
    if (country_id >= 3)
    {
        country_id = COUNTRY_ID_US; /*Set default to US*/
    }
    if (wlan_is_etsi_country(pmpriv->adapter, pmpriv->adapter->country_code))
    {
        country_id = COUNTRY_ID_EU; /** Country in EU */
    }

    switch (country_id)
    {
        case COUNTRY_ID_US:
            poper_bw_chan = oper_bw_chan_us;
            *arraysize    = (int)sizeof(oper_bw_chan_us);
            break;
        case COUNTRY_ID_JP:
            poper_bw_chan = oper_bw_chan_jp;
            *arraysize    = (int)sizeof(oper_bw_chan_jp);
            break;
        case COUNTRY_ID_CN:
            poper_bw_chan = oper_bw_chan_cn;
            *arraysize    = (int)sizeof(oper_bw_chan_cn);
            break;
        case COUNTRY_ID_EU:
            poper_bw_chan = oper_bw_chan_eu;
            *arraysize    = (int)sizeof(oper_bw_chan_eu);
            break;
        default:
            PRINTM(MERROR, "Country not support!\n");
            break;
    }

    LEAVE();
    return poper_bw_chan;
}

mlan_status wlan_get_global_nonglobal_oper_class(
    mlan_private *pmpriv, t_u8 channel, t_u8 bw, t_u8 *oper_class, t_u8 *global_op_class)
{
    oper_bw_chan *poper_bw_chan = MNULL;
#if CONFIG_11AC
    t_u8 center_freq_idx = 0;
#endif
    t_u8 center_freqs[] = {42, 50, 58, 106, 114, 122, 138, 155};
    int i = 0, arraysize = 0, channum = 0, table_size = 0;

    ENTER();
    poper_bw_chan = wlan_get_nonglobal_operclass_table(pmpriv, &arraysize);
    if (poper_bw_chan == MNULL)
    {
        PRINTM(MCMND, "Operating class table do not find!\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    for (i = 0; i < (int)sizeof(center_freqs); i++)
    {
        if (channel == center_freqs[i])
        {
            PRINTM(MERROR, "Invalid channel number %d!\n", channel);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
    }
#if CONFIG_11AC
    if (bw == (t_u8)BW_80MHZ)
    {
        center_freq_idx = wlan_get_center_freq_idx(pmpriv, BAND_AAC, channel, CHANNEL_BW_80MHZ);
        channel         = center_freq_idx;
    }
#endif
    table_size = arraysize / (int)sizeof(oper_bw_chan);
    for (i = 0; i < table_size; i++)
    {
        if (poper_bw_chan[i].bandwidth == bw)
        {
            for (channum = 0; channum < (int)(sizeof(poper_bw_chan[i].channel_list)); channum++)
            {
                if (poper_bw_chan[i].channel_list[channum] != (t_u8)0U &&
                    poper_bw_chan[i].channel_list[channum] == channel)
                {
                    if (oper_class != MNULL)
                    {
                        *oper_class = poper_bw_chan[i].oper_class;
                    }
                    if (global_op_class != MNULL)
                    {
                        *global_op_class = poper_bw_chan[i].global_oper_class;
                    }
                    return MLAN_STATUS_SUCCESS;
                }
            }
        }
    }
    PRINTM(MCMND, "Operating class not find!\n");
    LEAVE();
    return MLAN_STATUS_FAILURE;
}

int wlan_add_supported_oper_class_ie(mlan_private *pmpriv, t_u8 **pptlv_out, t_u8 curr_oper_class)
{
    t_u8 oper_class_us[] = {115,
                            118,
                            124,
                            121,
                            125,
                            81,
                            116,
                            119,
                            122,
                            126,
                            126,
                            117,
                            120,
                            123,
                            127,
                            127,
                            83,
                            84
#if CONFIG_11AC
                            ,
                            128,
                            129,
                            130
#endif
    };
    t_u8 oper_class_eu[] = {115,
                            118,
                            121,
                            81,
                            116,
                            119,
                            122,
                            117,
                            120,
                            123,
                            83,
                            84,
                            125
#if CONFIG_11AC
                            ,
                            128,
                            129,
                            130
#endif
    };
    t_u8 oper_class_jp[] = {115,
                            81,
                            82,
                            118,
                            118,
                            121,
                            121,
                            116,
                            119,
                            119,
                            122,
                            122,
                            117,
                            120,
                            120,
                            123,
                            123,
                            83,
                            84,
                            121
#if CONFIG_11AC
                            ,
                            128,
                            129,
                            130
#endif
    };
    t_u8 oper_class_cn[] = {115,
                            118,
                            125,
                            116,
                            119,
                            126,
                            81,
                            83,
                            84
#if CONFIG_11AC
                            ,
                            128,
                            129,
                            130
#endif
    };
    t_u8 country_code[][COUNTRY_CODE_LEN] = {"US", "JP", "CN"};
    int country_id = 0, ret = 0;
    MrvlIETypes_SuppOperClass_t *poper_class = MNULL;
    t_u32 meas_country_code_len              = 0;

    ENTER();

    for (country_id = 0; country_id < 3; country_id++)
    {
        meas_country_code_len = COUNTRY_CODE_LEN - 1U;
        if (__memcmp(pmpriv->adapter, pmpriv->adapter->country_code, country_code[country_id], meas_country_code_len) ==
            0)
        {
            break;
        }
    }
    if (country_id >= 3)
    {
        country_id = COUNTRY_ID_US; /*Set default to US*/
    }
    if (wlan_is_etsi_country(pmpriv->adapter, pmpriv->adapter->country_code))
    {
        country_id = COUNTRY_ID_EU; /** Country in EU */
    }
    poper_class = (MrvlIETypes_SuppOperClass_t *)(void *)*pptlv_out;
    (void)__memset(pmpriv->adapter, poper_class, 0, sizeof(MrvlIETypes_SuppOperClass_t));
    poper_class->header.type = wlan_cpu_to_le16(REGULATORY_CLASS);
    if (country_id == COUNTRY_ID_US)
    {
        poper_class->header.len = (t_u16)sizeof(oper_class_us);
        (void)__memcpy(pmpriv->adapter, &poper_class->oper_class, oper_class_us, sizeof(oper_class_us));
    }
    else if (country_id == COUNTRY_ID_JP)
    {
        poper_class->header.len = (t_u16)sizeof(oper_class_jp);
        (void)__memcpy(pmpriv->adapter, &poper_class->oper_class, oper_class_jp, sizeof(oper_class_jp));
    }
    else if (country_id == COUNTRY_ID_CN)
    {
        poper_class->header.len = (t_u16)sizeof(oper_class_cn);
        (void)__memcpy(pmpriv->adapter, &poper_class->oper_class, oper_class_cn, sizeof(oper_class_cn));
    }
    else if (country_id == COUNTRY_ID_EU)
    {
        poper_class->header.len = (t_u16)sizeof(oper_class_eu);
        (void)__memcpy(pmpriv->adapter, &poper_class->oper_class, oper_class_eu, sizeof(oper_class_eu));
    }
    else
    {
        /* Do nothing */
    }
    poper_class->current_oper_class = curr_oper_class;
    poper_class->header.len += (t_u16)sizeof(poper_class->current_oper_class);
    DBG_HEXDUMP(MCMD_D, "Operating class", (t_u8 *)poper_class, sizeof(MrvlIEtypesHeader_t) + poper_class->header.len);
    ret = (int)sizeof(MrvlIEtypesHeader_t) + (int)poper_class->header.len;
    *pptlv_out += ret;
    poper_class->header.len = wlan_cpu_to_le16(poper_class->header.len);

    LEAVE();
    return ret;
}

#if CONFIG_ECSA
/**
 *  @brief Check validation of given channel and oper class
 *
 *  @param pmpriv             A pointer to mlan_private structure
 *  @param channel            Channel number
 *  @param oper_class         operating class
 *
 *  @return                   MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_check_operclass_validation(mlan_private *pmpriv, t_u8 channel, t_u8 oper_class)
{
    int arraysize = 0, i = 0, channum = 0;
    oper_bw_chan *poper_bw_chan = MNULL;
#if CONFIG_11AC
    t_u8 center_freq_idx = 0;
#endif
    t_u8 center_freqs[] = {42, 50, 58, 106, 114, 122, 138, 155};

    ENTER();

    for (i = 0; i < (int)sizeof(center_freqs); i++)
    {
        if (channel == center_freqs[i])
        {
            PRINTM(MERROR, "Invalid channel number %d!\n", channel);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
    }
    if (oper_class <= 0 || oper_class > 130)
    {
        PRINTM(MERROR, "Invalid operating class!\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
#if CONFIG_11AC
    if (oper_class >= 128)
    {
        center_freq_idx = wlan_get_center_freq_idx(pmpriv, BAND_AAC, channel, CHANNEL_BW_80MHZ);
        channel         = center_freq_idx;
    }
#endif
    poper_bw_chan = wlan_get_nonglobal_operclass_table(pmpriv, &arraysize);

    if (!poper_bw_chan)
    {
        PRINTM(MCMND, "Operating class table do not find!\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    for (i = 0; i < (int)(arraysize / sizeof(oper_bw_chan)); i++)
    {
        if (poper_bw_chan[i].oper_class == oper_class || poper_bw_chan[i].global_oper_class == oper_class)
        {
            for (channum = 0; channum < (int)sizeof(poper_bw_chan[i].channel_list); channum++)
            {
                if (poper_bw_chan[i].channel_list[channum] && poper_bw_chan[i].channel_list[channum] == channel)
                {
                    LEAVE();
                    return MLAN_STATUS_SUCCESS;
                }
            }
        }
    }

    PRINTM(MCMND, "Operating class %d do not match channel %d!\n", oper_class, channel);
    LEAVE();
    return MLAN_STATUS_FAILURE;
}
#endif
