/** @file wifi-imu.c
 *
 *  @brief  This file provides WLAN Card related API
 *
 *  Copyright 2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <mlan_api.h>
#ifndef RW610
#include <mlan_sdio_api.h>
#endif
/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>
#include <wm_utils.h>
#include <mlan_fw.h>
#include "wifi-imu.h"
#include "wifi-internal.h"
#include "fsl_common.h"
#include "fsl_adapter_imu.h"
#include "fsl_imu.h"
#include "fsl_loader.h"

/* Buffer pointers to point to command and, command response buffer */
static uint8_t cmd_buf[WIFI_FW_CMDBUF_SIZE];
// static t_u32 seqnum;
// static int pm_handle;
#define IMU_OUTBUF_LEN       3072
#define IMU_INIT_FW_CMD_SIZE 256

#if defined(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
#ifndef MCI_WAKEUP_DONE_PRIORITY
#define MCI_WAKEUP_DONE_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#endif
#ifndef MCI_WAKEUP_SRC_PRIORITY
#define MCI_WAKEUP_SRC_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2)
#endif
#else
#ifndef MCI_WAKEUP_DONE_PRIORITY
#define MCI_WAKEUP_DONE_PRIORITY (3U)
#endif
#ifndef MCI_WAKEUP_SRC_PRIORITY
#define MCI_WAKEUP_SRC_PRIORITY (4U)
#endif
#endif
/*
 * Used to authorize the SDIO interrupt handler to accept the incoming
 * packet from the SDIO interface. If this flag is set a semaphore is
 * signalled.
 */
bool g_txrx_flag;

#ifdef RW610
bool cal_data_valid_rw610;
#endif

int mlan_subsys_init(void);
int mlan_subsys_deinit(void);

const uint8_t *wlanfw;

t_u32 last_resp_rcvd, last_cmd_sent;

OSA_MUTEX_HANDLE_DEFINE(txrx_mutex);

#ifndef RW610
osa_task_handle_t wifi_core_thread;
#endif
static struct
{
    /* Where the cmdresp/event should be dispached depends on its value */
    /* int special; */
    /* Default queue where the cmdresp/events will be sent */
    osa_msgq_handle_t event_queue;
    int (*wifi_low_level_input)(const uint8_t interface, const uint8_t *buffer, const uint16_t len);
} bus;

/* remove this after mlan integration complete */
enum
{
    MLAN_CARD_NOT_DETECTED = 3,
    MLAN_STATUS_FW_DNLD_FAILED,
    MLAN_STATUS_FW_NOT_DETECTED = 5,
    MLAN_STATUS_FW_NOT_READY,
    MLAN_STATUS_FW_XZ_FAILED,
    MLAN_CARD_CMD_TIMEOUT
};

/* @brief decription about the read/write buffer
 * The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
 * block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
 * can define the block length by yourself if the card supports partial access.
 * The address of the read/write buffer should align to the specific DMA data buffer address align value if
 * DMA transfer is used, otherwise the buffer address is not important.
 * At the same time buffer address/size should be aligned to the cache line size if cache is supported.
 */
/*! @brief Data written to the card */
SDK_ALIGN(uint8_t outbuf[IMU_OUTBUF_LEN], 32);

#ifdef RW610
SDK_ALIGN(uint8_t inbuf[2 * DATA_BUFFER_SIZE], 32);
#else
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t inbuf[SDIO_MP_AGGR_DEF_PKT_LIMIT * 2 * DATA_BUFFER_SIZE], 32);
#endif
#if CONFIG_AMSDU_IN_AMPDU
SDK_ALIGN(uint8_t amsdu_outbuf[MAX_SUPPORT_AMSDU_SIZE], 32);
#endif

hal_imumc_status_t imumc_cmdrsp_handler(IMU_Msg_t *pImuMsg, uint32_t length);
hal_imumc_status_t imumc_event_handler(IMU_Msg_t *pImuMsg, uint32_t length);
hal_imumc_status_t imumc_rxpkt_handler(IMU_Msg_t *pImuMsg, uint32_t length);
hal_imumc_status_t imumc_ctrl_handler(IMU_Msg_t *pImuMsg, uint32_t length);

/* Remove me: This structure is not present in mlan and can be removed later */
typedef MLAN_PACK_START struct
{
    t_u16 size;
    t_u16 pkttype;
    HostCmd_DS_COMMAND hostcmd;
} MLAN_PACK_END IMUPkt;

IMUPkt *imupkt = (IMUPkt *)outbuf;

#if CONFIG_PALLADIUM_SUPPORT
#define WIFI_POLL_CMD_RESP_TIME 1
#else
#define WIFI_POLL_CMD_RESP_TIME 10
#endif
#if CONFIG_TX_RX_ZERO_COPY
extern void net_tx_zerocopy_process_cb(void *destAddr, void *srcAddr, uint32_t len);
#endif
void wrapper_wlan_cmd_11n_cfg(void *hostcmd);
void wrapper_wifi_ret_mib(void *resp);
uint32_t dev_value1 = -1;
uint8_t dev_mac_addr[MLAN_MAC_ADDR_LENGTH];
uint8_t dev_mac_addr_uap[MLAN_MAC_ADDR_LENGTH];
static uint8_t dev_fw_ver_ext[MLAN_MAX_VER_STR_LEN];

static void wifi_init_imulink(void)
{
    /* Assign IMU channel for CPU1-CPU3 communication */
    HAL_ImuInit(kIMU_LinkCpu1Cpu3);
}

uint8_t cmd_seqno = 0;
static hal_imumc_status_t wifi_send_fw_cmd(t_u16 cmd_type, t_u8 *cmd_payload, t_u32 length)
{
    IMUPkt *imu_cmd         = (IMUPkt *)cmd_payload;
    HostCmd_DS_COMMAND *cmd = NULL;

    if (cmd_payload == NULL || length == 0)
    {
        return kStatus_HAL_ImumcError;
    }

    cmd          = &(imu_cmd->hostcmd);
    cmd->seq_num = (cmd->seq_num & 0xFF00) | cmd_seqno;
    cmd_seqno++;

#if (CONFIG_ENABLE_WARNING_LOGS) || (CONFIG_WIFI_CMD_RESP_DEBUG)
    wcmdr_d("DNLD_CMD: 0x%x, act 0x%x, len %d, seqno 0x%x", cmd->command, *(t_u16 *)((t_u8 *)cmd + S_DS_GEN), cmd->size,
            cmd->seq_num);
#endif /* CONFIG_ENABLE_WARNING_LOGS || CONFIG_WIFI_CMD_RESP_DEBUG*/

#if CONFIG_WIFI_IO_DUMP
    (void)PRINTF("OUT_CMD");
    dump_hex(cmd_payload, length);
#endif /* CONFIG_WIFI_IO_DUMP */

    while (kStatus_HAL_ImumcSuccess != HAL_ImuSendCommand(kIMU_LinkCpu1Cpu3, cmd_payload, length))
    {
        OSA_TimeDelay(1);
    }
    return kStatus_HAL_ImumcSuccess;
}

static hal_imumc_status_t wifi_send_fw_data(t_u8 *data, t_u32 length)
{
    if (data == NULL || length == 0U)
    {
        return kStatus_HAL_ImumcError;
    }
    w_pkt_d("Data TX SIG: Driver=>FW, len %d", length);
    return HAL_ImuSendTxData(kIMU_LinkCpu1Cpu3, data, length);
}

int wifi_imu_lock()
{
    return OSA_MutexLock((osa_mutex_handle_t)txrx_mutex, osaWaitForever_c);
}

void wifi_imu_unlock()
{
    (void)OSA_MutexUnlock((osa_mutex_handle_t)txrx_mutex);
}

uint32_t wifi_get_device_value1()
{
    return dev_value1;
}

