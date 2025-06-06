/** @file mlan_init.h
 *
 *  @brief This file defines the FW initialization data
 *  structures.
 *
 *  Copyright 2008-2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    10/13/2008: initial version
******************************************************/

#ifndef _MLAN_INIT_H_
#define _MLAN_INIT_H_

/** Tx buffer size for firmware download*/
#define FW_DNLD_TX_BUF_SIZE 620
/** Rx buffer size for firmware download*/
#define FW_DNLD_RX_BUF_SIZE 2048
/** Max firmware retry */
#define MAX_FW_RETRY 3

/** Firmware has last block */
#define FW_HAS_LAST_BLOCK 0x00000004

/** Firmware data transmit size */
#define FW_DATA_XMIT_SIZE sizeof(FWHeader) + DataLength + sizeof(t_u32)

/** FWHeader */
typedef struct _FWHeader
{
    /** FW download command */
    t_u32 dnld_cmd;
    /** FW base address */
    t_u32 base_addr;
    /** FW data length */
    t_u32 data_length;
    /** FW CRC */
    t_u32 crc;
} FWHeader;

/** FWData */
typedef struct _FWData
{
    /** FW data header */
    FWHeader fw_header;
    /** FW data sequence number */
    t_u32 seq_num;
    /** FW data buffer */
    t_u8 data[1];
} FWData;

/** FWSyncHeader */
typedef struct _FWSyncHeader
{
    /** FW sync header command */
    t_u32 cmd;
    /** FW sync header sequence number */
    t_u32 seq_num;
} FWSyncHeader;

/** Convert sequence number and command fields of fwheader to correct endian format */
#define endian_convert_syncfwheader(x)

#endif /* _MLAN_INIT_H_ */
