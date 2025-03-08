/** @file wlan_txpwrlimit_cfg_WW_rw610.h
 *
 *  @brief  This file provides WLAN World Wide Safe Mode Tx Power Limits.
 *
 *  Copyright 2008-2021 NXP
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  'Software'), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sub license, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject
 *  to the following conditions:
 *
 *  The above copyright notice and this permission notice (including the
 *  next paragraph) shall be included in all copies or substantial
 *  portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 *  IN NO EVENT SHALL NXP AND/OR ITS SUPPLIERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef _WLAN_TXPWRLIMIT_CFG_WW_RW610_H_
#define _WLAN_TXPWRLIMIT_CFG_WW_RW610_H_

#include <wlan.h>
#include <wifi.h>
// coverity[MISRA C-2012 Initializers:SUPPRESS]
static wlan_chanlist_t chanlist_2g_cfg = {
    .num_chans = 11,
    .chan_info[0] =
        {
            .chan_num                     = 1,
            .chan_freq                    = 2412,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[1] =
        {
            .chan_num                     = 2,
            .chan_freq                    = 2417,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[2] =
        {
            .chan_num                     = 3,
            .chan_freq                    = 2422,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[3] =
        {
            .chan_num                     = 4,
            .chan_freq                    = 2427,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[4] =
        {
            .chan_num                     = 5,
            .chan_freq                    = 2432,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[5] =
        {
            .chan_num                     = 6,
            .chan_freq                    = 2437,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[6] =
        {
            .chan_num                     = 7,
            .chan_freq                    = 2442,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[7] =
        {
            .chan_num                     = 8,
            .chan_freq                    = 2447,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[8] =
        {
            .chan_num                     = 9,
            .chan_freq                    = 2452,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[9] =
        {
            .chan_num                     = 10,
            .chan_freq                    = 2457,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[10] =
        {
            .chan_num                     = 11,
            .chan_freq                    = 2462,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[11] = {0},
	.chan_info[12] = {0},
	.chan_info[13] = {0},
    .chan_info[14] = {0},
    .chan_info[15] = {0},
    .chan_info[16] = {0},
    .chan_info[17] = {0},
    .chan_info[18] = {0},
    .chan_info[19] = {0},
    .chan_info[20] = {0},
    .chan_info[21] = {0},
    .chan_info[22] = {0},
    .chan_info[23] = {0},
    .chan_info[24] = {0},
    .chan_info[25] = {0},
    .chan_info[26] = {0},
    .chan_info[27] = {0},
    .chan_info[28] = {0},
    .chan_info[29] = {0},
    .chan_info[30] = {0},
    .chan_info[31] = {0},
    .chan_info[32] = {0},
    .chan_info[33] = {0},
    .chan_info[34] = {0},
    .chan_info[35] = {0},
    .chan_info[36] = {0},
    .chan_info[37] = {0},
    .chan_info[38] = {0},
    .chan_info[39] = {0},
    .chan_info[40] = {0},
    .chan_info[41] = {0},
    .chan_info[42] = {0},
    .chan_info[43] = {0},
    .chan_info[44] = {0},
    .chan_info[45] = {0},
    .chan_info[46] = {0},
    .chan_info[47] = {0},
    .chan_info[48] = {0},
    .chan_info[49] = {0},
    .chan_info[50] = {0},
    .chan_info[51] = {0},
    .chan_info[52] = {0},
    .chan_info[53] = {0},
};

#if CONFIG_5GHz_SUPPORT
// coverity[MISRA C-2012 Initializers:SUPPRESS]
static wlan_chanlist_t chanlist_5g_cfg = {
    .num_chans = 28,
    .chan_info[0] =
        {
            .chan_num                     = 36,
            .chan_freq                    = 5180,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[1] =
        {
            .chan_num                     = 40,
            .chan_freq                    = 5200,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[2] =
        {
            .chan_num                     = 44,
            .chan_freq                    = 5220,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[3] =
        {
            .chan_num                     = 48,
            .chan_freq                    = 5240,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[4] =
        {
            .chan_num                     = 52,
            .chan_freq                    = 5260,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[5] =
        {
            .chan_num                     = 56,
            .chan_freq                    = 5280,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[6] =
        {
            .chan_num                     = 60,
            .chan_freq                    = 5300,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[7] =
        {
            .chan_num                     = 64,
            .chan_freq                    = 5320,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[8] =
        {
            .chan_num                     = 100,
            .chan_freq                    = 5500,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[9] =
        {
            .chan_num                     = 104,
            .chan_freq                    = 5520,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[10] =
        {
            .chan_num                     = 108,
            .chan_freq                    = 5540,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[11] =
        {
            .chan_num                     = 112,
            .chan_freq                    = 5560,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[12] =
        {
            .chan_num                     = 116,
            .chan_freq                    = 5580,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[13] =
        {
            .chan_num                     = 120,
            .chan_freq                    = 5600,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[14] =
        {
            .chan_num                     = 124,
            .chan_freq                    = 5620,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[15] =
        {
            .chan_num                     = 128,
            .chan_freq                    = 5640,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[16] =
        {
            .chan_num                     = 132,
            .chan_freq                    = 5660,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[17] =
        {
            .chan_num                     = 136,
            .chan_freq                    = 5680,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[18] =
        {
            .chan_num                     = 140,
            .chan_freq                    = 5700,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[19] =
        {
            .chan_num                     = 144,
            .chan_freq                    = 5720,
            .passive_scan_or_radar_detect = true,
        },
    .chan_info[20] =
        {
            .chan_num                     = 149,
            .chan_freq                    = 5745,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[21] =
        {
            .chan_num                     = 153,
            .chan_freq                    = 5765,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[22] =
        {
            .chan_num                     = 157,
            .chan_freq                    = 5785,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[23] =
        {
            .chan_num                     = 161,
            .chan_freq                    = 5805,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[24] =
        {
            .chan_num                     = 165,
            .chan_freq                    = 5825,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[25] =
        {
            .chan_num                     = 169,
            .chan_freq                    = 5845,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[26] =
        {
            .chan_num                     = 173,
            .chan_freq                    = 5865,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[27] =
        {
            .chan_num                     = 177,
            .chan_freq                    = 5885,
            .passive_scan_or_radar_detect = false,
        },
    .chan_info[28] = {0},
    .chan_info[29] = {0},
    .chan_info[30] = {0},
    .chan_info[31] = {0},
    .chan_info[32] = {0},
    .chan_info[33] = {0},
    .chan_info[34] = {0},
    .chan_info[35] = {0},
    .chan_info[36] = {0},
    .chan_info[37] = {0},
    .chan_info[38] = {0},
    .chan_info[39] = {0},
    .chan_info[40] = {0},
    .chan_info[41] = {0},
    .chan_info[42] = {0},
    .chan_info[43] = {0},
    .chan_info[44] = {0},
    .chan_info[45] = {0},
    .chan_info[46] = {0},
    .chan_info[47] = {0},
    .chan_info[48] = {0},
    .chan_info[49] = {0},
    .chan_info[50] = {0},
    .chan_info[51] = {0},
    .chan_info[52] = {0},
    .chan_info[53] = {0},
};
#endif

#if CONFIG_COMPRESS_TX_PWTBL
static const t_u8 rg_rw610_bga[] = {
    0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x55, 0x53, 0x20, 0x10, 0x00, 0x01, 0x06, 0x02, 0x82, 0x00, 0x88, 0x88,
    0x03, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x53, 0x20, 0x10, 0x03, 0x07, 0x00, 0x02, 0x08, 0x05,
    0xa0, 0x00, 0x61, 0x40, 0x58, 0x02, 0x06, 0x05, 0x0a, 0x80, 0x42, 0x80, 0x40, 0x90, 0x06, 0x0c, 0x14, 0x05,
    0x43, 0x60, 0xc1, 0x28, 0x28, 0x0c, 0x27, 0x0a, 0x04, 0xc5, 0xe0, 0xc1, 0x38, 0xa8, 0x46, 0x0a, 0x2d, 0x8f,
    0x00, 0x09, 0x21, 0x08, 0xf8, 0x00, 0x08, 0x03, 0x07, 0x80, 0x01, 0x08, 0x00, 0x70, 0x0e, 0x1c, 0x2a, 0x0a,
    0x02, 0x22, 0xf0, 0x48, 0x48, 0x50, 0x0c, 0x00, 0x07, 0x05, 0x23, 0x30, 0xa0, 0x94, 0xe0, 0x28, 0x01, 0x09,
    0x82, 0x02, 0x41, 0x39, 0xc4, 0x36, 0x63, 0x33, 0x9e, 0xce, 0x67, 0x61, 0x4a, 0x4c, 0xfc, 0x24, 0x1b, 0xa2,
    0xc3, 0x8f, 0xc1, 0x1a, 0x80, 0x38, 0x23, 0x0e, 0x34, 0x80, 0x0d, 0x21, 0x10, 0x40, 0x42, 0x88, 0x0e, 0x08,
    0x54, 0x80};

// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
static const t_u8 rg_rw610_qfn[] = {
    0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x55, 0x53, 0x20, 0x10, 0x00, 0x01, 0x06, 0x02, 0x7b, 0x00, 0x88, 0x88,
    0x03, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x53, 0x20, 0x10, 0x93, 0x06, 0x00, 0x02, 0x08, 0x07,
    0x05, 0x01, 0xa1, 0x50, 0x08, 0x50, 0x0b, 0x04, 0x0a, 0x00, 0x82, 0x70, 0x50, 0x20, 0x02, 0x0a, 0x08, 0x85,
    0x85, 0x21, 0xa0, 0x10, 0x90, 0x36, 0x1d, 0x11, 0x88, 0x00, 0xa1, 0x31, 0x78, 0xb0, 0x04, 0x23, 0x16, 0x15,
    0xc7, 0xc0, 0x04, 0x70, 0x84, 0x80, 0x00, 0x04, 0x01, 0x83, 0xc0, 0x00, 0x84, 0x00, 0x38, 0x06, 0x01, 0x00,
    0x0a, 0x82, 0x81, 0x48, 0x40, 0x18, 0x01, 0x01, 0x0e, 0x4d, 0xc2, 0x80, 0x10, 0x98, 0x20, 0x24, 0x13, 0x9b,
    0x84, 0xa0, 0xa1, 0xb9, 0xd4, 0xe6, 0x03, 0x14, 0xa2, 0x03, 0xa8, 0x40, 0x03, 0xf0, 0x46, 0x92, 0x11, 0x98,
    0x1a, 0x40, 0x06, 0x90, 0x88, 0x20, 0x21, 0x40, 0x07, 0x04, 0x26, 0x07, 0xe0};

// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
static const t_u8 rg_rw610_csp[] = {
    0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x55, 0x53, 0x20, 0x10, 0x00, 0x01, 0x06, 0x02, 0x7f, 0x00, 0x88, 0x88,
    0x03, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x53, 0x20, 0x10, 0xd3, 0x06, 0x00, 0x02, 0x08, 0x07,
    0x05, 0x01, 0xa1, 0x50, 0x08, 0x50, 0x0b, 0x04, 0x0a, 0x00, 0x82, 0x70, 0x50, 0x20, 0x00, 0x02, 0x13, 0x06,
    0xc3, 0x82, 0x31, 0x18, 0x7c, 0x14, 0x10, 0x13, 0x01, 0x42, 0x61, 0xd1, 0x08, 0x94, 0x44, 0x57, 0x13, 0x00,
    0x11, 0xc2, 0x11, 0xe0, 0x00, 0x10, 0x06, 0x0f, 0x00, 0x02, 0x10, 0x00, 0xe0, 0x18, 0x04, 0x00, 0x2a, 0x0a,
    0x02, 0x02, 0xa0, 0xb0, 0xa0, 0x06, 0x10, 0x06, 0x00, 0x03, 0xa1, 0x13, 0x29, 0xa0, 0x22, 0x73, 0x04, 0x09,
    0x06, 0xe7, 0x21, 0x29, 0xb0, 0x4e, 0x63, 0x3f, 0x0a, 0x51, 0xe7, 0x40, 0x10, 0x88, 0x20, 0x24, 0x0b, 0xa6,
    0xd0, 0xe5, 0xc7, 0xe0, 0x8d, 0x08, 0x1c, 0x11, 0x97, 0x1a, 0x40, 0x06, 0x90, 0x85, 0x58, 0x21, 0x53};

// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
static const t_u8 rg_rw610_EU[] = {
    0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x45, 0x55, 0x20, 0x30, 0x00, 0x02, 0x06, 0x02, 0x8f, 0x00, 0x88, 0x88,
    0x03, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x55, 0x20, 0x30, 0xd3, 0x07, 0x00, 0x02, 0x08, 0x06,
    0xa0, 0x00, 0x21, 0x30, 0x68, 0x02, 0x06, 0x03, 0x0a, 0x01, 0xc2, 0x60, 0x38, 0x34, 0x2e, 0x13, 0x0b, 0x83,
    0x04, 0xa0, 0xb0, 0x30, 0x10, 0x50, 0x12, 0x13, 0x00, 0x84, 0x80, 0x30, 0xd0, 0x64, 0x6a, 0x0c, 0x11, 0x82,
    0x8a, 0xe4, 0x00, 0x02, 0x38, 0x42, 0x42, 0x00, 0x02, 0x00, 0xc1, 0xe0, 0x00, 0x2a, 0x00, 0x22, 0x03, 0x8d,
    0x8a, 0xa3, 0x01, 0x40, 0x34, 0xcc, 0x15, 0x18, 0x07, 0x01, 0x40, 0x01, 0x10, 0x9c, 0x56, 0x6a, 0x01, 0x0a,
    0x02, 0xa3, 0x53, 0x99, 0xdc, 0x68, 0x26, 0x08, 0x09, 0x02, 0xe8, 0x81, 0x1a, 0x35, 0x02, 0x7f, 0x41, 0x08,
    0x80, 0x69, 0x75, 0x19, 0x9d, 0x3c, 0x15, 0x51, 0xa9, 0xc6, 0x42, 0x55, 0x89, 0xd0, 0x46, 0xa2, 0x00, 0x3f,
    0x4f, 0x2b, 0x20, 0x03, 0x4d, 0x90, 0x22, 0x08, 0x08, 0x52, 0xab, 0xa1, 0x08, 0xd9, 0xf8};

// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
static const t_u8 rg_rw610_JP[] = {
    0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x4a, 0x50, 0x20, 0x40, 0x00, 0x03, 0x06, 0x02, 0x81, 0x00, 0x88, 0x88,
    0x03, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x50, 0x20, 0x40, 0xf3, 0x06, 0x00, 0x02, 0x08, 0x07,
    0x04, 0xc1, 0xe1, 0x40, 0x50, 0x48, 0x02, 0x11, 0x00, 0x80, 0x20, 0xe0, 0x18, 0x30, 0x50, 0x10, 0x12, 0x01,
    0x42, 0x21, 0x50, 0x80, 0x90, 0x2e, 0x25, 0x0b, 0x86, 0xc3, 0xe2, 0x30, 0x98, 0xc0, 0x04, 0x27, 0x1a, 0x8b,
    0x84, 0x41, 0xa0, 0x01, 0x5c, 0x88, 0x00, 0x47, 0x08, 0x48, 0xc5, 0x60, 0x00, 0x20, 0x0c, 0x1e, 0x80, 0x0b,
    0x00, 0xe1, 0x22, 0xa0, 0xa0, 0x10, 0x26, 0x04, 0x0a, 0x02, 0xc0, 0x01, 0x60, 0xa0, 0x16, 0x6f, 0x39, 0x9d,
    0x85, 0x23, 0xd0, 0xf9, 0xd0, 0x5a, 0x1b, 0x36, 0x9c, 0x51, 0x67, 0xb3, 0xfa, 0x54, 0x30, 0x27, 0x3b, 0x83,
    0x00, 0x0f, 0xc1, 0x1a, 0x78, 0x5a, 0x24, 0x69, 0x00, 0x1a, 0x42, 0x20, 0x80, 0x85, 0x14, 0x21, 0x09, 0x3f,
    0x00};

// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
static const t_u8 rg_rw610_CA[] = {
    0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x43, 0x4e, 0x20, 0x50, 0x00, 0x01, 0x06, 0x02, 0xa6, 0x00, 0x88, 0x88,
    0x03, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x4e, 0x20, 0x50, 0x43, 0x09, 0x00, 0x02, 0x08, 0x06,
    0xa0, 0x00, 0x21, 0x30, 0x58, 0x38, 0x02, 0x0d, 0x00, 0x80, 0x00, 0x21, 0x20, 0x40, 0x44, 0x02, 0x10, 0x00,
    0x83, 0x00, 0x20, 0x58, 0x48, 0x0a, 0x19, 0x0e, 0x88, 0x44, 0xa2, 0x80, 0x08, 0xb4, 0x36, 0x1f, 0x11, 0x89,
    0xc5, 0x41, 0xf1, 0xf0, 0x88, 0x1a, 0x21, 0x23, 0x8d, 0x48, 0xa4, 0x92, 0x60, 0x0c, 0xa2, 0x43, 0x1c, 0x91,
    0xc4, 0x24, 0xb2, 0x79, 0x04, 0x6c, 0x58, 0x11, 0x04, 0x4d, 0x25, 0x20, 0x02, 0x40, 0x40, 0x13, 0x2e, 0x9b,
    0x00, 0x00, 0x80, 0x30, 0x22, 0x00, 0x16, 0x00, 0x02, 0xa0, 0x02, 0x40, 0x01, 0x50, 0x50, 0x10, 0x00, 0x05,
    0x84, 0x80, 0x21, 0x40, 0x1c, 0x40, 0x00, 0x12, 0xa7, 0x54, 0x2a, 0x55, 0x4a, 0xb0, 0x48, 0x27, 0x4f, 0xa8,
    0x81, 0x2b, 0xb5, 0x90, 0x58, 0x4e, 0xa7, 0x55, 0x84, 0xd6, 0x2c, 0x16, 0x6a, 0xe5, 0xa6, 0x19, 0x5a, 0xb1,
    0x42, 0x49, 0x13, 0xc9, 0xc5, 0x40, 0x22, 0x05, 0x00, 0x1c, 0xaf, 0x37, 0x50, 0x58, 0x40, 0x09, 0x23, 0x9e,
    0x4f, 0x00};

// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
static const t_u8 rg_rw610_CN[] = {
	0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x43, 0x4e,
	0x20, 0x50, 0x00, 0x02, 0x06, 0x02, 0xaa, 0x00, 0x88, 0x88, 0x03, 0x01, 0x0b, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x43, 0x4e, 0x20, 0x50, 0x83, 0x09, 0x00, 0x02, 0x0c, 0x06, 0xab, 0x00, 0x21, 0x30,
	0x58, 0x38, 0x02, 0x0d, 0x00, 0x80, 0x00, 0x21, 0x20, 0x40, 0x44, 0x02, 0x10, 0x00, 0x83, 0x00,
	0x20, 0x58, 0x48, 0x0a, 0x19, 0x0e, 0x88, 0x44, 0xa2, 0x80, 0x08, 0xb4, 0x36, 0x1f, 0x11, 0x89,
	0xc5, 0x41, 0xf1, 0xf0, 0x88, 0x1a, 0x21, 0x23, 0x8d, 0x48, 0xa4, 0x92, 0x60, 0x0c, 0xa2, 0x43,
	0x1c, 0x91, 0xc4, 0x24, 0xb2, 0x79, 0x04, 0x6c, 0x58, 0x11, 0x04, 0x4d, 0x25, 0x20, 0x02, 0x40,
	0x40, 0x13, 0x2e, 0x9b, 0x03, 0x00, 0x80, 0xf0, 0x78, 0x30, 0x10, 0x8c, 0x07, 0x48, 0xc0, 0x02,
	0xa0, 0xa0, 0x4c, 0x24, 0x01, 0x0a, 0x00, 0xe2, 0x15, 0x10, 0x00, 0x38, 0x28, 0x14, 0xa7, 0xd4,
	0x6a, 0x60, 0x3a, 0xa8, 0x52, 0x06, 0x08, 0x09, 0x03, 0xe2, 0x01, 0x2a, 0xe0, 0x3a, 0xb2, 0x13,
	0x09, 0xd4, 0x2a, 0x56, 0xaa, 0xed, 0x36, 0xd5, 0x5b, 0xae, 0xc2, 0xc2, 0xf6, 0x3b, 0x2d, 0x3c,
	0x00, 0x7e, 0x08, 0x86, 0xea, 0xb0, 0xe0, 0x01, 0xa6, 0xfb, 0x38, 0x08, 0x51, 0x21, 0xf6, 0x58,
	0x85, 0xdc};

// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
static const t_u8 rg_rw610_WW[] = {
    0x01, 0x00, 0xee, 0x01, 0x06, 0x00, 0x57, 0x57, 0x20, 0x00, 0x00, 0x00, 0x06, 0x02, 0xb2, 0x00, 0x88, 0x88,
    0x03, 0x01, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x57, 0x20, 0x00, 0x03, 0x0a, 0x00, 0x02, 0x08, 0x05,
    0x82, 0x80, 0x61, 0x30, 0x58, 0x38, 0x02, 0x0d, 0x00, 0x84, 0xc0, 0x21, 0x10, 0x08, 0x48, 0x0f, 0x0c, 0x08,
    0x00, 0x41, 0x80, 0x10, 0x28, 0x04, 0x00, 0x02, 0x86, 0x43, 0xa2, 0x11, 0x28, 0xa4, 0x5a, 0x31, 0x0d, 0x87,
    0x80, 0x62, 0x31, 0x38, 0xac, 0x5c, 0x1f, 0x22, 0x85, 0x81, 0xa2, 0x32, 0x79, 0x24, 0x7a, 0x4f, 0x11, 0x08,
    0xca, 0x80, 0x32, 0xc8, 0xec, 0x9a, 0x51, 0x31, 0x95, 0xc7, 0x24, 0xa2, 0xc0, 0x88, 0x22, 0x73, 0x2d, 0x00,
    0x12, 0x02, 0x00, 0x99, 0xa4, 0xec, 0x00, 0x04, 0x01, 0x83, 0xc0, 0x00, 0x54, 0x00, 0x44, 0x07, 0x16, 0x15,
    0x42, 0x82, 0x90, 0x30, 0x20, 0x50, 0x15, 0x0a, 0x07, 0x01, 0x40, 0x01, 0x10, 0x98, 0x0a, 0xa6, 0x13, 0xaa,
    0x82, 0x82, 0x40, 0x1a, 0xcd, 0x6e, 0xc6, 0x13, 0x04, 0x04, 0xa0, 0xb5, 0xa0, 0x8d, 0x9c, 0x03, 0x5f, 0xb0,
    0xc3, 0x2c, 0xa1, 0x18, 0x65, 0x4a, 0xa9, 0x56, 0xb9, 0x5b, 0x23, 0x21, 0x2b, 0xcd, 0x6e, 0x18, 0x00, 0x3f,
    0x57, 0x2e, 0x77, 0xf3, 0x48, 0x00, 0xd3, 0x3d, 0x08, 0x5a, 0xeb, 0x71, 0x1c, 0x00};
#endif

#ifndef CONFIG_11AX
#ifndef CONFIG_11AC
static wifi_txpwrlimit_t
    tx_pwrlimit_2g_cfg =
        {
            .subband   = (wifi_SubBand_t)0x00,
            .num_chans = 14,
            .txpwrlimit_config[0] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 1,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 15}, {2, 15}, {3, 15}, {4, 13}, {5, 13}, {6, 13}},
                },
            .txpwrlimit_config[1] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 2,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 16}, {2, 16}, {3, 16}, {4, 16}, {5, 16}, {6, 16}},
                },
            .txpwrlimit_config[2] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 3,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 16}, {2, 16}, {3, 16}, {4, 16}, {5, 16}, {6, 16}},
                },
            .txpwrlimit_config[3] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 4,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18}, {3, 17}, {4, 16}, {5, 17}, {6, 16}},
                },
            .txpwrlimit_config[4] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 5,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18}, {3, 17}, {4, 16}, {5, 17}, {6, 16}},
                },
            .txpwrlimit_config[5] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 6,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18}, {3, 17}, {4, 16}, {5, 17}, {6, 16}},
                },
            .txpwrlimit_config[6] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 7,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18}, {3, 17}, {4, 16}, {5, 17}, {6, 16}},
                },
            .txpwrlimit_config[7] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 8,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18}, {3, 17}, {4, 16}, {5, 17}, {6, 16}},
                },
            .txpwrlimit_config[8] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 9,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 17}, {2, 17}, {3, 16}, {4, 16}, {5, 16}, {6, 16}},
                },
            .txpwrlimit_config[9] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 10,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 16}, {2, 16}, {3, 16}, {4, 15}, {5, 15}, {6, 15}},
                },
            .txpwrlimit_config[10] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 11,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 13}, {2, 13}, {3, 13}, {4, 12}, {5, 12}, {6, 12}},
                },
            .txpwrlimit_config[11] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 12,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18}, {3, 17}, {4, 16}, {5, 17}, {6, 16}},
                },
            .txpwrlimit_config[12] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 13,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18}, {3, 17}, {4, 16}, {5, 17}, {6, 16}},
                },
            .txpwrlimit_config[13] =
                {
                    .num_mod_grps = 7,
                    .chan_desc =
                        {
                            .start_freq = 2414,
                            .chan_width = 20,
                            .chan_num   = 14,
                        },
                    .txpwrlimit_entry = {{0, 19}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}},
                },
            .txpwrlimit_config[14] = {0},
            .txpwrlimit_config[15] = {0},
            .txpwrlimit_config[16] = {0},
            .txpwrlimit_config[17] = {0},
            .txpwrlimit_config[18] = {0},
            .txpwrlimit_config[19] = {0},
            .txpwrlimit_config[20] = {0},
            .txpwrlimit_config[21] = {0},
            .txpwrlimit_config[22] = {0},
            .txpwrlimit_config[23] = {0},
            .txpwrlimit_config[24] = {0},
            .txpwrlimit_config[25] = {0},
            .txpwrlimit_config[26] = {0},
            .txpwrlimit_config[27] = {0},
            .txpwrlimit_config[28] = {0},
            .txpwrlimit_config[29] = {0},
            .txpwrlimit_config[30] = {0},
            .txpwrlimit_config[31] = {0},
            .txpwrlimit_config[32] = {0},
            .txpwrlimit_config[33] = {0},
            .txpwrlimit_config[34] = {0},
            .txpwrlimit_config[35] = {0},
            .txpwrlimit_config[36] = {0},
            .txpwrlimit_config[37] = {0},
            .txpwrlimit_config[38] = {0},
            .txpwrlimit_config[39] = {0},
};

#if CONFIG_5GHz_SUPPORT
static wifi_txpwrlimit_t
    tx_pwrlimit_5g_cfg =
        {
            .subband   = (wifi_SubBand_t)0x00,
            .num_chans = 25,
            .txpwrlimit_config[0] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 36,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[1] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 40,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 19}, {4, 20}, {5, 20}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[2] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 44,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 18}, {4, 20}, {5, 20}, {6, 17}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[3] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 48,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 16}, {4, 20}, {5, 20}, {6, 16}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[4] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 52,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 17}, {4, 20}, {5, 20}, {6, 16}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[5] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 56,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 19}, {4, 20}, {5, 20}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[6] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 60,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 18}, {4, 20}, {5, 20}, {6, 17}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[7] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 64,
                        },
                    .txpwrlimit_entry = {{1, 19}, {2, 19}, {3, 16}, {4, 18}, {5, 18}, {6, 15}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[8] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 100,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 19}, {4, 20}, {5, 20}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[9] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 104,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 18}, {4, 20}, {5, 21}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[10] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 108,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 19}, {4, 20}, {5, 21}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[11] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 112,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 19}, {4, 20}, {5, 21}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[12] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 116,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 19}, {4, 21}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[13] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 120,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 19}, {4, 21}, {5, 20}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[14] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 124,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 18}, {4, 21}, {5, 20}, {6, 16}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[15] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 128,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 18}, {4, 21}, {5, 20}, {6, 16}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[16] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 132,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 18}, {4, 21}, {5, 20}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[17] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 136,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 18}, {4, 21}, {5, 20}, {6, 18}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[18] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 140,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 16}, {4, 21}, {5, 20}, {6, 15}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[19] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 144,
                        },
                    .txpwrlimit_entry = {{1, 21}, {2, 21}, {3, 16}, {4, 21}, {5, 20}, {6, 15}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[20] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 149,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 18}, {4, 21}, {5, 21}, {6, 17}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[21] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 153,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 18}, {4, 21}, {5, 21}, {6, 16}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[22] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 157,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 15}, {4, 21}, {5, 21}, {6, 14}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[23] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 161,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 13}, {4, 21}, {5, 21}, {6, 12}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[24] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 165,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 13}, {4, 20}, {5, 20}, {6, 12}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[25] = {0},
            .txpwrlimit_config[26] = {0},
            .txpwrlimit_config[27] = {0},
            .txpwrlimit_config[28] = {0},
            .txpwrlimit_config[29] = {0},
            .txpwrlimit_config[30] = {0},
            .txpwrlimit_config[31] = {0},
            .txpwrlimit_config[32] = {0},
            .txpwrlimit_config[33] = {0},
            .txpwrlimit_config[34] = {0},
            .txpwrlimit_config[35] = {0},
            .txpwrlimit_config[36] = {0},
            .txpwrlimit_config[37] = {0},
            .txpwrlimit_config[38] = {0},
            .txpwrlimit_config[39] = {0},
/* Rest of the channels listed below are not used and contains dummy power entries,
 * they belong to 4.9GHz Band for Public Safety.
 */