int wifi_get_device_mac_addr(wifi_mac_addr_t *mac_addr)
{
    (void)memcpy(mac_addr->mac, dev_mac_addr, MLAN_MAC_ADDR_LENGTH);
    return WM_SUCCESS;
}

int wifi_get_device_uap_mac_addr(wifi_mac_addr_t *mac_addr_uap)
{
    (void)memcpy(mac_addr_uap->mac, dev_mac_addr_uap, MLAN_MAC_ADDR_LENGTH);
    return WM_SUCCESS;
}

int wifi_get_device_firmware_version_ext(wifi_fw_version_ext_t *fw_ver_ext)
{
    (void)memcpy(fw_ver_ext->version_str, dev_fw_ver_ext, MLAN_MAX_VER_STR_LEN);
    return WM_SUCCESS;
}

/* Initializes the driver struct */
static int wlan_init_struct()
{
    osa_status_t status;

    status = OSA_MutexCreate((osa_mutex_handle_t)txrx_mutex);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }

    return imu_create_task_lock();
}

static int wlan_deinit_struct()
{
    osa_status_t status;

    status = OSA_MutexDestroy((osa_mutex_handle_t)txrx_mutex);
    if (status != KOSA_StatusSuccess)
    {
        wifi_io_e("%s mutex deletion error %d", __FUNCTION__, status);
        return -WM_FAIL;
    }

    imu_delete_task_lock();

    (void)memset(dev_mac_addr, 0, sizeof(dev_mac_addr));
    (void)memset(dev_fw_ver_ext, 0, sizeof(dev_fw_ver_ext));

    return WM_SUCCESS;
}

int raw_process_pkt_hdrs(void *pbuf, t_u32 payloadlen, t_u8 interface)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    IMUPkt *imuhdr       = (IMUPkt *)pbuf;
    TxPD *ptxpd          = (TxPD *)((uint8_t *)pbuf + INTF_HEADER_LEN);

    ptxpd->bss_type      = interface;
    ptxpd->bss_num       = GET_BSS_NUM(pmpriv);
    ptxpd->tx_pkt_offset = 0x14; /* we'll just make this constant */
    ptxpd->tx_pkt_length = payloadlen - ptxpd->tx_pkt_offset - INTF_HEADER_LEN;
    ptxpd->tx_pkt_type   = 0xE5;
    ptxpd->tx_control    = 0;
    ptxpd->priority      = 0;
    ptxpd->flags         = 0;
    ptxpd->pkt_delay_2ms = 0;
    /* set tx_token_id to 1 to get tx_status_event from FW */
    ptxpd->tx_token_id = 1;

    imuhdr->size = payloadlen + ptxpd->tx_pkt_offset + INTF_HEADER_LEN;

    return ptxpd->tx_pkt_offset + INTF_HEADER_LEN;
}

/*
 * fixme: mlan_sta_tx.c can be used directly here. This functionality is
 * already present there.
 */
/* SDIO  TxPD  PAYLOAD | 4 | 22 | payload | */

/* we return the offset of the payload from the beginning of the buffer */
void process_pkt_hdrs(void *pbuf, t_u32 payloadlen, t_u8 interface, t_u8 tid, t_u32 tx_control)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    IMUPkt *imuhdr       = (IMUPkt *)pbuf;
    TxPD *ptxpd          = (TxPD *)((uint8_t *)pbuf + INTF_HEADER_LEN);

    ptxpd->bss_type      = interface;
    ptxpd->bss_num       = GET_BSS_NUM(pmpriv);
    ptxpd->tx_pkt_offset = 0x16; /* we'll just make this constant */
    ptxpd->tx_pkt_length = payloadlen - ptxpd->tx_pkt_offset - INTF_HEADER_LEN;
    if (ptxpd->tx_pkt_type == 0xe5)
    {
        ptxpd->tx_pkt_offset = 0x14; /* Override for special frame */
        ptxpd->tx_pkt_length = payloadlen - ptxpd->tx_pkt_offset - INTF_HEADER_LEN;
    }
    ptxpd->tx_control    = tx_control;
    ptxpd->priority      = tid;
    ptxpd->flags         = 0;
    ptxpd->pkt_delay_2ms = 0;

    imuhdr->size = payloadlen;
}

#if CONFIG_AMSDU_IN_AMPDU
#if defined(RW610)
int process_amsdu_pkt_hdrs(void *pbuf, t_u32 payloadlen, mlan_wmm_ac_e ac, t_u8 interface)
#else
int process_amsdu_pkt_hdrs(void *pbuf, t_u32 payloadlen, mlan_wmm_ac_e ac)
#endif
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    IMUPkt *imuhdr       = (IMUPkt *)pbuf;
    TxPD *ptxpd          = (TxPD *)((uint8_t *)pbuf + INTF_HEADER_LEN);

#if defined(RW610)
    ptxpd->bss_type = interface;
#else
    TxPD *ptxpd_orig = (TxPD *)((uint8_t *)wifi_get_wmm_send_outbuf(ac, 0) + INTF_HEADER_LEN);
    ptxpd->bss_type  = ptxpd_orig->bss_type;
#endif
    ptxpd->bss_num       = GET_BSS_NUM(pmpriv);
    ptxpd->tx_pkt_offset = 0x16; /* we'll just make this constant */
    ptxpd->tx_pkt_length = payloadlen - ptxpd->tx_pkt_offset - INTF_HEADER_LEN;
    ptxpd->tx_pkt_type   = PKT_TYPE_AMSDU;
    ptxpd->tx_control    = 0;
    ptxpd->priority      = 0;
    ptxpd->flags         = 0;
    ptxpd->pkt_delay_2ms = 0;

    imuhdr->size = payloadlen;

    return ptxpd->tx_pkt_offset + INTF_HEADER_LEN;
}
#endif

void process_pkt_hdrs_flags(void *pbuf, t_u8 flags)
{
    TxPD *ptxpd  = (TxPD *)((uint8_t *)pbuf + INTF_HEADER_LEN);
    ptxpd->flags = flags;
}

int bus_register_event_queue(osa_msgq_handle_t event_queue)
{
    if (bus.event_queue != NULL)
    {
        return -WM_FAIL;
    }

    bus.event_queue = event_queue;

    return WM_SUCCESS;
}

void bus_deregister_event_queue()
{
    if (bus.event_queue != NULL)
    {
        bus.event_queue = NULL;
    }
}

int bus_register_data_input_function(int (*wifi_low_level_input)(const uint8_t interface,
                                                                 const uint8_t *buffer,
                                                                 const uint16_t len))
{
    if (bus.wifi_low_level_input != NULL)
    {
        return -WM_FAIL;
    }

    bus.wifi_low_level_input = wifi_low_level_input;

    return WM_SUCCESS;
}

void bus_deregister_data_input_funtion(void)
{
    bus.wifi_low_level_input = NULL;
}

void wifi_get_mac_address_from_cmdresp(void *resp, t_u8 *mac_addr);
void wifi_get_firmware_ver_ext_from_cmdresp(void *resp, t_u8 *fw_ver_ext);
void wifi_get_value1_from_cmdresp(void *resp, uint32_t *dev_value1);

#if CONFIG_FW_VDLLV2
static mlan_status wlan_handle_vdllv2_event_packet(t_u8 *pmbuf)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    pmlan_event_vdll_indication pevent_vdll_ind;

    pevent_vdll_ind = (pmlan_event_vdll_indication)(pmbuf);
    switch (wlan_le16_to_cpu(pevent_vdll_ind->vdllInd.type))
    {
        case VDLL_IND_TYPE_REQ:
            wevt_d("VDLL_IND (VDLL REQ).");
            (void)sb3_fw_download(LOAD_WIFI_VDLL_FIRMWARE, 1, pevent_vdll_ind->vdllInd.offset);
            break;
        case VDLL_IND_TYPE_INTF_RESET:
            wevt_d("VDLLV2_IND (INTF RESET).");
            HAL_ImuResetWlanTxq(kIMU_LinkCpu1Cpu3);
            break;
        default:
            PRINTF("receive vdll event type=%d, Unhandled!\r\n", pevent_vdll_ind->vdllInd.type);
            break;
    }
    return status;
}
#endif

