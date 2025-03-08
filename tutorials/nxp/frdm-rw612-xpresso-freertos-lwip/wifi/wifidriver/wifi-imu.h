/** @file wifi-imu.h
 *
 *  @brief WLAN on IMU
 *
 *  Copyright 2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __WIFI_IMU_H__
#define __WIFI_IMU_H__

#include <wifi.h>
#include "fsl_power.h"

#ifndef RW610
#include "firmware_dnld.h"
#endif

#define wifi_io_e(...) wmlog_e("wifi_io", ##__VA_ARGS__)
#define wifi_io_w(...) wmlog_w("wifi_io", ##__VA_ARGS__)

#if CONFIG_WIFI_IO_DEBUG
#define wifi_io_d(...) wmlog("wifi_io", ##__VA_ARGS__)
#else
#define wifi_io_d(...)
#endif /* ! CONFIG_WIFI_IO_DEBUG */

#define wifi_io_info_e(...) wmlog_e("wpkt", ##__VA_ARGS__)
#define wifi_io_info_w(...) wmlog_w("wpkt", ##__VA_ARGS__)

#if CONFIG_WIFI_IO_INFO_DUMP
#define wifi_io_info_d(...) wmlog("wpkt", ##__VA_ARGS__)
#else
#define wifi_io_info_d(...)
#endif

#define WLAN_MAGIC_NUM (('W' << 0) | ('L' << 8) | ('F' << 16) | ('W' << 24))

#define WIFI_FW_CMDBUF_SIZE 3072U

#define WIFI_RESP_WAIT_TIME 10

#define WLAN_VALUE1 0x45001114

/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (4U)
/*! @brief Data buffer size. */
#define DATA_BUFFER_SIZE (512 * DATA_BLOCK_COUNT)

/** Card Control Registers : Function 1 Block size 0 */
#define FN1_BLOCK_SIZE_0 0x110
/** Card Control Registers : Function 1 Block size 1 */
#define FN1_BLOCK_SIZE_1 0x111

/* Duplicated in wlan.c. keep in sync till we can be included directly */
typedef struct __nvram_backup_struct
{
    t_u32 ioport;
    t_u32 curr_wr_port;
    t_u32 curr_rd_port;
    t_u32 mp_end_port;
    t_u32 bss_num;
    t_u32 sta_mac_addr1;
    t_u32 sta_mac_addr2;
    t_u32 wifi_state;
} nvram_backup_t;
#ifndef RW610
extern osa_task_handle_t wifi_core_thread;
#endif
extern bool g_txrx_flag;
#ifdef WLAN_LOW_POWER_ENABLE
extern bool low_power_mode;
#endif
extern bool cal_data_valid;
extern bool mac_addr_valid;
#if CONFIG_WIFI_TX_BUFF
extern uint16_t tx_buf_size;
#endif
extern bool txpwrlimit_data_valid;
extern uint8_t trpc_country;
extern bus_operations bus_ops;

mlan_status imu_wifi_init(enum wlan_type type, const uint8_t *fw_ram_start_addr, const size_t size);
void imu_wifi_deinit(void);
void imu_uninstall_callback(void);

/*
 * @internal
 *
 *
 */
int wlan_send_imu_cmd(t_u8 *buf);

/*
 * @internal
 *
 *
 */
int wifi_send_cmdbuffer(void);

/*
 * @internal
 *
 *
 */
HostCmd_DS_COMMAND *wifi_get_command_buffer(void);

mlan_status wlan_xmit_pkt(t_u8 *buffer, t_u32 txlen, t_u8 interface, t_u32 tx_control);
int raw_process_pkt_hdrs(void *pbuf, t_u32 payloadlen, t_u8 interface);
uint32_t wifi_get_device_value1();

uint8_t *wifi_get_imu_outbuf(uint32_t *outbuf_len);

void process_pkt_hdrs(void *pbuf, t_u32 payloadlen, t_u8 interface, t_u8 tid, t_u32 tx_control);

#if CONFIG_WIFI_FW_DEBUG
extern void wifi_dump_firmware_info();
#endif /* CONFIG_WIFI_FW_DEBUG */

#if CONFIG_WMM
mlan_status wlan_xmit_wmm_pkt(t_u8 interface, t_u32 txlen, t_u8 *tx_buf);
mlan_status wlan_flush_wmm_pkt(int pkt_cnt);
mlan_status wlan_xmit_bypass_pkt(t_u8 *buffer, t_u32 txlen, t_u8 interface);
#if CONFIG_AMSDU_IN_AMPDU
uint8_t *wifi_get_amsdu_outbuf(uint32_t offset);
mlan_status wlan_xmit_wmm_amsdu_pkt(mlan_wmm_ac_e ac, t_u8 interface, t_u32 txlen, t_u8 *tx_buf, t_u8 amsdu_cnt);
#endif
#endif

void imu_wakeup_card();
#if CONFIG_WIFI_TX_BUFF
int _wlan_return_all_tx_buf(imu_link_t link);
#endif

int imu_create_task_lock(void);
void imu_delete_task_lock(void);
int imu_get_task_lock(void);
int imu_put_task_lock(void);
#endif /* __WIFI_SDIO_H__ */