#if 0
            .txpwrlimit_config[25] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 183,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[26] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 184,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[27] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 185,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[28] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 187,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[29] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 188,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[30] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 189,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[31] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 192,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[32] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 196,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[33] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 7,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[34] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 8,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[35] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 11,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[36] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 12,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[37] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 16,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
            .txpwrlimit_config[38] =
                {
                    .num_mod_grps = 9,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 34,
                        },
                    .txpwrlimit_entry = {{1, 20}, {2, 20}, {3, 20}, {4, 20}, {5, 20}, {6, 19}, {7, 0}, {8, 0}, {9, 0}},
                },
#endif
};
#endif
#else
static wifi_txpwrlimit_t
    tx_pwrlimit_2g_cfg =
        {
            .subband   = (wifi_SubBand_t)0x00,
            .num_chans = 14,
            .txpwrlimit_config[0] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 1,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 15},
                                         {2, 15},
                                         {3, 15},
                                         {4, 13},
                                         {5, 13},
                                         {6, 13},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 13},
                                         {11, 0}},
                },
            .txpwrlimit_config[1] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 2,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 16},
                                         {2, 16},
                                         {3, 16},
                                         {4, 16},
                                         {5, 16},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0}},
                },
            .txpwrlimit_config[2] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 3,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 16},
                                         {2, 16},
                                         {3, 16},
                                         {4, 16},
                                         {5, 16},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0}},
                },
            .txpwrlimit_config[3] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 4,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 18},
                                         {2, 18},
                                         {3, 17},
                                         {4, 16},
                                         {5, 17},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 15},
                                         {11, 0}},
                },
            .txpwrlimit_config[4] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 5,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 18},
                                         {2, 18},
                                         {3, 17},
                                         {4, 16},
                                         {5, 17},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 15},
                                         {11, 0}},
                },
            .txpwrlimit_config[5] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 6,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 18},
                                         {2, 18},
                                         {3, 17},
                                         {4, 16},
                                         {5, 17},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 14},
                                         {11, 0}},
                },
            .txpwrlimit_config[6] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 7,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 18},
                                         {2, 18},
                                         {3, 17},
                                         {4, 16},
                                         {5, 17},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0}},
                },
            .txpwrlimit_config[7] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 8,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 18},
                                         {2, 18},
                                         {3, 17},
                                         {4, 16},
                                         {5, 17},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0}},
                },
            .txpwrlimit_config[8] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 9,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 17},
                                         {2, 17},
                                         {3, 16},
                                         {4, 16},
                                         {5, 16},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 15},
                                         {11, 0}},
                },
            .txpwrlimit_config[9] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 10,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 16},
                                         {2, 16},
                                         {3, 16},
                                         {4, 15},
                                         {5, 15},
                                         {6, 15},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 15},
                                         {11, 0}},
                },
            .txpwrlimit_config[10] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 11,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 13},
                                         {2, 13},
                                         {3, 13},
                                         {4, 12},
                                         {5, 12},
                                         {6, 12},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 12},
                                         {11, 0}},
                },
            .txpwrlimit_config[11] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 12,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 18},
                                         {2, 18},
                                         {3, 17},
                                         {4, 16},
                                         {5, 17},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0}},
                },
            .txpwrlimit_config[12] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2407,
                            .chan_width = 20,
                            .chan_num   = 13,
                        },
                    .txpwrlimit_entry = {{0, 19},
                                         {1, 18},
                                         {2, 18},
                                         {3, 17},
                                         {4, 16},
                                         {5, 17},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0}},
                },
            .txpwrlimit_config[13] =
                {
                    .num_mod_grps = 12,
                    .chan_desc =
                        {
                            .start_freq = 2414,
                            .chan_width = 20,
                            .chan_num   = 14,
                        },
                    .txpwrlimit_entry =
                        {{0, 19},
                         {1, 0},
                         {2, 0},
                         {3, 0},
                         {4, 0},
                         {5, 0},
                         {6, 0},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 0},
                         {11, 0}},
                },
            .txpwrlimit_config[14] = {0},
            .txpwrlimit_config[15] = {0},
            .txpwrlimit_config[16] = {0},
            .txpwrlimit_config[17] = {0},
            .txpwrlimit_config[18] = {0},
            .txpwrlimit_config[19] = {0},
            .txpwrlimit_config[20] = {0},
            .txpwrlimit_config[21] = {0},
            .txpwrlimit_config[22] = {0},
            .txpwrlimit_config[23] = {0},
            .txpwrlimit_config[24] = {0},
            .txpwrlimit_config[25] = {0},
            .txpwrlimit_config[26] = {0},
            .txpwrlimit_config[27] = {0},
            .txpwrlimit_config[28] = {0},
            .txpwrlimit_config[29] = {0},
            .txpwrlimit_config[30] = {0},
            .txpwrlimit_config[31] = {0},
            .txpwrlimit_config[32] = {0},
            .txpwrlimit_config[33] = {0},
            .txpwrlimit_config[34] = {0},
            .txpwrlimit_config[35] = {0},
            .txpwrlimit_config[36] = {0},
            .txpwrlimit_config[37] = {0},
            .txpwrlimit_config[38] = {0},
            .txpwrlimit_config[39] = {0},
};