mlan_status wlan_handle_cmd_resp_packet(t_u8 *pmbuf)
{
    HostCmd_DS_GEN *cmdresp;
    t_u32 cmdtype;
    t_u32 cmdsize;
    int bss_type;

    cmdresp = (HostCmd_DS_GEN *)(pmbuf + INTF_HEADER_LEN); /* size + pkttype=4 */
    cmdtype = cmdresp->command & HostCmd_CMD_ID_MASK;
    cmdsize = cmdresp->size;
#if CONFIG_IMU_GDMA
    HAL_ImuGdmaCopyData(inbuf, cmdresp, cmdsize);
#else
    memcpy(inbuf, cmdresp, cmdsize);
#endif
    cmdresp  = (HostCmd_DS_GEN *)inbuf;
    bss_type = HostCmd_GET_BSS_TYPE(cmdresp->seq_num);

    last_resp_rcvd = cmdtype;

    if ((cmdresp->command & 0xf000) != 0x8000)
    {
        wifi_io_d("cmdresp->command = (0x%x)", cmdresp->command);
    }

    /* Do not process response of wlan firmware shutdown command
     *
     * This is required to flush out any previous response
     * from the wlan_deinit() which might have been called
     * prior to this.
     *
     */
    if ((cmdresp->command & 0x00ff) == HostCmd_CMD_FUNC_SHUTDOWN)
        return MLAN_STATUS_SUCCESS;

    if ((cmdresp->command & 0x0fff) != last_cmd_sent)
    {
        wifi_io_d("cmdresp->command = (0x%x) last_cmd_sent = (0x%x)", cmdresp->command, last_cmd_sent);
    }

    if (cmdresp->result != 0U)
    {
        wifi_io_d("cmdresp->result = (0x%x)", cmdresp->result);
    }

    wifi_io_d("Resp : (0x%x)", cmdtype);
    switch (cmdtype)
    {
        case HostCmd_CMD_MAC_CONTROL:
        case HostCmd_CMD_FUNC_INIT:
        case HostCmd_CMD_CFG_DATA:
            break;
        case HostCmd_CMD_MAC_REG_ACCESS:
            wifi_get_value1_from_cmdresp(cmdresp, &dev_value1);
            break;
        case HostCmd_CMD_802_11_MAC_ADDRESS:
            if (bss_type == MLAN_BSS_TYPE_UAP)
            {
                wifi_get_mac_address_from_cmdresp(cmdresp, dev_mac_addr_uap);
            }
            else
            {
                wifi_get_mac_address_from_cmdresp(cmdresp, dev_mac_addr);
            }
            break;
#ifdef OTP_CHANINFO
        case HostCmd_CMD_CHAN_REGION_CFG:
            wlan_ret_chan_region_cfg((mlan_private *)mlan_adap->priv[0], (HostCmd_DS_COMMAND *)cmdresp, NULL);
            break;
#endif
        case HostCmd_CMD_GET_HW_SPEC:
            wlan_ret_get_hw_spec((mlan_private *)mlan_adap->priv[0], (HostCmd_DS_COMMAND *)cmdresp, NULL);
#ifdef RW610
#if !defined(OVERRIDE_CALIBRATION_DATA)
            t_u32 fw_cap_ext_rw610;
            fw_cap_ext_rw610 = mlan_adap->priv[0]->adapter->fw_cap_ext;
            cal_data_valid_rw610 = (((fw_cap_ext_rw610 & 0x0800) == 0) ? 0 : 1);
#else
            cal_data_valid_rw610 = 0;
#endif
#endif
            break;
        case HostCmd_CMD_VERSION_EXT:
            wifi_get_firmware_ver_ext_from_cmdresp(cmdresp, dev_fw_ver_ext);
            break;
        case HostCmd_CMD_11N_CFG:
            break;
#if CONFIG_AMSDU_IN_AMPDU
        case HostCmd_CMD_AMSDU_AGGR_CTRL:
            wlan_ret_amsdu_aggr_ctrl((mlan_private *)mlan_adap->priv[0], (HostCmd_DS_COMMAND *)cmdresp, NULL);
            break;
#endif
        case HostCmd_CMD_FUNC_SHUTDOWN:
            break;
#ifdef WLAN_LOW_POWER_ENABLE
        case HostCmd_CMD_LOW_POWER_MODE:
            break;
#endif
        case HostCmd_CMD_ED_MAC_MODE:
        case HostCmd_CMD_CHANNEL_TRPC_CONFIG:
            break;
#if CONFIG_WIFI_TX_BUFF
        case HostCmd_CMD_RECONFIGURE_TX_BUFF:
            mlan_adap->tx_buffer_size = ((HostCmd_DS_COMMAND *)cmdresp)->params.tx_buf.buff_size;
            break;
#endif
        default:
            wifi_io_d("Unimplemented Resp : (0x%x)", cmdtype);
            break;
    }

    return MLAN_STATUS_SUCCESS;
}

/*
 * Accepts event and command packets. Redirects them to queues if
 * registered. If queues are not registered (as is the case during
 * initialization then the packet is given to lower layer cmd/event
 * handling part.
 */
static mlan_status wlan_decode_rx_packet(t_u8 *pmbuf, t_u32 upld_type)
{
    IMUPkt *imupkt    = (IMUPkt *)pmbuf;
    t_u32 event_cause = 0;
    int status;
    struct bus_message msg;
#if CONFIG_FW_VDLLV2
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
#endif

    if (upld_type == MLAN_TYPE_DATA)
    {
        return MLAN_STATUS_FAILURE;
    }
    if (upld_type == MLAN_TYPE_CMD)
    {
        wifi_io_d("  --- Rx: Cmd Response ---");
        wcmdr_d("CMD_RESP: 0x%x, result %d, len %d, seqno 0x%x", imupkt->hostcmd.command, imupkt->hostcmd.result,
                imupkt->hostcmd.size, imupkt->hostcmd.seq_num);

        if (mlan_adap->cmd_sent != 0U)
        {
            mlan_adap->cmd_sent = MFALSE;
        }
        PRINTM(MINFO, "cmd_sent=%d\n", mlan_adap->cmd_sent);
    }
    else
    {
        event_cause = *((t_u32 *)(pmbuf + INTF_HEADER_LEN));
        wifi_io_d(" --- Rx: EVENT Response ---");
        if (event_cause != EVENT_PS_SLEEP && event_cause != EVENT_PS_AWAKE)
        {
            wevt_d("Event: 0x%x", event_cause);
        }
    }

#if CONFIG_WIFI_IO_DUMP
    (void)PRINTF("Resp");
    dump_hex(pmbuf, imupkt->size);
#endif

#if CONFIG_FW_VDLLV2
    if (upld_type == MLAN_TYPE_EVENT && (wlan_le16_to_cpu(imupkt->hostcmd.command) == EVENT_VDLL_IND))
    {
        if (mlan_adap->ps_state == PS_STATE_SLEEP)
        {
            OSA_RWLockWriteUnlock(&sleep_rwlock);
            pmpriv->adapter->ps_state = PS_STATE_AWAKE;
        }
        return wlan_handle_vdllv2_event_packet(pmbuf + INTF_HEADER_LEN);
    }
#endif
    if (bus.event_queue != NULL)
    {
        if (upld_type == MLAN_TYPE_CMD)
        {
            msg.data = wifi_mem_malloc_cmdrespbuf();
        }
        else
        {
            msg.data = wifi_malloc_eventbuf(imupkt->size);
        }

        if (!msg.data)
        {
            wifi_io_e("[fail] Buffer alloc: T: %d S: %d", upld_type, imupkt->size);
            return MLAN_STATUS_FAILURE;
        }

        msg.event = upld_type;
#if CONFIG_IMU_GDMA
        HAL_ImuGdmaCopyData(msg.data, pmbuf, imupkt->size);
#else
        memcpy(msg.data, pmbuf, imupkt->size);
#endif

        status = OSA_MsgQPut(bus.event_queue, &msg);

        if (status != KOSA_StatusSuccess)
        {
            wifi_io_e("Failed to send response on Queue: upld_type=%d id=0x%x", upld_type,
                      (upld_type == MLAN_TYPE_CMD) ? imupkt->hostcmd.command : event_cause);
            if (upld_type != MLAN_TYPE_CMD)
            {
                wifi_free_eventbuf(msg.data);
            }
            return MLAN_STATUS_FAILURE;
        }
    }
    else
    {
        /* No queues registered yet. Use local handling */
        wlan_handle_cmd_resp_packet(pmbuf);
    }

    return MLAN_STATUS_SUCCESS;
}