#if CONFIG_5GHz_SUPPORT
static wifi_txpwrlimit_t
    tx_pwrlimit_5g_cfg =
        {
            .subband   = (wifi_SubBand_t)0x00,
            .num_chans = 25,
            .txpwrlimit_config[0] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 36,
                        },
                    .txpwrlimit_entry = {{0, 0},
                                         {1, 20},
                                         {2, 20},
                                         {3, 20},
                                         {4, 20},
                                         {5, 20},
                                         {6, 19},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0},
                                         {12, 0},
                                         {13, 0},
                                         {14, 0},
                                         {15, 0}},
                },
            .txpwrlimit_config[1] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 40,
                        },
                    .txpwrlimit_entry = {{0, 0},
                                         {1, 20},
                                         {2, 20},
                                         {3, 19},
                                         {4, 20},
                                         {5, 20},
                                         {6, 18},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0},
                                         {12, 0},
                                         {13, 0},
                                         {14, 0},
                                         {15, 0}},
                },
            .txpwrlimit_config[2] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 44,
                        },
                    .txpwrlimit_entry = {{0, 0},
                                         {1, 20},
                                         {2, 20},
                                         {3, 18},
                                         {4, 20},
                                         {5, 20},
                                         {6, 17},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0},
                                         {12, 0},
                                         {13, 0},
                                         {14, 0},
                                         {15, 0}},
                },
            .txpwrlimit_config[3] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 48,
                        },
                    .txpwrlimit_entry = {{0, 0},
                                         {1, 20},
                                         {2, 20},
                                         {3, 16},
                                         {4, 20},
                                         {5, 20},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 16},
                                         {11, 0},
                                         {12, 0},
                                         {13, 0},
                                         {14, 0},
                                         {15, 0}},
                },
            .txpwrlimit_config[4] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 52,
                        },
                    .txpwrlimit_entry = {{0, 0},
                                         {1, 20},
                                         {2, 20},
                                         {3, 17},
                                         {4, 20},
                                         {5, 20},
                                         {6, 16},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 15},
                                         {11, 0},
                                         {12, 0},
                                         {13, 0},
                                         {14, 0},
                                         {15, 0}},
                },
            .txpwrlimit_config[5] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 56,
                        },
                    .txpwrlimit_entry = {{0, 0},
                                         {1, 20},
                                         {2, 20},
                                         {3, 19},
                                         {4, 20},
                                         {5, 20},
                                         {6, 18},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 15},
                                         {11, 0},
                                         {12, 0},
                                         {13, 0},
                                         {14, 0},
                                         {15, 0}},
                },
            .txpwrlimit_config[6] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 60,
                        },
                    .txpwrlimit_entry = {{0, 0},
                                         {1, 20},
                                         {2, 20},
                                         {3, 18},
                                         {4, 20},
                                         {5, 20},
                                         {6, 17},
                                         {7, 0},
                                         {8, 0},
                                         {9, 0},
                                         {10, 15},
                                         {11, 0},
                                         {12, 0},
                                         {13, 0},
                                         {14, 0},
                                         {15, 0}},
                },
            .txpwrlimit_config[7] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 64,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 19},
                         {2, 19},
                         {3, 16},
                         {4, 18},
                         {5, 18},
                         {6, 15},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[8] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 100,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 19},
                         {4, 20},
                         {5, 20},
                         {6, 18},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[9] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 104,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 18},
                         {4, 20},
                         {5, 21},
                         {6, 18},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[10] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 108,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 19},
                         {4, 20},
                         {5, 21},
                         {6, 18},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[11] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 112,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 19},
                         {4, 20},
                         {5, 21},
                         {6, 18},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[12] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 116,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 19},
                         {4, 21},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[13] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 120,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 19},
                         {4, 21},
                         {5, 20},
                         {6, 18},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[14] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 124,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 18},
                         {4, 21},
                         {5, 20},
                         {6, 16},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[15] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 128,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 18},
                         {4, 21},
                         {5, 20},
                         {6, 16},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[16] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 132,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 18},
                         {4, 21},
                         {5, 20},
                         {6, 18},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[17] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 136,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 18},
                         {4, 21},
                         {5, 20},
                         {6, 18},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[18] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 140,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 16},
                         {4, 21},
                         {5, 20},
                         {6, 15},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[19] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 144,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 21},
                         {2, 21},
                         {3, 16},
                         {4, 21},
                         {5, 20},
                         {6, 15},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 15},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[20] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 149,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 18},
                         {4, 21},
                         {5, 21},
                         {6, 17},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 12},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[21] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 153,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 18},
                         {4, 21},
                         {5, 21},
                         {6, 16},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 12},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[22] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 157,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 15},
                         {4, 21},
                         {5, 21},
                         {6, 14},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 12},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[23] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 161,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 13},
                         {4, 21},
                         {5, 21},
                         {6, 12},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 12},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[24] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 165,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 13},
                         {4, 20},
                         {5, 20},
                         {6, 12},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 12},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[25] = {0},
            .txpwrlimit_config[26] = {0},
            .txpwrlimit_config[27] = {0},
            .txpwrlimit_config[28] = {0},
            .txpwrlimit_config[29] = {0},
            .txpwrlimit_config[30] = {0},
            .txpwrlimit_config[31] = {0},
            .txpwrlimit_config[32] = {0},
            .txpwrlimit_config[33] = {0},
            .txpwrlimit_config[34] = {0},
            .txpwrlimit_config[35] = {0},
            .txpwrlimit_config[36] = {0},
            .txpwrlimit_config[37] = {0},
            .txpwrlimit_config[38] = {0},
            .txpwrlimit_config[39] = {0},