static inline t_u32 wlan_get_next_seq_num()
{
    return 0;
}

void wifi_prepare_set_cal_data_cmd(void *cmd, int seq_number);
static int _wlan_set_cal_data()
{
    (void)memset(outbuf, 0, IMU_OUTBUF_LEN);

    /* imupkt = outbuf */
    wifi_prepare_set_cal_data_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_CFG_DATA;

    /* send CMD53 to write the command to get mac address */
    (void)wifi_send_fw_cmd(HostCmd_CMD_CFG_DATA, (uint8_t *)outbuf, imupkt->size);
    return true;
}

void wifi_prepare_get_mac_addr_cmd(void *cmd, int seq_number);
void wifi_prepare_get_channel_region_cfg_cmd(HostCmd_DS_COMMAND *cmd, int seq_number);
void wifi_prepare_get_hw_spec_cmd(HostCmd_DS_COMMAND *cmd, int seq_number);

#ifdef OTP_CHANINFO
static int wlan_get_channel_region_cfg()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);
    /* imupkt = outbuf */
    wifi_prepare_get_channel_region_cfg_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_CHAN_REGION_CFG;
    (void)wifi_send_fw_cmd(HostCmd_CMD_CHAN_REGION_CFG, (uint8_t *)outbuf, imupkt->size);

    return true;
}
#endif

static int wlan_get_hw_spec()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);
    /* imupkt = outbuf */
    wifi_prepare_get_hw_spec_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_GET_HW_SPEC;

    (void)wifi_send_fw_cmd(HostCmd_CMD_GET_HW_SPEC, (uint8_t *)outbuf, imupkt->size);
    return true;
}

static int wlan_get_mac_addr_sta()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wifi_prepare_get_mac_addr_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_802_11_MAC_ADDRESS;

    /* send CMD53 to write the command to get mac address */
    (void)wifi_send_fw_cmd(HostCmd_CMD_802_11_MAC_ADDRESS, (uint8_t *)outbuf, imupkt->size);
    return true;
}

#if UAP_SUPPORT
static int wlan_get_mac_addr_uap()
{
    int seq_number = 0;

    seq_number = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, MLAN_BSS_TYPE_UAP);
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wifi_prepare_get_mac_addr_cmd(&imupkt->hostcmd, seq_number);

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_802_11_MAC_ADDRESS;

    /* send CMD53 to write the command to get mac address */
    (void)wifi_send_fw_cmd(HostCmd_CMD_802_11_MAC_ADDRESS, (uint8_t *)outbuf, imupkt->size);
    return true;
}
#endif

void wifi_prepare_get_fw_ver_ext_cmd(void *cmd, int seq_number, int version_str_sel);
static int wlan_get_fw_ver_ext(int version_str_sel)
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wifi_prepare_get_fw_ver_ext_cmd(&imupkt->hostcmd, wlan_get_next_seq_num(), version_str_sel);

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_VERSION_EXT;

    /* send CMD53 to write the command to get mac address */
    (void)wifi_send_fw_cmd(HostCmd_CMD_VERSION_EXT, (uint8_t *)outbuf, imupkt->size);
    return true;
}

void wifi_prepare_get_value1(void *cmd, int seq_number);

static int wlan_get_value1()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wifi_prepare_get_value1(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_MAC_REG_ACCESS;
    (void)wifi_send_fw_cmd(HostCmd_CMD_MAC_REG_ACCESS, (uint8_t *)outbuf, imupkt->size);
    return true;
}

void wifi_prepare_set_mac_addr_cmd(void *cmd, int seq_number);
static int _wlan_set_mac_addr()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wifi_prepare_set_mac_addr_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_802_11_MAC_ADDRESS;

    /* send CMD53 to write the command to get mac address */
    (void)wifi_send_fw_cmd(HostCmd_CMD_802_11_MAC_ADDRESS, (uint8_t *)outbuf, imupkt->size);
    return true;
}

static int wlan_set_11n_cfg()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);
    wrapper_wlan_cmd_11n_cfg(&imupkt->hostcmd);
    /* imupkt = outbuf */
    imupkt->hostcmd.seq_num = wlan_get_next_seq_num();
    imupkt->pkttype         = MLAN_TYPE_CMD;
    imupkt->size            = imupkt->hostcmd.size + INTF_HEADER_LEN;
    last_cmd_sent           = HostCmd_CMD_11N_CFG;
    (void)wifi_send_fw_cmd(HostCmd_CMD_11N_CFG, (uint8_t *)outbuf, imupkt->size);

    return true;
}

#if CONFIG_WIFI_TX_BUFF
int _wlan_return_all_tx_buf(imu_link_t link)
{
    HAL_ImuReturnAllTxBuf(link);

    return true;
}

void wifi_prepare_set_tx_buf_size(void *cmd, int seq_number);
static int _wlan_recfg_tx_buf_size(uint16_t buf_size)
{
    (void)wifi_calibrate_tx_buf_size(buf_size);

    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wifi_prepare_set_tx_buf_size(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_RECONFIGURE_TX_BUFF;

    (void)wifi_send_fw_cmd(HostCmd_CMD_RECONFIGURE_TX_BUFF, (uint8_t *)outbuf, imupkt->size);

    return true;
}
#endif

#if CONFIG_AMSDU_IN_AMPDU
void wifi_prepare_enable_amsdu_cmd(void *cmd, int seq_number);
static int wlan_enable_amsdu()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wifi_prepare_enable_amsdu_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_AMSDU_AGGR_CTRL;

    (void)wifi_send_fw_cmd(HostCmd_CMD_AMSDU_AGGR_CTRL, (uint8_t *)outbuf, imupkt->size);

    return true;
}
#endif
/* This function was only used in imu_wifi_deinit, and now is replaced by wifi_send_shutdown_cmd with the same 0xaa cmd
 */
#if 0
static int wlan_cmd_shutdown()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    imupkt->hostcmd.command = HostCmd_CMD_FUNC_SHUTDOWN;
    imupkt->hostcmd.size    = S_DS_GEN;
    imupkt->hostcmd.seq_num = wlan_get_next_seq_num();
    imupkt->hostcmd.result  = 0;

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_FUNC_SHUTDOWN;

    wifi_send_fw_cmd(HostCmd_CMD_FUNC_SHUTDOWN, (uint8_t *)outbuf, imupkt->size);

    return true;
}
#endif

void wlan_prepare_mac_control_cmd(void *cmd, int seq_number);
static int wlan_set_mac_ctrl()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    wlan_prepare_mac_control_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_MAC_CONTROL;

    (void)wifi_send_fw_cmd(HostCmd_CMD_MAC_CONTROL, (uint8_t *)outbuf, imupkt->size);

    return true;
}

static int wlan_cmd_init()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */
    imupkt->hostcmd.command = HostCmd_CMD_FUNC_INIT;
    imupkt->hostcmd.size    = S_DS_GEN;
    imupkt->hostcmd.seq_num = wlan_get_next_seq_num();
    imupkt->hostcmd.result  = 0;

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_FUNC_INIT;

    (void)wifi_send_fw_cmd(HostCmd_CMD_FUNC_INIT, (uint8_t *)outbuf, imupkt->size);

    return true;
}

#ifdef WLAN_LOW_POWER_ENABLE
void wifi_prepare_low_power_mode_cmd(HostCmd_DS_COMMAND *cmd, int seq_number);
static int wlan_set_low_power_mode()
{
    (void)memset(outbuf, 0, IMU_INIT_FW_CMD_SIZE);

    /* imupkt = outbuf */

    wifi_prepare_low_power_mode_cmd(&imupkt->hostcmd, wlan_get_next_seq_num());

    imupkt->pkttype = MLAN_TYPE_CMD;
    imupkt->size    = imupkt->hostcmd.size + INTF_HEADER_LEN;

    last_cmd_sent = HostCmd_CMD_LOW_POWER_MODE;

    (void)wifi_send_fw_cmd(HostCmd_CMD_LOW_POWER_MODE, (uint8_t *)outbuf, imupkt->size);
    return true;
}
#endif

static int wlan_wait_for_last_resp_rcvd(t_u16 command)
{
    int retry_cnt = WIFI_COMMAND_RESPONSE_WAIT_MS / WIFI_POLL_CMD_RESP_TIME;

    while ((last_resp_rcvd != command) && (retry_cnt > 0))
    {
        OSA_TimeDelay(WIFI_POLL_CMD_RESP_TIME);
        retry_cnt--;
    }

    if (last_resp_rcvd == command)
    {
        return true;
    }
    else
    {
        wifi_io_e("%s: wait cmd 0x%x fail (last 0x%x)", __FUNCTION__, command, last_resp_rcvd);
        return false;
    }
}

// mlan_status wlan_process_int_status(mlan_adapter *pmadapter);
/* Setup the firmware with commands */
static int wlan_fw_init_cfg()
{
    wcmdr_d("FWCMD : INIT (0xa9)");

    /* Add while loop here to wait until command buffer has been attached */
    while (HAL_ImuLinkIsUp(kIMU_LinkCpu1Cpu3) != 0)
    {
        OSA_TimeDelay(WIFI_POLL_CMD_RESP_TIME);
    }

    wlan_cmd_init();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_FUNC_INIT) != true)
    {
        return false;
    }

#ifdef WLAN_LOW_POWER_ENABLE
    if (low_power_mode)
    {
        wcmdr_d("CMD : LOW_POWER_MODE (0x128)");

        (void)wlan_set_low_power_mode();

        if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_LOW_POWER_MODE) != true)
        {
            return false;
        }
    }
#endif

    if (mac_addr_valid)
    {
        wcmdr_d("CMD : SET_MAC_ADDR (0x4d)");

        _wlan_set_mac_addr();

        if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_802_11_MAC_ADDRESS) != true)
        {
            return false;
        }
    }

#ifdef OTP_CHANINFO
    wcmdr_d("CMD : Channel Region CFG (0x0242)");

    wlan_get_channel_region_cfg();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_CHAN_REGION_CFG) != true)
    {
        return false;
    }
#endif

    wcmdr_d("CMD : GET_HW_SPEC (0x03)");

    wlan_get_hw_spec();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_GET_HW_SPEC) != true)
    {
        return false;
    }
    if (cal_data_valid
#ifdef RW610
        && !cal_data_valid_rw610
#endif
    )
    {
        wcmdr_d("CMD : SET_CAL_DATA (0x8f)");

        _wlan_set_cal_data();

        if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_CFG_DATA) != true)
        {
            return false;
        }
    }

#if CONFIG_WIFI_TX_BUFF
    // TODO:Reconfig tx buffer size to 4K
    wcmdr_d("CMD : RECONFIGURE_TX_BUFF (0xd9)");

    _wlan_recfg_tx_buf_size(tx_buf_size);

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_RECONFIGURE_TX_BUFF) != true)
    {
        return false;
    }
#endif

    wcmdr_d("CMD : MAC_REG_ACCESS (0x19)");

    wlan_get_value1();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_MAC_REG_ACCESS) != true)
    {
        return false;
    }

    wcmdr_d("CMD : GET_FW_VER_EXT (0x97)");

    wlan_get_fw_ver_ext(0);

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_VERSION_EXT) != true)
    {
        return false;
    }

    wcmdr_d("CMD : GET_MAC_ADDR (0x4d)");

    wlan_get_mac_addr_sta();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_802_11_MAC_ADDRESS) != true)
    {
        return false;
    }

#if UAP_SUPPORT
    last_resp_rcvd = 0;

    wlan_get_mac_addr_uap();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_802_11_MAC_ADDRESS) != true)
    {
        return false;
    }
#endif

    wcmdr_d("CMD : GET_FW_VER_EXT (0x97)");

    wlan_get_fw_ver_ext(3);

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_VERSION_EXT) != true)
    {
        return false;
    }

    wcmdr_d("CMD : MAC_CTRL (0x28)");

    wlan_set_mac_ctrl();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_MAC_CONTROL) != true)
    {
        return false;
    }

    wcmdr_d("CMD : GET_FW_VER_EXT (0x97)");

    wlan_get_fw_ver_ext(4);

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_VERSION_EXT) != true)
    {
        return false;
    }

    wcmdr_d("CMD : 11N_CFG (0xcd)");
    wlan_set_11n_cfg();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_11N_CFG) != true)
    {
        return false;
    }

#if CONFIG_AMSDU_IN_AMPDU
    wcmdr_d("CMD : AMSDU_AGGR_CTRL (0xdf)");
    wlan_enable_amsdu();

    if (wlan_wait_for_last_resp_rcvd(HostCmd_CMD_AMSDU_AGGR_CTRL) != true)
    {
        return false;
    }
#endif

    return true;
}

int wlan_send_imu_cmd(t_u8 *buf)
{
    IMUPkt *imu_cmd = (IMUPkt *)outbuf;

    wifi_imu_lock();

    (void)memcpy(outbuf, buf, MIN(WIFI_FW_CMDBUF_SIZE, IMU_OUTBUF_LEN));
    imu_cmd->pkttype = MLAN_TYPE_CMD;
    imu_cmd->size    = imu_cmd->hostcmd.size + INTF_HEADER_LEN;
    (void)wifi_send_fw_cmd(imu_cmd->hostcmd.command, (uint8_t *)outbuf, imu_cmd->size);

    last_cmd_sent = imu_cmd->hostcmd.command;
    wifi_imu_unlock();

    return WM_SUCCESS;
}

int wifi_send_cmdbuffer(void)
{
    return wlan_send_imu_cmd(cmd_buf);
}

uint8_t *wifi_get_imu_outbuf(uint32_t *outbuf_len)
{
    *outbuf_len = sizeof(outbuf);
    return outbuf;
}
#if CONFIG_AMSDU_IN_AMPDU
uint8_t *wifi_get_amsdu_outbuf(uint32_t offset)
{
    return (amsdu_outbuf + offset);
}
#endif
t_u16 get_mp_end_port(void);
mlan_status wlan_xmit_pkt(t_u8 *buffer, t_u32 txlen, t_u8 interface, t_u32 tx_control)
{
    int ret;

    wifi_io_info_d("OUT: i/f: %d len: %d", interface, txlen);

    process_pkt_hdrs((t_u8 *)buffer, txlen, interface, 0, tx_control);
    /* send tx data via imu */
    ret = wifi_send_fw_data(buffer, txlen);

    if (ret != kStatus_HAL_ImumcSuccess)
    {
        wifi_io_e("Send tx data via imu failed (%d)", ret);
#if CONFIG_WIFI_FW_DEBUG
#if 0
        if (wm_wifi.wifi_usb_mount_cb != NULL)
        {
            ret = wm_wifi.wifi_usb_mount_cb();
            if (ret == WM_SUCCESS)
                wifi_dump_firmware_info(NULL);
            else
                wifi_e("USB mounting failed");
        }
        else
            wifi_e("USB mount callback is not registered");
#endif
        wifi_dump_firmware_info();
#endif
        return MLAN_STATUS_FAILURE;
    }
    return MLAN_STATUS_SUCCESS;
}