/* Rest of the channels listed below are not used and contains dummy power entries,
 * they belong to 4.9GHz Band for Public Safety.
 */
#if 0
            .txpwrlimit_config[25] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 183,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[26] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 184,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 18},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[27] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 185,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[28] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 187,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[29] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 188,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[30] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 189,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[31] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 192,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[32] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 196,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[33] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 7,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[34] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 8,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[35] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 11,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[36] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 12,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[37] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 16,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
            .txpwrlimit_config[38] =
                {
                    .num_mod_grps = 16,
                    .chan_desc =
                        {
                            .start_freq = 5000,
                            .chan_width = 20,
                            .chan_num   = 34,
                        },
                    .txpwrlimit_entry =
                        {{0, 0},
                         {1, 20},
                         {2, 20},
                         {3, 20},
                         {4, 20},
                         {5, 20},
                         {6, 19},
                         {7, 0},
                         {8, 0},
                         {9, 0},
                         {10, 16},
                         {11, 0},
                         {12, 0},
                         {13, 0},
                         {14, 0},
                         {15, 0}},
                },
#endif
};
#endif /* CONFIG_5GHz_SUPPORT */
#endif /* CONFIG_11AC */
#else
// coverity[MISRA C-2012 Initializers :SUPPRESS]
static wifi_txpwrlimit_t tx_pwrlimit_2g_cfg = {
    .subband   = (wifi_SubBand_t)0x00,
    .num_chans = 14,
    .txpwrlimit_config[0] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 1,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 15}, {2, 15},  {3, 15},  {4, 13}, {5, 13}, {6, 13},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 13}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 13}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[1] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 2,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 16}, {2, 16},  {3, 16},  {4, 16}, {5, 16}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 15}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[2] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 3,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 16}, {2, 16},  {3, 16},  {4, 16}, {5, 16}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 15}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[3] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 4,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18},  {3, 17},  {4, 16}, {5, 17}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 12}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[4] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 5,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18},  {3, 17},  {4, 16}, {5, 17}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 13}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[5] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 6,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18},  {3, 17},  {4, 16}, {5, 17}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 14}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[6] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 7,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18},  {3, 17},  {4, 16}, {5, 17}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 15}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[7] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 8,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18},  {3, 17},  {4, 16}, {5, 17}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[8] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 9,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 17}, {2, 17},  {3, 16},  {4, 16}, {5, 16}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[9] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 10,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 16}, {2, 16},  {3, 16},  {4, 15}, {5, 15}, {6, 15},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[10] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 11,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 13}, {2, 13},  {3, 13},  {4, 12}, {5, 12}, {6, 12},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 12}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 12}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[11] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 12,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18},  {3, 17},  {4, 16}, {5, 17}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[12] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2407,
                    .chan_width = 20,
                    .chan_num   = 13,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 18}, {2, 18},  {3, 17},  {4, 16}, {5, 17}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[13] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 2414,
                    .chan_width = 20,
                    .chan_num   = 14,
                },
            .txpwrlimit_entry = {{0, 19}, {1, 0},  {2, 0},  {3, 0},  {4, 0},  {5, 0},  {6, 0},
                                 {7, 0},  {8, 0},  {9, 0},  {10, 0}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[14] = {0},
    .txpwrlimit_config[15] = {0},
    .txpwrlimit_config[16] = {0},
    .txpwrlimit_config[17] = {0},
    .txpwrlimit_config[18] = {0},
    .txpwrlimit_config[19] = {0},
    .txpwrlimit_config[20] = {0},
    .txpwrlimit_config[21] = {0},
    .txpwrlimit_config[22] = {0},
    .txpwrlimit_config[23] = {0},
    .txpwrlimit_config[24] = {0},
    .txpwrlimit_config[25] = {0},
    .txpwrlimit_config[26] = {0},
    .txpwrlimit_config[27] = {0},
    .txpwrlimit_config[28] = {0},
    .txpwrlimit_config[29] = {0},
    .txpwrlimit_config[30] = {0},
    .txpwrlimit_config[31] = {0},
    .txpwrlimit_config[32] = {0},
    .txpwrlimit_config[33] = {0},
    .txpwrlimit_config[34] = {0},
    .txpwrlimit_config[35] = {0},
    .txpwrlimit_config[36] = {0},
    .txpwrlimit_config[37] = {0},
    .txpwrlimit_config[38] = {0},
    .txpwrlimit_config[39] = {0},
};