#if CONFIG_WMM
mlan_status wlan_xmit_bypass_pkt(t_u8 *buffer, t_u32 txlen, t_u8 interface)
{
    int ret;

    wifi_io_info_d("OUT: i/f: %d len: %d", interface, txlen);

    wifi_imu_lock();
    /* send tx data via imu */
    ret = wifi_send_fw_data(buffer, txlen);

    if (ret != kStatus_HAL_ImumcSuccess)
    {
        wifi_io_e("Send tx data via imu failed (%d)", ret);
#if CONFIG_WIFI_FW_DEBUG
#if 0
        if (wm_wifi.wifi_usb_mount_cb != NULL)
        {
            ret = wm_wifi.wifi_usb_mount_cb();
            if (ret == WM_SUCCESS)
                wifi_dump_firmware_info(NULL);
            else
                wifi_e("USB mounting failed");
        }
        else
            wifi_e("USB mount callback is not registered");
#endif
        wifi_dump_firmware_info();
#endif

        wifi_imu_unlock();
        return MLAN_STATUS_FAILURE;
    }

    wifi_imu_unlock();
    return MLAN_STATUS_SUCCESS;
}
#endif

#if CONFIG_WMM
mlan_status wlan_xmit_wmm_pkt(t_u8 interface, t_u32 txlen, t_u8 *tx_buf)
{
    int ret;
#if CONFIG_WMM_UAPSD
    bool last_packet = 0;
#endif

    wifi_io_info_d("OUT: i/f: %d len: %d", interface, txlen);

    wifi_imu_lock();
#if CONFIG_WMM_UAPSD
    if (mlan_adap->priv[interface]->adapter->pps_uapsd_mode &&
        wifi_check_last_packet_indication(mlan_adap->priv[interface]))
    {
#if CONFIG_TX_RX_ZERO_COPY
        process_pkt_hdrs_flags(&((outbuf_t *)tx_buf)->intf_header[0], MRVDRV_TxPD_POWER_MGMT_LAST_PACKET);
#else
        process_pkt_hdrs_flags((t_u8 *)tx_buf, MRVDRV_TxPD_POWER_MGMT_LAST_PACKET);
#endif
        last_packet = 1;
    }
#endif

#if CONFIG_TX_RX_ZERO_COPY
    ret = HAL_ImuAddWlanTxPacketExt(kIMU_LinkCpu1Cpu3, tx_buf, txlen, net_tx_zerocopy_process_cb);
#else
    ret              = HAL_ImuAddWlanTxPacket(kIMU_LinkCpu1Cpu3, tx_buf, txlen);
#endif

    if (ret != kStatus_HAL_ImumcSuccess)
    {
#if CONFIG_WMM_UAPSD
        if (last_packet)
        {
#if CONFIG_TX_RX_ZERO_COPY
            process_pkt_hdrs_flags(&((outbuf_t *)tx_buf)->intf_header[0], 0);
#else
            process_pkt_hdrs_flags((t_u8 *)tx_buf, 0);
#endif
        }
#endif

        wifi_imu_unlock();
        return MLAN_STATUS_FAILURE;
    }

#if CONFIG_WMM_UAPSD
    if (last_packet)
    {
        mlan_adap->priv[interface]->adapter->tx_lock_flag = MTRUE;
        OSA_SemaphoreWait((osa_semaphore_handle_t)uapsd_sem, osaWaitForever_c);
    }
#endif

    wifi_imu_unlock();
    return MLAN_STATUS_SUCCESS;
}

mlan_status wlan_flush_wmm_pkt(int pkt_cnt)
{
    int ret;

    if (pkt_cnt == 0)
    {
        return MLAN_STATUS_SUCCESS;
    }

    w_pkt_d("Data TX: Driver=>FW, pkt_cnt %d", pkt_cnt);

    ret = HAL_ImuSendMultiTxData(kIMU_LinkCpu1Cpu3);
    if (ret != kStatus_HAL_ImumcSuccess)
    {
        wifi_io_e("wlan_flush_wmm_pkt failed (%d)", ret);
#if CONFIG_WIFI_FW_DEBUG
#if 0
        if (wm_wifi.wifi_usb_mount_cb != NULL)
        {
            ret = wm_wifi.wifi_usb_mount_cb();
            if (ret == WM_SUCCESS)
                wifi_dump_firmware_info(NULL);
            else
                wifi_e("USB mounting failed");
        }
        else
            wifi_e("USB mount callback is not registered");
#endif
        wifi_dump_firmware_info();
#endif
        return MLAN_STATUS_FAILURE;
    }
    return MLAN_STATUS_SUCCESS;
}

#if CONFIG_AMSDU_IN_AMPDU
/**
 *  @brief This function checks if we need to send last amsdu indication.
 *
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return        MTRUE or MFALSE
 */
static t_u8 wifi_check_last_amsdu_packet_indication(mlan_private *priv, t_u8 amsdu_cnt)
{
    if ((wifi_wmm_get_packet_cnt() == amsdu_cnt) && priv->wmm_qosinfo && priv->curr_bss_params.wmm_uapsd_enabled)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

mlan_status wlan_xmit_wmm_amsdu_pkt(mlan_wmm_ac_e ac, t_u8 interface, t_u32 txlen, t_u8 *tx_buf, t_u8 amsdu_cnt)
{
    int ret;
#if CONFIG_WMM_UAPSD
    bool last_packet = false;
#endif

    wifi_io_info_d("OUT: i/f: %d len: %d", interface, txlen);

    wifi_imu_lock();
#if defined(RW610)
    (void)process_amsdu_pkt_hdrs((t_u8 *)tx_buf, txlen, ac, interface);
#if CONFIG_WMM_UAPSD
    if (mlan_adap->priv[interface]->adapter->pps_uapsd_mode &&
        wifi_check_last_amsdu_packet_indication(mlan_adap->priv[interface], amsdu_cnt))
    {
        process_pkt_hdrs_flags((t_u8 *)tx_buf, MRVDRV_TxPD_POWER_MGMT_LAST_PACKET);
        last_packet = true;
    }
#endif
#else
    (void)process_amsdu_pkt_hdrs((t_u8 *)tx_buf, txlen, ac);
#endif

    ret = HAL_ImuAddWlanTxPacket(kIMU_LinkCpu1Cpu3, tx_buf, txlen);

    if (ret != kStatus_HAL_ImumcSuccess)
    {
#if CONFIG_WMM_UAPSD
        if (last_packet)
        {
            process_pkt_hdrs_flags((t_u8 *)tx_buf, 0);
        }
#endif
        wifi_imu_unlock();
        return MLAN_STATUS_FAILURE;
    }

#if CONFIG_WMM_UAPSD
    if (last_packet)
    {
        mlan_adap->priv[interface]->adapter->tx_lock_flag = MTRUE;
        OSA_SemaphoreWait((osa_semaphore_handle_t)uapsd_sem, osaWaitForever_c);
    }
#endif

    wifi_imu_unlock();
    return MLAN_STATUS_SUCCESS;
}
#endif
#endif

mlan_status wlan_send_null_packet(pmlan_private priv, t_u8 flags)
{
    int ret;
    t_u8 pbuf[32] = {0};
    TxPD *ptxpd   = (TxPD *)((uint8_t *)pbuf + INTF_HEADER_LEN);

    ptxpd->bss_type      = priv->bss_type;
    ptxpd->bss_num       = GET_BSS_NUM(priv);
    ptxpd->tx_pkt_offset = 0x16; /* we'll just make this constant */
    ptxpd->tx_pkt_length = 0;
    ptxpd->tx_control    = 0;
    ptxpd->priority      = 0;
    ptxpd->flags         = flags;
    ptxpd->pkt_delay_2ms = 0;

    ret = wifi_send_fw_data(pbuf, sizeof(TxPD) + INTF_HEADER_LEN);
    if (ret != kStatus_HAL_ImumcSuccess)
    {
        wifi_io_e("imu_drv_write failed (%d)", ret);
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

hal_imumc_status_t imumc_cmdrsp_handler(IMU_Msg_t *pImuMsg, uint32_t length)
{
    assert(NULL != pImuMsg);
    assert(0 != length);
    assert(IMU_MSG_COMMAND_RESPONSE == pImuMsg->Hdr.type);

#if CONFIG_HOST_SLEEP
    if (POWER_GetWakeupStatus(WL_MCI_WAKEUP0_IRQn))
    {
        wakeup_by                      = WAKEUP_BY_WLAN;
        mlan_adap->wlan_wakeup.type    = IMU_MSG_COMMAND_RESPONSE;
        mlan_adap->wlan_wakeup.subtype = 0;
        mlan_adap->wlan_wakeup.id      = *(uint16_t *)((uint8_t *)pImuMsg->PayloadPtr[0] + 4);
        POWER_ClearWakeupStatus(WL_MCI_WAKEUP0_IRQn);
    }
#endif

    (void)wlan_decode_rx_packet((t_u8 *)pImuMsg->PayloadPtr[0], MLAN_TYPE_CMD);

    return kStatus_HAL_ImumcSuccess;
}

hal_imumc_status_t imumc_event_handler(IMU_Msg_t *pImuMsg, uint32_t length)
{
    assert(NULL != pImuMsg);
    assert(0 != length);
    assert(IMU_MSG_EVENT == pImuMsg->Hdr.type);

#if CONFIG_HOST_SLEEP
    if (POWER_GetWakeupStatus(WL_MCI_WAKEUP0_IRQn))
    {
        wakeup_by                      = WAKEUP_BY_WLAN;
        mlan_adap->wlan_wakeup.type    = IMU_MSG_EVENT;
        mlan_adap->wlan_wakeup.subtype = 0;
        mlan_adap->wlan_wakeup.id      = *(uint16_t *)((uint8_t *)pImuMsg->PayloadPtr[0] + 4);
        POWER_ClearWakeupStatus(WL_MCI_WAKEUP0_IRQn);
    }
#endif

#if CONFIG_CSI
    if (EVENT_CSI == *((t_u8 *)pImuMsg->PayloadPtr[0] + 4))
    {
        csi_save_data_to_local_buff((t_u8 *)pImuMsg->PayloadPtr[0] + 8);
    }
#endif

    (void)wlan_decode_rx_packet((t_u8 *)pImuMsg->PayloadPtr[0], MLAN_TYPE_EVENT);

    return kStatus_HAL_ImumcSuccess;
}

hal_imumc_status_t imumc_rxpkt_handler(IMU_Msg_t *pImuMsg, uint32_t length)
{
    IMUPkt *inimupkt;
    t_u32 size;
    t_u8 interface;
    t_u8 i = 0;

    assert(NULL != pImuMsg);
    assert(0 != length);
    assert((IMU_MSG_RX_DATA == pImuMsg->Hdr.type) || (IMU_MSG_MULTI_RX_DATA == pImuMsg->Hdr.type));

#if CONFIG_HOST_SLEEP
    wakelock_get();
    if (POWER_GetWakeupStatus(WL_MCI_WAKEUP0_IRQn))
    {
        wakeup_by                      = WAKEUP_BY_WLAN;
        mlan_adap->wlan_wakeup.type    = pImuMsg->Hdr.type;
        mlan_adap->wlan_wakeup.subtype = 0;
        mlan_adap->wlan_wakeup.id      = 0;
        POWER_ClearWakeupStatus(WL_MCI_WAKEUP0_IRQn);
    }
#endif

    for (i = 0; i < pImuMsg->Hdr.length; i++)
    {
        inimupkt = (IMUPkt *)pImuMsg->PayloadPtr[i];
        size     = inimupkt->size;
        if ((size <= INTF_HEADER_LEN) || (size > sizeof(inbuf)))
        {
#if CONFIG_HOST_SLEEP
            wakelock_put();
#endif
            wifi_io_e("pImuMsg->PayloadPtr[%u] has invalid size=%u", i, size);
            return kStatus_HAL_ImumcError;
        }

#if !CONFIG_TX_RX_ZERO_COPY
#if CONFIG_IMU_GDMA
        HAL_ImuGdmaCopyData(inbuf, inimupkt, size);
#else
        (void)memcpy(inbuf, inimupkt, size);
#endif
#endif
        interface = *((t_u8 *)inimupkt + INTF_HEADER_LEN);
        wifi_io_info_d("IN: i/f: %d len: %d", interface, size);
        w_pkt_d("Data RX: FW=>Driver, if %d, len %d", interface, size);

        if (bus.wifi_low_level_input != NULL)
#if CONFIG_TX_RX_ZERO_COPY
            bus.wifi_low_level_input(interface, (uint8_t *)inimupkt, size);
#else
            bus.wifi_low_level_input(interface, inbuf, size);
#endif
    }
#if CONFIG_HOST_SLEEP
    wakelock_put();
#endif
    /*! To be the last action of the handler*/
    return kStatus_HAL_ImumcSuccess;
}

static bool imu_fw_is_hang(void)
{
    uint32_t *peer_magic_addr = (uint32_t *)0x41380000;

    if ((*peer_magic_addr) == 0xDEADDEAD)
    {
        return true;
    }
    else
    {
        return false;
    }
}

hal_imumc_status_t imumc_ctrl_handler(IMU_Msg_t *pImuMsg, uint32_t length)
{
    t_u32 imuControlType;

    assert(NULL != pImuMsg);
    assert(IMU_MSG_CONTROL == pImuMsg->Hdr.type);

#if CONFIG_HOST_SLEEP
    if (POWER_GetWakeupStatus(WL_MCI_WAKEUP0_IRQn))
    {
        wakeup_by                      = WAKEUP_BY_WLAN;
        mlan_adap->wlan_wakeup.type    = IMU_MSG_CONTROL;
        mlan_adap->wlan_wakeup.subtype = pImuMsg->Hdr.sub_type;
        mlan_adap->wlan_wakeup.id      = 0;
        POWER_ClearWakeupStatus(WL_MCI_WAKEUP0_IRQn);
    }
#endif

    imuControlType = pImuMsg->Hdr.sub_type;
    switch (imuControlType)
    {
        case IMU_MSG_CONTROL_TX_BUF_ADDR:
#if CONFIG_WMM

            if (mlan_adap->wait_txbuf == true)
            {
                OSA_SemaphorePost((osa_semaphore_handle_t)txbuf_sem);
            }

            (void)send_wifi_driver_tx_data_event(0);
#endif
            break;
        default:
            break;
    }
    return kStatus_HAL_ImumcSuccess;
}

void imu_wakeup_card()
{
    /* Wakeup CPU1 */
    PMU_EnableWlanWakeup(1);
}

void WL_MCI_WAKEUP_DONE0_DriverIRQHandler(void)
{
    IRQn_Type irq_num = WL_MCI_WAKEUP_DONE0_IRQn;

    /* Mask IMU ICU interrupt */
    DisableIRQ(irq_num);
    /* Clear CPU1 wakeup register */
    PMU_DisableWlanWakeup(1);
    EnableIRQ(irq_num);
}

void mlan_init_wakeup_irq()
{
#ifndef __ZEPHYR__
    /* Enable WLAN wakeup done interrupt */
    NVIC_SetPriority(WL_MCI_WAKEUP_DONE0_IRQn, MCI_WAKEUP_DONE_PRIORITY);
    NVIC_EnableIRQ(WL_MCI_WAKEUP_DONE0_IRQn);
#endif
}

void mlan_deinit_wakeup_irq()
{
#ifndef __ZEPHYR__
    NVIC_DisableIRQ(WL_MCI_WAKEUP_DONE0_IRQn);
    NVIC_ClearPendingIRQ(WL_MCI_WAKEUP_DONE0_IRQn);
#endif
}

mlan_status imu_wifi_init(enum wlan_type type, const uint8_t *fw_ram_start_addr, const size_t size)
{
    mlan_status mlanstatus = MLAN_STATUS_SUCCESS;
    int ret                = 0;
    int retry_cnt          = 3;
    int retry_cnt_fw_init  = 3;
#ifdef RW610
    int temperature_val = 0;
#endif

    ret = wlan_init_struct();
    if (ret != WM_SUCCESS)
    {
        wifi_io_e("Init failed. Cannot create init struct");
        return MLAN_STATUS_FAILURE;
    }

    /* Initialize the mlan subsystem before initializing 878x driver */
    (void)mlan_subsys_init();

retry:
    /* Comment out this line if CPU1 image is downloaded through J-Link.
     * This is for load service case only.
     */
    power_off_device(LOAD_WIFI_FIRMWARE);
    wifi_io_d("%u IMU download WLAN FW.\n", OSA_TicksGet());
    /* Download firmware */
    ret = sb3_fw_download(LOAD_WIFI_FIRMWARE, 1, (uint32_t)fw_ram_start_addr);
    /* If fw download is failed, retry downloading for 3 times. */
    if (ret)
    {
        if (retry_cnt != 0)
        {
            retry_cnt--;
            goto retry;
        }
        else
        {
            wifi_io_e("Download firmware failed");
            mlanstatus = MLAN_STATUS_FAILURE;
            return mlanstatus;
        }
    }
    wifi_io_d("%u WLAN FW is active.\n", OSA_TicksGet());
#if CONFIG_WIFI_RECOVERY
    if (wifi_recovery_enable)
    {
        wifi_w("WiFi recovery mode done!");
        wifi_recovery_enable = false;
    }
#endif
    if (wifi_shutdown_enable)
    {
        wifi_shutdown_enable = false;
    }

#ifdef RW610
    wifi_cau_temperature_enable();
    wifi_pmip_v33_enable();
    temperature_val = wifi_cau_temperature_write_to_firmware();
    (void)PRINTF("Wi-Fi cau temperature : %d\r\n", temperature_val);
#endif

    wifi_init_imulink();

    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, imumc_cmdrsp_handler, IMU_MSG_COMMAND_RESPONSE);

    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, imumc_event_handler, IMU_MSG_EVENT);

    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, imumc_rxpkt_handler, IMU_MSG_RX_DATA);

    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, imumc_ctrl_handler, IMU_MSG_CONTROL);

    /* If we're running a Manufacturing image, start the tasks.
       If not, initialize and setup the firmware */
    switch (type)
    {
        case WLAN_TYPE_NORMAL:
            ret = wlan_fw_init_cfg();
            if (ret != true)
            {
                if (retry_cnt_fw_init > 0)
                {
                    wifi_io_e("wlan_fw_init_cfg failed: retry %d", retry_cnt_fw_init);
                    (void)HAL_ImuGetTaskLock();
                    mlan_deinit_wakeup_irq();
                    HAL_ImuDeinit(kIMU_LinkCpu1Cpu3, MBIT(1) | MBIT(0));
                    (void)HAL_ImuPutTaskLock();
                    retry_cnt_fw_init--;
                    goto retry;
                }
                else
                {
                    wifi_io_e("wlan_fw_init_cfg failed: return for retry done");
                    mlanstatus = MLAN_STATUS_FAILURE;
                    return mlanstatus;
                }
            }
            break;
        case WLAN_TYPE_WIFI_CALIB:
            g_txrx_flag = true;
            break;
        case WLAN_TYPE_FCC_CERTIFICATION:
            g_txrx_flag = true;
            break;
        default:
            wifi_io_e("Enter a valid input to sd_wifi_init");
            return MLAN_STATUS_FAILURE;
    }

    mlan_init_wakeup_irq();

    return mlanstatus;
}