#if CONFIG_5GHz_SUPPORT
static wifi_txpwrlimit_t tx_pwrlimit_5g_cfg = {
    .subband   = (wifi_SubBand_t)0x00,
    .num_chans = 25,
    .txpwrlimit_config[0] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 36,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[1] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 40,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 19},  {4, 20}, {5, 20}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[2] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 44,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 18},  {4, 20}, {5, 20}, {6, 17},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[3] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 48,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 16},  {4, 20}, {5, 20}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[4] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 52,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 17},  {4, 20}, {5, 20}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 15}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[5] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 56,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 19},  {4, 20}, {5, 20}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 15}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[6] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 60,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 18},  {4, 20}, {5, 20}, {6, 17},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 15}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[7] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 64,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 19}, {2, 19},  {3, 16},  {4, 18}, {5, 18}, {6, 15},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 15}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[8] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 100,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 19},  {4, 20}, {5, 20}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[9] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 104,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 18},  {4, 20}, {5, 21}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[10] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 108,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 19},  {4, 20}, {5, 21}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[11] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 112,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 19},  {4, 20}, {5, 21}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[12] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 116,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 19},  {4, 21}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[13] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 120,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 19},  {4, 21}, {5, 20}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[14] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 124,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 18},  {4, 21}, {5, 20}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[15] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 128,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 18},  {4, 21}, {5, 20}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[16] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 132,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 18},  {4, 21}, {5, 20}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[17] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 136,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 18},  {4, 21}, {5, 20}, {6, 18},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[18] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 140,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 16},  {4, 21}, {5, 20}, {6, 15},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[19] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 144,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 21}, {2, 21},  {3, 16},  {4, 21}, {5, 20}, {6, 15},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 15}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 14}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[20] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 149,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 18},  {4, 21}, {5, 21}, {6, 17},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 12}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 10}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[21] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 153,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 18},  {4, 21}, {5, 21}, {6, 16},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 12}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 10}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[22] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 157,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 15},  {4, 21}, {5, 21}, {6, 14},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 12}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 10}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[23] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 161,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 13},  {4, 21}, {5, 21}, {6, 12},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 12}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 10}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[24] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 165,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 13},  {4, 20}, {5, 20}, {6, 12},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 12}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 10}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[25] = {0},
    .txpwrlimit_config[26] = {0},
    .txpwrlimit_config[27] = {0},
    .txpwrlimit_config[28] = {0},
    .txpwrlimit_config[29] = {0},
    .txpwrlimit_config[30] = {0},
    .txpwrlimit_config[31] = {0},
    .txpwrlimit_config[32] = {0},
    .txpwrlimit_config[33] = {0},
    .txpwrlimit_config[34] = {0},
    .txpwrlimit_config[35] = {0},
    .txpwrlimit_config[36] = {0},
    .txpwrlimit_config[37] = {0},
    .txpwrlimit_config[38] = {0},
    .txpwrlimit_config[39] = {0},
/* Rest of the channels listed below are not used and contains dummy power entries,
 * they belong to 4.9GHz Band for Public Safety.
 */
#if 0
    .txpwrlimit_config[25] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 183,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[26] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 184,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 18}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 18}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[27] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 185,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[28] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 187,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[29] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 188,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[30] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 189,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[31] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 192,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[32] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 196,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[33] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 7,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[34] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 8,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[35] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 11,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[36] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 12,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[37] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 16,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
    .txpwrlimit_config[38] =
        {
            .num_mod_grps = 20,
            .chan_desc =
                {
                    .start_freq = 5000,
                    .chan_width = 20,
                    .chan_num   = 34,
                },
            .txpwrlimit_entry = {{0, 0},  {1, 20}, {2, 20},  {3, 20},  {4, 20}, {5, 20}, {6, 19},
                                 {7, 0},  {8, 0},  {9, 0},   {10, 16}, {11, 0}, {12, 0}, {13, 0},
                                 {14, 0}, {15, 0}, {16, 16}, {17, 0},  {18, 0}, {19, 0}},
        },
#endif
};
#endif /* CONFIG_5GHz_SUPPORT */
#endif /* CONFIG_11AX */

#if CONFIG_11AX
#if CONFIG_COMPRESS_RU_TX_PWTBL
const static uint8_t rutxpowerlimit_bga_cfg_set[] = {
    0x6d, 0x02, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x11, 0x11, 0x11, 0x12, 0x00,
    0x00, 0x00, 0x12, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d,
    0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11,
    0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00,
    0x09, 0x09, 0x09, 0x09, 0x00, 0x00, 0x00, 0x12, 0x12, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0f, 0x12, 0x11,
    0x11, 0x00, 0x00, 0x00, 0x0b, 0x0e, 0x11, 0x11, 0x00, 0x00, 0x00, 0x13, 0x13, 0x12, 0x10, 0x00};