void imu_wifi_deinit(void)
{
    uint32_t flag = 0;

#ifdef WLAN_LOW_POWER_ENABLE
    low_power_mode = false;
#endif
    cal_data_valid = false;
    mac_addr_valid = false;
#if 0
    wlan_cmd_shutdown();
    // sdio_drv_deinit();
#endif
    (void)wlan_deinit_struct();

    flag = MBIT(1) | MBIT(0);
#if CONFIG_WIFI_RECOVERY
    flag |= wifi_recovery_enable;
#endif

    mlan_deinit_wakeup_irq();
    HAL_ImuDeinit(kIMU_LinkCpu1Cpu3, flag);

    mlan_subsys_deinit();
}

void imu_uninstall_callback(void)
{
    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, NULL, IMU_MSG_COMMAND_RESPONSE);
    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, NULL, IMU_MSG_EVENT);
    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, NULL, IMU_MSG_RX_DATA);
    HAL_ImuInstallCallback(kIMU_LinkCpu1Cpu3, NULL, IMU_MSG_CONTROL);
}

HostCmd_DS_COMMAND *wifi_get_command_buffer()
{
    /* First 4 bytes reserved for SDIO pkt header */
    return (HostCmd_DS_COMMAND *)(cmd_buf + INTF_HEADER_LEN);
}

bus_operations bus_ops = {
    .fw_is_hang      = imu_fw_is_hang,
    .intf_header_len = INTF_HEADER_LEN,
};

int imu_create_task_lock(void)
{
    int ret = 0;

    ret = HAL_ImuCreateTaskLock();
    if (ret != WM_SUCCESS)
    {
        wifi_e("Create imu task lock failed.");
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

void imu_delete_task_lock(void)
{
    HAL_ImuDeleteTaskLock();
}

int imu_get_task_lock(void)
{
    int ret = 0;

    ret = HAL_ImuGetTaskLock();
    if (ret != WM_SUCCESS)
    {
        wifi_d("Get imu task lock failed.");
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int imu_put_task_lock(void)
{
    int ret = 0;

    ret = HAL_ImuPutTaskLock();
    if (ret != WM_SUCCESS)
    {
        wifi_d("Put imu task lock failed.");
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

#if CONFIG_HOST_SLEEP
void wifi_print_wakeup_reason(t_u16 hs_wakeup_reason)
{
    ARG_UNUSED(hs_wakeup_reason);
    if (mlan_adap->wlan_wakeup.type == IMU_MSG_CONTROL)
    {
        (void)PRINTF("Woken up by WLAN(IMU ctrl msg subtype 0x%x)\r\n", mlan_adap->wlan_wakeup.subtype);
    }
    else if (mlan_adap->wlan_wakeup.type == IMU_MSG_COMMAND_RESPONSE)
    {
        (void)PRINTF("Woken up by WLAN(command response 0x%x)\r\n", mlan_adap->wlan_wakeup.id);
    }
    else if (mlan_adap->wlan_wakeup.type == IMU_MSG_EVENT)
    {
        (void)PRINTF("Woken up by WLAN(event 0x%x)\r\n", mlan_adap->wlan_wakeup.id);
    }
    else if (mlan_adap->wlan_wakeup.type == IMU_MSG_RX_DATA || mlan_adap->wlan_wakeup.type == IMU_MSG_MULTI_RX_DATA)
    {
        (void)PRINTF("Woken up by WLAN(Rx data)\r\n");
    }
	else
    {
        ; // none to do.
    }
}

void wifi_clear_wakeup_reason(void)
{
    if (mlan_adap != NULL)
    {
        (void)memset(&mlan_adap->wlan_wakeup, 0x0, sizeof(wlan_wakeup_reason));
    }
}
#endif