const static uint8_t rutxpowerlimit_qfn_cfg_set[] = {
    0x6d, 0x02, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x11, 0x11, 0x11, 0x12, 0x00,
    0x00, 0x00, 0x12, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d,
    0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11,
    0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00,
    0x09, 0x09, 0x09, 0x09, 0x00, 0x00, 0x00, 0x12, 0x12, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0f, 0x12, 0x11,
    0x11, 0x00, 0x00, 0x00, 0x0b, 0x0e, 0x11, 0x11, 0x00, 0x00, 0x00, 0x14, 0x14, 0x12, 0x10, 0x00};

const static uint8_t rutxpowerlimit_csp_cfg_set[] = {
    0x6d, 0x02, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x11, 0x11, 0x11, 0x12, 0x00,
    0x00, 0x00, 0x12, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d,
    0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11,
    0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00,
    0x09, 0x09, 0x09, 0x09, 0x00, 0x00, 0x00, 0x12, 0x12, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0f, 0x12, 0x11,
    0x11, 0x00, 0x00, 0x00, 0x0b, 0x0e, 0x11, 0x11, 0x00, 0x00, 0x00, 0x14, 0x14, 0x11, 0x10, 0x00};

const static uint8_t rutxpowerlimit_cfg_set_WW[] = {
	0x6d, 0x02, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x04, 0x06, 0x0a, 0x0e,
	0x00, 0x00, 0x00, 0x04, 0x06, 0x0a, 0x0e, 0x00, 0x00, 0x00, 0x04, 0x06, 0x0a, 0x0e, 0x00, 0x00,
	0x00, 0x06, 0x08, 0x0b, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x05, 0x08, 0x0c, 0x00, 0x00, 0x00, 0x02,
	0x05, 0x08, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00, 0x06, 0x0b, 0x0e,
	0x11, 0x00, 0x00, 0x00, 0x06, 0x09, 0x09, 0x09, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00,
	0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x08, 0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00,
	0x0e, 0x0e, 0x0e, 0x0e, 0x00, 0x64, 0x02, 0x03, 0x00, 0x88, 0x88, 0x00};

const static uint8_t rutxpowerlimit_cfg_set_FCC[] = {
	0x6d, 0x02, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x11, 0x12, 0x12, 0x12,
	0x00, 0x00, 0x00, 0x11, 0x12, 0x12, 0x11, 0x00, 0x00, 0x00, 0x0f, 0x12, 0x11, 0x10, 0x00, 0x00,
	0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d,
	0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0d, 0x0f, 0x11,
	0x11, 0x00, 0x00, 0x00, 0x09, 0x09, 0x09, 0x09, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00,
	0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x08, 0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00,
	0x13, 0x13, 0x12, 0x10, 0x00, 0x64, 0x02, 0x03, 0x00, 0x88, 0x88, 0x00};

const static uint8_t rutxpowerlimit_cfg_set_EU[] = {
	0x6d, 0x02, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x06, 0x0b, 0x0f, 0x12,
	0x00, 0x00, 0x00, 0x0a, 0x0d, 0x10, 0x12, 0x00, 0x00, 0x00, 0x06, 0x09, 0x10, 0x11, 0x00, 0x00,
	0x00, 0x0d, 0x0f, 0x11, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09,
	0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f,
	0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00,
	0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00,
	0x0e, 0x0e, 0x0e, 0x0e, 0x00, 0x64, 0x02, 0x03, 0x00, 0x88, 0x88, 0x00};

const static uint8_t rutxpowerlimit_cfg_set_JP[] = {
	0x6d, 0x02, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x04, 0x06, 0x0a, 0x0e,
	0x00, 0x00, 0x00, 0x04, 0x06, 0x0a, 0x0e, 0x00, 0x00, 0x00, 0x04, 0x06, 0x0a, 0x0e, 0x00, 0x00,
	0x00, 0x06, 0x08, 0x0b, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x05, 0x08, 0x0c, 0x00, 0x00, 0x00, 0x02,
	0x05, 0x08, 0x0c, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f,
	0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00,
	0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00, 0x09, 0x0c, 0x0f, 0x11, 0x00, 0x00, 0x00,
	0x13, 0x13, 0x12, 0x10, 0x00, 0x64, 0x02, 0x03, 0x00, 0x88, 0x88, 0x00};

const static uint8_t rutxpowerlimit_cfg_set_CN[] = {
	0x6d, 0x02, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x01, 0x05, 0x09, 0x0c, 0x10,
	0x00, 0x00, 0x00, 0x05, 0x09, 0x0c, 0x10, 0x00, 0x00, 0x00, 0x05, 0x09, 0x0c, 0x10, 0x00, 0x00,
	0x00, 0x06, 0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00, 0x06, 0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00, 0x06,
	0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00, 0x06, 0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00, 0x06, 0x0b, 0x0e,
	0x11, 0x00, 0x00, 0x00, 0x06, 0x0b, 0x0e, 0x11, 0x00, 0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x00,
	0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00,
	0x10, 0x10, 0x10, 0x0f, 0x00, 0x64, 0x02, 0x03, 0x00, 0x88, 0x88, 0x00};
#else
#define MAX_2G_RU_PWR_CHANNELS 26
#define MAX_5G_RU_PWR_CHANNELS 69
// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
const static wlan_rutxpwrlimit_t rutxpowerlimit_2g_cfg_set = {
    .num_chans            = MAX_2G_RU_PWR_CHANNELS,
    .rupwrlimit_config[0] = {.start_freq = 2407, .width = 20, .chan_num = 1, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[1] = {.start_freq = 2407, .width = 20, .chan_num = 2, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[2] = {.start_freq = 2407, .width = 20, .chan_num = 3, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[3] = {.start_freq = 2407, .width = 20, .chan_num = 4, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[4] = {.start_freq = 2407, .width = 20, .chan_num = 5, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[5] = {.start_freq = 2407, .width = 20, .chan_num = 6, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[6] = {.start_freq = 2407, .width = 20, .chan_num = 7, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[7] = {.start_freq = 2407, .width = 20, .chan_num = 8, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[8] = {.start_freq = 2407, .width = 20, .chan_num = 9, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[9] = {.start_freq = 2407, .width = 20, .chan_num = 10, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[10] = {.start_freq = 2407, .width = 20, .chan_num = 11, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[11] = {.start_freq = 2407, .width = 20, .chan_num = 12, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[12] = {.start_freq = 2407, .width = 20, .chan_num = 13, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[13] = {.start_freq = 2407, .width = 40, .chan_num = 1, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[14] = {.start_freq = 2407, .width = 40, .chan_num = 2, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[15] = {.start_freq = 2407, .width = 40, .chan_num = 3, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[16] = {.start_freq = 2407, .width = 40, .chan_num = 4, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[17] = {.start_freq = 2407, .width = 40, .chan_num = 5, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[18] = {.start_freq = 2407, .width = 40, .chan_num = 6, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[19] = {.start_freq = 2407, .width = 40, .chan_num = 7, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[20] = {.start_freq = 2407, .width = 40, .chan_num = 8, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[21] = {.start_freq = 2407, .width = 40, .chan_num = 9, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[22] = {.start_freq = 2407, .width = 40, .chan_num = 10, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[23] = {.start_freq = 2407, .width = 40, .chan_num = 11, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[24] = {.start_freq = 2407, .width = 40, .chan_num = 12, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[25] = {.start_freq = 2407, .width = 40, .chan_num = 13, .ruPower = {-4, -1, 2, 5, 8, 0}},
};

#if CONFIG_5GHz_SUPPORT
// coverity[MISRA C-2012 Rule 2.2:SUPPRESS]
const static wlan_rutxpwrlimit_t rutxpowerlimit_5g_cfg_set = {
    .num_chans            = MAX_5G_RU_PWR_CHANNELS,
    .rupwrlimit_config[0] = {.start_freq = 5000, .width = 20, .chan_num = 36, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[1] = {.start_freq = 5000, .width = 20, .chan_num = 40, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[2] = {.start_freq = 5000, .width = 20, .chan_num = 44, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[3] = {.start_freq = 5000, .width = 20, .chan_num = 48, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[4] = {.start_freq = 5000, .width = 20, .chan_num = 52, .ruPower = {-2, 1, 4, 7, 0, 0}},

    .rupwrlimit_config[5] = {.start_freq = 5000, .width = 20, .chan_num = 56, .ruPower = {-2, 1, 4, 7, 0, 0}},

    .rupwrlimit_config[6] = {.start_freq = 5000, .width = 20, .chan_num = 60, .ruPower = {-2, 1, 4, 7, 0, 0}},

    .rupwrlimit_config[7] = {.start_freq = 5000, .width = 20, .chan_num = 64, .ruPower = {-2, 1, 4, 7, 0, 0}},

    .rupwrlimit_config[8] = {.start_freq = 5000, .width = 20, .chan_num = 100, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[9] = {.start_freq = 5000, .width = 20, .chan_num = 104, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[10] = {.start_freq = 5000, .width = 20, .chan_num = 108, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[11] = {.start_freq = 5000, .width = 20, .chan_num = 112, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[12] = {.start_freq = 5000, .width = 20, .chan_num = 116, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[13] = {.start_freq = 5000, .width = 20, .chan_num = 120, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[14] = {.start_freq = 5000, .width = 20, .chan_num = 124, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[15] = {.start_freq = 5000, .width = 20, .chan_num = 128, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[16] = {.start_freq = 5000, .width = 20, .chan_num = 132, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[17] = {.start_freq = 5000, .width = 20, .chan_num = 136, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[18] = {.start_freq = 5000, .width = 20, .chan_num = 140, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[19] = {.start_freq = 5000, .width = 20, .chan_num = 144, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[20] = {.start_freq = 5000, .width = 20, .chan_num = 149, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[21] = {.start_freq = 5000, .width = 20, .chan_num = 153, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[22] = {.start_freq = 5000, .width = 20, .chan_num = 157, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[23] = {.start_freq = 5000, .width = 20, .chan_num = 161, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[24] = {.start_freq = 5000, .width = 20, .chan_num = 165, .ruPower = {-1, 2, 5, 8, 0, 0}},

    .rupwrlimit_config[25] = {.start_freq = 5000, .width = 40, .chan_num = 36, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[26] = {.start_freq = 5000, .width = 40, .chan_num = 40, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[27] = {.start_freq = 5000, .width = 40, .chan_num = 44, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[28] = {.start_freq = 5000, .width = 40, .chan_num = 48, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[29] = {.start_freq = 5000, .width = 40, .chan_num = 52, .ruPower = {-5, -2, 1, 4, 7, 0}},

    .rupwrlimit_config[30] = {.start_freq = 5000, .width = 40, .chan_num = 56, .ruPower = {-5, -2, 1, 4, 7, 0}},

    .rupwrlimit_config[31] = {.start_freq = 5000, .width = 40, .chan_num = 60, .ruPower = {-5, -2, 1, 4, 7, 0}},

    .rupwrlimit_config[32] = {.start_freq = 5000, .width = 40, .chan_num = 64, .ruPower = {-5, -2, 1, 4, 7, 0}},

    .rupwrlimit_config[33] = {.start_freq = 5000, .width = 40, .chan_num = 100, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[34] = {.start_freq = 5000, .width = 40, .chan_num = 104, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[35] = {.start_freq = 5000, .width = 40, .chan_num = 108, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[36] = {.start_freq = 5000, .width = 40, .chan_num = 112, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[37] = {.start_freq = 5000, .width = 40, .chan_num = 116, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[38] = {.start_freq = 5000, .width = 40, .chan_num = 120, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[39] = {.start_freq = 5000, .width = 40, .chan_num = 124, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[40] = {.start_freq = 5000, .width = 40, .chan_num = 128, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[41] = {.start_freq = 5000, .width = 40, .chan_num = 132, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[42] = {.start_freq = 5000, .width = 40, .chan_num = 136, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[43] = {.start_freq = 5000, .width = 40, .chan_num = 140, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[44] = {.start_freq = 5000, .width = 40, .chan_num = 144, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[45] = {.start_freq = 5000, .width = 40, .chan_num = 149, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[46] = {.start_freq = 5000, .width = 40, .chan_num = 153, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[47] = {.start_freq = 5000, .width = 40, .chan_num = 157, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[48] = {.start_freq = 5000, .width = 40, .chan_num = 161, .ruPower = {-4, -1, 2, 5, 8, 0}},

    .rupwrlimit_config[49] = {.start_freq = 5000, .width = 80, .chan_num = 36, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[50] = {.start_freq = 5000, .width = 80, .chan_num = 40, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[51] = {.start_freq = 5000, .width = 80, .chan_num = 44, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[52] = {.start_freq = 5000, .width = 80, .chan_num = 48, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[53] = {.start_freq = 5000, .width = 80, .chan_num = 52, .ruPower = {-8, -5, -2, 1, 4, 7}},

    .rupwrlimit_config[54] = {.start_freq = 5000, .width = 80, .chan_num = 56, .ruPower = {-8, -5, -2, 1, 4, 7}},

    .rupwrlimit_config[55] = {.start_freq = 5000, .width = 80, .chan_num = 60, .ruPower = {-8, -5, -2, 1, 4, 7}},

    .rupwrlimit_config[56] = {.start_freq = 5000, .width = 80, .chan_num = 64, .ruPower = {-8, -5, -2, 1, 4, 7}},

    .rupwrlimit_config[57] = {.start_freq = 5000, .width = 80, .chan_num = 100, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[58] = {.start_freq = 5000, .width = 80, .chan_num = 104, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[59] = {.start_freq = 5000, .width = 80, .chan_num = 108, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[60] = {.start_freq = 5000, .width = 80, .chan_num = 112, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[61] = {.start_freq = 5000, .width = 80, .chan_num = 116, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[62] = {.start_freq = 5000, .width = 80, .chan_num = 120, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[63] = {.start_freq = 5000, .width = 80, .chan_num = 124, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[64] = {.start_freq = 5000, .width = 80, .chan_num = 128, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[65] = {.start_freq = 5000, .width = 80, .chan_num = 149, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[66] = {.start_freq = 5000, .width = 80, .chan_num = 153, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[67] = {.start_freq = 5000, .width = 80, .chan_num = 157, .ruPower = {-7, -4, -1, 2, 5, 8}},

    .rupwrlimit_config[68] = {.start_freq = 5000, .width = 80, .chan_num = 161, .ruPower = {-7, -4, -1, 2, 5, 8}},
};
#endif /* CONFIG_5GHz_SUPPORT */
#endif /* CONFIG_COMPRESS_RU_TX_PWTBL */
#endif /* CONFIG_11AX */

#endif /* _WLAN_TXPWRLIMIT_CFG_WW_RW610_H_ */
