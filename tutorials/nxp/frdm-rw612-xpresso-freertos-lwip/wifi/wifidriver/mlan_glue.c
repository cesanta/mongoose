/** @file mlan_glue.c
 *
 *  @brief This file acts as a glue between legacy wlan code and mlan based wlan
 *  code
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>
#include <wifi_events.h>
#include <wifi-debug.h>

#include <wifi.h>
#include "wifi-internal.h"
#if defined(RW610)
#include "wifi-imu.h"
#include "fsl_ocotp.h"
#else
#include "wifi-sdio.h"
#endif

#if CONFIG_WPA_SUPP
#include "wifi_nxp.h"
#endif

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>

#if CONFIG_WIFI_FW_DEBUG
#define FW_DEBUG_INFO_SIZE 400
#endif
#define RUTXCMD_RESP_BUFF_SIZE 1024

static const uint8_t wpa2_akmp_oui[4] = {0x00, 0x0f, 0xac, 0x01};
#ifdef WLAN_LOW_POWER_ENABLE
bool low_power_mode;
#endif
bool cal_data_valid = 0;
static uint8_t *cal_data;
static uint32_t cal_data_len;

bool mac_addr_valid;
static uint8_t *mac_addr;
#if CONFIG_WIFI_TX_BUFF
#if CONFIG_AMSDU_IN_AMPDU
uint16_t tx_buf_size = MLAN_TX_DATA_BUF_SIZE_4K;
#else
uint16_t tx_buf_size = MLAN_TX_DATA_BUF_SIZE_2K;
#endif
#endif
#ifdef RW610
extern t_u32 last_resp_rcvd;
#endif

extern uint8_t dev_mac_addr[MLAN_MAC_ADDR_LENGTH];
extern uint8_t dev_mac_addr_uap[MLAN_MAC_ADDR_LENGTH];

#if (CONFIG_11MC) || (CONFIG_11AZ)
extern ftm_start_param ftm_param;
static char ftm_address[255] = CIVIC_ADDRESS;
#if CONFIG_WLS_CSI_PROC
extern uint8_t wls_data[WLS_CSI_DATA_LEN];
#endif
#endif

#if CONFIG_WPA2_ENTP
bool scan_enable_wpa2_enterprise_ap_only;
#endif

#if CONFIG_11K
typedef MLAN_PACK_START struct _Event_Gen_t
{
    /** Event ID */
    uint16_t event_id;
    /** BSS index number for multiple BSS support */
    uint8_t bss_index;
    /** BSS type */
    uint8_t bss_type;
    /** No of bytes in packet including this field */
    uint16_t length;
    /** tlv */
    uint8_t tlv[1];
} MLAN_PACK_END Event_Gen_t;
#endif

/*
 *  fixme: This entry is not present in mlan_fw.h. It is likely possible
 *  that this code is no-op in the firmware. Keeping it as it is for
 *  maintaining backward compatibility. Can be removed later.
 */
#define host_CMD_802_11_AUTHENTICATE 0x0011

/* Following is allocated in mlan_register */
mlan_adapter *mlan_adap;

static mlan_device mlan_dev;

uint8_t g_rssi;
int16_t g_bcn_nf_last;

/* fixme: This global variable is needed
 *  to save the correct event since SLP_CFRM command
 *  returns incorrect bitmap. This will be  further investigated
 *  and if possile global variable will be removed.*/
static t_u16 ps_event;

#if CONFIG_NET_MONITOR
/* record status */
bool g_monitor_status = false;

/* monitor recv data user callback */
int (*net_monitor_callback)(void *buffer, t_u16 data_len) = NULL;
#endif

#if defined(RW610)
extern int wlan_send_hostcmd(const void *cmd_buf, uint32_t cmd_buf_len, void *host_resp_buf, uint32_t resp_buf_len, uint32_t *reqd_resp_len);
#endif

int mlan_subsys_init(void);
int mlan_subsys_deinit(void);

void wifi_get_mac_address_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *get_mac_addr);
void wifi_get_value1_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint32_t *dev_value1);
void wifi_get_firmware_ver_ext_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *fw_ver_ext);
int wifi_set_tx_power_ext(uint32_t len, uint32_t *power_data);
int wifi_send_bss_ioctl(mlan_ds_bss *bss);

void wifi_prepare_get_fw_ver_ext_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number, int version_str_sel);
void wifi_prepare_get_value1(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_enable_amsdu_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_get_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_get_hw_spec_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_set_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_set_cal_data_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wifi_prepare_reconfigure_tx_buf_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
void wlan_prepare_mac_control_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);

#ifdef OTP_CHANINFO
void wifi_prepare_get_channel_region_cfg_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number);
#endif

void wrapper_wlan_cmd_11n_cfg(HostCmd_DS_COMMAND *cmd);
void wrapper_deliver_amsdu_subframe(pmlan_buffer amsdu_pmbuf, t_u8 *data, t_u16 pkt_len);
int wrapper_wlan_set_regiontable(t_u8 region, t_u16 band);
int wrapper_wlan_handle_rx_packet(t_u16 datalen, RxPD *rxpd, void *p, void *payload);
int wrapper_get_wpa_ie_in_assoc(uint8_t *wpa_ie);

void wlan_process_hang(uint8_t fw_reload);

#if CONFIG_11N
/*
 * The command event received from the firmware (e.g. EVENT_ADDBA) cannot
 * be processed immediately. We need to do it the WLC manager context. For
 * that we will save the buffer lest some other event destroy it. WLC
 * manager will be notified of this event.
 *
 * Returns NULL if buffer allocation failed.
 */
static void *wifi_11n_save_request(Event_Ext_t *evt)
{
#if !CONFIG_MEM_POOLS
    void *dot11n_eventbuf = OSA_MemoryAllocate(evt->length);
#else
    void *dot11n_eventbuf = OSA_MemoryPoolAllocate(buf_256_MemoryPool);
#endif

    if (dot11n_eventbuf == MNULL)
    {
        wifi_w("11n eventbuf alloc failed %d", evt->length);
        return NULL;
    }

    (void)memcpy((void *)dot11n_eventbuf, (const void *)evt, evt->length);
    return dot11n_eventbuf;
}
#endif /* CONFIG_11N */

void wrapper_deliver_amsdu_subframe(pmlan_buffer amsdu_pmbuf, t_u8 *data, t_u16 pkt_len)
{
    RxPD *prx_pd = (RxPD *)(void *)amsdu_pmbuf->pbuf;
    w_pkt_d("[amsdu] [push]: BSS Type: %d L: %d", prx_pd->bss_type, pkt_len);
    wm_wifi.amsdu_data_input_callback(prx_pd->bss_type, data, pkt_len);
}

static mlan_status wrapper_moal_recv_packet(IN t_void *pmoal_handle, IN pmlan_buffer pmbuf)
{
    RxPD *prx_pd = (RxPD *)(void *)(pmbuf->pbuf + pmbuf->data_offset);
    if (pmbuf->data_offset != 0U)
    {
        wifi_e("pmbuf->data_offset != 0 (%d)?", pmbuf->data_offset);
        while (true)
        {
            ;
        }
    }

    w_pkt_d("[push] %p + %d:  DELV: %p L: %d", pmbuf->pbuf, pmbuf->data_offset, pmbuf->lwip_pbuf, pmbuf->data_len);

    wm_wifi.deliver_packet_above_callback((void *)prx_pd, prx_pd->bss_type, pmbuf->lwip_pbuf);

#if !(CONFIG_TX_RX_ZERO_COPY) && !(FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER)
    /* Free RxPD */
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(pmbuf->pbuf);
    OSA_MemoryFree(pmbuf);
#else
    OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf->pbuf);
    OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf);
#endif

#endif
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Copy memory from one area to another
 *
 *  @param pmoal Pointer to the MOAL context
 *  @param pdest    Pointer to the dest memory
 *  @param psrc     Pointer to the src memory
 *  @param num      Number of bytes to move
 *  @param dest_size size of dest memory.
 *
 *  @return         Pointer to the dest memory
 */
t_void *wrapper_moal_memcpy_ext(t_void *pmoal, t_void *pdest, const t_void *psrc, t_u32 num, t_u32 dest_size)
{
    t_void *p = pdest;
    if (pdest && psrc && num && dest_size)
        p = memcpy(pdest, psrc, MIN(num, dest_size));

    return p;
}

/** moal_init_timer*/
static mlan_status wrapper_moal_init_timer(IN t_void *pmoal_handle,
                                           OUT t_void *ptimer,
                                           IN t_void (*callback)(osa_timer_arg_t arg),
                                           IN t_void *pcontext)
{
    w_tmr_d("Creating timer");

    /*
     * As can be seen from the arguments to our function, we do not
     * have a timer period parameter. Nor do we have a parameter to
     * specify whether the user wants a periodic or a one-shot
     * timer. We have to wait for these paramters till the next call to
     * wrapper_moal_start_timer.
     *
     * But a timer object cannot be created in
     * wrapper_moal_start_timer. Because the timer handle is passed by
     * value.
     *
     * Considering all this we will need to create the timer object in the
     * current function. We will provide a dummy time here one and
     * change in later. One assumption from the current observation is
     * that only one shot timers are created. So we will assume this
     * and raise assert in *timer_start if this is not the case.
     */
#define DUMMY_TIME_PARAM 10

    osa_status_t status = OSA_TimerCreate((osa_timer_handle_t)ptimer, DUMMY_TIME_PARAM, callback, pcontext,
                                          KOSA_TimerOnce, OSA_TIMER_NO_ACTIVATE);
    if (status != KOSA_StatusSuccess)
    {
        w_tmr_e("Unable to create timer");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** moal_start_timer*/
static mlan_status wrapper_moal_start_timer(IN t_void *pmoal_handle, IN t_void *ptimer, IN bool periodic, IN t_u32 msec)
{
    w_tmr_d("Starting timer: %p for %d mS", ptimer, msec);

    if (periodic == MTRUE)
    {
        /* Check note in wrapper_moal_init_timer for details */
        w_tmr_w("Periodic is not supported temporarily");
        while (true)
        {
            ;
        }
        // wmpanic();
    }

    osa_status_t status = OSA_TimerChange((osa_timer_handle_t)ptimer, msec, 0);
    if (status != KOSA_StatusSuccess)
    {
        w_tmr_e("Unable to change period in timer");
        return MLAN_STATUS_FAILURE;
    }

    status = OSA_TimerActivate((osa_timer_handle_t)ptimer);
    if (status != KOSA_StatusSuccess)
    {
        w_tmr_e("Unable to start timer");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** moal_reset_timer*/
static mlan_status wrapper_moal_reset_timer(IN t_void *pmoal_handle, IN t_void *ptimer)
{
    w_tmr_d("Resetting timer: %p", ptimer);

    int rv = OSA_TimerReset((osa_timer_handle_t)ptimer);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to reset timer.");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** moal_stop_timer*/
static mlan_status wrapper_moal_stop_timer(IN t_void *pmoal_handle, IN t_void *ptimer)
{
    w_tmr_d("Stopping timer: %p", ptimer);

    int rv = OSA_TimerDeactivate((osa_timer_handle_t)ptimer);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to stop timer.");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/** moal_free_timer */
static mlan_status wrapper_moal_free_timer(IN t_void *pmoal_handle, IN t_void *ptimer)
{
    w_tmr_d("Deleting timer: %p", *((uint32_t *)ptimer));

    int rv = OSA_TimerDestroy((osa_timer_handle_t)ptimer);
    if (rv != WM_SUCCESS)
    {
        w_tmr_e("Unable to delete timer.");
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

static mlan_status wrapper_moal_init_lock(IN t_void *pmoal_handle, OUT t_void *plock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

/** moal_free_lock */
static mlan_status wrapper_moal_free_lock(IN t_void *pmoal_handle, IN t_void *plock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

/** moal_spin_lock */
static mlan_status wrapper_moal_spin_lock(IN t_void *pmoal_handle, IN t_void *plock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

/** moal_spin_unlock */
static mlan_status wrapper_moal_spin_unlock(IN t_void *pmoal_handle, IN t_void *plock)
{
#ifdef DEBUG_DEVELOPMENT
    wmprintf("%s: Locks not needed for now\n\r", __func__);
#endif /* DEBUG_DEVELOPMENT */
    return MLAN_STATUS_SUCCESS;
}

#if CONFIG_WMM
/* os_semaphore_t equals (t_void *)(*pplock) */
static mlan_status wrapper_moal_init_semaphore(IN t_void *pmoal_handle, IN const char *name, OUT t_void *plock)
{
    //    if (*((os_semaphore_t *)pplock) != MNULL)
    //        return MLAN_STATUS_SUCCESS;

    if (OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)plock) != KOSA_StatusSuccess)
        return MLAN_STATUS_FAILURE;

    OSA_SemaphorePost((osa_semaphore_handle_t)plock);
    return MLAN_STATUS_SUCCESS;
}

static mlan_status wrapper_moal_free_semaphore(IN t_void *pmoal_handle, IN t_void *plock)
{
    //    if (*((os_semaphore_t *)pplock) == MNULL)
    //        return MLAN_STATUS_SUCCESS;

    if (OSA_SemaphoreDestroy((osa_semaphore_handle_t)plock) != KOSA_StatusSuccess)
        return MLAN_STATUS_FAILURE;

    return MLAN_STATUS_SUCCESS;
}

static mlan_status wrapper_moal_semaphore_get(IN t_void *pmoal_handle, IN t_void *plock)
{
    if (OSA_SemaphoreWait((osa_semaphore_handle_t)plock, 60000) != KOSA_StatusSuccess)
    {
        assert(0);
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

static mlan_status wrapper_moal_semaphore_put(IN t_void *pmoal_handle, IN t_void *plock)
{
    if (OSA_SemaphorePost((osa_semaphore_handle_t)plock) != KOSA_StatusSuccess)
        return MLAN_STATUS_FAILURE;

    return MLAN_STATUS_SUCCESS;
}
#endif

/** woal_callbacks */
static mlan_callbacks woal_callbacks = {
    .moal_malloc      = wrapper_moal_malloc,
    .moal_mfree       = wrapper_moal_mfree,
    .moal_recv_packet = wrapper_moal_recv_packet,
    .moal_memcpy_ext  = wrapper_moal_memcpy_ext,
    .moal_init_timer  = wrapper_moal_init_timer,
    .moal_free_timer  = wrapper_moal_free_timer,
    .moal_start_timer = wrapper_moal_start_timer,
    .moal_reset_timer = wrapper_moal_reset_timer,
    .moal_stop_timer  = wrapper_moal_stop_timer,
    .moal_init_lock   = wrapper_moal_init_lock,
    .moal_free_lock   = wrapper_moal_free_lock,
    .moal_spin_lock   = wrapper_moal_spin_lock,
    .moal_spin_unlock = wrapper_moal_spin_unlock,
#if CONFIG_WMM
    .moal_init_semaphore = wrapper_moal_init_semaphore,
    .moal_free_semaphore = wrapper_moal_free_semaphore,
    .moal_semaphore_get  = wrapper_moal_semaphore_get,
    .moal_semaphore_put  = wrapper_moal_semaphore_put,
#endif
};

int mlan_subsys_init(void)
{
    (void)memcpy((void *)&mlan_dev.callbacks, (const void *)&woal_callbacks, sizeof(mlan_callbacks));

    /* The mlinux driver has an array of these which is dynamically allocated
     * in function woal_update_drv_tbl (moal_main.c). We have only one.
     */
    /** BSS type */
    mlan_dev.bss_attr[0].bss_type = MLAN_BSS_TYPE_STA;
    /** Data frame type: Ethernet II, 802.11, etc. */
    mlan_dev.bss_attr[0].frame_type = MLAN_DATA_FRAME_TYPE_ETH_II;
    /** The BSS is active (non-0) or not (0). */
    mlan_dev.bss_attr[0].active = MTRUE;
    /** BSS Priority */
    mlan_dev.bss_attr[0].bss_priority = 0;
    /** BSS number */
    mlan_dev.bss_attr[0].bss_num = 0;
    /* 11d initialization */
    /* mlan_dev.cfg_11d = 0; */

#if UAP_SUPPORT
    mlan_dev.bss_attr[1].bss_type = MLAN_BSS_TYPE_UAP;
    /** Data frame type: Ethernet II, 802.11, etc. */
    mlan_dev.bss_attr[1].frame_type = MLAN_DATA_FRAME_TYPE_ETH_II;
    /** The BSS is active (non-0) or not (0). */
    mlan_dev.bss_attr[1].active = MTRUE;
    /** BSS Priority */
    mlan_dev.bss_attr[1].bss_priority = 0;
    /** BSS number */
    mlan_dev.bss_attr[1].bss_num = 0;
#endif

    /* fixme: check return value above status */
    (void)mlan_register(&mlan_dev, (t_void **)&mlan_adap);

    (void)mlan_init_fw(mlan_adap);
    /* wlan_init_adapter(mlan_adap); */

    return WM_SUCCESS;
}

int mlan_subsys_deinit(void)
{
    wlan_clear_scan_bss();
    (void)mlan_unregister(mlan_adap);
    mlan_adap = MNULL;

    return WM_SUCCESS;
}

#if CONFIG_11N
/* Note: The saved_event_buff is allocated by the cmd resp/event handler
   thread. We need to free it in this function */
int wrapper_wlan_cmd_11n_addba_rspgen(void *saved_event_buff)
{
    Event_Ext_t *evt = (Event_Ext_t *)saved_event_buff;

    (void)wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    if (evt->bss_type == BSS_TYPE_STA)
    {
        cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, BSS_TYPE_STA);
        (void)wlan_cmd_11n_addba_rspgen((mlan_private *)mlan_adap->priv[evt->bss_type], cmd, &evt->reason_code);
    }
#if UAP_SUPPORT
    else if (evt->bss_type == BSS_TYPE_UAP)
    {
        cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, BSS_TYPE_UAP);

        (void)wlan_cmd_11n_uap_addba_rspgen((mlan_private *)mlan_adap->priv[evt->bss_type], cmd, &evt->reason_code);
    }
#endif
    else
    { /* Do Nothing */
    }

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(saved_event_buff);
#else
    OSA_MemoryPoolFree(buf_256_MemoryPool, saved_event_buff);
#endif

    (void)wifi_wait_for_cmdresp(NULL);

    return 0;
}

int wrapper_wlan_cmd_11n_ba_stream_timeout(void *saved_event_buff)
{
    Event_Ext_t *evt = (Event_Ext_t *)saved_event_buff;

    wlan_11n_ba_stream_timeout(mlan_adap->priv[evt->bss_type], (HostCmd_DS_11N_BATIMEOUT *)(void *)&evt->reason_code);

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(saved_event_buff);
#else
    OSA_MemoryPoolFree(buf_256_MemoryPool, saved_event_buff);
#endif

    return 0;
}

int wrapper_wlan_cmd_11n_delba_rspgen(void *saved_event_buff)
{
    Event_Ext_t *evt = (Event_Ext_t *)saved_event_buff;

    wlan_11n_delete_bastream((mlan_private *)mlan_adap->priv[evt->bss_type], (t_u8 *)&evt->reason_code);

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(saved_event_buff);
#else
    OSA_MemoryPoolFree(buf_256_MemoryPool, saved_event_buff);
#endif

    return 0;
}

#if UAP_SUPPORT
void wrapper_wlan_update_uap_rxrate_info(RxPD *rxpd)
{
    pmlan_private priv = mlan_adap->priv[1];

    priv->rxpd_rate = rxpd->rx_rate;
#ifdef SD8801
    priv->rxpd_htinfo = rxpd->ht_info;
#else
    priv->rxpd_rate_info = rxpd->rate_info;
#endif
}

int wrapper_wlan_uap_ampdu_enable(uint8_t *addr
#if CONFIG_WMM
                                  ,
                                  t_u8 tid
#endif
)
{
    int ret;
    TxBAStreamTbl *ptx_tbl;
    mlan_private *pmpriv_uap = mlan_adap->priv[1];

    if (!(pmpriv_uap->is_11n_enabled))
        return MLAN_STATUS_SUCCESS;

    wlan_request_ralist_lock(pmpriv_uap);
    wlan_11n_update_txbastream_tbl_tx_cnt(pmpriv_uap, addr);
    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(pmpriv_uap, addr)))
    {
        if (
#if CONFIG_WMM
            !ptx_tbl->ampdu_stat[tid] && ptx_tbl->ampdu_supported[tid]
#else
            !ptx_tbl->ampdu_stat[0] && ptx_tbl->ampdu_supported[0]
#endif
            && ptx_tbl->txpkt_cnt >= ptx_tbl->txba_thresh
            && ptx_tbl->ba_status != BA_STREAM_SETUP_INPROGRESS)
        {
            send_add_ba_param_t *addba = NULL;

            ptx_tbl->ba_status = BA_STREAM_SETUP_INPROGRESS;
            wlan_release_ralist_lock(pmpriv_uap);
#if !CONFIG_MEM_POOLS
            addba = (send_add_ba_param_t *)OSA_MemoryAllocate(sizeof(send_add_ba_param_t));
#else
            addba = (send_add_ba_param_t *)OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
            if (!addba)
            {
                wifi_w("No memory available for addba req");
                wlan_request_ralist_lock(pmpriv_uap);
                ptx_tbl->ba_status = BA_STREAM_NOT_SETUP;
                wlan_release_ralist_lock(pmpriv_uap);
                return MLAN_STATUS_FAILURE;
            }

            addba->interface = WLAN_BSS_TYPE_UAP;
#if CONFIG_WMM
            addba->tid = tid;
#else
            addba->tid = 0;
#endif
            (void)memcpy(addba->peer_mac, addr, MLAN_MAC_ADDR_LENGTH);
            ret = wifi_event_completion(WIFI_EVENT_11N_SEND_ADDBA, WIFI_EVENT_REASON_SUCCESS, addba);
            if (ret != WM_SUCCESS)
            {
                wifi_d("uap: failed to send addba req");
                wlan_request_ralist_lock(pmpriv_uap);
                ptx_tbl->ba_status = BA_STREAM_NOT_SETUP;
                wlan_release_ralist_lock(pmpriv_uap);
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree(addba);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, addba);
#endif
                return MLAN_STATUS_FAILURE;
            }
        }
        else
            wlan_release_ralist_lock(pmpriv_uap);
    }
    else
        wlan_release_ralist_lock(pmpriv_uap);
    return MLAN_STATUS_SUCCESS;
}
#endif

static mlan_status do_wlan_ret_11n_addba_resp(HostCmd_DS_COMMAND *resp)
{
    mlan_status rv = MLAN_STATUS_SUCCESS;

    if (resp->result != HostCmd_RESULT_OK)
    {
        wifi_d("Failed: ADDBA RESULT: %d", resp->result);
        return MLAN_STATUS_FAILURE;
    }

    int bss_type = HostCmd_GET_BSS_TYPE(resp->seq_num);

    if (bss_type == BSS_TYPE_STA)
    {
        mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
        rv                   = wlan_ret_11n_addba_resp(pmpriv, resp);
    }
#if UAP_SUPPORT
    if (bss_type == BSS_TYPE_UAP)
    {
        mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
        rv                   = wlan_ret_11n_addba_resp(pmpriv, resp);
    }
#endif

    return rv;
}

static mlan_status do_wlan_ret_11n_addba_req(mlan_private *priv, HostCmd_DS_COMMAND *resp)
{
    t_u8 tid;
    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp = (HostCmd_DS_11N_ADDBA_RSP *)&resp->params.add_ba_rsp;
    padd_ba_rsp->status_code              = wlan_le16_to_cpu(padd_ba_rsp->status_code);
    TxBAStreamTbl *ptx_ba_tbl;

    padd_ba_rsp->block_ack_param_set = wlan_le16_to_cpu(padd_ba_rsp->block_ack_param_set);
    padd_ba_rsp->block_ack_tmo       = wlan_le16_to_cpu(padd_ba_rsp->block_ack_tmo);
    padd_ba_rsp->ssn                 = (wlan_le16_to_cpu(padd_ba_rsp->ssn)) & SSN_MASK;
    padd_ba_rsp->status_code         = wlan_le16_to_cpu(padd_ba_rsp->status_code);

    tid = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_TID_MASK) >> BLOCKACKPARAM_TID_POS;

    if (padd_ba_rsp->status_code == BA_RESULT_SUCCESS)
    {
        wlan_request_ralist_lock(priv);
        if ((ptx_ba_tbl = wlan_11n_get_txbastream_tbl(priv, padd_ba_rsp->peer_mac_addr)))
        {
            wlan_11n_update_txbastream_tbl_ampdu_stat(priv, padd_ba_rsp->peer_mac_addr, MTRUE, tid);

            PRINTM(MCMND, "ADDBA REQ: %02x:%02x:%02x:%02x:%02x:%02x tid=%d ssn=%d win_size=%d,amsdu=%d\n",
                   padd_ba_rsp->peer_mac_addr[0], padd_ba_rsp->peer_mac_addr[1], padd_ba_rsp->peer_mac_addr[2],
                   padd_ba_rsp->peer_mac_addr[3], padd_ba_rsp->peer_mac_addr[4], padd_ba_rsp->peer_mac_addr[5], tid,
                   padd_ba_rsp->ssn,
                   ((padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_WINSIZE_MASK) >> BLOCKACKPARAM_WINSIZE_POS),
                   padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_AMSDU_SUPP_MASK);

            ptx_ba_tbl->ba_status = BA_STREAM_SETUP_COMPLETE;

            if ((padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_AMSDU_SUPP_MASK) && priv->add_ba_param.tx_amsdu)
                ptx_ba_tbl->amsdu = MTRUE;
            else
                ptx_ba_tbl->amsdu = MFALSE;
            wlan_release_ralist_lock(priv);
        }
        else
        {
            wlan_release_ralist_lock(priv);
            PRINTM(MERROR, "BA stream not created\n");
        }
    }
    else
    {
        if (padd_ba_rsp->add_rsp_result != BA_RESULT_TIMEOUT)
        {
            wlan_request_ralist_lock(priv);
            if ((ptx_ba_tbl = wlan_11n_get_txbastream_tbl(priv, padd_ba_rsp->peer_mac_addr)))
            {
                wlan_11n_update_txbastream_tbl_ampdu_stat(priv, padd_ba_rsp->peer_mac_addr, MFALSE, tid);
                ptx_ba_tbl->ampdu_supported[tid] = MFALSE;
                ptx_ba_tbl->ba_status = BA_STREAM_NOT_SETUP;
            }
            wlan_release_ralist_lock(priv);
        }
        else
        {
            wlan_request_ralist_lock(priv);
            if ((ptx_ba_tbl = wlan_11n_get_txbastream_tbl(priv, padd_ba_rsp->peer_mac_addr)))
            {
                /* Clear txpkt_cnt to avoid collision between our STA and our uAP */
                ptx_ba_tbl->txpkt_cnt = 0;
                ptx_ba_tbl->ba_status = BA_STREAM_NOT_SETUP;
            }
            wlan_release_ralist_lock(priv);
        }
        wifi_d("Failed: ADDBA req: %d", padd_ba_rsp->add_rsp_result);
    }

    return MLAN_STATUS_SUCCESS;
}

static mlan_status do_wlan_ret_11n_delba(mlan_private *priv, HostCmd_DS_COMMAND *resp)
{
    HostCmd_DS_11N_DELBA *pdel_ba = (HostCmd_DS_11N_DELBA *)&resp->params.del_ba;
    int tid;

    pdel_ba->del_ba_param_set = wlan_le16_to_cpu(pdel_ba->del_ba_param_set);
    tid                       = pdel_ba->del_ba_param_set >> DELBA_TID_POS;

    if (pdel_ba->del_result == BA_RESULT_SUCCESS)
    {
        mlan_11n_update_bastream_tbl(priv, tid, pdel_ba->peer_mac_addr, TYPE_DELBA_SENT,
                                     INITIATOR_BIT(pdel_ba->del_ba_param_set));
    }
    else
    {
        wifi_d("Failed: DELBA: %d", pdel_ba->del_result);
    }

    return MLAN_STATUS_SUCCESS;
}

// Only Enable AMPDU for station interface
int wrapper_wlan_sta_ampdu_enable(
#if CONFIG_WMM
    t_u8 tid
#endif
)
{
    int ret;
    mlan_private *pmpriv               = (mlan_private *)mlan_adap->priv[0];
    t_u8 cur_mac[MLAN_MAC_ADDR_LENGTH] = {0};
    TxBAStreamTbl *ptx_tbl             = NULL;

    wlan_request_ralist_lock(pmpriv);
    if (pmpriv->media_connected == MTRUE)
    {
        (void)memcpy(cur_mac, pmpriv->curr_bss_params.bss_descriptor.mac_address, MLAN_MAC_ADDR_LENGTH);
        ptx_tbl = wlan_11n_get_txbastream_tbl(pmpriv, cur_mac);
        if (!ptx_tbl)
        {
            wlan_11n_create_txbastream_tbl(pmpriv, cur_mac, BA_STREAM_NOT_SETUP);

            ptx_tbl              = wlan_11n_get_txbastream_tbl(pmpriv, cur_mac);
            ptx_tbl->txba_thresh = 1;

            if (pmpriv->curr_bss_params.bss_descriptor.pht_cap)
            {
                wlan_11n_update_txbastream_tbl_ampdu_supported(pmpriv, cur_mac, MTRUE);
            }
            else
            {
                wlan_release_ralist_lock(pmpriv);
                return MLAN_STATUS_SUCCESS;
            }
        }
    }
    else
    {
        wifi_d("sta not connected: failed to send addba req");
        wlan_release_ralist_lock(pmpriv);
        return MLAN_STATUS_FAILURE;
    }

    wlan_11n_update_txbastream_tbl_tx_cnt(pmpriv, cur_mac);

    if (
#if CONFIG_WMM
        !ptx_tbl->ampdu_stat[tid] && ptx_tbl->ampdu_supported[tid]
#else
        !ptx_tbl->ampdu_stat[0] && ptx_tbl->ampdu_supported[0]
#endif
        && (ptx_tbl->txpkt_cnt >= ptx_tbl->txba_thresh)
        && (ptx_tbl->ba_status != BA_STREAM_SETUP_INPROGRESS))
    {
        send_add_ba_param_t *addba = NULL;

        ptx_tbl->ba_status = BA_STREAM_SETUP_INPROGRESS;
        wlan_release_ralist_lock(pmpriv);
#if !CONFIG_MEM_POOLS
        addba = (send_add_ba_param_t *)OSA_MemoryAllocate(sizeof(send_add_ba_param_t));
#else
        addba = (send_add_ba_param_t *)OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
        if (!addba)
        {
            wifi_w("No memory available for addba req");
            wlan_request_ralist_lock(pmpriv);
            ptx_tbl->ba_status = BA_STREAM_NOT_SETUP;
            wlan_release_ralist_lock(pmpriv);
            return MLAN_STATUS_FAILURE;
        }

        addba->interface = WLAN_BSS_TYPE_STA;
#if CONFIG_WMM
        addba->tid = tid;
#else
        addba->tid = 0;
#endif
        (void)memcpy(addba->peer_mac, cur_mac, MLAN_MAC_ADDR_LENGTH);
        ret = wifi_event_completion(WIFI_EVENT_11N_SEND_ADDBA, WIFI_EVENT_REASON_SUCCESS, addba);
        if (ret != WM_SUCCESS)
        {
            wifi_d("sta: failed to send addba req");
            wlan_request_ralist_lock(pmpriv);
            ptx_tbl->ba_status = BA_STREAM_NOT_SETUP;
            wlan_release_ralist_lock(pmpriv);
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(addba);
#else
            OSA_MemoryPoolFree(buf_32_MemoryPool, addba);
#endif
            return MLAN_STATUS_FAILURE;
        }
    }
    else
        wlan_release_ralist_lock(pmpriv);

    return MLAN_STATUS_SUCCESS;
}
#endif /* CONFIG_11N */

int wrapper_wlan_11d_enable(t_u32 state)
{
    t_u32 enable_11d = state;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x00;
    cmd->result  = 0x0;

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET,
                             Dot11D_i, NULL, &enable_11d, cmd);

    return wifi_wait_for_cmdresp(NULL);
}

int wrapper_wlan_uap_11d_enable(t_u32 state)
{
    t_u32 enable_11d = state;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_UAP);
    cmd->result  = 0x0;

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET,
                             Dot11D_i, NULL, &enable_11d, cmd);

    return wifi_wait_for_cmdresp(NULL);
}

int wifi_11h_enable(void)
{
    t_u32 enable_11h = (t_u32)ENABLE_11H;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET,
                                   (t_u32)Dot11H_i, NULL, &enable_11h, cmd);

    return wifi_wait_for_cmdresp(NULL);
}

int wrapper_wlan_ecsa_enable(void)
{
    bool ecsa_enable = MTRUE;

    if ((mlan_adap->fw_cap_info & FW_CAPINFO_ECSA) == 0U)
    {
        return -WM_FAIL;
    }

    mlan_adap->ecsa_enable = MTRUE;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET,
                                   (t_u32)ECSAEnable_i, NULL, &ecsa_enable, cmd);

    return wifi_wait_for_cmdresp(NULL);
}

int wrapper_wlan_cmd_get_hw_spec(void)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    (void)wlan_cmd_get_hw_spec((mlan_private *)mlan_adap->priv[0], cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    return 0;
}

mlan_status wrapper_wlan_cmd_mgmt_ie(int bss_type, void *buffer, unsigned int len, t_u16 action)
{
    void *pdata_buf = NULL;
    HostCmd_DS_MGMT_IE_LIST_CFG ds_mgmt_ie_list_cfg;
    mlan_status status = MLAN_STATUS_SUCCESS;

    CHECK_BSS_TYPE(bss_type, MLAN_STATUS_FAILURE);
    (void)wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, bss_type);
    cmd->result  = 0x0;

    (void)memset(&ds_mgmt_ie_list_cfg, 0x00, sizeof(HostCmd_DS_MGMT_IE_LIST_CFG));

    (void)memcpy((void *)&ds_mgmt_ie_list_cfg, (const void *)buffer, len);

    pdata_buf = &ds_mgmt_ie_list_cfg;

#if UAP_SUPPORT
    /* !UAP_SUPPORT will return in the entry of function, thus won't come here */
    if (bss_type == MLAN_BSS_TYPE_UAP)
        status = wlan_ops_uap_prepare_cmd((mlan_private *)mlan_adap->priv[1], HOST_CMD_APCMD_SYS_CONFIGURE, action, 0,
                                          NULL, pdata_buf, cmd);
    else
#endif
        status = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MGMT_IE_LIST, action, 0, NULL,
                                          pdata_buf, cmd);

    (void)wifi_wait_for_cmdresp(buffer);
    return status;
}

int wrapper_wlan_handle_rx_packet(t_u16 datalen, RxPD *rxpd, void *p, void *payload)
{
    w_pkt_d("[recv]                  : %p T: %d L: %d", p, rxpd->rx_pkt_type, rxpd->rx_pkt_length);

#if !(CONFIG_TX_RX_ZERO_COPY) && !(FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER)
    /* fixme: Check if mlan buffer can be allocated from standard mlan
       function */
#if !CONFIG_MEM_POOLS
    pmlan_buffer pmbuf = OSA_MemoryAllocate(sizeof(mlan_buffer));
#else
    pmlan_buffer pmbuf = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif

    if (pmbuf == MNULL)
    {
        /* No mlan_buffer available. Drop this packet */
        /* fixme: Need to note this event. */
        wifi_w("No memory available. Have to drop packet.");
        return -WM_FAIL;
    }
#else
    pmlan_buffer pmbuf = (pmlan_buffer)((t_u8 *)rxpd - INTF_HEADER_LEN - sizeof(mlan_buffer));
#endif
    /** Buffer descriptor, e.g. skb in Linux */
    /* Note: We are storing payload member here. We need to unwind
       pointer when passing pbuf to lwip */
    pmbuf->pdesc = payload;

    /* Store it for later retrivval. mlan stack does not know about this */
    pmbuf->lwip_pbuf = p;

    /* fixme: check that no data is accessed after this pointer
      Note: Important: mlan requires RxPD in number of places. We
    already have duplicated the ethernet packet in the pbuf. We cannot
    afford to duplicate the entire SDIO buffer and reduplicate the
    ethernet part of it into pbuf which the lwip requires. So, we will
    duplicate only the RxPD for ourselves. We will have to change all
    the code which assumes that there is ethernet packet after RxPD */
    /** Pointer to buffer */
    /* fixme: CHK this*/
#if !(CONFIG_TX_RX_ZERO_COPY) && !(FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER)
#if !CONFIG_MEM_POOLS
    pmbuf->pbuf = (t_u8 *)OSA_MemoryAllocate(sizeof(RxPD));
#else
    pmbuf->pbuf        = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif
    if (pmbuf->pbuf == MNULL)
    {
        /* No buffer available. Drop this packet */
        /* fixme: Need to note this event. */
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(pmbuf);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf);
#endif
        wifi_w("No memory available. Have to drop packet.");
        return -WM_FAIL;
    }
    (void)memcpy((void *)pmbuf->pbuf, (const void *)rxpd, sizeof(RxPD));
#else
    pmbuf->pbuf        = (t_u8 *)rxpd;
#endif
    /** Offset to data */
    /* This should ideally be INTF_HEADER_LEN. But we not be storing
       initial INTF_HEADER_LEN bytes. We will store RxPD directly */
    /* fixme: CHK this*/
    pmbuf->data_offset = 0;

    /** Data length */
    /* fixme: CHK this*/
    pmbuf->data_len = datalen;
    /** Buffer type: data, cmd, event etc. */
    pmbuf->buf_type = MLAN_BUF_TYPE_DATA;
    /** Fields below are valid for data packet only */
    /** QoS priority */
    /* t_u32 priority; */
    /** Time stamp when packet is received (seconds) */
    /* t_u32 in_ts_sec; */
    /** Time stamp when packet is received (micro seconds) */
    /* t_u32 in_ts_usec; */
    /** Time stamp when packet is processed (seconds) */
    /* t_u32 out_ts_sec; */
    /** Time stamp when packet is processed (micro seconds) */
    /* t_u32 out_ts_usec; */

    /** Fields below are valid for MLAN module only */
    /** Pointer to parent mlan_buffer */
    /* struct _mlan_buffer *pparent; */
    /** Use count for this buffer */
    /* t_u32 use_count; */

    mlan_status rv = wlan_handle_rx_packet(mlan_adap, pmbuf);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        /* mlan did not accept this packet for some reason. Some
           probable reasons are - duplicate packet - packet outside
           the current 11n aggregation window.

           We need to free allocated structures.
        */
#if !(CONFIG_TX_RX_ZERO_COPY) && !(FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER)
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(pmbuf->pbuf);
        OSA_MemoryFree(pmbuf);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf->pbuf);
        OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf);
#endif
#endif
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

#if CONFIG_NET_MONITOR
void register_monitor_user_callback(int (*monitor_cb)(void *frame, t_u16 len))
{
    net_monitor_callback = monitor_cb;
}

void deregister_monitor_user_callback()
{
    net_monitor_callback = NULL;
}

void set_monitor_flag(bool flag)
{
    g_monitor_status = flag;
}

bool get_monitor_flag()
{
    return g_monitor_status;
}

void user_recv_monitor_data(const t_u8 *rcvdata)
{
    t_s8 rssi              = 0;
    t_u16 datalen          = 0;
    t_u8 *net_monitor_data = NULL;
    RxPD *rxpd             = (RxPD *)((t_u8 *)rcvdata + INTF_HEADER_LEN);
    t_u16 inimupkt_len     = *(t_u16 *)rcvdata;

    datalen = rxpd->rx_pkt_length + sizeof(t_s8);
    rssi    = rxpd->snr - rxpd->nf;

    if ((rxpd->rx_pkt_length + rxpd->rx_pkt_offset + INTF_HEADER_LEN) != inimupkt_len)
    {
        wifi_w("rx_pkt_length + rx_pkt_offset + INTF_HEADER_LEN is not equal to inimupkt_len \n\r");
        wifi_w("Invalid data, discard \n\r");
        wifi_w("rx_pkt_length :%d \n\r", rxpd->rx_pkt_length);
        wifi_w("rx_pkt_offset :%d \n\r", rxpd->rx_pkt_offset);
        wifi_w("inimupkt_len  :%d \n\r", inimupkt_len);
        return;
    }

    if (net_monitor_callback != NULL)
    {
        net_monitor_data = OSA_MemoryAllocate(datalen);

        if (!net_monitor_data)
        {
            wifi_w("No mem. Cannot process net monitor data");
            return;
        }

        memcpy(net_monitor_data, &rssi, sizeof(t_s8));
        memcpy(net_monitor_data + sizeof(t_s8), ((t_u8 *)rcvdata + INTF_HEADER_LEN + rxpd->rx_pkt_offset),
               rxpd->rx_pkt_length);

        net_monitor_callback((void *)net_monitor_data, datalen);

        OSA_MemoryFree(net_monitor_data);
    }

    return;
}
#endif
void wrapper_wlan_cmd_11n_cfg(HostCmd_DS_COMMAND *cmd)
{
    mlan_ds_11n_tx_cfg txcfg = {/** HTTxCap */
                                .httxcap = DOT11N_CFG_ENABLE_GREENFIELD_XMIT | DOT11N_CFG_ENABLE_SHORT_GI_20MHZ |
                                           DOT11N_CFG_ENABLE_SHORT_GI_40MHZ,
                                /** HTTxInfo */
                                .httxinfo = DOT11N_CFG_ENABLE_RIFS,
                                /** Band selection */
                                .misc_cfg = BAND_SELECT_BOTH};

    (void)wlan_cmd_11n_cfg((mlan_private *)mlan_adap->priv[0], cmd, HostCmd_ACT_GEN_SET, &txcfg);
}

/*
  fixme: Following three functions will not be needed when
  wlan_11h_handle_event_chanswann is used to handle the relevant event */
void set_event_chanswann(void)
{
    mlan_adap->state_11h.recvd_chanswann_event = MTRUE;
}

void clear_event_chanswann(void)
{
    mlan_adap->state_11h.recvd_chanswann_event = MFALSE;
}

void wrapper_wlan_11d_clear_parsedtable(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    (void)wlan_11d_clear_parsedtable(pmpriv);
}

/*
  This was added to solve the radar DFS issue. The function
  wlan_11h_radar_detect_required() checks for this and sets the scan to
  active if media is connected. The temporary problem with our current port
  of mlan is that after a deauth is sent after channel switch event from
  the AP, the media connected event is still MTRUE. Hence, during scan the
  old channel of the AP is marked as active scan which is not a correct way
  of scanning on that channel (since radar interference is detected on
  it). So, for now we clear this bit manually to avoid active scan on the
  old channel. This bit will be set again at the next association completion.

  fixme: Check this later.
 */
void wrapper_clear_media_connected_event(void)
{
    mlan_private *pmpriv    = (mlan_private *)mlan_adap->priv[0];
    pmpriv->media_connected = MFALSE;
}

mlan_status wifi_prepare_and_send_cmd(IN mlan_private *pmpriv,
                                      IN t_u16 cmd_no,
                                      IN t_u16 cmd_action,
                                      IN t_u32 cmd_oid,
                                      IN t_void *pioctl_buf,
                                      IN t_void *pdata_buf,
                                      mlan_bss_type bss_type,
                                      void *priv)
{
    pmlan_ioctl_req pioctl_req = (mlan_ioctl_req *)pioctl_buf;
    mlan_status rv             = MLAN_STATUS_SUCCESS;

    if (pioctl_req != NULL)
    {
        if (pioctl_req->bss_index == 1U)
        {
            bss_type = MLAN_BSS_TYPE_UAP;
        }
    }

    CHECK_BSS_TYPE(bss_type, MLAN_STATUS_FAILURE);

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u8)bss_type);
    cmd->result  = 0x0;

#if UAP_SUPPORT
    /* !UAP_SUPPORT will return in the entry of function, thus won't come here */
    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        rv = wlan_ops_uap_prepare_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, cmd);
    }
    else
#endif
    {
        rv = wlan_ops_sta_prepare_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, cmd);
    }

    if (rv != MLAN_STATUS_SUCCESS)
    {
        wifi_d("Failed to prepare cmd.");
        wm_wifi.cmd_resp_priv = NULL;
        (void)wifi_put_command_lock();
        return rv;
    }

    (void)wifi_wait_for_cmdresp(priv);
    return MLAN_STATUS_SUCCESS;
}

bool wmsdk_is_11N_enabled(void)
{
#if CONFIG_11N
    return true;
#else
    return false;
#endif /* CONFIG_11N */
}

/* static */ int wifi_send_bss_ioctl(mlan_ds_bss *bss)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)bss;
    req.buf_len   = sizeof(mlan_ds_bss);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_BSS;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wrapper_get_wpa_ie_in_assoc(uint8_t *wpa_ie)
{
    mlan_private *priv = (mlan_private *)mlan_adap->priv[0];

    /* Reset all state variables */
    (void)memcpy((void *)wpa_ie, (const void *)&priv->wpa_ie, sizeof(priv->wpa_ie));

    return (int)priv->wpa_ie_len;
}

static int wifi_send_11n_cfg_ioctl(mlan_act_ioctl action, mlan_ds_11n_cfg *ds_11n_cfg)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_11n_cfg;
    req.buf_len   = sizeof(mlan_ds_11n_cfg);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_11N_CFG;
    req.action    = action;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

static int wifi_send_uap_11n_cfg_ioctl(mlan_act_ioctl action, mlan_ds_11n_cfg *ds_11n_cfg)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_11n_cfg;
    req.buf_len   = sizeof(mlan_ds_11n_cfg);
    req.bss_index = 1;
    req.req_id    = MLAN_IOCTL_11N_CFG;
    req.action    = action;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_txbfcap(unsigned int tx_bf_cap)
{
    mlan_ds_11n_cfg ds_11n_cfg;

    (void)memset(&ds_11n_cfg, 0x00, sizeof(mlan_ds_11n_cfg));

    ds_11n_cfg.sub_command     = MLAN_OID_11N_CFG_TX_BF_CAP;
    ds_11n_cfg.param.tx_bf_cap = tx_bf_cap;

    return wifi_send_11n_cfg_ioctl(MLAN_ACT_SET, &ds_11n_cfg);
}

int wifi_set_htcapinfo(unsigned int htcapinfo)
{
    mlan_ds_11n_cfg ds_11n_cfg;

    (void)memset(&ds_11n_cfg, 0x00, sizeof(mlan_ds_11n_cfg));

    ds_11n_cfg.sub_command           = MLAN_OID_11N_HTCAP_CFG;
    ds_11n_cfg.param.htcap_cfg.htcap = htcapinfo;
#if CONFIG_5GHz_SUPPORT
    ds_11n_cfg.param.htcap_cfg.misc_cfg = BAND_SELECT_BOTH;
#else
    ds_11n_cfg.param.htcap_cfg.misc_cfg = BAND_SELECT_BG;
#endif

    return wifi_send_11n_cfg_ioctl(MLAN_ACT_SET, &ds_11n_cfg);
}

int wifi_set_httxcfg(unsigned short httxcfg)
{
    mlan_ds_11n_cfg ds_11n_cfg;

    (void)memset(&ds_11n_cfg, 0x00, sizeof(mlan_ds_11n_cfg));

    ds_11n_cfg.sub_command          = MLAN_OID_11N_CFG_TX;
    ds_11n_cfg.param.tx_cfg.httxcap = httxcfg;
#if CONFIG_5GHz_SUPPORT
    ds_11n_cfg.param.tx_cfg.misc_cfg = BAND_SELECT_BOTH;
#else
    ds_11n_cfg.param.tx_cfg.misc_cfg    = BAND_SELECT_BG;
#endif

    return wifi_send_11n_cfg_ioctl(MLAN_ACT_SET, &ds_11n_cfg);
}

int wifi_uap_set_httxcfg_int(unsigned short httxcfg)
{
    mlan_ds_11n_cfg ds_11n_cfg;

    (void)memset(&ds_11n_cfg, 0x00, sizeof(mlan_ds_11n_cfg));

    ds_11n_cfg.sub_command          = MLAN_OID_11N_CFG_TX;
    ds_11n_cfg.param.tx_cfg.httxcap = httxcfg;
#if CONFIG_5GHz_SUPPORT
    ds_11n_cfg.param.tx_cfg.misc_cfg = BAND_SELECT_BOTH;
#else
    ds_11n_cfg.param.tx_cfg.misc_cfg    = BAND_SELECT_BG;
#endif

    return wifi_send_uap_11n_cfg_ioctl(MLAN_ACT_SET, &ds_11n_cfg);
}

static int wifi_send_tx_power_cfg_ioctl(mlan_act_ioctl action, mlan_ds_power_cfg *ds_power_cfg)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_power_cfg;
    req.buf_len   = sizeof(mlan_ds_power_cfg);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_POWER_CFG;
    req.action    = action;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_tx_power(t_u32 power_level)
{
    mlan_ds_power_cfg ds_power_cfg;

    (void)memset(&ds_power_cfg, 0x00, sizeof(mlan_ds_power_cfg));

    ds_power_cfg.sub_command                   = MLAN_OID_POWER_CFG;
    ds_power_cfg.param.power_cfg.is_power_auto = MFALSE;
    ds_power_cfg.param.power_cfg.power_level   = power_level;

    return wifi_send_tx_power_cfg_ioctl(MLAN_ACT_SET, &ds_power_cfg);
}

int wifi_set_tx_power_ext(uint32_t len, uint32_t *power_data)
{
    mlan_ds_power_cfg ds_power_cfg;

    (void)memset(&ds_power_cfg, 0x00, sizeof(mlan_ds_power_cfg));

    ds_power_cfg.sub_command         = MLAN_OID_POWER_CFG_EXT;
    ds_power_cfg.param.power_ext.len = len;
    (void)memcpy((void *)ds_power_cfg.param.power_ext.power_data, (const void *)power_data, len);

    return wifi_send_tx_power_cfg_ioctl(MLAN_ACT_SET, &ds_power_cfg);
}

int wifi_get_tx_power(t_u32 *power_level)
{
    int ret;
    mlan_ds_power_cfg ds_power_cfg;
    mlan_ioctl_req ioctl_req_pwr_cfg;
    (void)memset(&ds_power_cfg, 0x00, sizeof(mlan_ds_power_cfg));
    (void)memset(&ioctl_req_pwr_cfg, 0x00, sizeof(mlan_ioctl_req));

    ds_power_cfg.sub_command                   = MLAN_OID_POWER_CFG;
    ds_power_cfg.param.power_cfg.is_power_auto = MFALSE;
    wm_wifi.cmd_resp_ioctl                     = &ioctl_req_pwr_cfg;
    ioctl_req_pwr_cfg.pbuf                     = (t_u8 *)(&ds_power_cfg);
    ioctl_req_pwr_cfg.buf_len                  = sizeof(mlan_ds_power_cfg);
    ret                                        = wifi_send_tx_power_cfg_ioctl(MLAN_ACT_GET, &ds_power_cfg);
    wm_wifi.cmd_resp_ioctl                     = NULL;

    if (ret == WM_SUCCESS)
    {
        *power_level = ds_power_cfg.param.power_cfg.power_level;
    }
    return ret;
}

static int wifi_send_tx_rate_cfg_ioctl(mlan_act_ioctl action, mlan_ds_rate *ds_rate_cfg, mlan_bss_type bss_type)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)ds_rate_cfg;
    req.buf_len   = sizeof(mlan_ds_rate);
    req.bss_index = bss_type;
    req.req_id    = MLAN_IOCTL_RATE;
    req.action    = action;

    mlan_status rv = MLAN_STATUS_SUCCESS;
    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        if (!is_uap_started())
        {
            wifi_e("uap isn't up\n\r");
            return -WM_FAIL;
        }
        rv = wlan_ops_uap_ioctl(mlan_adap, &req);
    }
    else if (bss_type == MLAN_BSS_TYPE_STA)
    {
        if (is_sta_connected())
        {
            rv = wlan_ops_sta_ioctl(mlan_adap, &req);
        }
        else
        {
            wifi_e("sta connection required before setting tx rate\n\r");
        }
    }

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_txratecfg(wifi_ds_rate ds_rate, mlan_bss_type bss_type)
{
    mlan_ds_rate ds_rate_cfg;

    if (ds_rate.sub_command != WIFI_DS_RATE_CFG)
    {
        return -WM_FAIL;
    }

    (void)memset(&ds_rate_cfg, 0x00, sizeof(mlan_ds_rate));
#if CONFIG_AUTO_NULL_TX
    ds_rate_cfg.auto_null_fixrate_enable = 0xff;
#endif

    ds_rate_cfg.sub_command = MLAN_OID_RATE_CFG;
    if (ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_AUTO)
    {
        ds_rate_cfg.param.rate_cfg.is_rate_auto = MTRUE;
    }
    else
    {
#if CONFIG_AUTO_NULL_TX
        ds_rate_cfg.auto_null_fixrate_enable = ds_rate.auto_null_fixrate_enable;
#endif
        ds_rate_cfg.param.rate_cfg.is_rate_auto = MFALSE;
        ds_rate_cfg.param.rate_cfg.rate_type    = MLAN_RATE_INDEX;
        ds_rate_cfg.param.rate_cfg.rate         = ds_rate.param.rate_cfg.rate_index;
        ds_rate_cfg.param.rate_cfg.rate_format  = ds_rate.param.rate_cfg.rate_format;
#ifdef SD8801
        if (ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_HT)
            ds_rate_cfg.param.rate_cfg.rate += MLAN_RATE_INDEX_MCS0;
#endif
#if CONFIG_11AC
        if (ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_VHT
#if CONFIG_11AX
            || ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_HE
#endif
        )
            ds_rate_cfg.param.rate_cfg.nss = ds_rate.param.rate_cfg.nss;
#endif
        if (ds_rate.param.rate_cfg.rate_setting != 0xffff)
            ds_rate_cfg.param.rate_cfg.rate_setting = ds_rate.param.rate_cfg.rate_setting & ~0x0C00;
        else
            ds_rate_cfg.param.rate_cfg.rate_setting = ds_rate.param.rate_cfg.rate_setting;
    }
    return wifi_send_tx_rate_cfg_ioctl(MLAN_ACT_SET, &ds_rate_cfg, bss_type);
}

int wifi_get_txratecfg(wifi_ds_rate *ds_rate, mlan_bss_type bss_type)
{
    int ret;

    mlan_ds_rate ds_rate_cfg;

    if (ds_rate->sub_command != WIFI_DS_RATE_CFG)
    {
        return -WM_FAIL;
    }

    (void)memset(&ds_rate_cfg, 0x00, sizeof(mlan_ds_rate));

    ds_rate_cfg.sub_command = MLAN_OID_RATE_CFG;

    wm_wifi.cmd_resp_ioctl = ds_rate;

    ret = wifi_send_tx_rate_cfg_ioctl(MLAN_ACT_GET, &ds_rate_cfg, bss_type);

    wm_wifi.cmd_resp_ioctl = NULL;

    return ret;
}

bool wrapper_wlan_11d_support_is_enabled(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    return wlan_11d_support_is_enabled(pmpriv);
}

/*
 * fixme: This function is temporarily present till the mlan transition is complete.
 */
int wrapper_wifi_assoc(
    const unsigned char *bssid, int wlan_security, bool is_wpa_tkip, unsigned int owe_trans_mode, bool is_ft)
{
#if !CONFIG_11R
    (void)is_ft;
#endif
    mlan_private *priv = (mlan_private *)mlan_adap->priv[0];
    t_u8 country_code[COUNTRY_CODE_LEN];
    /* BSSDescriptor_t *bssDesc = OSA_MemoryAllocate(sizeof(BSSDescriptor_t)); */
    /* if (!bssDesc) */
    /* 	return -WM_FAIL; */
    int idx = wlan_find_bssid_in_list(priv, (const unsigned char *)bssid, MLAN_BSS_MODE_NEGATIVE);
    if (idx == -1)
    {
        wifi_w("Could not find BSSID in mlan scan list");
        return -WM_FAIL;
    }

    /* Reset all state variables */
    (void)memset(&priv->wpa_ie, 0, sizeof(priv->wpa_ie));
    priv->wpa_ie_len                   = 0;
    priv->sec_info.wpa2_enabled        = false;
    priv->sec_info.wapi_enabled        = false;
    priv->sec_info.ewpa_enabled        = false;
    priv->sec_info.wpa_enabled         = false;
    priv->sec_info.authentication_mode = MLAN_AUTH_MODE_AUTO;

#if CONFIG_11K
    if (priv->assoc_req_size != 0U)
    {
        /* Append the passed data to the end of
         * the genIeBuffer */
        __memcpy(priv->adapter, priv->gen_ie_buf, priv->assoc_req_buf, priv->assoc_req_size);

        /* Increment the stored buffer length by
         * the size passed */
        priv->gen_ie_buf_len = priv->assoc_req_size;
    }
#endif

#if CONFIG_11R
    priv->sec_info.is_ft = is_ft;
    if (is_ft)
    {
        priv->sec_info.authentication_mode = MLAN_AUTH_MODE_FT;
    }
#endif

    BSSDescriptor_t *d = &mlan_adap->pscan_table[idx];
    /* fixme: This code is quite hacky and is present only because
     * security part is yet not fully integrated into mlan. This will
     * not be necessary after the integration is complete.
     */
#if CONFIG_WPA2_ENTP
    if (d->prsn_ie && (wlan_security == WLAN_SECURITY_EAP_TLS || wlan_security == WLAN_SECURITY_EAP_PEAP_MSCHAPV2))
    {
        priv->sec_info.wpa2_enabled = true;
        if (d->rsn_ie_buff_len <= sizeof(priv->wpa_ie))
        {
            (void)memcpy(priv->wpa_ie, d->rsn_ie_buff, d->rsn_ie_buff_len);
            priv->wpa_ie_len = d->rsn_ie_buff_len;
        }
        else
        {
            wifi_e("Failed to copy RSN IE");
            return -WM_FAIL;
        }
    }
    else
#endif
        if ((d->pwpa_ie != MNULL) && (d->prsn_ie != MNULL) && (wlan_security == WLAN_SECURITY_WPA_WPA2_MIXED))
    {
        priv->sec_info.is_wpa_tkip  = is_wpa_tkip;
        priv->sec_info.wpa2_enabled = true;
        if (d->rsn_ie_buff_len <= sizeof(priv->wpa_ie))
        {
            (void)memcpy((void *)priv->wpa_ie, (const void *)d->rsn_ie_buff, d->rsn_ie_buff_len);
            priv->wpa_ie_len = (t_u8)d->rsn_ie_buff_len;
        }
        else
        {
            wifi_e("Failed to copy RSN IE");
            return -WM_FAIL;
        }
    }
    else if ((d->pwpa_ie != MNULL) &&
             (wlan_security == WLAN_SECURITY_WPA || wlan_security == WLAN_SECURITY_WPA_WPA2_MIXED))
    {
        priv->sec_info.is_wpa_tkip = is_wpa_tkip;
        priv->sec_info.wpa_enabled = true;
        if (d->wpa_ie_buff_len <= sizeof(priv->wpa_ie))
        {
            (void)memcpy((void *)priv->wpa_ie, (const void *)d->wpa_ie_buff, d->wpa_ie_buff_len);
            priv->wpa_ie_len = (t_u8)d->wpa_ie_buff_len;
        }
        else
        {
            wifi_e("Failed to copy WPA IE");
            return -WM_FAIL;
        }
    }
    else if ((d->prsn_ie != MNULL) &&
#if CONFIG_11R
             (!is_ft) &&
#endif
             (wlan_security == WLAN_SECURITY_WPA2 || wlan_security == WLAN_SECURITY_WPA_WPA2_MIXED ||
#if CONFIG_DRIVER_OWE
              owe_trans_mode == OWE_TRANS_MODE_OWE || wlan_security == WLAN_SECURITY_OWE_ONLY ||
#endif
              wlan_security == WLAN_SECURITY_WPA3_SAE || wlan_security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED
#if CONFIG_11R
              || wlan_security == WLAN_SECURITY_WPA2_FT
#endif
              ))
    {
        priv->sec_info.is_wpa_tkip  = is_wpa_tkip;
        priv->sec_info.wpa2_enabled = true;
        if (d->rsn_ie_buff_len <= sizeof(priv->wpa_ie))
        {
            (void)memcpy((void *)priv->wpa_ie, (const void *)d->rsn_ie_buff, d->rsn_ie_buff_len);
            priv->wpa_ie_len = (t_u8)d->rsn_ie_buff_len;
        }
        else
        {
            wifi_e("Failed to copy RSN IE.");
            return -WM_FAIL;
        }
#if CONFIG_11R
        if ((!is_ft) && (wlan_security == WLAN_SECURITY_WPA2 || wlan_security == WLAN_SECURITY_WPA3_SAE ||
                         wlan_security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED || wlan_security == WLAN_SECURITY_WPA2_FT))
        {
            if (d->md_ie_buff_len <= sizeof(priv->md_ie))
            {
                (void)memcpy((void *)priv->md_ie, (const void *)d->md_ie_buff, (size_t)d->md_ie_buff_len);
                priv->md_ie_len = d->md_ie_buff_len;
            }
        }
#endif
        /* In case of WPA3 SAE-PSK mixed mode AP, RSN IE processing sets the SAE AKM,
         * but if the configured security is WPA2 PSK then AKM must be of PSK
         * hence update the AKM to WPA2 PSK and reset the PMF capabilities
         */

        /* Skip index to 1 byte (RSN information) + 1 byte (Tag length) + 2 byte (RSN version) + 4 byte (Group Cipher
         * Suite)*/
        int index      = 8;
        /* Get pairwise count value from wpa_ie (2 Bytes LE) */
        uint16_t pairwise_count = priv->wpa_ie[index + 1] << 8 | priv->wpa_ie[index];
        /* Skip 2 bytes pairwise_count + all pairwise cipher suite in list (Each 4 bytes) */
        index = index + 2 + pairwise_count * 4;
        /* Get AKM count value from wpa_ie (2 Bytes LE) */
        uint16_t akm_count = priv->wpa_ie[index + 1] << 8 | priv->wpa_ie[index];
        /* Skip 2 bytes akm_count */
        index = index + 2;
        if (wlan_security == WLAN_SECURITY_WPA2)
        {
            /* Skip OUI */
            index = index + 3;
            for (int i = 0; i < akm_count; i++)
            {
                /* Check AKM type field is SAE (0x8) */
                if (priv->wpa_ie[index] == 0x8)
                {
                    /* Replace AKM type to PSK (0x2) */
                    priv->wpa_ie[index] = 0x02;
                }
                /* Skip OUI (1 byte) + AKM type (1 byte) */
                index = index + 4;
            }
        }
    }
    else
    { /* Do Nothing */
    }

    if ((MNULL != d) && (*d->country_info.country_code) && (d->country_info.len > COUNTRY_CODE_LEN) &&
        (!priv->adapter->country_ie_ignore))
    {
        country_code[0] = d->country_info.country_code[0];
        country_code[1] = d->country_info.country_code[1];
        country_code[2] = ' ';

        wlan_set_country_code((const char *)country_code);
    }

    /* The original assoc cmd handling function of mlan sends
       additional two commands to the firmware; both
       HostCmd_CMD_802_11D_DOMAIN_INFO. In the current wmsdk wifi
       driver design we have only one command buffer. Thus, we cannot
       new cmd while the current cmd is being sent. We will change the
       default flow a little and send the 801.11D domain info commands
       first and skip that step later */
    if (wlan_11d_support_is_enabled(priv) && !mlan_adap->country_ie_ignore)
    {
        if (priv->support_11d != NULL)
        {
            if (priv->support_11d->wlan_11d_create_dnld_countryinfo_p(priv, BAND_B) != MLAN_STATUS_SUCCESS)
            {
                PRINTM(MERROR, "Dnld_countryinfo_11d failed\n");
                return -WM_FAIL;
            }
#if CONFIG_5GHz_SUPPORT
            if ((!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
                && (priv->support_11d->wlan_11d_create_dnld_countryinfo_p(priv, BAND_A) != MLAN_STATUS_SUCCESS))
            {
                PRINTM(MERROR, "Dnld_countryinfo_11d failed\n");
                return -WM_FAIL;
            }
#endif
            if (priv->support_11d->wlan_11d_parse_dnld_countryinfo_p(priv, d) != MLAN_STATUS_SUCCESS)
            {
                return -WM_FAIL;
            }
        }
    }
    /* fixme: check if this needs to go on heap */
    mlan_ds_bss bss;
    (void)memset(&bss, 0x00, sizeof(mlan_ds_bss));
    bss.sub_command          = MLAN_OID_BSS_START;
    bss.param.ssid_bssid.idx = (t_u32)idx + 1UL; /* + 1 req. by mlan */
    return wifi_send_bss_ioctl(&bss);
}

#if CONFIG_WPA_SUPP
static mlan_status wlan_set_wpa_ie_helper(mlan_private *priv, t_u8 *ie_data_ptr, t_u16 ie_len)
{
    ENTER();

    if (ie_len)
    {
        if (ie_len > sizeof(priv->wpa_ie))
        {
            PRINTM(MERROR, "failed to copy, WPA IE is too big\n");
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        __memcpy(priv->adapter, priv->wpa_ie, ie_data_ptr, ie_len);
        priv->wpa_ie_len = (t_u8)ie_len;
        PRINTM(MIOCTL, "Set Wpa_ie_len=%d IE=%#x\n", priv->wpa_ie_len, priv->wpa_ie[0]);
        DBG_HEXDUMP(MCMD_D, "Wpa_ie", priv->wpa_ie, priv->wpa_ie_len);
        if (priv->wpa_ie[0] == WPA_IE)
        {
            priv->sec_info.wpa_enabled = MTRUE;
        }
        else if (priv->wpa_ie[0] == RSN_IE)
        {
            priv->sec_info.wpa2_enabled = MTRUE;
        }
        else
        {
            priv->sec_info.wpa_enabled  = MFALSE;
            priv->sec_info.wpa2_enabled = MFALSE;
        }
    }
    else
    {
        __memset(priv->adapter, priv->wpa_ie, 0, sizeof(priv->wpa_ie));
        priv->wpa_ie_len = 0;
        PRINTM(MINFO, "Reset Wpa_ie_len=%d IE=%#x\n", priv->wpa_ie_len, priv->wpa_ie[0]);
        priv->sec_info.wpa_enabled  = MFALSE;
        priv->sec_info.wpa2_enabled = MFALSE;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

static mlan_status wlan_set_gen_ie_helper(mlan_private *priv, t_u8 *ie_data_ptr, t_u16 ie_len)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    IEEEtypes_VendorHeader_t *pvendor_ie;
    const t_u8 wpa_oui[] = {0x00, 0x50, 0xf2, 0x01};
#if CONFIG_WPA_SUPP_WPS
    const t_u8 wps_oui[] = {0x00, 0x50, 0xf2, 0x04};
#endif
#if CONFIG_WPA_SUPP_WPS
    const t_u8 wpse_oui[] = {0x00, 0x50, 0xf2, 0x10};
#endif
#ifdef ENABLE_HOTSPOT
    const t_u8 osen_oui[] = {0x50, 0x6f, 0x9a, 0x12};
#endif
    t_u8 i = 0, temp[12] = {0};

    ENTER();

    /* If the passed length is zero, reset the buffer */
    if (!ie_len)
    {
        priv->gen_ie_buf_len = 0;
#if CONFIG_WPA_SUPP_WPS
        priv->wps.session_enable = MFALSE;
#endif
        wlan_set_wpa_ie_helper(priv, MNULL, 0);
#ifdef ENABLE_HOTSPOT
        wlan_set_osen_ie(priv, MNULL, 0);
#endif
    }
    else if (!ie_data_ptr)
    {
        /* MNULL check */
        ret = MLAN_STATUS_FAILURE;
    }
    else
    {
        pvendor_ie = (IEEEtypes_VendorHeader_t *)ie_data_ptr;
        if (pvendor_ie->element_id == EXT_CAPABILITY)
        {
            __memcpy(priv->adapter, temp, &priv->ext_cap, sizeof(priv->ext_cap));
            for (i = 0; i < MIN(sizeof(priv->ext_cap), pvendor_ie->len); i++)
                temp[i] |= ie_data_ptr[2 + i];
            __memcpy(priv->adapter, &priv->ext_cap, temp, sizeof(priv->ext_cap));
        }
        else
            /* Test to see if it is a WPA IE, if not, then it is a
               gen IE*/
            if (((pvendor_ie->element_id == WPA_IE) &&
                 (!__memcmp(priv->adapter, pvendor_ie->oui, wpa_oui, sizeof(pvendor_ie->oui))) &&
                 (pvendor_ie->oui_type == wpa_oui[3U])) ||
                (pvendor_ie->element_id == RSN_IE))
            {
                /* IE is a WPA/WPA2 IE so call set_wpa function */
                ret = wlan_set_wpa_ie_helper(priv, ie_data_ptr, ie_len);
#if CONFIG_WPA_SUPP_WPS
                priv->wps.session_enable = MFALSE;
#endif /* CONFIG_WPA_SUPP_WPS */
            }
            else
#ifdef ENABLE_HOTSPOT
                if ((pvendor_ie->element_id == VENDOR_SPECIFIC_221) &&
                    (!__memcmp(priv->adapter, pvendor_ie->oui, osen_oui, sizeof(pvendor_ie->oui))) &&
                    (pvendor_ie->oui_type == osen_oui[3U]))
            {
                /* IE is a OSEN IE so call set_osen function */
                ret = wlan_set_osen_ie(priv, ie_data_ptr, ie_len);
            }
            else
#endif
#if CONFIG_WPA_SUPP_WPS
                if (pvendor_ie->element_id == VENDOR_SPECIFIC_221 &&
                    (!__memcmp(priv->adapter, pvendor_ie->oui, wps_oui, sizeof(pvendor_ie->oui))) &&
                    (pvendor_ie->oui_type == wps_oui[3U]))
            {
                /*
                 * Discard first two byte (Element ID and Length)
                 * because they are not needed in the case of setting
                 * WPS_IE
                 */
                if (pvendor_ie->len > 4)
                {
                    __memcpy(priv->adapter, (t_u8 *)&priv->wps.wps_ie, ie_data_ptr, ie_len);

                    HEXDUMP("wps_ie", (t_u8 *)&priv->wps.wps_ie, priv->wps.wps_ie.vend_hdr.len + 2);
                }
                else
                {
                    /* Only wps oui exist, reset driver wps buffer
                     */
                    __memset(priv->adapter, (t_u8 *)&priv->wps.wps_ie, 0x00, sizeof(priv->wps.wps_ie));
                    PRINTM(MINFO, "wps_ie cleared\n");
                }
            }
            else
#endif
            {
                /*
                 * Verify that the passed length is not larger than
                 * the available space remaining in the buffer
                 */
                if (ie_len < (sizeof(priv->gen_ie_buf) - priv->gen_ie_buf_len))
                {
#if CONFIG_WPA_SUPP_WPS
                    /* Test to see if it is a WPS IE, if so, enable
                     * wps session flag */
                    pvendor_ie = (IEEEtypes_VendorHeader_t *)ie_data_ptr;
                    if ((pvendor_ie->element_id == WPS_IE) &&
                        (!__memcmp(priv->adapter, pvendor_ie->oui, wps_oui, sizeof(pvendor_ie->oui))) &&
                        (pvendor_ie->oui_type == wps_oui[3U]))
                    {
                        priv->wps.session_enable = MTRUE;
                        PRINTM(MINFO, "WPS Session Enabled.\n");
                    }
#endif

#if CONFIG_WPA_SUPP_WPS
                    /* Test to see if it is a WPSE IE, if so,
                     * enable wps session flag */
                    pvendor_ie = (IEEEtypes_VendorHeader_t *)ie_data_ptr;
                    if ((pvendor_ie->element_id == WPS_IE) &&
                        (!__memcmp(priv->adapter, pvendor_ie->oui, wpse_oui, sizeof(pvendor_ie->oui))) &&
                        (pvendor_ie->oui_type == wpse_oui[3U]))
                    {
                        priv->wps.session_enable = MTRUE;
                        PRINTM(MINFO, "WPSE Session Enabled.\n");
                    }
#endif
                    /* Append the passed data to the end of
                     * the genIeBuffer */
                    __memcpy(priv->adapter, priv->gen_ie_buf + priv->gen_ie_buf_len, ie_data_ptr, ie_len);

                    /* Increment the stored buffer length by
                     * the size passed */
                    priv->gen_ie_buf_len += ie_len;
                }
                else
                {
                    /* Passed data does not fit in the
                     * remaining buffer space */
                    ret = MLAN_STATUS_FAILURE;
                }
            }
    }

    /* Return MLAN_STATUS_SUCCESS, or MLAN_STATUS_FAILURE for error case */
    LEAVE();
    return ret;
}

static int wifi_set_ies_cfg(mlan_private *priv, t_u8 *ie, int ie_len)
{
    int bytes_left     = ie_len;
    t_u8 *pcurrent_ptr = ie;
    int total_ie_len;
    t_u8 element_len;
    int ret = WM_SUCCESS;
    IEEEtypes_ElementId_e element_id;
#if CONFIG_WPA_SUPP_WPS
    IEEEtypes_VendorSpecific_t *pvendor_ie;
    t_u8 wps_oui[] = {0x00, 0x50, 0xf2, 0x04};
#endif
#ifdef HOST_CCX
    t_u8 ccx_oui[] = {0x00, 0x40, 0x96, 0x03};
#endif /* HOST_CCX */
#ifdef ENABLE_HOTSPOT
    t_u8 hs20_oui[] = {0x50, 0x6f, 0x9a, 0x10};
#endif /* ENABLE_HOTSPOT */

#ifdef EASYMESH
    t_u8 multiap_oui[] = {0x50, 0x6f, 0x9a, 0x1b};
    t_u8 multiap_flag  = 0;
#endif

    while (bytes_left >= 2)
    {
        element_id   = (IEEEtypes_ElementId_e)(*((t_u8 *)pcurrent_ptr));
        element_len  = *((t_u8 *)pcurrent_ptr + 1);
        total_ie_len = element_len + sizeof(IEEEtypes_Header_t);
        if (bytes_left < total_ie_len)
        {
            PRINTM(MERROR, "InterpretIE: Error in processing IE, bytes left < IE length\n");
            bytes_left = 0;
            continue;
        }
        switch (element_id)
        {
            case RSN_IE:
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    PRINTM(MERROR, "Fail to set RSN IE\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                PRINTM(MIOCTL, "Set RSN IE\n");
                break;
            case VENDOR_SPECIFIC_221:
#if CONFIG_WPA_SUPP_WPS
                pvendor_ie = (IEEEtypes_VendorSpecific_t *)pcurrent_ptr;
                if (!memcmp(pvendor_ie->vend_hdr.oui, wps_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                    (pvendor_ie->vend_hdr.oui_type == wps_oui[3]))
                {
                    wifi_d("Enable WPS session\r\n");
                    priv->wps.session_enable = MTRUE;
                }
#endif

#ifdef EASYMESH
                if (!memcmp(pvendor_ie->vend_hdr.oui, multiap_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                    (pvendor_ie->vend_hdr.oui_type == multiap_oui[3]))
                {
                    multiap_flag = pvendor_ie->data[0];
                    if (MLAN_STATUS_SUCCESS != woal_multi_ap_cfg(priv, wait_option, multiap_flag))
                    {
                        PRINTM(MERROR, "%s: failed to configure multi ap\n", __func__);
                        ret = -WM_FAIL;
                        goto done;
                    }
                }
#endif

#ifdef HOST_CCX
                if (!memcmp(pvendor_ie->vend_hdr.oui, ccx_oui, sizeof(ccx_oui)))
                {
                    PRINTM(MIOCTL, "CCX is enabled for this bss\n");
                    priv->is_ccx = 1;
                }
#endif
#ifdef ENABLE_HOTSPOT
                if (!memcmp(pvendor_ie->vend_hdr.oui, hs20_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                    (pvendor_ie->vend_hdr.oui_type == hs20_oui[3]))
                {
                    PRINTM(MIOCTL, "Hotspot2.0 is enabled for this bss\n");
                    if (MLAN_STATUS_SUCCESS !=
                        woal_set_hotspotcfg(priv, wait_option, (HOTSPOT_BY_SUPPLICANT | HOTSPOT_ENABLED)))
                    {
                        PRINTM(MERROR, "Fail to enable hotspot 2.0\n");
                        ret = -WM_FAIL;
                        goto done;
                    }
                }
#endif
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    PRINTM(MERROR, "Fail to Set VENDOR SPECIFIC IE\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                wifi_d("Set VENDOR SPECIFIC IE, OUI: %02x:%02x:%02x:%02x\r\n", pvendor_ie->vend_hdr.oui[0],
                       pvendor_ie->vend_hdr.oui[1], pvendor_ie->vend_hdr.oui[2], pvendor_ie->vend_hdr.oui_type);
                break;
#if CONFIG_11R
            case MOBILITY_DOMAIN:
                (void)memcpy((void *)priv->md_ie, (const void *)pcurrent_ptr, (size_t)total_ie_len);
                priv->md_ie_len = (size_t)total_ie_len;
                wifi_d("Set MD IE\r\n");
                break;
            case FAST_BSS_TRANSITION:
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    wifi_d(
                        "Fail to set"
                        "FAST_BSS_TRANSITION IE\r\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                wifi_d("Set FAST_BSS_TRANSITION IE\r\n");
                priv->sec_info.is_ft = true;
                break;
            case RIC:
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    wifi_d(
                        "Fail to set"
                        "RESOURCE INFORMATION CONTAINER IE\r\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                wifi_d("Set RESOURCE INFORMATION CONTAINER IE\r\n");
                break;
#endif
            case EXT_CAPABILITY:
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    PRINTM(MERROR, "Fail to set Extended Capabilites IE\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                wifi_d("Set Extended Capabilities IE\n");
                break;
#if CONFIG_HOST_MLME
            case EXTENSION:
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    PRINTM(MERROR, "Fail to set Extension IE\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                PRINTM(MIOCTL, "Set Extension IE\n");
                break;
            case FRAGMENT:
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    PRINTM(MERROR, "Fail to set Fragmented IE\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                PRINTM(MIOCTL, "Set Fragmented IE\n");
                break;
#endif
            default:
                if (MLAN_STATUS_SUCCESS != wlan_set_gen_ie_helper(priv, pcurrent_ptr, total_ie_len))
                {
                    PRINTM(MERROR, "Fail to set GEN IE\n");
                    ret = -WM_FAIL;
                    goto done;
                }
                PRINTM(MIOCTL, "Set GEN IE\n");
                break;
        }
        pcurrent_ptr += element_len + 2;
        /* Need to account for IE ID and IE Len */
        bytes_left -= (element_len + 2);
    }
done:
    return ret;
}

int wifi_set_scan_ies(void *ie, size_t ie_len)
{
    mlan_private *priv = (mlan_private *)mlan_adap->priv[0];
    int ret            = -WM_FAIL;
#if (CONFIG_WPA_SUPP) && (CONFIG_WPA_SUPP_WPS)
    priv->wps.session_enable = MFALSE;
#endif

    /* Reset the generic IE buffer */
    priv->gen_ie_buf_len = 0;

    ret = wifi_set_ies_cfg(priv, (t_u8 *)ie, ie_len);

    if (ret != MLAN_STATUS_SUCCESS)
    {
        wifi_w("Could not set the IEs");
        return (int)-WM_FAIL;
    }

    return WM_SUCCESS;
}

#if CONFIG_WPA_SUPP
#if CONFIG_WPA_SUPP_WPS
static int wifi_assocreq_wps_ie_cfg(mlan_private *priv)
{
    int ret       = WM_SUCCESS;
    int wpsie_len = 0;
    u8 *wps_buf   = NULL;
    wpsie_len     = sizeof(IEEEtypes_Header_t) + priv->wps.wps_ie.vend_hdr.len;
    wps_buf       = (t_u8 *)OSA_MemoryAllocate(wpsie_len);
    (void)memset(wps_buf, 0, wpsie_len);
    (void)__memcpy(priv->adapter, wps_buf, (t_u8 *)&priv->wps.wps_ie, wpsie_len);
    priv->wps.wps_mgmt_bitmap_index =
        wifi_set_mgmt_ie2(priv->bss_type, MGMT_MASK_ASSOC_REQ | MGMT_MASK_REASSOC_REQ, (void *)wps_buf, wpsie_len);
    if (-WM_FAIL != priv->wps.wps_mgmt_bitmap_index)
        ret = WM_SUCCESS;
    else
        ret = -WM_FAIL;
    OSA_MemoryFree(wps_buf);
    return ret;
}
#endif
#endif

int wifi_nxp_send_assoc(nxp_wifi_assoc_info_t *assoc_info)
{
    int ret                    = -WM_FAIL;
    const unsigned char *bssid = (const unsigned char *)&assoc_info->bssid;

    mlan_private *priv = (mlan_private *)mlan_adap->priv[0];
    t_u8 country_code[COUNTRY_CODE_LEN];

    if (priv->auth_alg == WLAN_AUTH_SAE)
    {
        priv->auth_flag = HOST_MLME_AUTH_DONE;
    }

    wifi_remain_on_channel(false, 0, 0);

    if (priv->auth_flag && !(priv->auth_flag & HOST_MLME_AUTH_DONE))
    {
        return -WM_FAIL;
    }

    int idx = wlan_find_bssid_in_list(priv, (const unsigned char *)bssid, MLAN_BSS_MODE_NEGATIVE);
    if (idx == -1)
    {
        wifi_w("Could not find BSSID in mlan scan list");
        return ret;
    }

    BSSDescriptor_t *d = &mlan_adap->pscan_table[idx];

#if CONFIG_HOST_MLME
    priv->curr_bss_params.host_mlme = 1;
#endif

    __memcpy(priv->adapter, &priv->curr_bss_params.prev_bssid, assoc_info->prev_bssid, MLAN_MAC_ADDR_LENGTH);

    /* Reset all state variables */
    (void)memset(&priv->wpa_ie, 0, sizeof(priv->wpa_ie));
    priv->wpa_ie_len                   = 0;
    priv->sec_info.wpa2_enabled        = MFALSE;
    priv->sec_info.wapi_enabled        = MFALSE;
    priv->sec_info.ewpa_enabled        = MFALSE;
    priv->sec_info.wpa_enabled         = MFALSE;
    priv->sec_info.authentication_mode = MLAN_AUTH_MODE_AUTO;

    priv->sec_info.is_wpa_tkip = MFALSE;
#if CONFIG_11R
    priv->sec_info.is_ft = MFALSE;
    priv->md_ie_len      = 0;
#endif

    /* Reset the generic IE buffer */
    priv->gen_ie_buf_len = 0;

#if CONFIG_WPA_SUPP_WPS
    priv->wps.session_enable = MFALSE;
#endif

    ret = wifi_set_ies_cfg(priv, (t_u8 *)assoc_info->wpa_ie.ie, assoc_info->wpa_ie.ie_len);

    if (ret != WM_SUCCESS)
    {
        wifi_w("Could not set the IEs");
        return -WM_FAIL;
    }
#if CONFIG_WPA_SUPP
#if CONFIG_WPA_SUPP_WPS
    if (priv->wps.wps_mgmt_bitmap_index != -1)
    {
        ret = wifi_clear_mgmt_ie2(priv->bss_type, priv->wps.wps_mgmt_bitmap_index);
        if (ret != WM_SUCCESS)
        {
            wifi_e("Clear Assoc req IE failed");
            return -WM_FAIL;
        }
        priv->wps.wps_mgmt_bitmap_index = -1;
    }
    else if (priv->wps.session_enable == MTRUE)
    {
        ret = wifi_assocreq_wps_ie_cfg(priv);
        if (ret != WM_SUCCESS)
        {
            wifi_w("add WPS_IE to assocreq fail");
            return -WM_FAIL;
        }
    }
#endif
#endif

    (void)wifi_set_rx_mgmt_indication(MLAN_BSS_TYPE_STA, WIFI_MGMT_DEAUTH | WIFI_MGMT_DIASSOC | WIFI_MGMT_ACTION);

#if CONFIG_11R
    if (priv->sec_info.is_ft)
    {
        priv->sec_info.authentication_mode = MLAN_AUTH_MODE_FT;
#if CONFIG_GTK_REKEY_OFFLOAD
        (void)__memset(pmadapter, &priv->gtk_rekey, 0, sizeof(priv->gtk_rekey));
#endif
    }
#endif

    if ((MNULL != d) && (*d->country_info.country_code) && (d->country_info.len > COUNTRY_CODE_LEN) &&
        (!priv->adapter->country_ie_ignore))
    {
        country_code[0] = d->country_info.country_code[0];
        country_code[1] = d->country_info.country_code[1];
        country_code[2] = ' ';

        wlan_set_country_code((const char *)country_code);
    }

    /* The original assoc cmd handling function of mlan sends
       additional two commands to the firmware; both
       HostCmd_CMD_802_11D_DOMAIN_INFO. In the current wmsdk wifi
       driver design we have only one command buffer. Thus, we cannot
       new cmd while the current cmd is being sent. We will change the
       default flow a little and send the 801.11D domain info commands
       first and skip that step later */
    if (wlan_11d_support_is_enabled(priv) && !mlan_adap->country_ie_ignore)
    {
        if (priv->support_11d != NULL)
        {
            if (priv->support_11d->wlan_11d_create_dnld_countryinfo_p(priv, BAND_B) != MLAN_STATUS_SUCCESS)
            {
                PRINTM(MERROR, "Dnld_countryinfo_11d failed\n");
                return -WM_FAIL;
            }
#if CONFIG_5GHz_SUPPORT
            if ((!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
                && (priv->support_11d->wlan_11d_create_dnld_countryinfo_p(priv, BAND_A) != MLAN_STATUS_SUCCESS))
            {
                PRINTM(MERROR, "Dnld_countryinfo_11d failed\n");
                return -WM_FAIL;
            }
#endif
            if (priv->support_11d->wlan_11d_parse_dnld_countryinfo_p(priv, d) != MLAN_STATUS_SUCCESS)
            {
                return -WM_FAIL;
            }
        }
    }
    /* fixme: check if this needs to go on heap */
    mlan_ds_bss bss;
    (void)memset(&bss, 0x00, sizeof(mlan_ds_bss));
    bss.sub_command          = MLAN_OID_BSS_START;
    bss.param.ssid_bssid.idx = (t_u32)idx + 1UL; /* + 1 req. by mlan */
    return wifi_send_bss_ioctl(&bss);
}
#endif

static void compute_rssi_values(HostCmd_DS_802_11_RSSI_INFO_RSP *prssi_info_rsp, wifi_rssi_info_t *rssi_info)
{
    g_rssi        = prssi_info_rsp->bcn_rssi_last;
    g_bcn_nf_last = prssi_info_rsp->bcn_nf_last;

    rssi_info->bcn_rssi_last  = prssi_info_rsp->bcn_rssi_last;
    rssi_info->bcn_rssi_avg   = prssi_info_rsp->bcn_rssi_avg;
    rssi_info->data_rssi_last = prssi_info_rsp->data_rssi_last;
    rssi_info->data_rssi_avg  = prssi_info_rsp->data_rssi_avg;

    rssi_info->bcn_snr_last  = CAL_SNR(prssi_info_rsp->bcn_rssi_last, prssi_info_rsp->bcn_nf_last);
    rssi_info->bcn_snr_avg   = CAL_SNR(prssi_info_rsp->bcn_rssi_avg, prssi_info_rsp->bcn_nf_avg);
    rssi_info->data_snr_last = CAL_SNR(prssi_info_rsp->data_rssi_last, prssi_info_rsp->data_nf_last);
    rssi_info->data_snr_avg  = CAL_SNR(prssi_info_rsp->data_rssi_avg, prssi_info_rsp->data_nf_avg);

    rssi_info->bcn_nf_last  = prssi_info_rsp->bcn_nf_last;
    rssi_info->bcn_nf_avg   = prssi_info_rsp->bcn_nf_avg;
    rssi_info->data_nf_last = prssi_info_rsp->data_nf_last;
    rssi_info->data_nf_avg  = prssi_info_rsp->data_nf_avg;
}
/* Function of copy hostcmd response to buffers set in wm_wifi structure */
static void wifi_set_hostcmd_resp(const HostCmd_DS_COMMAND *resp)
{
    if (wm_wifi.cmd_resp_priv == MNULL)
    {
        return;
    }
    /* reset is_hostcmd to 0, so that other commands can be processed */
    wm_wifi.hostcmd_cfg.is_hostcmd = false;

    hostcmd_cfg_t *hcmd_cfg = (hostcmd_cfg_t *)wm_wifi.cmd_resp_priv;

    *hcmd_cfg->reqd_resp_len = resp->size;
    /* Copy if response buffer is suffcient to hold response data */
    if (hcmd_cfg->resp_buf_len >= resp->size)
    {
        (void)memcpy((void *)hcmd_cfg->resp_buf, (const void *)resp, resp->size);
    }
    /* Initialize user pointer to NULL*/
    wm_wifi.cmd_resp_priv = NULL;
}

#if UAP_SUPPORT
static void load_bss_list(const HostCmd_DS_STA_LIST *sta_list)
{
    if (wm_wifi.cmd_resp_priv == MNULL)
    {
        return;
    }

    /* Initialize user pointer */
    *(wifi_sta_list_t **)wm_wifi.cmd_resp_priv = NULL;

    t_u16 c = sta_list->sta_count;
    if (c > MAX_NUM_CLIENTS)
    {
        wifi_w("Too many clients: %d", c);
        return;
    }

#if !CONFIG_MEM_POOLS
    wifi_sta_list_t *sl = OSA_MemoryAllocate(sizeof(wifi_sta_list_t) + c * sizeof(wifi_sta_info_t));
#else
    wifi_sta_list_t *sl = OSA_MemoryPoolAllocate(buf_256_MemoryPool);
#endif

    if (sl == MNULL)
    {
        return;
    }

    sl->count            = c;
    wifi_sta_info_t *sta = (wifi_sta_info_t *)(void *)(((t_u8 *)&sl->count) + sizeof(int));

    int i;
    MrvlIEtypes_sta_info_t *si = (MrvlIEtypes_sta_info_t *)(((t_u8 *)&sta_list->sta_count) + sizeof(t_u16));
    for (i = 0; i < c && i < MAX_NUM_CLIENTS; i++)
    {
        if ((si->rssi & 0x80) != 0)
        {
            // coverity[overrun-local:SUPPRESS]
            sta[i].rssi = -(256 - si->rssi);
        }
        else
        {
            sta[i].rssi = si->rssi;
        }

        (void)memcpy(sta[i].mac, si->mac_address, MLAN_MAC_ADDR_LENGTH);
        sta[i].power_mgmt_status = si->power_mfg_status;
        si = (MrvlIEtypes_sta_info_t *)((t_u8 *)si + (si->header.len + sizeof(MrvlIEtypesHeader_t)));

        wifi_d("RSSI: 0x%x %d dbm", sta[i].rssi, sta[i].rssi);
    }

    /* Note: Caller needs to free this */
    *(wifi_sta_list_t **)wm_wifi.cmd_resp_priv = sl;

    wm_wifi.cmd_resp_status = WM_SUCCESS;
    wm_wifi.cmd_resp_priv   = NULL;
}
#endif

static void load_ver_ext(HostCmd_DS_COMMAND *resp)
{
    if (wm_wifi.cmd_resp_priv == MNULL)
    {
        return;
    }

    HostCmd_DS_VERSION_EXT *ver_ext     = &resp->params.verext;
    wifi_fw_version_ext_t *user_ver_ext = (wifi_fw_version_ext_t *)wm_wifi.cmd_resp_priv;

    (void)memcpy((void *)user_ver_ext->version_str, (const void *)ver_ext->version_str, resp->size - 10U);

    wm_wifi.cmd_resp_status = WM_SUCCESS;
    wm_wifi.cmd_resp_priv   = NULL;
}

int wifi_process_cmd_response(HostCmd_DS_COMMAND *resp)
{
    mlan_private *pmpriv          = (mlan_private *)mlan_adap->priv[0];
    mlan_status rv                = MLAN_STATUS_SUCCESS;
    enum wifi_event_reason result = WIFI_EVENT_REASON_FAILURE;
    t_u8 *sta_addr;

    t_u16 command = (resp->command & HostCmd_CMD_ID_MASK);
#ifdef RW610
    last_resp_rcvd = command;
#endif

#if !CONFIG_WIFI_PS_DEBUG
    if ((resp->command & 0x0fffU) != HostCmd_CMD_802_11_PS_MODE_ENH)
    {
        wcmdr_d("CMD_RESP - : 0x%x, result %d, len %d, seqno 0x%x", resp->command, resp->result, resp->size,
                resp->seq_num);
    }
#else
    wcmdr_d("CMD_RESP - : 0x%x, result %d, len %d, seqno 0x%x", resp->command, resp->result, resp->size, resp->seq_num);
#endif

#if CONFIG_FW_VDLL
    mlan_adap->vdll_in_progress = MFALSE;
#endif

    mlan_bss_type bss_type = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);

    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
#if UAP_SUPPORT
        pmpriv = (mlan_private *)mlan_adap->priv[1];
#else
        wcmdr_w("wifi_process_cmd_response receive UAP command response 0x%X when UAP not supported", resp->command);
        return -WM_FAIL;
#endif
    }
    else
    {
        pmpriv = (mlan_private *)mlan_adap->priv[0];
    }

    if (mlan_adap->ps_state == PS_STATE_SLEEP)
    {
        OSA_RWLockWriteUnlock(&sleep_rwlock);
        mlan_adap->ps_state = PS_STATE_AWAKE;
    }
#ifdef SD9177
    if (resp->result == HostCmd_RESULT_PRE_ASLEEP)
    {
        return WIFI_EVENT_CMD_BLOCK_PRE_ASLEEP;
    }
#endif
    /* Check if the command is a user issued host command */
    if (wm_wifi.hostcmd_cfg.is_hostcmd == true)
    {
        wifi_set_hostcmd_resp(resp);
        rv = MLAN_STATUS_SUCCESS;
    }
    else
    {
        switch (command)
        {
#if UAP_SUPPORT
            case HOST_CMD_SMART_MODE_CFG:
            {
#if CONFIG_WIFI_EXTRA_DEBUG
                HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)&resp->params.sys_config;

                if (sys_config_cmd->action == HostCmd_ACT_GEN_GET)
                    dump_hex(resp, resp->size);
#endif
            }
            break;
            case HOST_CMD_APCMD_BSS_STOP:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    pmpriv->uap_bss_started = MFALSE;
                    bss_type                = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);
                    if (bss_type == MLAN_BSS_TYPE_UAP)
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                        wlan_clean_txrx(pmpriv);
                        (void)wifi_event_completion(WIFI_EVENT_UAP_STOPPED, WIFI_EVENT_REASON_SUCCESS, NULL);
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCMD_APCMD_ACS_SCAN:
#ifdef SD8801
            case HostCmd_MMH_ACS_CFG:
#endif
            {
#ifdef SD8801
                HostCmd_DS_ACS_CONFIG *acs_scan = (HostCmd_DS_ACS_CONFIG *)&resp->params.acs_scan;
#else
                HostCMD_DS_APCMD_ACS_SCAN *acs_scan = (HostCMD_DS_APCMD_ACS_SCAN *)&resp->params.acs_scan;
#endif
                wm_wifi.acs_chan = 0;

                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.acs_chan = acs_scan->chan;

                    bss_type = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);
                    if (bss_type == MLAN_BSS_TYPE_UAP)
                    {
#if CONFIG_WPA_SUPP
                        nxp_wifi_acs_params acs_params;
#ifndef SD8801
                        t_u8 chan_offset;
#endif

                        wm_wifi.cmd_resp_status = WM_SUCCESS;
#ifndef SD8801
                        wifi_d("ACS scan done: bandcfg=%x, channel=%d\r\n", acs_scan->bandcfg, acs_scan->chan);
#else
                        wifi_d("ACS scan done: bandcfg=0, channel=%d\r\n", acs_scan->chan);
#endif

                        memset(&acs_params, 0, sizeof(nxp_wifi_acs_params));

#if defined(SD8801) || defined(RW610)
#ifdef SD8801
                        acs_params.pri_freq = channel_to_frequency(acs_scan->chan, 0);
                        acs_params.hw_mode  = 1;
#else
                        acs_params.pri_freq = channel_to_frequency(acs_scan->chan, acs_scan->bandcfg.chanBand);
                        acs_params.hw_mode  = acs_scan->bandcfg.chanBand == 0 ? 1 : 2;
#endif
                        acs_params.ch_width = 20;
#else
                        acs_params.pri_freq = channel_to_frequency(acs_scan->chan, acs_scan->bandcfg.chanBand);

                        chan_offset = wifi_get_sec_channel_offset(acs_scan->chan);
                        if (chan_offset == SEC_CHAN_ABOVE)
                        {
                            acs_params.sec_freq = acs_params.pri_freq + 20;
                        }
                        else if (chan_offset == SEC_CHAN_BELOW)
                        {
                            acs_params.sec_freq = acs_params.pri_freq - 20;
                        }
                        else
                        {
                            acs_params.sec_freq = acs_params.pri_freq;
                        }

#if CONFIG_11AC
                        if (wm_wifi.bandwidth == BANDWIDTH_80MHZ)
                        {
#if CONFIG_5GHz_SUPPORT
                            if (acs_scan->chan > MAX_CHANNELS_BG)
                            {
                                acs_params.ch_width = 80;
                            }
#endif
                        }
                        else
#endif
                            if (wm_wifi.bandwidth == BANDWIDTH_40MHZ)
                        {
                            acs_params.ch_width = 40;
                        }
                        else
                        {
                            acs_params.sec_freq = 0;
                            acs_params.ch_width = 20;
                            wifi_uap_set_bandwidth(BANDWIDTH_20MHZ);
#if CONFIG_WPA_SUPP
#if CONFIG_HOSTAPD
                            wifi_event_completion(WIFI_EVENT_ACS_COMPLETE, WIFI_EVENT_REASON_SUCCESS, NULL);
#endif
#endif
                        }
                        if (acs_scan->chan == 165)
                        {
                            acs_params.sec_freq = 0;
                            acs_params.ch_width = 20;
                            wifi_uap_set_bandwidth(BANDWIDTH_20MHZ);
#if CONFIG_WPA_SUPP
#if CONFIG_HOSTAPD
                            wifi_event_completion(WIFI_EVENT_ACS_COMPLETE, WIFI_EVENT_REASON_SUCCESS, NULL);
#endif
#endif
                        }
                        acs_params.hw_mode = acs_scan->bandcfg.chanBand == 0 ? 1 : 2;
#endif
                        if (wm_wifi.supp_if_callbk_fns->acs_channel_sel_callbk_fn)
                        {
                            wm_wifi.supp_if_callbk_fns->acs_channel_sel_callbk_fn(wm_wifi.hapd_if_priv, &acs_params);
                        }
#endif
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HOST_CMD_APCMD_BSS_START:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    bss_type = (mlan_bss_type)HostCmd_GET_BSS_TYPE(resp->seq_num);
                    if (bss_type == MLAN_BSS_TYPE_UAP)
                    {
#if CONFIG_WMM_UAPSD
                        /* disable uapsd for uAP */
                        mlan_adap->pps_uapsd_mode = MFALSE;
                        mlan_adap->tx_lock_flag   = MFALSE;
#endif
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                        (void)wifi_event_completion(WIFI_EVENT_UAP_STARTED, WIFI_EVENT_REASON_SUCCESS, NULL);
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HOST_CMD_APCMD_STA_LIST:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    const HostCmd_DS_STA_LIST *sta_list = &resp->params.sta_list;
                    load_bss_list(sta_list);
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif /* UAP_SUPPORT */
            case HostCmd_CMD_802_11_TX_RATE_QUERY:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
#if UAP_SUPPORT
                    int bss_type = HostCmd_GET_BSS_TYPE(resp->seq_num);
                    if (bss_type == MLAN_BSS_TYPE_UAP)
                        pmpriv = (mlan_private *)mlan_adap->priv[1];
#endif
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_ds_rate *ds_rate = (wifi_ds_rate *)wm_wifi.cmd_resp_priv;
                        rv                    = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, ds_rate);
                        if (rv != MLAN_STATUS_SUCCESS)
                        {
                            wm_wifi.cmd_resp_status = -WM_FAIL;
                        }
                        else
                        {
                            wm_wifi.cmd_resp_status = WM_SUCCESS;
                        }
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_TBTT_OFFSET:
            {
                HostCmd_DS_TBTT_OFFSET *tbtt_offset;
                tbtt_offset = (HostCmd_DS_TBTT_OFFSET *)&resp->params.tbtt_offset;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (tbtt_offset->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_tbtt_offset_t *tbtt_offset_t = (wifi_tbtt_offset_t *)wm_wifi.cmd_resp_priv;

                            tbtt_offset_t->min_tbtt_offset = tbtt_offset->min_tbtt_offset;
                            tbtt_offset_t->max_tbtt_offset = tbtt_offset->max_tbtt_offset;
                            tbtt_offset_t->avg_tbtt_offset = tbtt_offset->avg_tbtt_offset;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_MEF_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_802_11_RF_TX_POWER:
            {
                const HostCmd_DS_802_11_RF_TX_POWER *txp = &resp->params.txp;
                if (txp->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_tx_power_t *tx_p = (wifi_tx_power_t *)wm_wifi.cmd_resp_priv;
                        tx_p->current_level   = txp->current_level;
                        tx_p->max_power       = txp->max_power;
                        tx_p->min_power       = txp->min_power;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_802_11D_DOMAIN_INFO:
            {
                HostCmd_DS_802_11D_DOMAIN_INFO *domain_info =
                    (HostCmd_DS_802_11D_DOMAIN_INFO *)&resp->params.domain_info;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else if (domain_info->action == HostCmd_ACT_SPC_SET)
                {
                    /*FW not supported yet, always set command response status success for action code
                     * HostCmd_ACT_SPC_SET*/
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_GET_HW_SPEC:
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }

                (void)wifi_event_completion(WIFI_EVENT_GET_HW_SPEC, WIFI_EVENT_REASON_SUCCESS, NULL);
                break;
            case HostCmd_CMD_802_11_SCAN:
                if (resp->result != HostCmd_RESULT_OK)
                {
                    if (resp->result == HostCmd_RESULT_BUSY)
                    {
                        wifi_w("ED MAC: Scan temporary aborted, due to noisy environment");
                    }
                    else
                    {
                        wifi_w("Scan temporary failure");
                    }
                    /*
                     * Abort the split scan. The firmware has returned
                     * scan failure.
                     */
                    wlan_abort_split_scan();
                    wifi_user_scan_config_cleanup();

                    (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_FAILURE, NULL);
                    break;
                }

                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    wifi_user_scan_config_cleanup();
                    return -WM_FAIL;
                }

                if (is_split_scan_complete())
                {
                    wifi_d("Split scan complete");
                    wifi_user_scan_config_cleanup();

                    (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_SUCCESS, NULL);
                }
                break;
#if CONFIG_EXT_SCAN_SUPPORT
            case HostCmd_CMD_802_11_SCAN_EXT:
                if (resp->result != 0)
                {
                    wifi_w("Scan temporary failure");
                    /*
                     * Abort the split scan. The firmware has returned
                     * scan failure.
                     */
                    wlan_abort_split_scan();
                    wifi_user_scan_config_cleanup();

                    (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_FAILURE, NULL);
                }
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }
                break;
#endif
            case HostCmd_CMD_802_11_DEAUTHENTICATE:
                (void)wlan_ret_802_11_deauthenticate(pmpriv, resp, NULL);
#if !CONFIG_WPA_SUPP
                (void)wifi_event_completion(WIFI_EVENT_DEAUTHENTICATION, WIFI_EVENT_REASON_SUCCESS, NULL);
#endif
                break;
            case HostCmd_CMD_802_11_HS_CFG_ENH:
#if CONFIG_HOST_SLEEP
                wifi_process_hs_cfg_resp((t_u8 *)resp);
#endif
                break;
            case HostCmd_CMD_802_11_PS_MODE_ENH:
            {
#if !CONFIG_MEM_POOLS
                t_u16 *ps_action_p = (t_u16 *)OSA_MemoryAllocate(sizeof(t_u16));
#else
                t_u16 *ps_action_p = (t_u16 *)OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
                if (ps_action_p != NULL)
                {
                    result = wifi_process_ps_enh_response((t_u8 *)resp, &ps_event, ps_action_p);

                    if (ps_event != (t_u16)WIFI_EVENT_PS_INVALID)
                    {
#if CONFIG_WNM_PS
                        if (ps_event == WIFI_EVENT_WNM_PS)
                        {
                            if (wifi_event_completion((enum wifi_event)ps_event, result,
                                                      (void *)((t_u32)ps_action_p)) != WM_SUCCESS)
                            {
#if !CONFIG_MEM_POOLS
                                OSA_MemoryFree((void *)ps_action_p);
#else
                                OSA_MemoryPoolFree(buf_32_MemoryPool, ps_action_p);
#endif
                            }
                        }
                        else
#endif
                        {
                            if (wifi_event_completion((enum wifi_event)ps_event, result, (void *)ps_action_p) !=
                                WM_SUCCESS)
                            {
#if !CONFIG_MEM_POOLS
                                OSA_MemoryFree((void *)ps_action_p);
#else
                                OSA_MemoryPoolFree(buf_32_MemoryPool, ps_action_p);
#endif
                            }
                            else
                            {
                                /*do nothing*/
                            }
                        }
                    }
                    else
                    {
#if !CONFIG_MEM_POOLS
                        OSA_MemoryFree((void *)ps_action_p);
#else
                        OSA_MemoryPoolFree(buf_32_MemoryPool, ps_action_p);
#endif
                    }
                }
            }
            break;
#if 0
            case HostCmd_CMD_SUPPLICANT_PMK:
                (void)wifi_event_completion(WIFI_EVENT_SUPPLICANT_PMK,
                        WIFI_EVENT_REASON_SUCCESS,
                        resp);
                break;
#endif
#if CONFIG_WMM_UAPSD
            case HostCmd_CMD_802_11_SLEEP_PERIOD:
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_priv);
                if (rv != MLAN_STATUS_SUCCESS)
                    return -WM_FAIL;
                break;
#endif
            case HostCmd_CMD_802_11_RF_CHANNEL:
            {
                const HostCmd_DS_802_11_RF_CHANNEL *ch = &resp->params.rf_channel;
                if (ch->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_rf_channel_t *rf_c = (wifi_rf_channel_t *)wm_wifi.cmd_resp_priv;
                        rf_c->current_channel   = ch->current_channel;
                        rf_c->rf_type           = ch->rf_type;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
                /* Is this required */
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
            }
            break;
#ifdef SD8801
            case HostCmd_CMD_ROBUST_COEX:
            {
                const HostCmd_DS_ExtBLECoex_Config_t *coex = &resp->params.ext_ble_coex_cfg;
                if (coex->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_ext_coex_stats_t *stats = (wifi_ext_coex_stats_t *)wm_wifi.cmd_resp_priv;
                        stats->ext_radio_req_count   = coex->coex_cfg_data.EXT_RADIO_REQ_count;
                        stats->ext_radio_pri_count   = coex->coex_cfg_data.EXT_RADIO_PRI_count;
                        stats->wlan_grant_count      = coex->coex_cfg_data.WLAN_GRANT_count;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
#endif
#if CONFIG_11N
            case HostCmd_CMD_11N_CFG:
                rv = wlan_ret_11n_cfg(pmpriv, resp, NULL);
                break;
            case HostCmd_CMD_11N_ADDBA_REQ:
                (void)do_wlan_ret_11n_addba_req(pmpriv, resp);
                break;
            case HostCmd_CMD_11N_DELBA:
                (void)do_wlan_ret_11n_delba(pmpriv, resp);
                break;
            case HostCmd_CMD_11N_ADDBA_RSP:
                (void)do_wlan_ret_11n_addba_resp(resp);
                break;
#if CONFIG_AMSDU_IN_AMPDU
            case HostCmd_CMD_AMSDU_AGGR_CTRL:
                rv = wlan_ret_amsdu_aggr_ctrl(pmpriv, resp, NULL);
                break;
#endif
#endif /* CONFIG_11N */
            case HostCmd_CMD_MAC_MULTICAST_ADR:
                break;
            case HostCmd_CMD_802_11_ASSOCIATE:
            {
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
#if !CONFIG_WPA_SUPP
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    (void)wifi_event_completion(WIFI_EVENT_ASSOCIATION, WIFI_EVENT_REASON_FAILURE, NULL);
                    return -WM_FAIL;
                }
#endif

#if CONFIG_WPA_SUPP
                nxp_wifi_assoc_event_mlme_t *assoc_resp = &wm_wifi.assoc_resp;
                memset(assoc_resp, 0, sizeof(nxp_wifi_assoc_event_mlme_t));

                IEEEtypes_AssocRsp_t *passoc_rsp1 = (IEEEtypes_AssocRsp_t *)(void *)&resp->params;

                if (passoc_rsp1->status_code == WLAN_STATUS_UNSPECIFIED_FAILURE)
                {
                    result = WIFI_EVENT_REASON_FAILURE;
                    goto assoc_resp_ret;
                }

                IEEEtypes_AssocRsp_t *passoc_rsp =
                    (IEEEtypes_AssocRsp_t *)((t_u8 *)(&resp->params) + sizeof(IEEEtypes_MgmtHdr_t));

#if CONFIG_11R
                pmpriv->ft_roam = MFALSE;
#endif
                assoc_resp->frame.frame_len = resp->size - S_DS_GEN;
                if (assoc_resp->frame.frame_len > (int)sizeof(assoc_resp->frame.frame))
                {
                    wifi_e("Assocate response payload length (%d) overs the max length(%d), dropping it",
                           assoc_resp->frame.frame_len, sizeof(assoc_resp->frame.frame));
                    assoc_resp->frame.frame_len = 0;
                    result                      = WIFI_EVENT_REASON_FAILURE;
                    goto assoc_resp_ret;
                }
                // coverity[overrun-buffer-arg:SUPPRESS]
                memcpy(assoc_resp->frame.frame, passoc_rsp1, assoc_resp->frame.frame_len);

                if (pmpriv->assoc_req_size && (pmpriv->assoc_req_size <= (int)sizeof(assoc_resp->req_ie)))
                {
                    assoc_resp->req_ie_len = pmpriv->assoc_req_size;
                    // coverity[overrun-buffer-arg:SUPPRESS]
                    memcpy(assoc_resp->req_ie, pmpriv->assoc_req_buf, assoc_resp->req_ie_len);
                }
                if (wm_wifi.supp_if_callbk_fns->assoc_resp_callbk_fn)
                {
                    pmpriv->auth_flag = 0;

                    wm_wifi.supp_if_callbk_fns->assoc_resp_callbk_fn(wm_wifi.if_priv, assoc_resp,
                                                                     sizeof(nxp_wifi_assoc_event_mlme_t));
                }
#else
                IEEEtypes_AssocRsp_t *passoc_rsp = (IEEEtypes_AssocRsp_t *)(void *)&resp->params;
#endif
                if (passoc_rsp->status_code == 0U)
                {
                    result = WIFI_EVENT_REASON_SUCCESS;
                }
                else
                {
#if CONFIG_WIFI_EXTRA_DEBUG
                    wifi_w("Assoc failed: Status: %d\n\r", passoc_rsp->status_code);
                    wifi_show_assoc_fail_reason(passoc_rsp->status_code);
#endif
                    /* Since we have failed assoc attempt clear this */
                    pmpriv->media_connected = MFALSE;
                }
                /** Clear BSS blacklist if association is successful */
                if (result == WIFI_EVENT_REASON_SUCCESS)
                {
                    (void)memset(&pmpriv->adapter->blacklist_bss.bssids, 0x0,
                                 sizeof(pmpriv->adapter->blacklist_bss.bssids));
                    pmpriv->adapter->blacklist_bss.num_bssid = 0;
                }
#if CONFIG_WPA_SUPP
            assoc_resp_ret:
#endif
                (void)wifi_event_completion(WIFI_EVENT_ASSOCIATION, result, NULL);
            }
            break;
            case HostCmd_CMD_802_11_MAC_ADDRESS:
            {
                HostCmd_DS_802_11_MAC_ADDRESS *pmac_addr = &resp->params.mac_addr;
                int ret                                  = -WM_FAIL;
#if !CONFIG_MEM_POOLS
                sta_addr = OSA_MemoryAllocate(MLAN_MAC_ADDR_LENGTH);
#else
                sta_addr                         = OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
                if (sta_addr == MNULL)
                {
                    wifi_w("No mem. Cannot process MAC address command");
                    break;
                }
                (void)memcpy((void *)sta_addr, (const void *)((uint8_t *)&pmac_addr->mac_addr), MLAN_MAC_ADDR_LENGTH);
                if (bss_type == MLAN_BSS_TYPE_STA)
                {
                    (void)memcpy(dev_mac_addr, sta_addr, MLAN_MAC_ADDR_LENGTH);
                    ret = wifi_event_completion(WIFI_EVENT_STA_MAC_ADDR_CONFIG, WIFI_EVENT_REASON_SUCCESS, sta_addr);
                }
                else if (bss_type == MLAN_BSS_TYPE_UAP)
                {
                    (void)memcpy(dev_mac_addr_uap, sta_addr, MLAN_MAC_ADDR_LENGTH);
                    ret = wifi_event_completion(WIFI_EVENT_UAP_MAC_ADDR_CONFIG, WIFI_EVENT_REASON_SUCCESS, sta_addr);
                }

                if (ret != WM_SUCCESS)
                {
                    /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                    OSA_MemoryFree((void *)sta_addr);
#else
                    OSA_MemoryPoolFree(buf_32_MemoryPool, sta_addr);
#endif
                }
            }
            break;
            case HostCmd_CMD_802_11_KEY_MATERIAL:
                break;
            case HostCmd_CMD_MAC_REG_ACCESS:
            {
                HostCmd_DS_MAC_REG_ACCESS *reg;
                reg = (HostCmd_DS_MAC_REG_ACCESS *)&resp->params.mac_reg;
                if (reg->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *mac_reg = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *mac_reg          = reg->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_BBP_REG_ACCESS:
            {
                HostCmd_DS_BBP_REG_ACCESS *reg;
                reg = (HostCmd_DS_BBP_REG_ACCESS *)&resp->params.bbp_reg;
                if (reg->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *bbp_reg = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *bbp_reg          = reg->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_RF_REG_ACCESS:
            {
                HostCmd_DS_RF_REG_ACCESS *reg;
                reg = (HostCmd_DS_RF_REG_ACCESS *)&resp->params.rf_reg;
                if (reg->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *rf_reg = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *rf_reg          = reg->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_CAU_REG_ACCESS:
            {
                HostCmd_DS_RF_REG_ACCESS *reg;
                reg = (HostCmd_DS_RF_REG_ACCESS *)&resp->params.rf_reg;
                if (reg->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *cau_reg = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *cau_reg          = reg->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_802_11_EEPROM_ACCESS:
            {
                HostCmd_DS_802_11_EEPROM_ACCESS *eeprom;
                eeprom = (HostCmd_DS_802_11_EEPROM_ACCESS *)&resp->params.eeprom;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                    (void)memcpy((void *)buf, (const void *)&eeprom->value, eeprom->byte_count);
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_MEM_ACCESS:
            {
                HostCmd_DS_MEM_ACCESS *mem;
                mem = (HostCmd_DS_MEM_ACCESS *)&resp->params.mem;
                if (mem->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *mem_value = (uint32_t *)wm_wifi.cmd_resp_priv;
                        *mem_value          = mem->value;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_MGMT_IE_LIST:
            {
                HostCmd_DS_MGMT_IE_LIST_CFG *ie_list_cfg;
                ie_list_cfg = (HostCmd_DS_MGMT_IE_LIST_CFG *)&resp->params.mgmt_ie_list;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                    (void)memcpy((void *)buf, (const void *)(&ie_list_cfg->ds_mgmt_ie.ie_data_list[0]),
                                 ie_list_cfg->ds_mgmt_ie.len);
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_VERSION_EXT:
                load_ver_ext(resp);
                break;
            case HostCmd_CMD_802_11_RF_ANTENNA:
            {
                HostCmd_DS_802_11_RF_ANTENNA *rf_antenna_ctrl;
                rf_antenna_ctrl = (HostCmd_DS_802_11_RF_ANTENNA *)&resp->params.antenna;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (rf_antenna_ctrl->action == HostCmd_ACT_GET_BOTH)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_antcfg_t *antcfg      = (wifi_antcfg_t *)wm_wifi.cmd_resp_priv;
                            *(antcfg->ant_mode)        = rf_antenna_ctrl->antenna_mode;
                            *(antcfg->evaluate_time)   = rf_antenna_ctrl->evaluate_time;
                            *(antcfg->current_antenna) = rf_antenna_ctrl->current_antenna;
#ifdef RW610
                            *(antcfg->evaluate_mode) = rf_antenna_ctrl->evaluate_mode;
#endif
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_CW_MODE_CTRL:
            {
                HostCmd_DS_CW_MODE_CTRL *cw_mode_ctrl;
                cw_mode_ctrl = (HostCmd_DS_CW_MODE_CTRL *)&resp->params.cwmode;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (cw_mode_ctrl->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_cw_mode_ctrl_t *cwmode_ctrl = (wifi_cw_mode_ctrl_t *)wm_wifi.cmd_resp_priv;
                            cwmode_ctrl->mode                = cw_mode_ctrl->mode;
                            cwmode_ctrl->channel             = cw_mode_ctrl->channel;
                            cwmode_ctrl->chanInfo            = cw_mode_ctrl->chanInfo;
                            cwmode_ctrl->txPower             = wlan_cpu_to_le16(cw_mode_ctrl->txPower);
                            cwmode_ctrl->rateInfo            = wlan_cpu_to_le32(cw_mode_ctrl->rateInfo);
                            cwmode_ctrl->pktLength           = wlan_cpu_to_le16(cw_mode_ctrl->pktLength);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_RSSI_INFO:
            {
                HostCmd_DS_802_11_RSSI_INFO_RSP *prssi_info_rsp =
                    (HostCmd_DS_802_11_RSSI_INFO_RSP *)(void *)&resp->params;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    wifi_rssi_info_t *rssi_info = (wifi_rssi_info_t *)wm_wifi.cmd_resp_priv;
                    compute_rssi_values(prssi_info_rsp, rssi_info);
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case HostCmd_CMD_802_11_NET_MONITOR:
                wm_wifi.cmd_resp_status = WM_SUCCESS;
                break;
#if UAP_SUPPORT
            case HOST_CMD_APCMD_SYS_CONFIGURE:
                wifi_uap_handle_cmd_resp(resp);
                break;
#endif
            case HostCmd_CMD_TXPWR_CFG:
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_ioctl);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }
                break;
            case HostCmd_CMD_TX_RATE_CFG:
            {
                const HostCmd_DS_TX_RATE_CFG *tx_rate_cfg = &resp->params.tx_rate_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (tx_rate_cfg->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_ioctl != NULL)
                        {
                            wifi_ds_rate *ds_rate = (wifi_ds_rate *)wm_wifi.cmd_resp_ioctl;
                            rv                    = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, ds_rate);
                            if (rv != MLAN_STATUS_SUCCESS)
                            {
                                wm_wifi.cmd_resp_status = -WM_FAIL;
                            }
                            else
                            {
                                wm_wifi.cmd_resp_status = WM_SUCCESS;
                            }
                        }
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_802_11_SNMP_MIB:
#if CONFIG_TURBO_MODE
                if (resp->result == HostCmd_RESULT_OK)
                {
                    t_u8 *turbo_mode           = (t_u8 *)wm_wifi.cmd_resp_priv;
                    t_u8 *tlv                  = (t_u8 *)((t_u8 *)resp + S_DS_GEN);
                    turbo_mode_para *turbo_ptr = (turbo_mode_para *)tlv;
                    if (turbo_ptr->action == ACTION_GET)
                    {
                        (void)memcpy(turbo_mode, &turbo_ptr->mode, sizeof(t_u8));
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
#endif
                rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                if (rv != MLAN_STATUS_SUCCESS)
                {
                    return -WM_FAIL;
                }
                break;
            case HostCmd_CMD_OTP_READ_USER_DATA:
            {
                const HostCmd_DS_OTP_USER_DATA *user_data = &resp->params.otp_user_data;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (user_data->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                            (void)memcpy((void *)buf, (const void *)user_data->user_data, user_data->user_data_length);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_CFG_DATA:
            {
                const HostCmd_DS_802_11_CFG_DATA *cfg_data = &resp->params.cfg_data;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (cfg_data->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_cal_data_t *cal_data_cfg = (wifi_cal_data_t *)wm_wifi.cmd_resp_priv;
#if !CONFIG_MEM_POOLS
                            cal_data_cfg->data = (uint8_t *)OSA_MemoryAllocate(cfg_data->data_len);
#else
                            cal_data_cfg->data = (uint8_t *)OSA_MemoryPoolAllocate(buf_2048_MemoryPool);
#endif
                            if (cal_data_cfg->data == MNULL)
                            {
                                wifi_w(
                                    "No mem. Cannot"
                                    "process CAL DATA command");
                                break;
                            }
                            cal_data_cfg->data_len = cfg_data->data_len;

                            (void)memcpy((void *)cal_data_cfg->data, (const void *)cfg_data->data, cfg_data->data_len);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_AUTO_RECONNECT:
            {
                const HostCmd_DS_AUTO_RECONNECT *auto_reconn = &resp->params.auto_reconnect;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (auto_reconn->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_auto_reconnect_config_t *auto_reconnect_config =
                                (wifi_auto_reconnect_config_t *)wm_wifi.cmd_resp_priv;

                            auto_reconnect_config->reconnect_counter  = auto_reconn->reconnect_counter;
                            auto_reconnect_config->reconnect_interval = auto_reconn->reconnect_interval;
                            auto_reconnect_config->flags              = auto_reconn->flags;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_HS_WAKEUP_REASON:
            {
                const HostCmd_DS_HS_WAKEUP_REASON *hs_wakeup_reason = &resp->params.hs_wakeup_reason;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        t_u16 *wakeup_reason = (t_u16 *)wm_wifi.cmd_resp_priv;
                        *wakeup_reason       = hs_wakeup_reason->wakeup_reason;
                    }

                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_RX_MGMT_IND:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_CHANNEL_TRPC_CONFIG:
            {
                HostCmd_DS_CHAN_TRPC_CONFIG *chan_trpc_cfg = &resp->params.chan_trpc_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (chan_trpc_cfg->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            t_u8 i;
                            t_u8 mod_num = 0;
                            t_u8 *pByte  = NULL;
                            t_u16 left_len;
                            MrvlIETypes_ChanTRPCConfig_t *trpc_tlv = NULL;
                            MrvlIEtypes_Data_t *pTlvHdr;
                            wifi_txpwrlimit_t *txpwrlimit = (wifi_txpwrlimit_t *)wm_wifi.cmd_resp_priv;

                            txpwrlimit->subband = (wifi_SubBand_t)chan_trpc_cfg->reserved;

                            pByte    = (t_u8 *)chan_trpc_cfg + 4;
                            left_len = resp->size - (t_u16)S_DS_GEN - 4U;
                            while (left_len >= sizeof(pTlvHdr->header))
                            {
                                pTlvHdr             = (MrvlIEtypes_Data_t *)(void *)pByte;
                                pTlvHdr->header.len = wlan_le16_to_cpu(pTlvHdr->header.len);

                                switch (wlan_le16_to_cpu(pTlvHdr->header.type))
                                {
                                    case TLV_TYPE_CHAN_TRPC_CONFIG:
                                        trpc_tlv = (MrvlIETypes_ChanTRPCConfig_t *)(void *)pTlvHdr;
#if !CONFIG_11AC
                                        /*
                                         * For 2.4 GHz band, we do not support HT40 Modulation Groups.
                                         * Limit the number of mod groups to 7.
                                         */
                                        if (!txpwrlimit->subband)
                                        {
                                            mod_num = 7;
                                        }
                                        else
                                        {
#endif /* CONFIG_11AC */
                                            mod_num =
                                                ((t_u8)pTlvHdr->header.len - 4U) / ((t_u8)sizeof(mod_group_setting));
#if !CONFIG_11AC
                                        }
#endif /* CONFIG_11AC */
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].num_mod_grps = mod_num;
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].chan_desc.start_freq =
                                            trpc_tlv->start_freq;
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].chan_desc.chan_width =
                                            trpc_tlv->width;
                                        txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans].chan_desc.chan_num =
                                            trpc_tlv->chan_num;
                                        for (i = 0; i < mod_num; i++)
                                        {
                                            // coverity[overrun-local:SUPPRESS]
                                            txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans]
                                                .txpwrlimit_entry[i]
                                                .mod_group = trpc_tlv->mod_group[i].mod_group;
                                            txpwrlimit->txpwrlimit_config[txpwrlimit->num_chans]
                                                .txpwrlimit_entry[i]
                                                .tx_power = trpc_tlv->mod_group[i].power;
                                        }
                                        txpwrlimit->num_chans++;
                                        break;
                                    default:
                                        PRINTM(MINFO, "Unexpected host cmd response \n");
                                        break;
                                }
                                left_len -= (pTlvHdr->header.len + (t_u16)sizeof(pTlvHdr->header));
                                pByte += (t_u8)pTlvHdr->header.len + (t_u8)sizeof(pTlvHdr->header);
                            }
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_802_11_GET_LOG:
            {
                const HostCmd_DS_802_11_GET_LOG *get_log = &resp->params.get_log;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint8_t *buf = (uint8_t *)wm_wifi.cmd_resp_priv;
                        (void)memcpy((void *)buf, (const void *)get_log, sizeof(HostCmd_DS_802_11_GET_LOG));
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_RECONFIGURE_TX_BUFF:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
                break;
#if UAP_SUPPORT
            case HostCmd_CMD_PMF_PARAMS:
            {
                const HostCmd_DS_PMF_PARAMS *get_pmf_params = &resp->params.pmf_params;
                if ((resp->result == HostCmd_RESULT_OK) && (get_pmf_params->action == HostCmd_ACT_GEN_GET))
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_pmf_params_t *wifi_pmf_params = (wifi_pmf_params_t *)wm_wifi.cmd_resp_priv;

                        wifi_pmf_params->mfpc = get_pmf_params->params.mfpc;
                        wifi_pmf_params->mfpr = get_pmf_params->params.mfpr;
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
#if CONFIG_RF_TEST_MODE
            case HostCmd_CMD_MFG_COMMAND:
            {
                if ((resp->result == HostCmd_RESULT_OK) && wm_wifi.cmd_resp_priv)
                {
                    rv = wlan_ret_mfg(pmpriv, resp, wm_wifi.cmd_resp_priv);
                    if (rv == MLAN_STATUS_SUCCESS)
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
            case HostCmd_CMD_GET_TSF:
            {
                const HostCmd_DS_TSF *tsf_pointer = (HostCmd_DS_TSF *)&resp->params.tsf_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        t_u64 *tsf = (t_u64 *)(wm_wifi.cmd_resp_priv);

                        *tsf = tsf_pointer->tsf;

                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
            case HostCmd_CMD_BOOT_SLEEP:
            {
                const HostCmd_DS_BOOT_SLEEP *boot_sleep_pointer = (HostCmd_DS_BOOT_SLEEP *)&resp->params.tsf_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        t_u16 *enable = (t_u16 *)(wm_wifi.cmd_resp_priv);

                        *enable = boot_sleep_pointer->enable;

                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#if CONFIG_WIFI_CLOCKSYNC
            case HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_ioctl);
                    if (rv != MLAN_STATUS_SUCCESS)
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
#endif
            case HostCmd_CMD_AUTO_TX:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
#if CONFIG_CLOUD_KEEP_ALIVE
                    const HostCmd_DS_AUTO_TX *auto_tx              = (HostCmd_DS_AUTO_TX *)&resp->params.auto_tx;
                    MrvlIEtypes_Cloud_Keep_Alive_t *keep_alive_tlv = MNULL;
                    t_u8 *enable;
                    MrvlIEtypesHeader_t *header = (MrvlIEtypesHeader_t *)auto_tx->tlv_buffer;
                    header->type                = wlan_le16_to_cpu(header->type);

                    if ((auto_tx->action == HostCmd_ACT_GEN_GET) && (header->type == TLV_TYPE_CLOUD_KEEP_ALIVE) &&
                        (wm_wifi.cmd_resp_priv != NULL))
                    {
                        keep_alive_tlv        = (MrvlIEtypes_Cloud_Keep_Alive_t *)auto_tx->tlv_buffer;
                        enable                = (t_u8 *)wm_wifi.cmd_resp_priv;
                        *enable               = keep_alive_tlv->enable;
                        wm_wifi.cmd_resp_priv = NULL;
                    }
#endif
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#ifdef OTP_CHANINFO
            case HostCmd_CMD_CHAN_REGION_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                    if (rv != MLAN_STATUS_SUCCESS)
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
#if CONFIG_11AX
            case HostCmd_CMD_11AX_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        mlan_ds_11ax_he_cfg *cfg = (mlan_ds_11ax_he_cfg *)wm_wifi.cmd_resp_priv;
                        rv                       = wlan_ret_11ax_cfg(pmpriv, resp, cfg);
                        if (rv != MLAN_STATUS_SUCCESS)
                            wm_wifi.cmd_resp_status = -WM_FAIL;
                        else
                            wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#if CONFIG_11AX_TWT
            case HostCmd_CMD_TWT_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        if (resp->params.twtcfg.sub_id == MLAN_11AX_TWT_SETUP_SUBID)
                        {
                            mlan_ds_twt_setup *cfg = (mlan_ds_twt_setup *)wm_wifi.cmd_resp_priv;
                            (void)memcpy(cfg, &resp->params.twtcfg.param.twt_setup, sizeof(mlan_ds_twt_setup));
                        }
                        else if (resp->params.twtcfg.sub_id == MLAN_11AX_TWT_REPORT_SUBID)
                        {
                            mlan_ds_twt_report *cfg = (mlan_ds_twt_report *)wm_wifi.cmd_resp_priv;
                            (void)memcpy(cfg, &resp->params.twtcfg.param.twt_report, sizeof(mlan_ds_twt_report));
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif /* CONFIG_11AX_TWT */
            case HostCmd_CMD_11AX_CMD:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_ioctl);
                    if (rv != MLAN_STATUS_SUCCESS)
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    else
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#if CONFIG_MMSF
            case HostCmd_CMD_DBGS_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        wifi_mmsf_cfg_t *mmsf_cfg     = (wifi_mmsf_cfg_t *)wm_wifi.cmd_resp_priv;
                        HostCmd_DS_MMSF_CFG *MMSF_CFG = (HostCmd_DS_MMSF_CFG *)&resp->params.mmsf_cfg;
                        if (MMSF_CFG->action == HostCmd_ACT_GEN_GET)
                        {
                            (void)memcpy(mmsf_cfg->enable, &MMSF_CFG->enableMMSF, sizeof(MMSF_CFG->enableMMSF));
                            (void)memcpy(mmsf_cfg->Density, &MMSF_CFG->ampduDensity, sizeof(MMSF_CFG->ampduDensity));
                            (void)memcpy(mmsf_cfg->MMSF, &MMSF_CFG->ampduMMSF, sizeof(MMSF_CFG->ampduMMSF));
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
#endif
#endif
#if (CONFIG_WIFI_TX_PER_TRACK) || (CONFIG_TX_RX_HISTOGRAM)
            case HostCmd_CMD_TX_RX_PKT_STATS:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
#if CONFIG_TX_RX_HISTOGRAM
                    const HostCmd_DS_TX_RX_HISTOGRAM *txrx_histogram = &resp->params.histogram;
                    if (txrx_histogram->action != HostCmd_ACT_SET_TX_PER_TRACKING)
                    {
                        t_u16 cmdsize   = wlan_le16_to_cpu(resp->size);
                        t_u16 length    = 0;
                        t_u16 data_size = 0;
                        t_u8 *pos       = NULL;

                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            t_u8 *tx_rx_histogram_data = wm_wifi.cmd_resp_priv;
                            (void)memcpy(&data_size, tx_rx_histogram_data, sizeof(data_size));
                            length = cmdsize - S_DS_GEN - sizeof(HostCmd_DS_TX_RX_HISTOGRAM);

                            if (length > 0 && (data_size >= length + sizeof(length)))
                            {
                                (void)memcpy(tx_rx_histogram_data, (t_u8 *)&length, sizeof(length));
                                pos = (t_u8 *)resp + S_DS_GEN + sizeof(HostCmd_DS_TX_RX_HISTOGRAM);
                                (void)memcpy(tx_rx_histogram_data + sizeof(length), pos, length);
                            }
                            else
                            {
                                wifi_w("TX RX histogram data error\n");
                            }
                        }
                    }
#endif
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
#if CONFIG_BG_SCAN
            case HostCmd_CMD_802_11_BG_SCAN_CONFIG:
            {
                HostCmd_DS_802_11_BG_SCAN_CONFIG *bg_scan = &resp->params.bg_scan_config;
                /* TODO: command response handler for GET command */
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (bg_scan->enable)
                        pmpriv->roaming_configured = MTRUE;
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
            case HostCmd_CMD_802_11_BG_SCAN_QUERY:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, NULL);
                    if (rv != MLAN_STATUS_SUCCESS)
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    else
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                        wifi_d("BG scan query complete");

                        (void)wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_SUCCESS, NULL);
                    }
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
#endif
            case HostCmd_CMD_802_11_SUBSCRIBE_EVENT:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_priv);
                    if (rv != MLAN_STATUS_SUCCESS)
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    else
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
#if CONFIG_WIFI_EU_CRYPTO
            case HostCmd_CMD_EU_CRYPTO:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        EU_Crypto *EU_Data = (EU_Crypto *)wm_wifi.cmd_resp_priv;
                        HostCmd_DS_EU_AES_CRYPTO *cmd_aes_ccmp =
                            (HostCmd_DS_EU_AES_CRYPTO *)&resp->params.eu_aes_crypto;
                        HostCmd_DS_EU_CRYPTO *cmd_eu_crypto = (HostCmd_DS_EU_CRYPTO *)&resp->params.eu_crypto;
                        if (cmd_eu_crypto->Algorithm == CRYPTO_RC4 || cmd_eu_crypto->Algorithm == CRYPTO_AES_ECB ||
                            cmd_eu_crypto->Algorithm == CRYPTO_AES_WRAP)
                        {
                            memcpy(EU_Data->DataLength, &cmd_eu_crypto->DataLength, sizeof(t_u16) / sizeof(t_u8));
                            memcpy(EU_Data->Data, cmd_eu_crypto->Data, cmd_eu_crypto->DataLength);
                        }
                        else if (cmd_aes_ccmp->Algorithm == CRYPTO_AES_CCMP ||
                                 cmd_aes_ccmp->Algorithm == CRYPTO_AES_GCMP)
                        {
                            memcpy(EU_Data->DataLength, &cmd_aes_ccmp->DataLength, sizeof(t_u16) / sizeof(t_u8));
                            memcpy(EU_Data->Data, cmd_aes_ccmp->Data, cmd_aes_ccmp->DataLength);
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#endif
#if CONFIG_MULTI_CHAN
            case HostCmd_CMD_MULTI_CHAN_CONFIG:
            case HostCmd_CMD_MULTI_CHAN_POLICY:
            case HostCmd_CMD_DRCS_CONFIG:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_ioctl);
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    rv                      = MLAN_STATUS_FAILURE;
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
                break;
#endif
#if CONFIG_WPA_SUPP
            case HostCmd_CMD_802_11_REMAIN_ON_CHANNEL:
            {
                const HostCmd_DS_REMAIN_ON_CHANNEL *remain_channel = &resp->params.remain_on_chan;
#if !CONFIG_MEM_POOLS
                t_u8 *cancel_channel = (t_u8 *)OSA_MemoryAllocate(sizeof(t_u8));
#else
                t_u8 *cancel_channel = (t_u8 *)OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
                if (cancel_channel != NULL)
                {
                    *cancel_channel              = remain_channel->action == HostCmd_ACT_GEN_REMOVE ? MTRUE : MFALSE;
                    mlan_adap->remain_on_channel = remain_channel->action == HostCmd_ACT_GEN_REMOVE ? MFALSE : MTRUE;
                    if (*cancel_channel)
                    {
                        if (wifi_event_completion(WIFI_EVENT_REMAIN_ON_CHANNEL, WIFI_EVENT_REASON_SUCCESS,
                                                  (void *)cancel_channel) != WM_SUCCESS)
                        {
#if !CONFIG_MEM_POOLS
                            OSA_MemoryFree(cancel_channel);
#else
                            OSA_MemoryPoolFree(buf_32_MemoryPool, cancel_channel);
#endif
                            cancel_channel = NULL;
                        }
                    }
                    else
                    {
                        if (wifi_event_completion(WIFI_EVENT_REMAIN_ON_CHANNEL, WIFI_EVENT_REASON_SUCCESS,
                                                  (void *)cancel_channel) != WM_SUCCESS)
                        {
#if !CONFIG_MEM_POOLS
                            OSA_MemoryFree(cancel_channel);
#else
                            OSA_MemoryPoolFree(buf_32_MemoryPool, cancel_channel);
#endif
                            cancel_channel = NULL;
                        }
                    }
                }
            }
            break;
#endif

#if CONFIG_RX_ABORT_CFG
            case HostCmd_CMD_RX_ABORT_CFG:
            {
                HostCmd_DS_RX_ABORT_CFG *cfg;
                cfg = (HostCmd_DS_RX_ABORT_CFG *)&resp->params.rx_abort_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (cfg->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            rx_abort_cfg_t *rx_abort_cfg = (rx_abort_cfg_t *)wm_wifi.cmd_resp_priv;
                            rx_abort_cfg->enable         = cfg->enable;
                            rx_abort_cfg->rssi_threshold = (int)cfg->rssi_threshold;
                            if (rx_abort_cfg->rssi_threshold > 0x7f)
                            {
                                rx_abort_cfg->rssi_threshold = -(256 - rx_abort_cfg->rssi_threshold);
                            }
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
#endif
#if CONFIG_RX_ABORT_CFG_EXT
            case HostCmd_CMD_RX_ABORT_CFG_EXT:
            {
                HostCmd_DS_RX_ABORT_CFG_EXT *cfg;
                cfg = (HostCmd_DS_RX_ABORT_CFG_EXT *)&resp->params.rx_abort_cfg_ext;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (cfg->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            rx_abort_cfg_ext_t *rx_abort_cfg_ext   = (rx_abort_cfg_ext_t *)wm_wifi.cmd_resp_priv;
                            rx_abort_cfg_ext->enable               = cfg->enable;
                            rx_abort_cfg_ext->rssi_margin          = (int)cfg->rssi_margin;
                            rx_abort_cfg_ext->ceil_rssi_threshold  = (int)cfg->ceil_rssi_threshold;
                            rx_abort_cfg_ext->floor_rssi_threshold = (int)cfg->floor_rssi_threshold;
                            rx_abort_cfg_ext->current_dynamic_rssi_threshold = (int)cfg->current_dynamic_rssi_threshold;
                            rx_abort_cfg_ext->rssi_default_config            = (int)cfg->rssi_default_config;
                            rx_abort_cfg_ext->edmac_enable                   = (int)cfg->edmac_enable;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
#endif
#if CONFIG_CCK_DESENSE_CFG
            case HostCmd_CMD_CCK_DESENSE_CFG:
            {
                HostCmd_DS_CCK_DESENSE_CFG *cfg_cmd;
                cfg_cmd = (HostCmd_DS_CCK_DESENSE_CFG *)&resp->params.cck_desense_cfg;
                if (resp->result == HostCmd_RESULT_OK)
                {
                    if (cfg_cmd->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            cck_desense_cfg_t *cck_desense_cfg = (cck_desense_cfg_t *)wm_wifi.cmd_resp_priv;
                            cck_desense_cfg->mode              = wlan_le16_to_cpu(cfg_cmd->mode);
                            cck_desense_cfg->margin            = (int)cfg_cmd->margin;
                            if (cck_desense_cfg->margin > 0x7f)
                            {
                                cck_desense_cfg->margin = -(256 - cck_desense_cfg->margin);
                            }
                            cck_desense_cfg->ceil_thresh = (int)cfg_cmd->ceil_thresh;
                            if (cck_desense_cfg->ceil_thresh > 0x7f)
                            {
                                cck_desense_cfg->ceil_thresh = -(256 - cck_desense_cfg->ceil_thresh);
                            }
                            cck_desense_cfg->num_on_intervals  = (int)cfg_cmd->num_on_intervals;
                            cck_desense_cfg->num_off_intervals = (int)cfg_cmd->num_off_intervals;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
#endif

#if CONFIG_TX_AMPDU_PROT_MODE
            case HostCmd_CMD_TX_AMPDU_PROT_MODE:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    const HostCmd_DS_CMD_TX_AMPDU_PROT_MODE *data = &resp->params.tx_ampdu_prot_mode;
                    if (data->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            tx_ampdu_prot_mode_para *prot_mode = (tx_ampdu_prot_mode_para *)wm_wifi.cmd_resp_priv;
                            prot_mode->mode                    = data->mode;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                break;
#endif
#if CONFIG_TSP
            case HostCmd_CMD_TSP_CFG:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    TSP_CFG *tsp_get_cfg     = (TSP_CFG *)wm_wifi.cmd_resp_priv;
                    HostCmd_DS_TSP_CFG *data = &resp->params.tsp_cfg;
                    if (data->action == HostCmd_ACT_GEN_GET)
                    {
                        *(tsp_get_cfg->thermalPowerMgmtenable) = data->thermalPowerMgmtenable;
                        *(tsp_get_cfg->powerMgmtBackoff)       = data->powerMgmtBackoff;
                        *(tsp_get_cfg->lowPwrBOThrshld)        = data->lowPwrBOThrshld;
                        *(tsp_get_cfg->highPwrBOThrshld)       = data->highPwrBOThrshld;
                        *(tsp_get_cfg->dutycycstep)            = data->dutycycstep;
                        *(tsp_get_cfg->dutycycmin)             = data->dutycycmin;
                        *(tsp_get_cfg->highthrtemp)            = data->highthrtemp;
                        *(tsp_get_cfg->lowthrtemp)             = data->lowthrtemp;
                        *(tsp_get_cfg->currCAUTemp)            = data->currCAUTemp;
                        *(tsp_get_cfg->currRFUTemp)            = data->currRFUTemp;
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                break;
#endif
#if (CONFIG_11MC) || (CONFIG_11AZ)
            case HostCmd_CMD_FTM_SESSION_CFG:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                    wifi_ftm_process_cfg_resp(resp);
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                break;
            case HostCmd_CMD_FTM_SESSION_CTRL:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                    wifi_ftm_process_ctrl_resp(resp);
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                break;
#endif
#if (CONFIG_COEX_DUTY_CYCLE) || (CONFIG_EXTERNAL_COEX_PTA) || (CONFIG_IMD3_CFG)
            case HostCmd_CMD_ROBUST_COEX:
                if (resp->result == HostCmd_RESULT_OK)
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                break;
#endif
#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
            case HostCmd_CMD_INDEPENDENT_RESET_CFG:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    rv = wlan_ops_sta_process_cmdresp(pmpriv, command, resp, wm_wifi.cmd_resp_ioctl);
                    if (rv != MLAN_STATUS_SUCCESS)
                    {
                        wm_wifi.cmd_resp_status = -WM_FAIL;
                    }
                    else
                    {
                        wm_wifi.cmd_resp_status = WM_SUCCESS;
                    }
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
            }
            break;
#endif
#if CONFIG_INACTIVITY_TIMEOUT_EXT
            case HostCmd_CMD_INACTIVITY_TIMEOUT_EXT:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    const HostCmd_DS_INACTIVITY_TIMEOUT_EXT *data = &resp->params.inactivity_to;
                    if (data->action == HostCmd_ACT_GEN_GET)
                    {
                        if (wm_wifi.cmd_resp_priv != NULL)
                        {
                            wifi_inactivity_to_t *inac_to = (wifi_inactivity_to_t *)wm_wifi.cmd_resp_priv;
                            inac_to->timeout_unit         = data->timeout_unit;
                            inac_to->unicast_timeout      = data->unicast_timeout;
                            inac_to->mcast_timeout        = data->mcast_timeout;
                            inac_to->ps_entry_timeout     = data->ps_entry_timeout;
                            inac_to->ps_cmd_timeout       = data->ps_cmd_timeout;
                        }
                    }
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                break;
#endif
            case HostCmd_CMD_802_11_TX_FRAME:
            {
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
            }
            break;
#if UAP_SUPPORT
            case HostCmd_CMD_ADD_NEW_STATION:
                if (resp->result == HostCmd_RESULT_OK)
                {
                    wm_wifi.cmd_resp_status = WM_SUCCESS;
                }
                else
                {
                    wm_wifi.cmd_resp_status = -WM_FAIL;
                }
                break;
#endif
            default:
                /* fixme: Currently handled by the legacy code. Change this
                   handling later. Also check the default return value then*/
                break;
        }
    }

    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
/**
 *  @brief This function will search for the specific ie
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param ie_buf  A pointer to ie_buf
 *  @param ie_len  total ie length
 *  @param id      ie's id
 *
 *  @return	       ie's poiner or MNULL
 */
t_u8 *wlan_get_specific_ie(pmlan_private priv, t_u8 *ie_buf, t_u8 ie_len, IEEEtypes_ElementId_e id, t_u8 ext_id)
{
    t_u8 bytes_left    = ie_len;
    t_u8 *pcurrent_ptr = ie_buf;
    t_u8 total_ie_len;
    t_u8 *ie_ptr = MNULL;
    IEEEtypes_ElementId_e element_id;
    t_u8 element_len;
#if CONFIG_11AX
    t_u8 element_eid;
#endif
    ENTER();

    DBG_HEXDUMP(MCMD_D, "ie", ie_buf, ie_len);
    while (bytes_left >= 2U)
    {
        element_id  = (IEEEtypes_ElementId_e)(*((t_u8 *)pcurrent_ptr));
        element_len = *((t_u8 *)pcurrent_ptr + 1);
#if CONFIG_11AX
        element_eid = *((t_u8 *)pcurrent_ptr + 2);
#endif
        total_ie_len = element_len + (t_u8)sizeof(IEEEtypes_Header_t);
        if (bytes_left < total_ie_len)
        {
            PRINTM(MERROR,
                   "InterpretIE: Error in processing IE, "
                   "bytes left < IE length\n");
            break;
        }
        if (
#if CONFIG_11AX
            ((ext_id == 0U) &&
#endif
             element_id == id)
#if CONFIG_11AX

            || (id == EXTENSION && element_id == id && ext_id == element_eid))
#endif
        {
#if CONFIG_11AX
            PRINTM(MCMND, "Find IE: id=%d ext_id=%d\n", id, ext_id);
#else
            PRINTM(MCMND, "Find IE: id=%d\n", id);
#endif
            DBG_HEXDUMP(MCMND, "IE", pcurrent_ptr, total_ie_len);
            ie_ptr = pcurrent_ptr;
            break;
        }
        pcurrent_ptr += element_len + 2U;
        /* Need to account for IE ID and IE Len */
        bytes_left -= (element_len + 2U);
    }

    LEAVE();

    return ie_ptr;
}

/**
 *  @brief This function will search for the specific ie
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *  @param sta_ptr A pointer to sta_node
 *
 *  @return	       N/A
 */
static void wrapper_wlan_check_sta_capability(pmlan_private priv, Event_Ext_t *pevent, sta_node *sta_ptr)
{
    t_u16 tlv_type, tlv_len;
    t_u16 frame_control, frame_sub_type = 0;
    t_u8 *assoc_req_ie = MNULL;
    t_u8 ie_len = 0, assoc_ie_len = 0;
    IEEEtypes_HTCap_t *pht_cap = MNULL;
#if CONFIG_11AC
    IEEEtypes_VHTCap_t *pvht_cap = MNULL;
#endif
#if CONFIG_11AX
    IEEEtypes_Extension_t *phe_cap = MNULL;
#endif
    int tlv_buf_left         = (int)pevent->length - (int)INTF_HEADER_LEN - (int)ASSOC_EVENT_FIX_SIZE;
    MrvlIEtypesHeader_t *tlv = (MrvlIEtypesHeader_t *)(void *)((char *)pevent + INTF_HEADER_LEN + ASSOC_EVENT_FIX_SIZE);
    MrvlIETypes_MgmtFrameSet_t *mgmt_tlv = MNULL;

    ENTER();
    while (tlv_buf_left >= (int)sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) > (unsigned int)tlv_buf_left)
        {
            PRINTM(MERROR, "wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len, tlv_buf_left);
            break;
        }
        if (tlv_type == TLV_TYPE_UAP_MGMT_FRAME)
        {
            mgmt_tlv = (MrvlIETypes_MgmtFrameSet_t *)(void *)tlv;
            (void)memcpy((void *)&frame_control, (const void *)((t_u8 *)&(mgmt_tlv->frame_control)),
                         sizeof(frame_control));
            frame_sub_type = IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(frame_control);
            if ((mgmt_tlv->frame_control.type == 0U) &&
                ((frame_sub_type == SUBTYPE_ASSOC_REQUEST) || (frame_sub_type == SUBTYPE_REASSOC_REQUEST)))
            {
                if (frame_sub_type == SUBTYPE_ASSOC_REQUEST)
                {
                    assoc_ie_len = (t_u8)sizeof(IEEEtypes_AssocRqst_t);
                }
                else if (frame_sub_type == SUBTYPE_REASSOC_REQUEST)
                {
                    assoc_ie_len = (t_u8)sizeof(IEEEtypes_ReAssocRqst_t);
                }
                else
                { /* Do Nothing */
                }

                ie_len       = (t_u8)tlv_len - (t_u8)sizeof(IEEEtypes_FrameCtl_t) - assoc_ie_len;
                assoc_req_ie = (t_u8 *)tlv + sizeof(MrvlIETypes_MgmtFrameSet_t) + assoc_ie_len;
                pht_cap =
                    (IEEEtypes_HTCap_t *)(void *)wlan_get_specific_ie(priv, assoc_req_ie, ie_len, HT_CAPABILITY, 0);

                if (pht_cap != NULL)
                {
                    PRINTM(MCMND, "STA supports 11n\n");
                    sta_ptr->is_11n_enabled = MTRUE;
                    if (GETHT_MAXAMSDU(pht_cap->ht_cap.ht_cap_info) != 0U)
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                    }
                    else
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                    }
                }
                else
                {
                    PRINTM(MCMND,
                           "STA doesn't "
                           "support 11n\n");
                }
#if CONFIG_11AC
                pvht_cap =
                    (IEEEtypes_VHTCap_t *)(void *)wlan_get_specific_ie(priv, assoc_req_ie, ie_len, VHT_CAPABILITY, 0);
                if ((pvht_cap != MNULL) && (priv->is_11ac_enabled == MTRUE))
                {
                    PRINTM(MCMND, "STA supports 11ac\n");
                    sta_ptr->is_11ac_enabled = MTRUE;
                    if (GET_VHTCAP_MAXMPDULEN(wlan_le32_to_cpu(pvht_cap->vht_cap.vht_cap_info)) == 2U)
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_12K;
                    }
                    else if (GET_VHTCAP_MAXMPDULEN(wlan_le32_to_cpu(pvht_cap->vht_cap.vht_cap_info)) == 1U)
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                    }
                    else
                    {
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                    }
                }
                else
                {
                    PRINTM(MCMND, "STA doesn't support 11ac\n");
                }
#endif
#if CONFIG_11AX
                phe_cap =
                    (IEEEtypes_Extension_t *)wlan_get_specific_ie(priv, assoc_req_ie, ie_len, EXTENSION, HE_CAPABILITY);
                if (phe_cap && (priv->is_11ax_enabled == MTRUE))
                {
                    PRINTM(MCMND, "STA supports 11ax\n");
                    sta_ptr->is_11ax_enabled = MTRUE;
                    (void)memcpy((void *)((t_u8 *)&sta_ptr->he_cap), (const void *)phe_cap,
                                 phe_cap->ieee_hdr.len + sizeof(IEEEtypes_Header_t));
                    sta_ptr->he_cap.ieee_hdr.len =
                        MIN(phe_cap->ieee_hdr.len, sizeof(IEEEtypes_HECap_t) - sizeof(IEEEtypes_Header_t));
                }
                else
                    PRINTM(MCMND, "STA doesn't support 11ax\n");
#endif
                break;
            }
        }
        tlv_buf_left -= (int)sizeof(MrvlIEtypesHeader_t) + (int)tlv_len;
        tlv = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
    LEAVE();

    return;
}

/** Fixed size of bss start event */
#define BSS_START_EVENT_FIX_SIZE 12U

/**
 *  @brief This function will search for the specific ie
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *
 *  @return           N/A
 */
static void wrapper_wlan_check_uap_capability(pmlan_private priv, Event_Ext_t *pevent)
{
    t_u16 tlv_type, tlv_len;
    int tlv_buf_left = (int)pevent->length - (int)INTF_HEADER_LEN - (int)BSS_START_EVENT_FIX_SIZE;
    MrvlIEtypesHeader_t *tlv =
        (MrvlIEtypesHeader_t *)(void *)((char *)pevent + INTF_HEADER_LEN + BSS_START_EVENT_FIX_SIZE);
#if CONFIG_11AX
    MrvlIEtypes_He_cap_t *pext_tlv = MNULL;
#endif
    priv->wmm_enabled    = MFALSE;
    priv->pkt_fwd        = 0U;
    priv->is_11n_enabled = MFALSE;
#if CONFIG_11AC
    priv->is_11ac_enabled = MFALSE;
#endif
#if CONFIG_11AX
    priv->is_11ax_enabled = MFALSE;
#endif
    ENTER();

    while (tlv_buf_left >= (int)sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) > (unsigned int)tlv_buf_left)
        {
            PRINTM(MERROR, "wrong tlv: tlvLen=%d, tlvBufLeft=%d\n", tlv_len, tlv_buf_left);
            break;
        }
        if (tlv_type == (t_u16)HT_CAPABILITY)
        {
            DBG_HEXDUMP(MCMD_D, "HT_CAP tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
            priv->is_11n_enabled = MTRUE;
        }
#if CONFIG_11AC
        if (tlv_type == (t_u16)VHT_CAPABILITY)
        {
            DBG_HEXDUMP(MCMD_D, "VHT_CAP tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
            priv->is_11ac_enabled = MTRUE;
        }
#endif
#if CONFIG_11AX
        if (tlv_type == EXTENSION)
        {
            pext_tlv = (MrvlIEtypes_He_cap_t *)tlv;
            if (pext_tlv->ext_id == HE_CAPABILITY)
            {
                DBG_HEXDUMP(MCMD_D, "HE_CAP tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
                priv->is_11ax_enabled = MTRUE;
            }
        }
#endif
        if (tlv_type == (t_u16)TLV_TYPE_UAP_PKT_FWD_CTL)
        {
            DBG_HEXDUMP(MCMD_D, "pkt_fwd tlv", tlv, tlv_len + sizeof(MrvlIEtypesHeader_t));
            priv->pkt_fwd = *((t_u8 *)tlv + sizeof(MrvlIEtypesHeader_t));
            PRINTM(MCMND, "pkt_fwd FW: 0x%x\n", priv->pkt_fwd);
            if ((priv->pkt_fwd & PKT_FWD_FW_BIT) != 0U)
            {
                priv->pkt_fwd = 0U;
            }
            else
            {
                priv->pkt_fwd |= PKT_FWD_ENABLE_BIT;
            }
            PRINTM(MCMND, "pkt_fwd DRV: 0x%x\n", priv->pkt_fwd);
        }
        tlv_buf_left -= (int)sizeof(MrvlIEtypesHeader_t) + (int)tlv_len;
        tlv = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
    LEAVE();
}
#endif /* CONFIG_UAP_AMPDU_TX || CONFIG_UAP_AMPDU_RX */

#if (CONFIG_WNM_PS)
void wlan_update_wnm_ps_status(wnm_ps_result *wnm_ps_result)
{
    if ((wnm_ps_result->action == 0) && (wnm_ps_result->result == 0))
    {
        /* Do nothing */
    }
    else if ((wnm_ps_result->action == 1) && (wnm_ps_result->result == 0))
    {
        ((mlan_private *)mlan_adap->priv[0])->wnm_set = false;
    }
    else if ((wnm_ps_result->action == 0) && (wnm_ps_result->result == 1))
    {
        ((mlan_private *)mlan_adap->priv[0])->wnm_set = false;
    }
    else if ((wnm_ps_result->action == 1) && (wnm_ps_result->result == 1))
    {
        /* Do nothing */
    }
    else
    {
        /* Do nothing */
    }
}
#endif

#if CONFIG_WMM
static inline void wifi_wmm_queue_lock(mlan_private *priv, t_u8 ac)
{
    priv->adapter->callbacks.moal_semaphore_get(priv->adapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[ac].ra_list.plock);
}

static inline void wifi_wmm_queue_unlock(mlan_private *priv, t_u8 ac)
{
    priv->adapter->callbacks.moal_semaphore_put(priv->adapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[ac].ra_list.plock);
}

static inline void wifi_wmm_trigger_tx(t_u8 tx_pause)
{
    if (tx_pause == MFALSE)
    {
        send_wifi_driver_tx_data_event(MLAN_BSS_TYPE_STA);
    }
}

void wifi_sta_handle_event_data_pause(void *tx_pause)
{
    int i;
    t_u8 *bssid                         = MNULL;
    raListTbl *ra_list                  = MNULL;
    t_u8 zero_mac[MLAN_MAC_ADDR_LENGTH] = {0};
    mlan_private *priv     = mlan_adap->priv[0];
    MrvlIEtypes_tx_pause_t *tx_pause_tlv = (MrvlIEtypes_tx_pause_t *)tx_pause;

    if (!priv->media_connected)
        return;

    bssid = priv->curr_bss_params.bss_descriptor.mac_address;
    if (!__memcmp(mlan_adap, bssid, zero_mac, MLAN_MAC_ADDR_LENGTH))
        return;

    if (!__memcmp(mlan_adap, bssid, tx_pause_tlv->peermac, MLAN_MAC_ADDR_LENGTH))
    {
        priv->tx_pause = (tx_pause_tlv->tx_pause) ? MTRUE : MFALSE;

        /* update ralists for finding alternative buffer when queue full */
        for (i = 0; i < MAX_AC_QUEUES; i++)
        {
            wifi_wmm_queue_lock(priv, i);

            ra_list = wlan_wmm_get_ralist_node(priv, i, tx_pause_tlv->peermac);
            if (ra_list == MNULL)
            {
                wifi_wmm_queue_unlock(priv, i);
                continue;
            }

            ra_list->tx_pause = priv->tx_pause;

            wifi_wmm_queue_unlock(priv, i);
        }
    }

    wifi_wmm_trigger_tx(tx_pause_tlv->tx_pause);
}

#if UAP_SUPPORT
/*
 *  update uap tx pause status
 *  for self address, update the whole priv interface status
 *  for other addresses, update corresponding ralist status
 *  trigger tx handler if this is an unpause event
 */
void wifi_uap_handle_event_data_pause(void *tx_pause)
{
    int i;
    raListTbl *ra_list = MNULL;
    mlan_private *priv_uap = mlan_adap->priv[1];
    MrvlIEtypes_tx_pause_t *tx_pause_tlv = (MrvlIEtypes_tx_pause_t *)tx_pause;

    if (!memcmp(priv_uap->curr_addr, tx_pause_tlv->peermac, MLAN_MAC_ADDR_LENGTH))
    {
        priv_uap->tx_pause = (tx_pause_tlv->tx_pause) ? MTRUE : MFALSE;
    }
    else
    {
        for (i = 0; i < MAX_AC_QUEUES; i++)
        {
            wifi_wmm_queue_lock(priv_uap, i);

            ra_list = wlan_wmm_get_ralist_node(priv_uap, i, tx_pause_tlv->peermac);
            if (ra_list == MNULL)
            {
                wifi_wmm_queue_unlock(priv_uap, i);
                continue;
            }

            ra_list->tx_pause = (tx_pause_tlv->tx_pause) ? MTRUE : MFALSE;

            wifi_wmm_queue_unlock(priv_uap, i);
        }
    }

    wifi_wmm_trigger_tx(tx_pause_tlv->tx_pause);
}
#endif

void wifi_handle_event_data_pause(void *data)
{
    /* Event_Ext_t shares the same header but from reason_code, payload differs with tx_pause cmd */
    Event_Ext_t *evt = (Event_Ext_t *)data;
    t_u16 tlv_type, tlv_len;
    int tlv_buf_left         = evt->length - MLAN_FIELD_OFFSET(Event_Ext_t, reason_code);
    MrvlIEtypesHeader_t *tlv = (MrvlIEtypesHeader_t *)&evt->reason_code;

    /* set tx pause */
    while (tlv_buf_left >= (int)sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);

        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) > (unsigned int)tlv_buf_left)
        {
            wifi_e("wrong tlv: tlvLen=%d, tlvBufLeft=%d", tlv_len, tlv_buf_left);
            break;
        }

        if (tlv_type == TLV_TYPE_TX_PAUSE)
        {
#if !CONFIG_MEM_POOLS
            void *tx_pause_tlv = OSA_MemoryAllocate(sizeof(MrvlIEtypes_tx_pause_t));
#else
            void *tx_pause_tlv = OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif

            (void)memcpy(tx_pause_tlv, (void *)tlv, sizeof(MrvlIEtypes_tx_pause_t));
            if (evt->bss_type == MLAN_BSS_TYPE_STA)
            {
                if (wifi_event_completion(WIFI_EVENT_TX_DATA_PAUSE, WIFI_EVENT_REASON_SUCCESS, tx_pause_tlv) !=
                    WM_SUCCESS)
                {
#if !CONFIG_MEM_POOLS
                    OSA_MemoryFree(tx_pause_tlv);
#else
                    OSA_MemoryPoolFree(buf_32_MemoryPool, tx_pause_tlv);
#endif
                }
            }
#if UAP_SUPPORT
            else if (evt->bss_type == MLAN_BSS_TYPE_UAP)
            {
                if (wifi_event_completion(WIFI_EVENT_UAP_TX_DATA_PAUSE, WIFI_EVENT_REASON_SUCCESS, tx_pause_tlv) !=
                    WM_SUCCESS)
                {
#if !CONFIG_MEM_POOLS
                    OSA_MemoryFree(tx_pause_tlv);
#else
                    OSA_MemoryPoolFree(buf_32_MemoryPool, tx_pause_tlv);
#endif
                }
            }
#endif
            else
            {
                wifi_w("Not support bss_type %d", evt->bss_type);
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree(tx_pause_tlv);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, tx_pause_tlv);
#endif
            }
        }

        /* iterate */
        tlv_buf_left -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv = (MrvlIEtypesHeader_t *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
}
#endif

static void wifi_handle_event_tx_status_report(Event_Ext_t *evt)
{
#if CONFIG_WPA_SUPP
    tx_status_event *tx_status = MNULL;
    unsigned int bss_type      = (unsigned int)evt->bss_type;

    tx_status = (tx_status_event *)(void *)&evt->reason_code;

    if (tx_status->packet_type == 0xe5)
    {
        if (tx_status->status == 0U)
        {
            (void)wifi_event_completion(WIFI_EVENT_MGMT_TX_STATUS, WIFI_EVENT_REASON_SUCCESS, (void *)bss_type);
        }
        return;
    }
#endif
}

#define REASON_CODE_BSS_BLOCKED 0x21
static void wifi_handle_blocked_sta_report(Event_Ext_t *evt)
{
    mlan_adapter *pmadapter = mlan_adap;
    t_u16 reason_code       = 0;
    int idx                 = 0;

    reason_code = evt->reason_code;
    if (reason_code == REASON_CODE_BSS_BLOCKED)
    {
        if (pmadapter->blacklist_bss.num_bssid == MLAN_MAX_BSS_NUM)
        {
            wifi_e("BSSID blacklist is full!");
            return;
        }
        idx = pmadapter->blacklist_bss.num_bssid;
        memcpy(pmadapter->blacklist_bss.bssids[idx], evt->src_mac_addr, MLAN_MAC_ADDR_LENGTH);
        pmadapter->blacklist_bss.num_bssid++;
        (void)PRINTF("Added BSSID ");
        print_mac((const char *)evt->src_mac_addr);
        (void)PRINTF("to blacklist\r\n");
    }
    else
        wifi_w("Unknown reason code! Ignore the event");
}

/* fixme: duplicated from legacy. needs to be cleaned up later */
#define IEEEtypes_REASON_UNSPEC                     1U
#define IEEEtypes_REASON_PRIOR_AUTH_INVALID         2U
#define IEEEtypes_REASON_DEAUTH_LEAVING             3
#define IEEEtypes_REASON_DISASSOC_DUE_TO_INACTIVITY 4U
#define AP_DEAUTH_REASON_MAC_ADDR_BLOCKED           6U

#if CONFIG_WIFI_TX_PER_TRACK
#define OFFSET_SEQNUM 8
static void wifi_tx_pert_report(void *pbuf)
{
    (void)PRINTF("current PER is %d%%\r\n", wlan_le16_to_cpu(*(t_u16 *)((t_u8 *)pbuf + 8)));
    (void)PRINTF("User configure:\r\n");
    (void)PRINTF("       tx_pert_check_period : %d sec\r\n", mlan_adap->tx_pert.tx_pert_check_peroid);
    (void)PRINTF("       tx_pert_check_ratio  : %d%%\r\n", mlan_adap->tx_pert.tx_pert_check_ratio);
    (void)PRINTF("       tx_pert_check_num    : %d\r\n", mlan_adap->tx_pert.tx_pert_check_num);
    return;
}
#endif

#if CONFIG_BG_SCAN
int wifi_request_bgscan(mlan_private *pmpriv)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = 0;
    cmd->result  = 0x0;
    wlan_ops_sta_prepare_cmd(pmpriv, HostCmd_CMD_802_11_BG_SCAN_CONFIG, HostCmd_ACT_GEN_SET, 0, NULL, &pmpriv->scan_cfg,
                             cmd);
    wifi_wait_for_cmdresp(NULL);
    return wm_wifi.cmd_resp_status;
}
#endif

int wifi_set_rssi_low_threshold(uint8_t *low_rssi)
{
#if (CONFIG_11K) || (CONFIG_11V) || (CONFIG_ROAMING)
    mlan_private *pmpriv = mlan_adap->priv[0];
    mlan_ds_subscribe_evt subscribe_evt;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num                = 0;
    cmd->result                 = 0x0;
    subscribe_evt.evt_action    = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    subscribe_evt.evt_bitmap    = SUBSCRIBE_EVT_RSSI_LOW;
    subscribe_evt.low_rssi      = *low_rssi;
    subscribe_evt.low_rssi_freq = 0;
    wlan_ops_sta_prepare_cmd(pmpriv, HostCmd_CMD_802_11_SUBSCRIBE_EVENT, HostCmd_ACT_GEN_SET, 0, NULL, &subscribe_evt,
                             cmd);
    wifi_wait_for_cmdresp(NULL);

    if ((*low_rssi + RSSI_HYSTERESIS) <= LOWEST_RSSI_THRESHOLD)
    {
        *low_rssi += RSSI_HYSTERESIS;
    }

    return wm_wifi.cmd_resp_status;
#else
    return 0;
#endif
}

#if CONFIG_BG_SCAN
int wifi_request_bgscan_query(mlan_private *pmpriv)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = 0;
    cmd->result  = 0x0;
    wlan_ops_sta_prepare_cmd(pmpriv, HostCmd_CMD_802_11_BG_SCAN_QUERY, HostCmd_ACT_GEN_GET, 0, NULL, NULL, cmd);
    wifi_wait_for_cmdresp(NULL);
    return wm_wifi.cmd_resp_status;
}

int wifi_config_bgscan_and_rssi(const char *ssid)
{
    mlan_private *pmpriv = mlan_adap->priv[0];
    int band             = 0;
    int ret              = -WM_FAIL;

    ENTER();

    if (pmpriv->roaming_enabled == MFALSE)
    {
        wifi_d("Roaming is disabled");
        goto done;
    }

    memset(&pmpriv->scan_cfg, 0, sizeof(pmpriv->scan_cfg));

    if (wm_wifi.g_user_scan_cfg != NULL)
    {
        pmpriv->scan_cfg.start_later = MTRUE;
    }

    /* Fill scan config field for bg scan */
    strncpy((char *)pmpriv->scan_cfg.ssid_list[0].ssid, (char *)ssid, MLAN_MAX_SSID_LENGTH);
    pmpriv->scan_cfg.ssid_list[0].ssid[MLAN_MAX_SSID_LENGTH] = '\0';
    pmpriv->scan_cfg.ssid_list[0].max_len                    = 0x00;
    pmpriv->scan_cfg.report_condition                        = BG_SCAN_SSID_RSSI_MATCH | BG_SCAN_WAIT_ALL_CHAN_DONE;
    pmpriv->scan_cfg.rssi_threshold                          = pmpriv->rssi_low;
    pmpriv->scan_cfg.repeat_count                            = DEF_REPEAT_COUNT;
    pmpriv->scan_cfg.scan_interval                           = MIN_BGSCAN_INTERVAL;
    pmpriv->scan_cfg.chan_per_scan                           = WLAN_USER_SCAN_CHAN_MAX;
    pmpriv->scan_cfg.num_probes                              = 2;
#if CONFIG_SCAN_CHANNEL_GAP
    pmpriv->scan_cfg.scan_chan_gap = SCAN_CHANNEL_GAP_VALUE;
#endif

    wifi_get_band(pmpriv, &band);
    switch (band)
    {
        case WIFI_FREQUENCY_BAND_2GHZ:
            pmpriv->scan_cfg.chan_list[0].radio_type = 0 | BAND_SPECIFIED;
            break;
        case WIFI_FREQUENCY_BAND_5GHZ:
            pmpriv->scan_cfg.chan_list[0].radio_type = 1 | BAND_SPECIFIED;
            break;
        default:
            PRINTM(MINFO, "Unexpected freq band \n");
            break;
    }
    pmpriv->scan_cfg.bss_type = MLAN_BSS_MODE_INFRA;
    pmpriv->scan_cfg.action   = BG_SCAN_ACT_SET;
    pmpriv->scan_cfg.enable   = MTRUE;
    ret                       = wifi_request_bgscan(pmpriv);
    if (ret)
    {
        wifi_d("Failed to request bgscan");
    }

done:
    LEAVE();

    return ret;
}

mlan_status wifi_stop_bgscan()
{
    mlan_private *pmpriv = mlan_adap->priv[0];
    mlan_status ret      = MLAN_STATUS_SUCCESS;

    (void)memset(&pmpriv->scan_cfg, 0, sizeof(pmpriv->scan_cfg));
    pmpriv->scan_cfg.bss_type = MLAN_BSS_MODE_INFRA;
    pmpriv->scan_cfg.action   = BG_SCAN_ACT_SET;
    pmpriv->scan_cfg.enable   = MFALSE;
    ret                       = (mlan_status)wifi_request_bgscan(pmpriv);
    if (ret)
        PRINTM(MERROR, "Failed to stop bgscan\n");
    return ret;
}
#endif

int wifi_handle_fw_event(struct bus_message *msg)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
#if UAP_SUPPORT
    mlan_private *pmpriv_uap = (mlan_private *)mlan_adap->priv[1];
    t_u8 *sta_addr = NULL, *event_sta_addr = NULL;
    wifi_uap_client_disassoc_t *disassoc_resp;
#endif
#if CONFIG_WMM_UAPSD
    t_u8 tx_lock_flag_org = 0;
#endif
#if CONFIG_EXT_SCAN_SUPPORT
    mlan_status rv = MLAN_STATUS_SUCCESS;
#endif
#if CONFIG_WPA_SUPP
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.if_priv;
#endif
    Event_Ext_t *evt           = ((Event_Ext_t *)msg->data);
    wifi_ecsa_info *pecsa_info = NULL;

#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
    sta_node *sta_node_ptr;
#endif /* CONFIG_UAP_AMPDU_TX || CONFIG_UAP_AMPDU_RX */
#if CONFIG_EXT_SCAN_SUPPORT
    mlan_event_scan_result *pext_scan_result;
#endif
    if (evt == NULL)
    {
        wevt_d("No mem allocated for msg.data");
        return -WM_FAIL;
    }

#if !CONFIG_WIFI_PS_DEBUG
    if ((evt->event_id != EVENT_PS_SLEEP) && (evt->event_id != EVENT_PS_AWAKE))
    {
        wevt_d("EVENT - : 0x%x Len : %d Reason: %d", evt->event_id, evt->length, evt->reason_code);
    }
#else
    wevt_d("EVENT - : 0x%x Len : %d Reason: %d", evt->event_id, evt->length, evt->reason_code);
#endif

    if ((evt->event_id != EVENT_PS_AWAKE) && (mlan_adap->ps_state == PS_STATE_SLEEP))
    {
        OSA_RWLockWriteUnlock(&sleep_rwlock);
        pmpriv->adapter->ps_state = PS_STATE_AWAKE;
    }

#if !UAP_SUPPORT
    if (evt->bss_type > MLAN_BSS_ROLE_STA)
    {
        wevt_w("wifi_handle_fw_event 0x%X receive UAP event when UAP not supported", evt->event_id);
        return -WM_FAIL;
    }
#endif

    switch (evt->event_id)
    {
        case EVENT_LINK_LOST:
            (void)wifi_event_completion(WIFI_EVENT_LINK_LOSS, WIFI_EVENT_REASON_FAILURE,
                                        (void *)IEEEtypes_REASON_DEAUTH_LEAVING);
#if CONFIG_WPA_SUPP
            wifi_if_ctx_rtos->associated = MFALSE;

            wpa_supp_handle_link_lost(pmpriv);
#else
#if CONFIG_11N
            /* fixme: Should this be outside CONFIG_11N ? */
            wlan_handle_disconnect_event(pmpriv);
#endif
#endif
            break;
        case EVENT_DEAUTHENTICATED:
#if CONFIG_WPA_SUPP
            wifi_if_ctx_rtos->associated = MFALSE;
#endif
            if (evt->reason_code == 0U)
            {
                (void)wifi_event_completion(WIFI_EVENT_LINK_LOSS, WIFI_EVENT_REASON_FAILURE,
                                            (void *)IEEEtypes_REASON_DEAUTH_LEAVING);
            }
            else
            {
                (void)wifi_event_completion(WIFI_EVENT_AUTHENTICATION, WIFI_EVENT_REASON_FAILURE,
                                            (void *)&evt->reason_code);
            }

#if CONFIG_11N
            /* fixme: Should this be outside CONFIG_11N ? */
            wlan_handle_disconnect_event(pmpriv);
#endif
            break;
        case EVENT_DISASSOCIATED:
#if CONFIG_WPA_SUPP
            wifi_if_ctx_rtos->associated = MFALSE;
#endif

#if !CONFIG_WPA_SUPP
            (void)wifi_event_completion(WIFI_EVENT_DISASSOCIATION, WIFI_EVENT_REASON_FAILURE,
                                        (void *)IEEEtypes_REASON_DEAUTH_LEAVING);
#else
            (void)wifi_event_completion(WIFI_EVENT_DISASSOCIATION, WIFI_EVENT_REASON_SUCCESS,
                                        (void *)&evt->reason_code);
#endif

#if CONFIG_11N
            /* fixme: Should this be outside CONFIG_11N ? */
            wlan_handle_disconnect_event(pmpriv);
#endif
            break;
        case EVENT_PORT_RELEASE:
#if !CONFIG_WPA_SUPP
            (void)wifi_event_completion(WIFI_EVENT_AUTHENTICATION, WIFI_EVENT_REASON_SUCCESS, NULL);
#endif
            break;
        case EVENT_PS_SLEEP:
#if CONFIG_WIFI_PS_DEBUG
            wevt_d("_");
#endif
            if (mlan_adap->ps_state != PS_STATE_PRE_SLEEP)
            {
                mlan_adap->ps_state = PS_STATE_PRE_SLEEP;
#if CONFIG_HOST_SLEEP
                wakelock_get();
#endif
                if (split_scan_in_progress == false)
                {
                    /* When received EVENT_PS_SLEEP, firstly send msg to wifi_powersave task
                     * with lowest priority, then send msg to wlcmgr task. This will let all
                     * TX data transmitted, then continue the 0xe4 cmd handshake */
                    struct wifi_message ps_msg;
                    ps_msg.reason = WIFI_EVENT_REASON_SUCCESS;
                    ps_msg.event  = WIFI_EVENT_SLEEP;
                    OSA_MsgQPut((osa_msgq_handle_t)wm_wifi.powersave_queue, &ps_msg);
                }
                else
                {
                    /** Do Nothing */
                }
            }
            else
            {
                /* Unexpected PS SLEEP event */
                wevt_w("Receive PS SLEEP event when presleep: %d", mlan_adap->ps_state);
            }
            break;
        case EVENT_PS_AWAKE:
#if CONFIG_WIFI_PS_DEBUG
            wevt_d("|");
#endif
#if CONFIG_WMM_UAPSD
            if (!pmpriv->adapter->pps_uapsd_mode && pmpriv->media_connected && pmpriv->adapter->sleep_period.period)
            {
                pmpriv->adapter->pps_uapsd_mode = MTRUE;
                PRINTM(MEVENT, "PPS/UAPSD mode activated\n");
            }

            tx_lock_flag_org = pmpriv->adapter->tx_lock_flag;

            if (pmpriv->adapter->pps_uapsd_mode)
            {
                OSA_SemaphorePost((osa_semaphore_handle_t)uapsd_sem);
                /* For the continous 0xA event handling case, wifi_driver_tx task
                 * is not allowed to send packets until the last 0xA is processed */
                pmpriv->adapter->tx_lock_flag = MTRUE;
                /* As the wifi_driver task has priority of 3, so sleep 1ms to yield to the CMD sending task */
                OSA_TimeDelay(1);
            }

            /* If original tx_lock_flag is false, we don't change the tx_lock_flag to
             * false again, to avoid sending two last packets to FW in one sleep period */
            if (tx_lock_flag_org == MTRUE)
            {
                pmpriv->adapter->tx_lock_flag = MFALSE;
            }

            if (pmpriv->adapter->pps_uapsd_mode && pmpriv->media_connected && pmpriv->adapter->gen_null_pkt &&
                wifi_check_no_packet_indication(pmpriv))
            {
                /* wifi_driver_tx task process the null packet sending */
                send_wifi_driver_tx_null_data_event(MLAN_BSS_TYPE_STA);
            }
#endif
            if (mlan_adap->ps_state == PS_STATE_SLEEP)
            {
                OSA_RWLockWriteUnlock(&sleep_rwlock);
                mlan_adap->ps_state = PS_STATE_AWAKE;
            }
            else if (mlan_adap->ps_state == PS_STATE_PRE_SLEEP)
            {
                /* If driver did not send out sleep confirm in the expected time,
                   FW would take it as timeout, switch to awake and send out PS AWAKE event */
                wevt_w("Receive PS AWAKE event when presleep: %d", mlan_adap->ps_state);
                OSA_RWLockWriteUnlock(&sleep_rwlock);
                mlan_adap->ps_state = PS_STATE_AWAKE;
            }
            else
            {
                /*Do Nothing*/
            }
            break;
#if (CONFIG_WNM_PS)
        case EVENT_WNM_PS:
        {
#if !CONFIG_MEM_POOLS
            t_u16 *wnm_action_p = (t_u16 *)OSA_MemoryAllocate(sizeof(t_u16));
#else
            t_u16 *wnm_action_p = (t_u16 *)OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
            if (!wnm_action_p)
            {
                wifi_w("No mem. Cannot process wnm ps event");
                break;
            }

            *wnm_action_p = evt->reason_code;
            wlan_update_wnm_ps_status((wnm_ps_result *)wnm_action_p);
            if (wifi_event_completion(WIFI_EVENT_WNM_PS, WIFI_EVENT_REASON_SUCCESS, (void *)((t_u32)wnm_action_p)) !=
                WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree((void *)wnm_action_p);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, wnm_action_p);
#endif
            }
        }
        break;
#endif
        case EVENT_MIC_ERR_MULTICAST:
            (void)wifi_event_completion(WIFI_EVENT_ERR_MULTICAST, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_MIC_ERR_UNICAST:
            (void)wifi_event_completion(WIFI_EVENT_ERR_UNICAST, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
#if CONFIG_BG_SCAN
        case EVENT_BG_SCAN_REPORT:
            pmpriv->adapter->bgscan_reported = MTRUE;
            pmpriv->roaming_configured       = MFALSE;
            (void)wifi_event_completion(WIFI_EVENT_BG_SCAN_REPORT, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_BG_SCAN_STOPPED:
            pmpriv->roaming_configured = MFALSE;
            (void)wifi_event_completion(WIFI_EVENT_BG_SCAN_STOPPED, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
#endif
#if CONFIG_HOST_SLEEP
        case EVENT_HS_ACT_REQ:
            if (pmpriv->adapter->is_hs_configured == MTRUE)
                (void)wifi_event_completion(WIFI_EVENT_HS_CONFIG, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
#endif
#if defined(SD9177) || defined(IW610)
        case EVENT_IMD3_CAL_START:
            wifi_d("got event: IMD3 cal started");
            break;
        case EVENT_IMD3_CAL_END:
            wifi_d("got event: IMD3 cal stopped");
            break;
#endif
        case EVENT_RSSI_LOW:
            (void)wifi_event_completion(WIFI_EVENT_RSSI_LOW, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
#if CONFIG_SUBSCRIBE_EVENT_SUPPORT
        case EVENT_RSSI_HIGH:
            (void)wifi_event_completion(WIFI_EVENT_RSSI_HIGH, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_SNR_LOW:
            (void)wifi_event_completion(WIFI_EVENT_SNR_LOW, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_SNR_HIGH:
            (void)wifi_event_completion(WIFI_EVENT_SNR_HIGH, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_MAX_FAIL:
            (void)wifi_event_completion(WIFI_EVENT_MAX_FAIL, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_DATA_RSSI_LOW:
            (void)wifi_event_completion(WIFI_EVENT_DATA_RSSI_LOW, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_DATA_RSSI_HIGH:
            (void)wifi_event_completion(WIFI_EVENT_DATA_RSSI_HIGH, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_DATA_SNR_LOW:
            (void)wifi_event_completion(WIFI_EVENT_DATA_SNR_LOW, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_DATA_SNR_HIGH:
            (void)wifi_event_completion(WIFI_EVENT_DATA_SNR_HIGH, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_LINK_QUALITY:
            (void)wifi_event_completion(WIFI_EVENT_FW_LINK_QUALITY, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_PRE_BEACON_LOST:
            (void)wifi_event_completion(WIFI_EVENT_FW_PRE_BCN_LOST, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
#endif
#if CONFIG_11N
        case EVENT_ADDBA:
        {
            void *saved_event_buff = wifi_11n_save_request(evt);
            if (saved_event_buff != NULL)
            {
                if (wifi_event_completion(WIFI_EVENT_11N_RECV_ADDBA, WIFI_EVENT_REASON_SUCCESS, saved_event_buff) !=
                    WM_SUCCESS)
                {
                    /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                    OSA_MemoryFree((void *)saved_event_buff);
#else
                    OSA_MemoryPoolFree(buf_256_MemoryPool, saved_event_buff);
#endif
                }
            }
            /* If allocation failed ignore this event quietly ! */
        }
        break;
        case EVENT_DELBA:
        {
            wlan_11n_delete_bastream((mlan_private *)mlan_adap->priv[evt->bss_type], (t_u8 *)&evt->reason_code);
        }
        break;
        case EVENT_BA_STREAM_TIMEOUT:
        {
            void *saved_event_buff = wifi_11n_save_request(evt);
            if (saved_event_buff != NULL)
            {
                if (wifi_event_completion(WIFI_EVENT_11N_BA_STREAM_TIMEOUT, WIFI_EVENT_REASON_SUCCESS,
                                          saved_event_buff) != WM_SUCCESS)
                {
                    /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                    OSA_MemoryFree((void *)saved_event_buff);
#else
                    OSA_MemoryPoolFree(buf_256_MemoryPool, saved_event_buff);
#endif
                }
            }
            /* If allocation failed ignore this event quietly ! */
        }
        break;
        case EVENT_AMSDU_AGGR_CTRL:
            /*
             * Currently this event buffer isn't being used. Enable
             * this when required.
             */
            /* wifi_11n_save_request(evt); */
            (void)wifi_event_completion(WIFI_EVENT_11N_AGGR_CTRL, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
#endif /* CONFIG_11N */
        case EVENT_CHANNEL_SWITCH_ANN:
            (void)wifi_event_completion(WIFI_EVENT_CHAN_SWITCH_ANN, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case EVENT_CHANNEL_SWITCH:
        {
#if !CONFIG_MEM_POOLS
            pecsa_info = OSA_MemoryAllocate(sizeof(wifi_ecsa_info));
#else
            pecsa_info = OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
            if (!pecsa_info)
            {
                wifi_w("No mem. Cannot process new channel from channel switch");
                break;
            }

            MrvlIEtypes_channel_band_t *tlv = (MrvlIEtypes_channel_band_t *)&evt->reason_code;

            pecsa_info->bss_type    = evt->bss_type;
            pecsa_info->band_config = tlv->band_config;
            pecsa_info->channel     = tlv->channel;

            if (wifi_event_completion(WIFI_EVENT_CHAN_SWITCH, WIFI_EVENT_REASON_SUCCESS, pecsa_info) != WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree((void *)pecsa_info);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, pecsa_info);
#endif
            }
        }
        break;
#if UAP_SUPPORT
        case EVENT_MICRO_AP_STA_ASSOC:
        {
            /*
             * Alloc memory to store the STA mac id. This will be
             * passed to event receiver thread. Freeing this is
             * responsibility of the receiving thread.
             */
#if !CONFIG_MEM_POOLS
            sta_addr = OSA_MemoryAllocate(MLAN_MAC_ADDR_LENGTH);
#else
            sta_addr = OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
            if (sta_addr == MNULL)
            {
                wifi_w("No mem. Cannot process MAC address from assoc");
                break;
            }

            event_sta_addr = (t_u8 *)&evt->src_mac_addr;
            (void)memcpy((void *)sta_addr, (const void *)event_sta_addr, MLAN_MAC_ADDR_LENGTH);

#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
            wlan_request_ralist_lock(mlan_adap->priv[1]);
            /* Clear corresponding tx/rx table if necessary */
            if (wlan_11n_get_txbastream_tbl((mlan_private *)mlan_adap->priv[1], sta_addr))
                wlan_11n_delete_txbastream_tbl_entry((mlan_private *)mlan_adap->priv[1], sta_addr);

            wlan_cleanup_reorder_tbl((mlan_private *)mlan_adap->priv[1], sta_addr);
#if CONFIG_WMM
            wlan_ralist_del_enh(mlan_adap->priv[1], sta_addr);
#endif
            /* txbastream table also is used as connected STAs data base */
            wlan_11n_create_txbastream_tbl((mlan_private *)mlan_adap->priv[1], sta_addr, BA_STREAM_NOT_SETUP);
            wlan_11n_update_txbastream_tbl_tx_thresh((mlan_private *)mlan_adap->priv[1], sta_addr, 3);

#if !CONFIG_MEM_POOLS
            sta_node_ptr = OSA_MemoryAllocate(sizeof(sta_node));
#else
            sta_node_ptr = OSA_MemoryPoolAllocate(buf_256_MemoryPool);
#endif
            if (sta_node_ptr == MNULL)
            {
                wifi_w("No mem. Cannot check station type");
                wlan_release_ralist_lock(mlan_adap->priv[1]);
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree(sta_addr);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, sta_addr);
#endif
                break;
            }
            memset(sta_node_ptr, 0x00, sizeof(sta_node));

            wrapper_wlan_check_sta_capability((mlan_private *)mlan_adap->priv[1], msg->data, sta_node_ptr);

            if (sta_node_ptr->is_11n_enabled)
            {
                wlan_11n_update_txbastream_tbl_ampdu_supported((mlan_private *)mlan_adap->priv[1], sta_addr, MTRUE);
            }

            wlan_release_ralist_lock(mlan_adap->priv[1]);

#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(sta_node_ptr);
#else
            OSA_MemoryPoolFree(buf_256_MemoryPool, sta_node_ptr);
#endif

#endif /* CONFIG_UAP_AMPDU_TX || CONFIG_UAP_AMPDU_RX */

#if CONFIG_WMM
            wlan_ralist_add_enh(mlan_adap->priv[1], sta_addr);
#endif

            if (wifi_event_completion(WIFI_EVENT_UAP_CLIENT_ASSOC, WIFI_EVENT_REASON_SUCCESS, sta_addr) != WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree((void *)sta_addr);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, sta_addr);
#endif
            }
        }
        break;
        case EVENT_MICRO_AP_RSN_CONNECT:
        {
            /*
             * Alloc memory to store the STA mac id. This will be
             * passed to event receiver thread. Freeing this is
             * responsibility of the receiving thread.
             */
#if !CONFIG_MEM_POOLS
            sta_addr = OSA_MemoryAllocate(MLAN_MAC_ADDR_LENGTH);
#else
            sta_addr = OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
            if (sta_addr == MNULL)
            {
                wifi_w("No mem. Cannot process MAC address from conn");
                break;
            }

            event_sta_addr = (t_u8 *)&evt->src_mac_addr;
            (void)memcpy((void *)sta_addr, (const void *)event_sta_addr, MLAN_MAC_ADDR_LENGTH);
            if (wifi_event_completion(WIFI_EVENT_UAP_CLIENT_CONN, WIFI_EVENT_REASON_SUCCESS, sta_addr) != WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree((void *)sta_addr);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, sta_addr);
#endif
            }
        }
        break;
        case EVENT_MICRO_AP_STA_DEAUTH:
            /*
             * Alloc memory to store the STA mac id. This will be
             * passed to event receiver thread. Freeing this is
             * responsibility of the receiving thread.
             */
#if !CONFIG_MEM_POOLS
            disassoc_resp = OSA_MemoryAllocate(sizeof(wifi_uap_client_disassoc_t));
#else
            disassoc_resp = OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
            if (disassoc_resp == MNULL)
            {
                wifi_w("No mem. Cannot add mac and reason code for deauth event to app");
                break;
            }
            sta_addr                   = disassoc_resp->sta_addr;
            disassoc_resp->reason_code = (int)evt->reason_code;

            event_sta_addr = (t_u8 *)&evt->src_mac_addr;
            (void)memcpy((void *)sta_addr, (const void *)event_sta_addr, MLAN_MAC_ADDR_LENGTH);

#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
            if (pmpriv_uap->is_11n_enabled)
            {
                wlan_cleanup_reorder_tbl(pmpriv_uap, sta_addr);
                wlan_request_ralist_lock(pmpriv_uap);
                wlan_11n_delete_txbastream_tbl_entry(pmpriv_uap, sta_addr);
                wlan_release_ralist_lock(pmpriv_uap);
            }
#endif /* CONFIG_UAP_AMPDU_TX || CONFIG_UAP_AMPDU_RX */

            wlan_delete_station_entry(pmpriv_uap, sta_addr);

#if CONFIG_HOSTAPD
            /* BIT 14 indicate deauth is initiated by FW */
            if (disassoc_resp->reason_code & MBIT(14))
            {
                if (disassoc_resp->reason_code & MBIT(1))
                {
                    t_u16 reason_code = IEEEtypes_REASON_DISASSOC_DUE_TO_INACTIVITY;
                    wifi_nxp_uap_disconnect(pmpriv_uap, reason_code, sta_addr);
                }
                else
                {
                    wifi_nxp_uap_disconnect(pmpriv_uap, 0, sta_addr);
                }
            }
#endif

            if (wifi_event_completion(WIFI_EVENT_UAP_CLIENT_DEAUTH, WIFI_EVENT_REASON_SUCCESS, disassoc_resp) !=
                WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree((void *)disassoc_resp);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, disassoc_resp);
#endif
            }
#if CONFIG_WMM
            wlan_ralist_del_enh(mlan_adap->priv[1], evt->src_mac_addr);
#endif
            if (evt->reason_code == AP_DEAUTH_REASON_MAC_ADDR_BLOCKED)
            {
                wevt_d("EVENT: Blacklist sta %02x:%02x:%02x:%02x:%02x:%02x: try to join the network \r\n",
                       evt->src_mac_addr[0], evt->src_mac_addr[1], evt->src_mac_addr[2], evt->src_mac_addr[3],
                       evt->src_mac_addr[4], evt->src_mac_addr[5]);
            }
            break;
        case EVENT_MICRO_AP_BSS_START:
            wifi_d("uAP start event received");
            /*
             * wmsdk: statement copied from
             * mlan_uap_cmdevent.c. Necessary for other uAP functions.
             */
#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
            wrapper_wlan_check_uap_capability((mlan_private *)mlan_adap->priv[1], msg->data);
#endif /* CONFIG_UAP_AMPDU_TX || CONFIG_UAP_AMPDU_RX */
            pmpriv_uap->uap_bss_started = MTRUE;
            /* set uap mac addr */
            (void)memcpy((void *)pmpriv_uap->curr_addr, (const void *)evt->src_mac_addr, (size_t)MLAN_MAC_ADDR_LENGTH);
            break;
        case EVENT_MICRO_AP_BSS_ACTIVE:
            PRINTM(MEVENT, "EVENT: MICRO_AP_BSS_ACTIVE\n");
            pmpriv_uap->media_connected = MTRUE;
            break;
        case EVENT_MICRO_AP_BSS_IDLE:
            PRINTM(MEVENT, "EVENT: MICRO_AP_BSS_IDLE\n");
            pmpriv_uap->media_connected = MFALSE;
            wlan_clean_txrx(pmpriv_uap);
            wlan_delete_station_list(pmpriv_uap);
            break;
#endif /* UAP_SUPPORT */
#if CONFIG_WMM
        case EVENT_TX_DATA_PAUSE:
            wifi_handle_event_data_pause(evt);
            break;
#endif
#if CONFIG_WIFI_TX_PER_TRACK
        case EVENT_PER_STATUS_REPORT:
            PRINTM(MEVENT, "EVENT: PER_STATUS_REPORT\n");
            wifi_tx_pert_report((void *)evt);
            break;
#endif
        case EVENT_TX_STATUS_REPORT:
            wifi_handle_event_tx_status_report(evt);
            break;
        case EVENT_BLOCKEDSTA_AUTH_REPORT:
            wifi_handle_blocked_sta_report(evt);
            break;
#if CONFIG_EXT_SCAN_SUPPORT
        case EVENT_EXT_SCAN_REPORT:
            pext_scan_result = (mlan_event_scan_result *)((t_u8 *)msg->data + 4);

            rv = wlan_handle_event_ext_scan_report(pmpriv, (t_u8 *)pext_scan_result);

            if (rv != MLAN_STATUS_SUCCESS)
            {
                wifi_user_scan_config_cleanup();
                return -WM_FAIL;
            }
#ifndef SD8801
            if (is_split_scan_complete() && !pext_scan_result->more_event)
            {
                wifi_d("Split scan complete");
                wifi_user_scan_config_cleanup();
                wifi_event_completion(WIFI_EVENT_SCAN_RESULT, WIFI_EVENT_REASON_SUCCESS, NULL);
            }
#endif
            break;
#endif
#if CONFIG_WIFI_FW_DEBUG
        case EVENT_FW_DEBUG_INFO:
        {
#if !CONFIG_MEM_POOLS
            t_u8 *debug = (t_u8 *)OSA_MemoryAllocate(FW_DEBUG_INFO_SIZE);
#else
            t_u8 *debug = (t_u8 *)OSA_MemoryPoolAllocate(buf_512_MemoryPool);
#endif
            if (!debug)
            {
                wifi_w("No mem. Cannot print debug event");
                break;
            }

            (void)memcpy((void *)debug, (const void *)((uint8_t *)&evt->reason_code), evt->length - 8);
            (void)PRINTF("EVENT: FW Debug Info %s\r\n", debug);
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree((void *)debug);
#else
            OSA_MemoryPoolFree(buf_512_MemoryPool, debug);
#endif
        }
        break;
#endif
#if (CONFIG_CSI) || (((CONFIG_11AZ) || (CONFIG_11MC)))
        case EVENT_CSI:
        {
            PRINTM(MEVENT, "EVENT: EVENT_CSI\n");
#if CONFIG_CSI
            csi_deliver_data_to_user();
#endif
#if (CONFIG_11AZ) || (CONFIG_11MC)
#if CONFIG_WLS_CSI_PROC
            // wifi_get_wls_csi_sem(); // Get wls csi sem to prevent CSI event raw data from being overwritten before
            //  processing.
            if (g_csi_event_for_wls)
            {
                memcpy(wls_data, (t_u8 *)msg->data, WLS_CSI_DATA_LEN);
                wifi_event_completion(WIFI_EVENT_WLS_CSI, WIFI_EVENT_REASON_SUCCESS, wls_data);
            }
#endif
#endif
        }
        break;
#if CONFIG_CSI
        case EVENT_CSI_STATUS:
        {
            wifi_csi_status_info *pcsi_status = NULL;
#if !CONFIG_MEM_POOLS
            pcsi_status = OSA_MemoryAllocate(sizeof(wifi_csi_status_info));
#else
            pcsi_status = OSA_MemoryPoolAllocate(buf_32_MemoryPool);
#endif
            wifi_csi_status_info *pstatus = (wifi_csi_status_info *)&evt->reason_code;

            pcsi_status->status  = pstatus->status;
            pcsi_status->channel = pstatus->channel;
            pcsi_status->cnt     = pstatus->cnt;
            if (wifi_event_completion(WIFI_EVENT_CSI_STATUS, WIFI_EVENT_REASON_SUCCESS, pcsi_status) != WM_SUCCESS)
            {
                /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree((void *)pcsi_status);
#else
                OSA_MemoryPoolFree(buf_32_MemoryPool, pcsi_status);
#endif
            }
#endif
        }
        break;
#endif
        case EVENT_MEF_HOST_WAKEUP:
            wifi_d("Host recevied host wake-up event from firmware");
            break;
#if CONFIG_RSN_REPLAY_DETECTION
        case EVENT_RESET_PN_ON_REKEY:
            PRINTM(MEVENT, "EVENT: RESET PN ON REKEY\n");
            wlan_reset_pn_on_rekey(pmpriv, (t_u8 *)(msg->data) + 8);
            break;
#endif
#if CONFIG_MULTI_CHAN
        case EVENT_MULTI_CHAN_INFO:
            break;
#endif
#if CONFIG_WPA_SUPP
        case EVENT_ASSOC_REQ_IE:
#if CONFIG_AUTO_RECONNECT
            if (pmpriv->media_connected == MTRUE)
            {
                wifi_event_completion(WIFI_EVENT_ASSOCIATION_NOTIFY, WIFI_EVENT_REASON_SUCCESS, NULL);
            }
#endif
            pmpriv->assoc_req_size = evt->length - 8;
            (void)memcpy((void *)pmpriv->assoc_req_buf, (const void *)((uint8_t *)&evt->reason_code),
                         pmpriv->assoc_req_size);
            break;
#endif
#if CONFIG_5GHz_SUPPORT
        case EVENT_RADAR_DETECTED:
            wevt_d("EVENT:RADAR_DETECTED -> detect_count=%d, reg_domain=%d, det_type=%d(%s)\n",
                   wlan_le32_to_cpu(((Event_Radar_Detected_Info *)msg->data)->detect_count),
                   ((Event_Radar_Detected_Info *)msg->data)->reg_domain,
                   ((Event_Radar_Detected_Info *)msg->data)->main_det_type,
                   (((Event_Radar_Detected_Info *)msg->data)->main_det_type == 2) ? "PRI" :
                   (((Event_Radar_Detected_Info *)msg->data)->main_det_type == 1) ? "PW" :
                                                                                    "");

            break;
#endif
#if CONFIG_CLOUD_KEEP_ALIVE
        case EVENT_CLOUD_KEEP_ALIVE_RETRY_FAIL:
            wevt_d("EVENT: EVENT_CLOUD_KEEP_ALIVE_RETRY_FAIL received\n\r");
            break;
#endif
#if (CONFIG_11MC) || (CONFIG_11AZ)
        case EVENT_WLS_FTM_COMPLETE:
            wifi_ftm_process_event(msg->data);
            break;
#endif
        case EVENT_ACCESS_BY_HOST:
            break;
        case EVENT_REMAIN_ON_CHANNEL_EXPIRED:
            mlan_adap->remain_on_channel = MFALSE;
            /* Restore tx after remain on channel expired */
            wifi_set_tx_status(WIFI_DATA_RUNNING);

#if CONFIG_WMM
            send_wifi_driver_tx_data_event(MLAN_BSS_TYPE_STA);
            send_wifi_driver_tx_data_event(MLAN_BSS_TYPE_UAP);
#endif
            break;
        default:
            wifi_d("Event 0x%x not implemented", evt->event_id);
            break;
    }

    return WM_SUCCESS;
}

static void process_rsn_ie(t_u8 *rsn_ie,
                           _Cipher_t *mcstCipher,
                           _Cipher_t *ucstCipher,
                           t_u8 *ap_mfpc,
                           t_u8 *ap_mfpr,
                           _SecurityMode_t *WPA_WPA2_WEP)
{
    t_u8 *temp;
    t_u16 count;
    t_u16 group_cipher_count    = 0;
    t_u16 pairwise_cipher_count = 0;
    t_u16 akm_suite_count       = 0;
    t_u16 rsn_cap               = 0;
    t_u8 wpa2_oui01[4]          = {0x00, 0x0f, 0xac, 0x01};
    t_u8 wpa2_oui02[4]          = {0x00, 0x0f, 0xac, 0x02};
    t_u8 wpa2_oui04[4]          = {0x00, 0x0f, 0xac, 0x04};
    t_u8 wpa2_oui06[4]          = {0x00, 0x0f, 0xac, 0x06};

    t_u8 wpa3_oui05[4]    = {0x00, 0x0f, 0xac, 0x05};
    t_u8 wpa3_oui08[4] = {0x00, 0x0f, 0xac, 0x08};
    t_u8 wpa3_oui0b[4] = {0x00, 0x0f, 0xac, 0x0b};
    t_u8 wpa3_oui0c[4] = {0x00, 0x0f, 0xac, 0x0c};
#if CONFIG_DRIVER_OWE
    t_u8 wpa3_oui12[4] = {0x00, 0x0f, 0xac, 0x12};
#endif
#if CONFIG_11R
    t_u8 rsn_ft_1x_oui[4]  = {0x00, 0x0f, 0xac, 0x03};
    t_u8 rsn_ft_psk_oui[4] = {0x00, 0x0f, 0xac, 0x04};
    t_u8 rsn_ft_sae_oui[4] = {0x00, 0x0f, 0xac, 0x09};
    t_u8 wpa3_oui0d[4]     = {0x00, 0x0f, 0xac, 0x0d};
#endif

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
    /*  2 bytes header + 2 bytes version + 4 bytes group_cipher_suite +
     *  2 bytes pairwise_cipher_count + pairwise_cipher_count *
     * PAIRWISE_CIPHER_SUITE_LEN (4) + 2 bytes akm_suite_count +
     * akm_suite_count * AKM_SUITE_LEN (4)
     */
    count              = *(t_u16 *)(void *)(rsn_ie + 2);
    count              = wlan_le16_to_cpu(count);
    group_cipher_count = count;
    temp               = (t_u8 *)(rsn_ie + 2 + sizeof(t_u16));

    while (count > 0U)
    {
        if (!memcmp(temp, (const void *)wpa2_oui04, sizeof(wpa2_oui04)))
        {
            mcstCipher->ccmp = true;
        }
        else if (!memcmp(temp, (const void *)wpa2_oui02, sizeof(wpa2_oui02)))
        {
            mcstCipher->tkip = true;
        }
        else if (!memcmp(temp, (const void *)wpa3_oui05, sizeof(wpa3_oui05)))
        {
            mcstCipher->wep104 = true;
        }
        else if (!memcmp(temp, (const void *)wpa2_oui01, sizeof(wpa2_oui01)))
        {
            mcstCipher->wep40 = true;
        }
        else
        { /* Do nothing */
        }
        count--;
        temp += 4;
    }

    count                 = *(t_u16 *)(void *)(rsn_ie + 2 + sizeof(t_u16) + (int)group_cipher_count * 4);
    count                 = wlan_le16_to_cpu(count);
    pairwise_cipher_count = count;

    temp = (t_u8 *)(rsn_ie + 2 + sizeof(t_u16) + (int)group_cipher_count * 4 + (int)sizeof(t_u16));

    while (count > 0U)
    {
        if (!memcmp(temp, (const void *)wpa2_oui04, sizeof(wpa2_oui04)))
        {
            ucstCipher->ccmp = true;
        }
        else if (!memcmp(temp, (const void *)wpa2_oui02, sizeof(wpa2_oui02)))
        {
            ucstCipher->tkip = true;
        }
        else
        { /* Do Nothing */
        }
        count--;
        temp += 4;
    }

    count           = *(t_u16 *)(void *)(rsn_ie + 2 + sizeof(t_u16) + (int)group_cipher_count * 4 + (int)sizeof(t_u16) +
                               (int)pairwise_cipher_count * 4);
    count           = wlan_le16_to_cpu(count);
    akm_suite_count = count;

    temp = (t_u8 *)(rsn_ie + 2 + sizeof(t_u16) + (int)group_cipher_count * 4 + (int)sizeof(t_u16) +
                    (int)pairwise_cipher_count * 4 + (int)sizeof(t_u16));

    while (count > 0U)
    {
        if (memcmp(temp, wpa3_oui08, sizeof(wpa3_oui08)) == 0)
        {
            WPA_WPA2_WEP->wpa3_sae = 1;
        }
        else if (memcmp(temp, wpa3_oui0b, sizeof(wpa3_oui0b)) == 0)
        {
            WPA_WPA2_WEP->wpa3_1x_sha256 = 1;
        }
        else if (memcmp(temp, wpa3_oui0c, sizeof(wpa3_oui0c)) == 0)
        {
            WPA_WPA2_WEP->wpa3_1x_sha384 = 1;
        }
#if CONFIG_DRIVER_OWE
        else if (memcmp(temp, wpa3_oui12, sizeof(wpa3_oui12)) == 0)
        {
            WPA_WPA2_WEP->owe = 1;
        }
#endif
        else if (memcmp(temp, wpa2_oui01, sizeof(wpa2_oui01)) == 0)
        {
            WPA_WPA2_WEP->wpa2_entp = 1;
        }
        else if (memcmp(temp, wpa2_oui02, sizeof(wpa2_oui02)) == 0)
        {
            WPA_WPA2_WEP->wpa2 = 1;
        }
        else if (memcmp(temp, wpa3_oui05, sizeof(wpa3_oui05)) == 0)
        {
            WPA_WPA2_WEP->wpa3_entp = 1;
        }
        else if (memcmp(temp, wpa2_oui06, sizeof(wpa2_oui06)) == 0)
        {
            WPA_WPA2_WEP->wpa2_sha256 = 1;
        }

#if CONFIG_11R
        else if (memcmp(temp, rsn_ft_1x_oui, sizeof(rsn_ft_1x_oui)) == 0)
        {
            WPA_WPA2_WEP->ft_1x = 1;
            WPA_WPA2_WEP->wpa2  = 1;
        }
        else if (memcmp(temp, wpa3_oui0d, sizeof(wpa3_oui0d)) == 0)
        {
            WPA_WPA2_WEP->ft_1x_sha384 = 1;
        }
        else if (memcmp(temp, rsn_ft_psk_oui, sizeof(rsn_ft_psk_oui)) == 0)
        {
            WPA_WPA2_WEP->ft_psk = 1;
            WPA_WPA2_WEP->wpa2   = 1;
        }
        else if (memcmp(temp, rsn_ft_sae_oui, sizeof(rsn_ft_sae_oui)) == 0)
        {
            WPA_WPA2_WEP->ft_sae   = 1;
            WPA_WPA2_WEP->wpa3_sae = 1;
        }
#endif
        else
        { /* Do Nothing */
        }
        temp += 4;
        count--;
    }

    rsn_cap = *(t_u16 *)(void *)(rsn_ie + 2 + sizeof(t_u16) + 4 * (int)sizeof(t_u8) + (int)sizeof(t_u16) +
                                 (int)pairwise_cipher_count * 4 + (int)sizeof(t_u16) + (int)akm_suite_count * 4);
    rsn_cap = (t_u16)wlan_le16_to_cpu(rsn_cap);

    *ap_mfpc = ((rsn_cap & (0x1 << MFPC_BIT)) == (0x1 << MFPC_BIT));
    *ap_mfpr = ((rsn_cap & (0x1 << MFPR_BIT)) == (0x1 << MFPR_BIT));
done:
    LEAVE();
}

static void process_wpa_ie(t_u8 *wpa_ie,
                           _Cipher_t *mcstCipher,
                           _Cipher_t *ucstCipher,
                           t_u8 *ap_mfpc,
                           t_u8 *ap_mfpr,
                           _SecurityMode_t *WPA_WPA2_WEP)
{
    t_u16 wpa_ie_len;
    t_u8 *temp;
    t_u8 *ptr;
    t_u8 *wpa_ie_len_ptr;
    t_u8 *pairwise_cipher_count_ptr;
    t_u16 count;
    t_u16 group_cipher_count    = 0;
    t_u16 pairwise_cipher_count = 0;
    t_u8 wpa_oui01[4]           = {0x00, 0x50, 0xf2, 0x01};
    t_u8 wpa_oui02[4]           = {0x00, 0x50, 0xf2, 0x02};
    t_u8 wpa_oui04[4]           = {0x00, 0x50, 0xf2, 0x04};
    t_u8 wpa_oui05[4]           = {0x00, 0x50, 0xf2, 0x05};

    ENTER();

    if (wpa_ie == MNULL)
    {
        goto done;
    }
    else
    {
        /* Do nothing */
    }

    if (wpa_ie[0] != (t_u8)WPA_IE)
    {
        goto done;
    }
    else
    {
        /* Do nothing */
    }
    /*  2 bytes header + 4 bytes oui + 2 bytes version + 4 bytes group_cipher_suite +
     *  2 bytes pairwise_cipher_count + pairwise_cipher_count *
     * PAIRWISE_CIPHER_SUITE_LEN (4) + 2 bytes akm_suite_count +
     * akm_suite_count * AKM_SUITE_LEN (4)
     */
    ptr            = (t_u8 *)(wpa_ie + 1);
    wpa_ie_len_ptr = ptr;

    wpa_ie_len = wlan_le16_to_cpu(*(t_u16 *)ptr);

    count              = *(t_u16 *)(void *)(wpa_ie + 2 + 4);
    count              = wlan_le16_to_cpu(count);
    group_cipher_count = count;
    temp               = (t_u8 *)(wpa_ie + 2 + 4 + sizeof(t_u16));

    while (count > 0U)
    {
        if (!memcmp(temp, (const void *)wpa_oui04, sizeof(wpa_oui04)))
        {
            mcstCipher->ccmp = true;
        }
        else if (!memcmp(temp, (const void *)wpa_oui02, sizeof(wpa_oui02)))
        {
            mcstCipher->tkip = true;
        }
        else if (!memcmp(temp, (const void *)wpa_oui05, sizeof(wpa_oui05)))
        {
            mcstCipher->wep104 = true;
        }
        else if (!memcmp(temp, (const void *)wpa_oui01, sizeof(wpa_oui01)))
        {
            mcstCipher->wep40 = true;
        }
        else
        { /* Do nothing */
        }
        count--;
        temp += 4;
    }

    ptr                       = (t_u8 *)(wpa_ie + 2 + 4 + sizeof(t_u16) + (int)group_cipher_count * 4);
    pairwise_cipher_count_ptr = ptr;

    count                 = *(t_u16 *)(void *)ptr;
    count                 = wlan_le16_to_cpu(count);
    pairwise_cipher_count = count;
    temp                  = (t_u8 *)(wpa_ie + 2 + 4 + sizeof(t_u16) + (int)group_cipher_count * 4 + (int)sizeof(t_u16));

    while (count > 0U)
    {
        if (!memcmp(temp, (const void *)wpa_oui04, sizeof(wpa_oui04)))
        {
            ucstCipher->ccmp = true;
        }
        else if (!memcmp(temp, (const void *)wpa_oui02, sizeof(wpa_oui02)))
        {
            ucstCipher->tkip = true;
        }
        else
        { /* Do Nothing */
        }
        count--;
        temp += 4;
    }

    if (pairwise_cipher_count == 2)
    {
        ptr           = wpa_ie_len_ptr;
        *(t_u16 *)ptr = wlan_cpu_to_le16(wpa_ie_len - 4);
        ptr           = pairwise_cipher_count_ptr;
        *(t_u16 *)ptr = wlan_cpu_to_le16(1);
        temp          = (t_u8 *)(wpa_ie + 2 + 4 + sizeof(t_u16) + (int)group_cipher_count * 4 + (int)sizeof(t_u16));

        if (!memcmp((temp + 4), (const void *)wpa_oui04, sizeof(wpa_oui04)))
        {
            memmove(temp, (temp + 4), (4 + sizeof(t_u16) + 4));
        }
        else
        {
            /* 2 bytes header + 4 bytes oui + 2 bytes version + 4 bytes group_cipher_suite +
             *  2 bytes pairwise_cipher_count + pairwise_cipher_1 (4) +
             *  pairwise_cipher_2 (4) + 2 bytes akm_suite_count +
             *  akm_suite_count * AKM_SUITE_LEN (4)
             *
             *  Here move memory of 4bytes(pairwise_cipher_2) if the cipher type is not wpa_oui04,
             *  dest = (temp + 4) and src = (temp + 8),
             *  this memmove will keep akm data intact */
            memmove((temp + 4), (temp + 8), (sizeof(t_u16) + 4));
        }
    }
done:
    LEAVE();
}

int wrapper_bssdesc_first_set(int bss_index,
                              uint8_t *BssId,
                              bool *is_ibss_bit_set,
                              int *ssid_len,
                              uint8_t *ssid,
                              uint8_t *Channel,
                              uint8_t *RSSI,
                              uint16_t *beacon_period,
                              uint16_t *dtim_period,
                              _SecurityMode_t *WPA_WPA2_WEP,
                              _Cipher_t *wpa_mcstCipher,
                              _Cipher_t *wpa_ucstCipher,
                              _Cipher_t *rsn_mcstCipher,
                              _Cipher_t *rsn_ucstCipher,
                              t_u8 *ap_mfpc,
                              t_u8 *ap_mfpr,
                              t_u8 *ap_pwe)
{
    uint8_t i = WLAN_SUPPORTED_RATES;
    if (bss_index >= (int)mlan_adap->num_in_scan_table)
    {
        wifi_w("Unable to find given entry %d in BSS table", bss_index);
        return -WM_FAIL;
    }

    /* TODO
     * Removed : const BSSDescriptor_t *d = &mlan_adap->pscan_table[bss_index]; */
    BSSDescriptor_t *d = &mlan_adap->pscan_table[bss_index];

    (void)memcpy((void *)BssId, (const void *)d->mac_address, MLAN_MAC_ADDR_LENGTH);

    if (d->cap_info.ibss != 0U)
    {
        *is_ibss_bit_set = true;
    }
    else
    {
        *is_ibss_bit_set = false;
    }

    if (d->ssid.ssid_len <= MLAN_MAX_SSID_LENGTH)
    {
        *ssid_len = (int)d->ssid.ssid_len;
        (void)memcpy((void *)ssid, (const void *)d->ssid.ssid, d->ssid.ssid_len);
    }

    *Channel = d->channel; /*!< Channel associated to the BSSID */

    *RSSI = (uint8_t)d->rssi;
    /* uint8_t RSSI; */ /*!< Received signal strength */
    *beacon_period = d->beacon_period;
    *dtim_period   = d->dtim_period;

    if (d->pwpa_ie != MNULL || d->prsn_ie != MNULL)
    {
        if (d->pwpa_ie != MNULL)
        {
            WPA_WPA2_WEP->wpa = 1;

            process_wpa_ie(d->wpa_ie_buff, wpa_mcstCipher, wpa_ucstCipher, ap_mfpc, ap_mfpr, WPA_WPA2_WEP);
        }

        if (d->prsn_ie != MNULL)
        {
            process_rsn_ie(d->rsn_ie_buff, rsn_mcstCipher, rsn_ucstCipher, ap_mfpc, ap_mfpr, WPA_WPA2_WEP);
        }
    }
    else
    {
        /* Check if WEP */
        if (d->cap_info.privacy != 0U)
        {
            WPA_WPA2_WEP->wepStatic = 1;
        }
    }

    if ((d->prsnx_ie != MNULL) && (d->prsnx_ie->data[0] & (0x1 << SAE_H2E_BIT)))
    {
        *ap_pwe = 2;
        for (i = WLAN_SUPPORTED_RATES; i > 0; i--)
        {
            if (d->data_rates[i - 1] == 0xFB)
            {
                *ap_pwe = 1;
                break;
            }
        }
    }
    else
    {
        *ap_pwe = 0;
    }

    return WM_SUCCESS;
}

int wrapper_bssdesc_second_set(int bss_index,
                               bool *phtcap_ie_present,
                               bool *phtinfo_ie_present,
#if CONFIG_11AC
                               bool *pvhtcap_ie_present,
#endif
#if CONFIG_11AX
                               bool *phecap_ie_present,
#endif
                               bool *wmm_ie_present,
                               uint16_t *band,
                               bool *wps_IE_exist,
                               uint16_t *wps_session,
                               bool *wpa2_entp_IE_exist,
#if CONFIG_11R
                               uint16_t *mdid,
#endif
#if CONFIG_11K
                               bool *neighbor_report_supported,
#endif
#if CONFIG_11V
                               bool *bss_transition_supported,
#endif
                               uint8_t *trans_mode,
                               uint8_t *trans_bssid,
                               int *trans_ssid_len,
                               uint8_t *trans_ssid
#if CONFIG_DRIVER_MBO
                               ,
                               bool *mbo_assoc_disallowed
#endif
)
{
    if (bss_index >= mlan_adap->num_in_scan_table)
    {
        wifi_w("Unable to find given entry %d in BSS table", bss_index);
        return -WM_FAIL;
    }
    const BSSDescriptor_t *d = &mlan_adap->pscan_table[bss_index];
#if CONFIG_11R
    IEEEtypes_MobilityDomain_t *pmd_ie;
#endif

    if (d->pht_cap != NULL)
    {
        *phtcap_ie_present = true;
    }
    else
    {
        *phtcap_ie_present = false;
    }

    if (d->pht_info != NULL)
    {
        *phtinfo_ie_present = true;
    }
    else
    {
        *phtinfo_ie_present = false;
    }
#if CONFIG_11AC
    if (d->pvht_cap != NULL)
    {
        *pvhtcap_ie_present = true;
    }
    else
    {
        *pvhtcap_ie_present = false;
    }
#endif
#if CONFIG_11AX
    if (d->phe_cap != NULL)
    {
        *phecap_ie_present = true;
    }
    else
    {
        *phecap_ie_present = false;
    }
#endif
    if (d->wmm_ie.vend_hdr.element_id == WMM_IE)
    {
        *wmm_ie_present = true;
    }
    else
    {
        *wmm_ie_present = false;
    }

#if CONFIG_DRIVER_MBO
    *mbo_assoc_disallowed = d->mbo_assoc_disallowed;
#endif

    *band = d->bss_band;

    if (wps_IE_exist != NULL)
    {
        *wps_IE_exist = d->wps_IE_exist;
    }
    if (wps_session != NULL)
    {
        *wps_session = d->wps_session;
    }

    if (wpa2_entp_IE_exist != NULL)
    {
        *wpa2_entp_IE_exist = d->wpa2_entp_IE_exist;
    }

#if CONFIG_11R
    if (mdid != NULL)
    {
        pmd_ie = (IEEEtypes_MobilityDomain_t *)d->md_ie_buff;
        *mdid  = pmd_ie->mdid;
    }
#endif
#if CONFIG_11K
    if (neighbor_report_supported != NULL)
    {
        *neighbor_report_supported = (bool)d->rm_cap_saved.RrmEnabledCapabilities.NborRpt;
    }
#endif
#if CONFIG_11V
    if (bss_transition_supported != NULL)
    {
        *bss_transition_supported = (bool)d->ext_cap_saved.ext_cap.BSS_Transition;
    }
#endif

#if CONFIG_DRIVER_OWE
    *trans_mode = d->owe_transition_mode;
#endif
    (void)memcpy((void *)trans_bssid, (const void *)d->trans_mac_address, MLAN_MAC_ADDR_LENGTH);

    if (d->trans_ssid.ssid_len <= MLAN_MAX_SSID_LENGTH)
    {
        *trans_ssid_len = (int)d->trans_ssid.ssid_len;
        (void)memcpy((void *)trans_ssid, (const void *)d->trans_ssid.ssid, d->trans_ssid.ssid_len);
    }

    return WM_SUCCESS;
}

int wifi_get_scan_result_count(unsigned *count)
{
    if (count == MNULL)
    {
        return -WM_E_INVAL;
    }

    *count = mlan_adap->num_in_scan_table;
    return WM_SUCCESS;
}

int wrapper_wlan_set_regiontable(t_u8 region, t_u16 band)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    mlan_status rv       = wlan_set_regiontable(pmpriv, region, band);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

/*
 * This function is supposed to be called after scan is complete.
 */
void wifi_scan_process_results(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    /*
     * This function is normally supposed to be called from inside
     * mlan_scan.c during parsing of scanning results. But we do not
     * have async. cmd send functionality in the current design. So, we
     * have moved this outside.
     */
    wlan_scan_process_results(pmpriv);
}

#if CONFIG_WPA2_ENTP
void wifi_scan_enable_wpa2_enterprise_ap_only()
{
    scan_enable_wpa2_enterprise_ap_only = true;
}

bool wifi_get_scan_enable_wpa2_enterprise_ap_only()
{
    return scan_enable_wpa2_enterprise_ap_only;
}
#endif

bool check_for_wpa2_entp_ie(bool *wpa2_entp_IE_exist, const void *element_data, unsigned element_len)
{
    uint8_t wpa2_ent_IE[4];
    uint16_t len;

    (void)memcpy((void *)&len, (const void *)element_data, sizeof(len));

    if (len * 4U >= element_len)
    {
        return false;
    }

    (void)memcpy((void *)&wpa2_ent_IE, (const void *)((const char *)element_data + len * 4U + 2U * sizeof(len)), 4U);

    if (!memcmp(wpa2_ent_IE, wpa2_akmp_oui, sizeof(wpa2_akmp_oui)))
    {
        /* WPA2 Enterprise IE is present in probe response. */
        wifi_d("WPA2 Enterprise AKMP IE :: %x:%x:%x:%x\r\n", wpa2_ent_IE[0], wpa2_ent_IE[1], wpa2_ent_IE[2],
               wpa2_ent_IE[3]);

        *wpa2_entp_IE_exist = true;

        return true;
    }

    return false;
}

/*
 * The below wifi authenticate code, data structures and macros seems to be
 * completely absent from mlan. Our current code, including the wlc manager
 * currently depends on this command and its response (though it may be a
 * no-op).
 *
 * fixme: remove this later when appropriate.
 */
#define host_CMD_802_11_AUTHENTICATE 0x0011
#define AUTH_OPEN_SYSTEM             0

typedef PACK_START struct host_802_11Authenticate_t
{
    uint8_t MacAddr[MLAN_MAC_ADDR_LENGTH];
    uint8_t AuthType;
    uint16_t TimeOut;
    uint8_t Reserved[2];
} PACK_END host_802_11Authenticate_t;

void wifi_get_value1_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint32_t *dev_value1)
{
    if (resp->params.mac_reg.offset == wlan_cpu_to_le16((t_u16)WLAN_VALUE1))
    {
        *dev_value1 = resp->params.mac_reg.value;
    }
}

/*
 * fixme: This function will be present till mlan integration is complete
 */
void wifi_get_mac_address_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *get_mac_addr)
{
    (void)memcpy((void *)get_mac_addr, (const void *)&resp->params.mac_addr.mac_addr, MLAN_MAC_ADDR_LENGTH);
}

void wifi_get_firmware_ver_ext_from_cmdresp(const HostCmd_DS_COMMAND *resp, uint8_t *fw_ver_ext)
{
    uint8_t comma = 0x2C, space = 0x20;
#ifdef RW610
    uint8_t ver_str_len = resp->size - WIFI_HOST_CMD_FIXED_HEADER_LEN - sizeof(resp->params.verext.version_str_sel);
#endif

    if (!resp->params.verext.version_str_sel)
    {
        /* TODO: Below change is added to change 8978 firmware name to IW416.
         * This change is temporary and can be removed once firmware changes are in place */
        if (strstr((const char *)&resp->params.verext.version_str, "w8978o") != NULL)
        {
            (void)memcpy((void *)fw_ver_ext, (const void *)"IW416", 6);
            (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)),
                         (const void *)&resp->params.verext.version_str[6],
                         strlen((const char *)(&resp->params.verext.version_str)) - strlen((const char *)fw_ver_ext));
        }
        else
        {
            (void)memcpy((void *)fw_ver_ext, (const void *)&resp->params.verext.version_str,
#ifdef RW610
                         ver_str_len
#else
                         strlen((const char *)(&resp->params.verext.version_str))
#endif
            );
        }
    }
    else if (resp->params.verext.version_str_sel == 3U && strlen((const char *)(&resp->params.verext.version_str)))
    {
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&comma, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&space, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)),
                     (const void *)&resp->params.verext.version_str,
#ifdef RW610
                     ver_str_len
#else
                     strlen((const char *)(&resp->params.verext.version_str))
#endif
        );
    }
    else if (resp->params.verext.version_str_sel == 4U && strlen((const char *)(&resp->params.verext.version_str)))
    {
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&comma, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)), (const void *)&space, 1);
        (void)memcpy((void *)(fw_ver_ext + strlen((const char *)fw_ver_ext)),
                     (const void *)&resp->params.verext.version_str,
#ifdef RW610
                     ver_str_len
#else
                     strlen((const char *)(&resp->params.verext.version_str))
#endif
        );
    }
    else
    { /* Do Nothing */
    }
}

void wifi_prepare_set_cal_data_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_CFG_DATA;
    cmd->size    = (t_u16)sizeof(HostCmd_DS_802_11_CFG_DATA) + (t_u16)S_DS_GEN + (t_u16)cal_data_len - (t_u16)1;
    cmd->seq_num = seq_number;
    cmd->result  = 0;
    cmd->params.cfg_data.action   = HostCmd_ACT_GEN_SET;
    cmd->params.cfg_data.type     = 0x02;
    cmd->params.cfg_data.data_len = cal_data_len;
    (void)memcpy((void *)cmd->params.cfg_data.data, (const void *)cal_data, cal_data_len);
}

#ifdef OTP_CHANINFO
void wifi_prepare_get_channel_region_cfg_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command               = HostCmd_CMD_CHAN_REGION_CFG;
    cmd->size                  = sizeof(HostCmd_DS_CHAN_REGION_CFG) + S_DS_GEN;
    cmd->seq_num               = seq_number;
    cmd->result                = 0;
    cmd->params.reg_cfg.action = HostCmd_ACT_GEN_GET;
}
#endif

#if CONFIG_COMPRESS_TX_PWTBL
int wifi_set_region_power_cfg(const t_u8 *data, t_u16 len)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, 0 /* bss_type */);
    cmd->result  = 0x0;
    cmd->size    = len + S_DS_GEN;

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_REGION_POWER_CFG, HostCmd_ACT_GEN_SET, 0,
                             NULL, (void *)data, cmd);
    wifi_wait_for_cmdresp(NULL);
    return WM_SUCCESS;
}
#endif

void wifi_prepare_get_hw_spec_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_GET_HW_SPEC;
    cmd->size    = sizeof(HostCmd_DS_GET_HW_SPEC) + S_DS_GEN;
    cmd->seq_num = seq_number;
    cmd->result  = 0;
}

void wifi_prepare_reconfigure_tx_buf_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_RECONFIGURE_TX_BUFF;
    /* TODO: Replace hardcoded size with logical implementation */
    cmd->size                    = 16;
    cmd->seq_num                 = seq_number;
    cmd->result                  = 0;
    cmd->params.tx_buf.action    = HostCmd_ACT_GEN_SET;
    cmd->params.tx_buf.buff_size = 2048;
}

/*
 * fixme: This function will be present till mlan integration is complete
 */
void wifi_prepare_get_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command                = HostCmd_CMD_802_11_MAC_ADDRESS;
    cmd->size                   = sizeof(HostCmd_DS_802_11_MAC_ADDRESS) + S_DS_GEN;
    cmd->seq_num                = seq_number;
    cmd->result                 = 0;
    cmd->params.mac_addr.action = HostCmd_ACT_GEN_GET;
}

void wifi_prepare_get_fw_ver_ext_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number, int version_str_sel)
{
    cmd->command                       = HostCmd_CMD_VERSION_EXT;
    cmd->size                          = sizeof(HostCmd_DS_VERSION_EXT) + S_DS_GEN;
    cmd->seq_num                       = seq_number;
    cmd->result                        = 0;
    cmd->params.verext.version_str_sel = version_str_sel;
}

void wifi_prepare_get_value1(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command               = HostCmd_CMD_MAC_REG_ACCESS;
    cmd->command               = wlan_cpu_to_le16(cmd->command);
    cmd->size                  = wlan_cpu_to_le16(sizeof(HostCmd_DS_MAC_REG_ACCESS) + S_DS_GEN);
    cmd->seq_num               = 0x0;
    cmd->result                = 0x0;
    cmd->params.mac_reg.action = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
    cmd->params.mac_reg.offset = wlan_cpu_to_le16((t_u16)WLAN_VALUE1);
    cmd->params.mac_reg.value  = 0x0;
}

void wifi_prepare_set_mac_addr_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command                = HostCmd_CMD_802_11_MAC_ADDRESS;
    cmd->size                   = sizeof(HostCmd_DS_802_11_MAC_ADDRESS) + S_DS_GEN;
    cmd->seq_num                = seq_number;
    cmd->result                 = 0;
    cmd->params.mac_addr.action = HostCmd_ACT_GEN_SET;
    (void)memcpy((void *)cmd->params.mac_addr.mac_addr, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
}

#if CONFIG_WIFI_TX_BUFF
void wifi_prepare_set_tx_buf_size(HostCmd_DS_COMMAND *cmd, int seq_number)
{
    cmd->command                 = HostCmd_CMD_RECONFIGURE_TX_BUFF;
    cmd->size                    = sizeof(HostCmd_DS_TXBUF_CFG) + S_DS_GEN;
    cmd->seq_num                 = seq_number;
    cmd->result                  = 0;
    cmd->params.tx_buf.action    = HostCmd_ACT_GEN_SET;
    cmd->params.tx_buf.buff_size = tx_buf_size;
}
#endif

void wifi_prepare_enable_amsdu_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command                              = HostCmd_CMD_AMSDU_AGGR_CTRL;
    cmd->size                                 = sizeof(HostCmd_DS_AMSDU_AGGR_CTRL) + S_DS_GEN;
    cmd->seq_num                              = seq_number;
    cmd->result                               = 0;
    cmd->params.amsdu_aggr_ctrl.action        = HostCmd_ACT_GEN_SET;
    cmd->params.amsdu_aggr_ctrl.enable        = 0x1;
    cmd->params.amsdu_aggr_ctrl.curr_buf_size = 0x0;
}

#ifdef WLAN_LOW_POWER_ENABLE
void wifi_prepare_low_power_mode_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_LOW_POWER_MODE;
    cmd->size    = sizeof(HostCmd_CONFIG_LOW_PWR_MODE) + S_DS_GEN;
    cmd->seq_num = seq_number;
    cmd->result  = 0;

    cmd->params.low_pwr_mode_cfg.enable = 1;

    return;
}
#endif

void wlan_prepare_mac_control_cmd(HostCmd_DS_COMMAND *cmd, t_u16 seq_number)
{
    cmd->command = HostCmd_CMD_MAC_CONTROL;
    cmd->size    = sizeof(HostCmd_DS_MAC_CONTROL) + S_DS_GEN;
    cmd->seq_num = seq_number;
    cmd->result  = 0;

    cmd->params.mac_ctrl.action =
#if CONFIG_11AC
        HostCmd_ACT_MAC_STATIC_DYNAMIC_BW_ENABLE |
#endif
        HostCmd_ACT_MAC_RTS_CTS_ENABLE | HostCmd_ACT_MAC_RX_ON | HostCmd_ACT_MAC_TX_ON |
        HostCmd_ACT_MAC_ETHERNETII_ENABLE;

    return;
}

void wifi_set_cal_data(const uint8_t *cdata, const unsigned int clen)
{
    cal_data       = (uint8_t *)cdata;
    cal_data_len   = (unsigned int)clen;
    cal_data_valid = true;
}

void wifi_set_mac_addr(uint8_t *mac)
{
    mac_addr       = mac;
    mac_addr_valid = true;
}

void _wifi_set_mac_addr(const uint8_t *mac, mlan_bss_type bss_type)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, bss_type);

    cmd->result = 0x0;
    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_MAC_ADDRESS,
                                   HostCmd_ACT_GEN_SET, 0, NULL, (uint8_t *)mac, cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    /* Also need to update priv->curr_addr, as rx reorder will check mac address using priv->curr_addr */
    if (bss_type == MLAN_BSS_TYPE_STA)
    {
        (void)memcpy(&mlan_adap->priv[0]->curr_addr[0], &mac[0], MLAN_MAC_ADDR_LENGTH);
#if CONFIG_WPA_SUPP
        if (wm_wifi.supp_if_callbk_fns && wm_wifi.supp_if_callbk_fns->mac_changed_callbk_fn)
        {
            wm_wifi.supp_if_callbk_fns->mac_changed_callbk_fn(wm_wifi.if_priv);
        }
#endif
    }
#if UAP_SUPPORT
    else if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        (void)memcpy(&mlan_adap->priv[1]->curr_addr[0], &mac[0], MLAN_MAC_ADDR_LENGTH);
#if CONFIG_WPA_SUPP
        if (wm_wifi.supp_if_callbk_fns && wm_wifi.supp_if_callbk_fns->mac_changed_callbk_fn)
        {
            wm_wifi.supp_if_callbk_fns->mac_changed_callbk_fn(wm_wifi.hapd_if_priv);
        }
#endif
    }
#endif
}

#if CONFIG_WMM_UAPSD
/**
 *  @brief This function checks if there is no packet indication.
 *
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return        true or false
 */
t_u8 wifi_check_no_packet_indication(mlan_private *priv)
{
    if ((wifi_wmm_get_packet_cnt() == 0) && priv->wmm_qosinfo && priv->curr_bss_params.wmm_uapsd_enabled)
        return true;
    else
        return false;
}

/**
 *  @brief This function checks if we need to send last packet indication.
 *
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return        true or false
 */
t_u8 wifi_check_last_packet_indication(mlan_private *priv)
{
    if ((wifi_wmm_get_packet_cnt() == 1) && priv->wmm_qosinfo && priv->curr_bss_params.wmm_uapsd_enabled)
        return true;
    else
        return false;
}

int wifi_wmm_qos_cfg(t_u8 *qos_cfg, t_u8 action)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req req;
    mlan_ds_wmm_cfg cfg;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    (void)memset(&cfg, 0x00, sizeof(mlan_ds_wmm_cfg));
    cfg.sub_command = MLAN_OID_WMM_CFG_QOS;
    if (action == HostCmd_ACT_GEN_SET)
        cfg.param.qos_cfg = *qos_cfg;
    req.pbuf    = (t_u8 *)&cfg;
    req.buf_len = sizeof(mlan_ds_wmm_cfg);
    req.req_id  = MLAN_IOCTL_WMM_CFG;

    if (action == HostCmd_ACT_GEN_SET)
        req.action = MLAN_ACT_SET;
    else if (action == HostCmd_ACT_GEN_GET)
        req.action = MLAN_ACT_GET;
    ret = wlan_ops_sta_ioctl(mlan_adap, &req);

    if (action == HostCmd_ACT_GEN_GET)
        *qos_cfg = cfg.param.qos_cfg;
    return ret;
}

int wifi_sleep_period(unsigned int *sleep_period, int action)
{
    int ret = WM_SUCCESS;
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, MLAN_BSS_TYPE_STA);
    cmd->result  = 0x0;
    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SLEEP_PERIOD, action, 0, NULL,
                             sleep_period, cmd);
    if (action == HostCmd_ACT_GEN_SET)
        ret = wifi_wait_for_cmdresp(NULL);
    else if (action == HostCmd_ACT_GEN_GET)
    {
        mlan_ds_pm_cfg pm_cfg;
        mlan_ioctl_req pioctl_buf;
        pioctl_buf.pbuf = (t_u8 *)&pm_cfg;
        memset((t_u8 *)&pioctl_buf, 0, sizeof(pioctl_buf));
        memset((t_u8 *)&pm_cfg, 0, sizeof(pm_cfg));
        ret           = wifi_wait_for_cmdresp(&pioctl_buf);
        pm_cfg        = *(mlan_ds_pm_cfg *)pioctl_buf.pbuf;
        *sleep_period = pm_cfg.param.sleep_period;
    }
    return ret;
}
#endif

#if CONFIG_WIFI_TX_BUFF
bool wifi_calibrate_tx_buf_size(uint16_t buf_size)
{
    uint16_t pre_tx_buf_size = tx_buf_size;
    tx_buf_size              = MLAN_TX_DATA_BUF_SIZE_2K;

#if defined(RW610)
    if (buf_size >= MLAN_TX_DATA_BUF_SIZE_4K)
        tx_buf_size = MLAN_TX_DATA_BUF_SIZE_4K;
    _wlan_return_all_tx_buf(kIMU_LinkCpu1Cpu3);
#else
    if (buf_size >= MLAN_TX_DATA_BUF_SIZE_8K)
        tx_buf_size = MLAN_TX_DATA_BUF_SIZE_8K;
    else if (buf_size >= MLAN_TX_DATA_BUF_SIZE_4K)
        tx_buf_size = MLAN_TX_DATA_BUF_SIZE_4K;
#endif

    return (tx_buf_size != pre_tx_buf_size) ? true : false;
}

void wifi_recfg_tx_buf_size(uint16_t buf_size)
{
    tx_buf_size = buf_size;
}

void _wifi_recfg_tx_buf_size(uint16_t buf_size, mlan_bss_type bss_type)
{
    if (!wifi_calibrate_tx_buf_size(buf_size))
        return;

    wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);
    cmd->result  = 0x0;

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_RECONFIGURE_TX_BUFF, HostCmd_ACT_GEN_SET,
                             0, NULL, &tx_buf_size, cmd);
    wifi_wait_for_cmdresp(NULL);
}

#endif

#if CONFIG_WIFI_TX_PER_TRACK
int wifi_set_tx_pert(void *cfg, mlan_bss_type bss_type)
{
    tx_pert_info *tx_pert = (tx_pert_info *)cfg;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    /* Store tx per tracking config in driver */
    (void)memcpy((t_u8 *)&(mlan_adap->tx_pert), tx_pert, sizeof(tx_pert_info));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);
    cmd->result  = 0x0;
    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_TX_RX_PKT_STATS,
                             HostCmd_ACT_SET_TX_PER_TRACKING, 0, NULL, tx_pert, cmd);
    wifi_wait_for_cmdresp(NULL);
    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_TX_RX_HISTOGRAM
int wifi_set_txrx_histogram(void *cfg, t_u8 *data)
{
    txrx_histogram_info *txrx_histogram    = (txrx_histogram_info *)cfg;
    txrx_histogram_info txrx_histogram_cmd = {0};

    txrx_histogram_cmd.enable = txrx_histogram->enable;
    txrx_histogram_cmd.action = txrx_histogram->action;

    if (txrx_histogram_cmd.enable == GET_TX_RX_HISTOGRAM)
    {
        txrx_histogram_cmd.enable = ENABLE_TX_RX_HISTOGRAM;
    }
    else
    {
        txrx_histogram_cmd.action = 0;
    }

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_TX_RX_PKT_STATS, HostCmd_ACT_GEN_GET, 0,
                             NULL, &txrx_histogram_cmd, cmd);
    wifi_wait_for_cmdresp(data);
    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_RX_ABORT_CFG
int wifi_set_get_rx_abort_cfg(void *cfg, t_u16 action)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    cmd->seq_num            = 0x0;
    cmd->result             = 0x0;
    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_RX_ABORT_CFG, action, 0, NULL, cfg, cmd);
    wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? cfg : NULL);
    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_RX_ABORT_CFG_EXT
int wifi_set_get_rx_abort_cfg_ext(void *cfg, t_u16 action)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_RX_ABORT_CFG_EXT, action, 0, NULL, cfg,
                             cmd);
    wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? cfg : NULL);
    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_CCK_DESENSE_CFG
int wifi_set_get_cck_desense_cfg(void *cfg, t_u16 action)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    cmd->seq_num            = 0x0;
    cmd->result             = 0x0;
    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_CCK_DESENSE_CFG, action, 0, NULL, cfg,
                             cmd);
    wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? cfg : NULL);
    return wm_wifi.cmd_resp_status;
}
#endif

#ifdef WLAN_LOW_POWER_ENABLE
void wifi_enable_low_pwr_mode()
{
    low_power_mode = true;
}
#endif

#if CONFIG_ROAMING
int wifi_config_roaming(const int enable, uint8_t *rssi_low)
{
    mlan_private *pmpriv = mlan_adap->priv[0];
    int ret              = WM_SUCCESS;

    if (enable)
    {
        pmpriv->roaming_enabled = MTRUE;
        pmpriv->rssi_low        = *rssi_low;
        ret                     = wifi_set_rssi_low_threshold(rssi_low);
        if (ret != WM_SUCCESS)
        {
            wifi_e("Failed to config rssi threshold for roaming");
            return -WM_FAIL;
        }
    }
    else
    {
        if (pmpriv->roaming_enabled)
        {
            pmpriv->roaming_enabled = MFALSE;
            pmpriv->rssi_low        = 0;
            wifi_stop_bgscan();
        }
    }
    return ret;
}
#endif

#if CONFIG_11AX
int wifi_set_11ax_tx_omi(const mlan_bss_type bss_type,
                         const t_u16 tx_omi,
                         const t_u8 tx_option,
                         const t_u8 num_data_pkts)
{
    mlan_ioctl_req req;

    mlan_ds_11ax_cmd_cfg cfg;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    (void)memset(&cfg, 0x00, sizeof(mlan_ds_11ax_cmd_cfg));

    req.req_id  = MLAN_IOCTL_11AX_CFG;
    req.action  = MLAN_ACT_SET;
    req.pbuf    = (t_u8 *)&cfg;
    req.buf_len = sizeof(mlan_ds_11ax_cmd_cfg);

    cfg.sub_command                   = MLAN_OID_11AX_CMD_CFG;
    cfg.sub_id                        = MLAN_11AXCMD_TXOMI_SUBID;
    cfg.param.txomi_cfg.omi           = tx_omi;
    cfg.param.txomi_cfg.tx_option     = tx_option;
    cfg.param.txomi_cfg.num_data_pkts = num_data_pkts;

    mlan_status rv;

    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        req.bss_index = (t_u32)MLAN_BSS_TYPE_UAP;
        rv            = wlan_ops_uap_ioctl(mlan_adap, &req);
    }
    else
    {
        req.bss_index = (t_u32)MLAN_BSS_TYPE_STA;
        rv            = wlan_ops_sta_ioctl(mlan_adap, &req);
    }

    wm_wifi.cmd_resp_ioctl = NULL;

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_11ax_tol_time(const t_u32 tol_time)
{
    mlan_ioctl_req req;

    mlan_ds_11ax_cmd_cfg cfg;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    (void)memset(&cfg, 0x00, sizeof(mlan_ds_11ax_cmd_cfg));

    req.req_id  = MLAN_IOCTL_11AX_CFG;
    req.action  = MLAN_ACT_SET;
    req.pbuf    = (t_u8 *)&cfg;
    req.buf_len = sizeof(mlan_ds_11ax_cmd_cfg);

    cfg.sub_command                = MLAN_OID_11AX_CMD_CFG;
    cfg.sub_id                     = MLAN_11AXCMD_OBSS_TOLTIME_SUBID;
    cfg.param.toltime_cfg.tol_time = tol_time;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
        return MLAN_STATUS_FAILURE;

    return MLAN_STATUS_SUCCESS;
}

int wifi_set_11ax_rutxpowerlimit(const void *rutx_pwr_cfg, uint32_t rutx_pwr_cfg_len)
{
    int ret;
    uint32_t reqd_len = 0;

#if !CONFIG_MEM_POOLS
    void *rutxcmd_resp_buff = OSA_MemoryAllocate(RUTXCMD_RESP_BUFF_SIZE);
#else
    void *rutxcmd_resp_buff = OSA_MemoryPoolAllocate(buf_1024_MemoryPool);
#endif

    if (rutxcmd_resp_buff == NULL)
    {
        return -WM_FAIL;
    }

    ret = wlan_send_hostcmd(rutx_pwr_cfg, rutx_pwr_cfg_len / sizeof(uint8_t), rutxcmd_resp_buff, RUTXCMD_RESP_BUFF_SIZE,
                            &reqd_len);

    if (ret != WM_SUCCESS)
    {
        wifi_d("RUTXcmd failed error: %d", ret);
    }

    if (rutxcmd_resp_buff != NULL)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(rutxcmd_resp_buff);
#else
        OSA_MemoryPoolFree(buf_1024_MemoryPool, rutxcmd_resp_buff);
#endif

        return ret;
    }

    return WM_SUCCESS;
}

int wifi_set_11ax_rutxpowerlimit_legacy(const wifi_rutxpwrlimit_t *ru_pwr_cfg)
{
    t_u8 i;
    int ret;
    HostCmd_DS_COMMAND *cmd                    = wifi_get_command_buffer();
    t_u8 *pByte                                = NULL;
    mlan_ds_11ax_chanlrupwrcft_cmd *chrupc_tlv = NULL;

    (void)wifi_get_command_lock();

    cmd->command = HostCmd_CMD_11AX_CMD;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    cmd->size    = sizeof(HostCmd_DS_11AX_CMD_CFG) + S_DS_GEN +
                ru_pwr_cfg->num_chans * (sizeof(wifi_rupwrlimit_config_t) + sizeof(MrvlIEtypesHeader_t));

    HostCmd_DS_11AX_CMD_CFG *axcmd = &cmd->params.axcmd;

    axcmd->action = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
    axcmd->sub_id = wlan_cpu_to_le16(MLAN_11AXCMD_RUPOWER_SUBID);

    pByte = (t_u8 *)axcmd->val;
    for (i = 0; i < ru_pwr_cfg->num_chans; i++)
    {
        t_u8 j;
        chrupc_tlv                               = (mlan_ds_11ax_chanlrupwrcft_cmd *)(void *)pByte;
        chrupc_tlv->type                         = TLV_TYPE_CHANNEL_RU_PWR_CONFIG;
        chrupc_tlv->len                          = sizeof(wifi_rupwrlimit_config_t);
        chrupc_tlv->rupwrlimit_config.start_freq = ru_pwr_cfg->rupwrlimit_config[i].start_freq;
        chrupc_tlv->rupwrlimit_config.width      = ru_pwr_cfg->rupwrlimit_config[i].width;
        chrupc_tlv->rupwrlimit_config.chan_num   = ru_pwr_cfg->rupwrlimit_config[i].chan_num;
        for (j = 0; j < MAX_RU_COUNT; j++)
        {
            chrupc_tlv->rupwrlimit_config.ruPower[j] = (t_s8)ru_pwr_cfg->rupwrlimit_config[i].ruPower[j];
        }
        pByte += chrupc_tlv->len + sizeof(MrvlIEtypesHeader_t);
    }
    ret = wifi_wait_for_cmdresp(NULL);
    return ret;
}

int wifi_get_11ax_rutxpowerlimit_legacy(wifi_rutxpwrlimit_t *ru_pwr_cfg)
{
    int ret;

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)wifi_get_command_lock();

    cmd->command = HostCmd_CMD_11AX_CMD;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    cmd->size    = S_DS_GEN + 2U * sizeof(t_u16);

    HostCmd_DS_11AX_CMD_CFG *rutxpwrlimit_config = (HostCmd_DS_11AX_CMD_CFG *)(void *)((uint8_t *)cmd + S_DS_GEN);

    rutxpwrlimit_config->action = HostCmd_ACT_GEN_GET;

    rutxpwrlimit_config->sub_id = MLAN_11AXCMD_RUPOWER_SUBID;

    ret = wifi_wait_for_cmdresp(ru_pwr_cfg);

    return ret;
}

int wifi_set_11ax_cfg(wifi_11ax_config_t *ax_config)
{
    /* alloc cmd and wait for response in prepare cmd, no need to deal with cmd outside */
    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_11AX_CFG, HostCmd_ACT_GEN_SET, 0, NULL,
                             ax_config, NULL);
    return WM_SUCCESS;
}

#if CONFIG_11AX_TWT
int wifi_set_btwt_cfg(const wifi_btwt_config_t *btwt_config)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_STA);
    cmd->result  = 0x0;
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_DBGS_CFG);
    cmd->size    = sizeof(wifi_btwt_config_t) + S_DS_GEN;
    (void)memcpy(&cmd->params, btwt_config, sizeof(wifi_btwt_config_t));

    wifi_wait_for_cmdresp(NULL);
    return WM_SUCCESS;
}

int wifi_set_twt_setup_cfg(const wifi_twt_setup_config_t *twt_setup)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    mlan_ds_twtcfg twt_cfg  = {0};
    MrvlIEtypes_He_cap_t *hw_he_cap;
    MrvlIEtypes_He_cap_t *hw_2g_he_cap;
    int ret = 0;

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_STA);
    cmd->result  = 0x0;

    twt_cfg.sub_id = MLAN_11AX_TWT_SETUP_SUBID;
    (void)memcpy(&twt_cfg.param.twt_setup, twt_setup, sizeof(twt_cfg.param.twt_setup));

    /* block BTWT setup command if firmware does not support BTWT */
    if (twt_cfg.param.twt_setup.negotiation_type == 3)
    {
        hw_he_cap    = (MrvlIEtypes_He_cap_t *)&mlan_adap->hw_he_cap[0];
        hw_2g_he_cap = (MrvlIEtypes_He_cap_t *)&mlan_adap->hw_2g_he_cap[0];
        /* BTWT support bit 20 */
        if ((hw_he_cap->he_mac_cap[2] & MBIT(4)) == 0 && (hw_2g_he_cap->he_mac_cap[2] & MBIT(4)) == 0)
        {
            wifi_put_command_lock();
            wifi_e("Firmware does not support Broadcast TWT, please use support BTWT version");
            return -WM_FAIL;
        }
    }

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_TWT_CFG, HostCmd_ACT_GEN_SET, 0, NULL,
                             &twt_cfg, cmd);
    ret = wifi_wait_for_cmdresp(&twt_cfg.param.twt_setup);
    if (ret == WM_SUCCESS)
    {
        if (wm_wifi.cmd_resp_status != WM_SUCCESS)
        {
            wifi_e("TWT setup error. State code=%d", twt_cfg.param.twt_setup.twt_setup_state);
        }
        else
        {
            (void)PRINTF("TWT setup success. Flow id: %d, use this in future TWT operation.\r\n",
                         twt_cfg.param.twt_setup.flow_identifier);
        }
    }
    return WM_SUCCESS;
}

int wifi_set_twt_teardown_cfg(const wifi_twt_teardown_config_t *teardown_config)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    mlan_ds_twtcfg twt_cfg  = {0};

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_STA);
    cmd->result  = 0x0;

    twt_cfg.sub_id = MLAN_11AX_TWT_TEARDOWN_SUBID;
    (void)memcpy(&twt_cfg.param.twt_teardown, teardown_config, sizeof(twt_cfg.param.twt_teardown));

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_TWT_CFG, HostCmd_ACT_GEN_SET, 0, NULL,
                             &twt_cfg, cmd);
    wifi_wait_for_cmdresp(NULL);
    return WM_SUCCESS;
}

int wifi_get_twt_report(wifi_twt_report_t *twt_report)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    mlan_ds_twtcfg twt_cfg  = {0};

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_STA);
    cmd->result  = 0x0;

    twt_cfg.sub_id = MLAN_11AX_TWT_REPORT_SUBID;

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_TWT_CFG, HostCmd_ACT_GEN_GET, 0, NULL,
                             &twt_cfg, cmd);
    wifi_wait_for_cmdresp(twt_report);
    return WM_SUCCESS;
}

#endif /* CONFIG_11AX_TWT */
#endif

#if CONFIG_WIFI_CLOCKSYNC
int wifi_set_clocksync_cfg(const wifi_clock_sync_gpio_tsf_t *tsf_latch, mlan_bss_type bss_type)
{
    int ret;
    mlan_ds_misc_cfg misc;
    mlan_ioctl_req req;
    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

    (void)memcpy((void *)&misc.param.gpio_tsf_latch_config, (const void *)tsf_latch,
                 sizeof(misc.param.gpio_tsf_latch_config));

    misc.sub_command       = (t_u32)MLAN_OID_MISC_GPIO_TSF_LATCH;
    wm_wifi.cmd_resp_ioctl = &req;
    req.pbuf               = (t_u8 *)&misc;
    req.buf_len            = sizeof(mlan_ds_misc_cfg);
    req.req_id             = (t_u32)MLAN_IOCTL_MISC_CFG;
    req.action             = MLAN_ACT_SET;

    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        req.bss_index = (t_u32)MLAN_BSS_TYPE_UAP;
        ret           = (int)wlan_ops_uap_ioctl(mlan_adap, &req);
    }
    else
    {
        req.bss_index = (t_u32)MLAN_BSS_TYPE_STA;
        ret           = (int)wlan_ops_sta_ioctl(mlan_adap, &req);
    }

    wm_wifi.cmd_resp_ioctl = NULL;
    return ret;
}

int wifi_get_tsf_info(wifi_tsf_info_t *tsf_info)
{
    int ret;
    mlan_ds_misc_cfg misc;
    mlan_ioctl_req req;
    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

    (void)memcpy((void *)&misc.param.tsf_info, (void *)tsf_info, sizeof(misc.param.tsf_info));

    misc.sub_command       = (t_u32)MLAN_OID_MISC_GET_TSF_INFO;
    wm_wifi.cmd_resp_ioctl = &req;
    req.pbuf               = (t_u8 *)&misc;
    req.buf_len            = sizeof(mlan_ds_misc_cfg);
    req.bss_index          = 0;
    req.req_id             = (t_u32)MLAN_IOCTL_MISC_CFG;
    req.action             = MLAN_ACT_GET;

    ret                    = (int)wlan_ops_sta_ioctl(mlan_adap, &req);
    wm_wifi.cmd_resp_ioctl = NULL;

    if (ret == WM_SUCCESS)
    {
        (void)memcpy((void *)tsf_info, (void *)&misc.param.tsf_info, sizeof(mlan_ds_tsf_info));
    }
    return ret;
}
#endif /* CONFIG_WIFI_CLOCKSYNC */

#if CONFIG_WMM
static void wifi_wmm_tx_stats_dump_ralist(mlan_list_head *ra_list_head)
{
    raListTbl *ra_list = MNULL;

    ra_list = (raListTbl *)util_peek_list(mlan_adap->pmoal_handle, ra_list_head, MNULL, MNULL);
    while (ra_list && ra_list != (raListTbl *)ra_list_head)
    {
        wifi_w("    [%02X:XX:XX:XX:%02X:%02X] drop_cnt[%d] total_pkts[%d]", ra_list->ra[0], ra_list->ra[4],
               ra_list->ra[5], ra_list->drop_count, ra_list->total_pkts);

        ra_list = ra_list->pnext;
    }
}

void wifi_wmm_tx_stats_dump(int bss_type)
{
    int i;
    mlan_private *priv = MNULL;

    CHECK_BSS_TYPE_RET_VOID(bss_type);

    if (bss_type == MLAN_BSS_TYPE_STA)
        priv = mlan_adap->priv[0];
    else if (bss_type == MLAN_BSS_TYPE_UAP)
        priv = mlan_adap->priv[1];
    else
        return;

    for (i = 0; i < MAX_AC_QUEUES; i++)
    {
        wifi_w("Dump priv[%d] ac_queue[%d]", bss_type, i);
        wifi_wmm_tx_stats_dump_ralist(&priv->wmm.tid_tbl_ptr[i].ra_list);
    }

    wifi_w("Dump priv[%d] driver_error_cnt:", bss_type);
    wifi_w("    tx_no_media[%hu]", priv->driver_error_cnt.tx_no_media);
    wifi_w("    tx_err_mem[%hu]", priv->driver_error_cnt.tx_err_mem);
    wifi_w("    tx_wmm_retried_drop[%hu]", priv->driver_error_cnt.tx_wmm_retried_drop);
    wifi_w("    tx_wmm_pause_drop[%hu]", priv->driver_error_cnt.tx_wmm_pause_drop);
    wifi_w("    tx_wmm_pause_replaced[%hu]", priv->driver_error_cnt.tx_wmm_pause_replaced);
    wifi_w("    rx_reorder_drop[%hu]", priv->driver_error_cnt.rx_reorder_drop);

    int free_cnt_real   = 0;
    mlan_linked_list *p = MNULL;

    mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);

    p = util_peek_list(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list, MNULL, MNULL);
    while (p && p != (mlan_linked_list *)&mlan_adap->outbuf_pool.free_list)
    {
        free_cnt_real++;
        p = p->pnext;
    }

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);
    wifi_w("TX buffer pool: free_cnt[%d] real_free_cnt[%d]", mlan_adap->outbuf_pool.free_cnt, free_cnt_real);

#if CONFIG_WMM_DEBUG
    for (i = 0; i < MAX_AC_QUEUES; i++)
    {
        wifi_w("Dump priv[%d] ac_queue[%d] history ra", bss_type, i);
        wifi_wmm_tx_stats_dump_ralist(&priv->wmm.hist_ra[i]);
    }
#endif
}
#endif

#if CONFIG_MULTI_CHAN
int wifi_set_mc_cfg(uint32_t channel_time)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd               = wifi_get_command_buffer();
    mlan_ds_multi_chan_cfg multi_chan_cfg = {0};

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, 0U /* bss_type */);
    cmd->result  = 0x0;

    multi_chan_cfg.channel_time  = channel_time;
    multi_chan_cfg.buffer_weight = 1;

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MULTI_CHAN_CONFIG,
                                   HostCmd_ACT_GEN_SET, 0, NULL, &multi_chan_cfg, cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    return wm_wifi.cmd_resp_status;
}

int wifi_get_mc_cfg(uint32_t *channel_time)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd    = wifi_get_command_buffer();
    mlan_ioctl_req ioctl_req   = {0};
    mlan_ds_misc_cfg ioctl_cfg = {0};

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, 0U /* bss_type */);
    cmd->result  = 0x0;

    wm_wifi.cmd_resp_ioctl = &ioctl_req;
    ioctl_req.pbuf         = (t_u8 *)&ioctl_cfg;
    ioctl_req.buf_len      = sizeof(ioctl_cfg);

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MULTI_CHAN_CONFIG,
                                   HostCmd_ACT_GEN_GET, 0, NULL, &ioctl_cfg.param.multi_chan_cfg, cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    wm_wifi.cmd_resp_ioctl = NULL;
    (*channel_time)        = ioctl_cfg.param.multi_chan_cfg.channel_time;
    return wm_wifi.cmd_resp_status;
}

int wifi_set_mc_policy(const int status)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd    = wifi_get_command_buffer();
    t_u16 mc_policy            = (t_u16)status;
    mlan_ioctl_req ioctl_req   = {0};
    mlan_ds_misc_cfg ioctl_cfg = {0};

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, 0U /* bss_type */);
    cmd->result  = 0x0;

    wm_wifi.cmd_resp_ioctl            = &ioctl_req;
    ioctl_req.action                  = (mlan_act_ioctl)HostCmd_ACT_GEN_SET;
    ioctl_req.pbuf                    = (t_u8 *)&ioctl_cfg;
    ioctl_req.buf_len                 = sizeof(ioctl_cfg);
    ioctl_cfg.param.multi_chan_policy = mc_policy;
    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MULTI_CHAN_POLICY,
                                   HostCmd_ACT_GEN_SET, 0, NULL, &mc_policy, cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    wm_wifi.cmd_resp_ioctl = NULL;
    return wm_wifi.cmd_resp_status;
}

int wifi_get_mc_policy(void)
{
    return (int)mlan_adap->mc_policy;
}

int wifi_set_mc_cfg_ext(const wifi_drcs_cfg_t *drcs, const int num)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd      = wifi_get_command_buffer();
    mlan_ds_drcs_cfg drcs_cfg[2] = {0};

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, 0U /* bss_type */);
    cmd->result  = 0x0;

    /* struct align is different */
    (void)memcpy(&drcs_cfg[0], drcs, sizeof(mlan_ds_drcs_cfg));
    if (num == 2)
    {
        (void)memcpy(&drcs_cfg[1], drcs + 1, sizeof(mlan_ds_drcs_cfg));
    }

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_DRCS_CONFIG, HostCmd_ACT_GEN_SET, 0,
                                   NULL, &drcs_cfg, cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    return wm_wifi.cmd_resp_status;
}

int wifi_get_mc_cfg_ext(wifi_drcs_cfg_t *drcs, int num)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd    = wifi_get_command_buffer();
    mlan_ioctl_req ioctl_req   = {0};
    mlan_ds_misc_cfg ioctl_cfg = {0};

    if (num != 2)
    {
        wifi_e("drcs should get 2 drcs config");
        (void)wifi_put_command_lock();
        return -WM_FAIL;
    }

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, 0U /* bss_type */);
    cmd->result  = 0x0;

    wm_wifi.cmd_resp_ioctl = &ioctl_req;
    ioctl_req.pbuf         = (t_u8 *)&ioctl_cfg;
    ioctl_req.buf_len      = sizeof(ioctl_cfg);

    (void)wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_DRCS_CONFIG, HostCmd_ACT_GEN_GET, 0,
                                   NULL, &ioctl_cfg.param.drcs_cfg[0], cmd);
    (void)wifi_wait_for_cmdresp(NULL);
    wm_wifi.cmd_resp_ioctl = NULL;

    /* struct align is different */
    (void)memcpy(drcs, &ioctl_cfg.param.drcs_cfg[0], sizeof(mlan_ds_drcs_cfg));
    (void)memcpy(drcs + 1, &ioctl_cfg.param.drcs_cfg[1], sizeof(mlan_ds_drcs_cfg));
    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_SUBSCRIBE_EVENT_SUPPORT
/**
 *  @brief This function submit the subscribe event command to firmware.
 *
 *  @param priv       A pointer to mlan_private structure.
 *  @sub_evt          subscribe event to submit.
 *
 *  @return           MLAN_STATUS_SUCCESS, WM_E_INVAL or MLAN_STATUS_FAILURE
 */
int wifi_subscribe_event_submit(mlan_private *pmpriv, mlan_ds_subscribe_evt *sub_evt)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->command = HostCmd_CMD_802_11_SUBSCRIBE_EVENT;
    cmd->seq_num = 0;
    cmd->result  = 0x0;

    if (sub_evt->evt_action != HostCmd_ACT_GEN_GET)
    {
        wlan_ops_sta_prepare_cmd(pmpriv, HostCmd_CMD_802_11_SUBSCRIBE_EVENT, HostCmd_ACT_GEN_SET, 0, NULL, sub_evt,
                                 cmd);
        wifi_wait_for_cmdresp(NULL);
    }
    else if (sub_evt->evt_action == HostCmd_ACT_GEN_GET)
    {
        wlan_ops_sta_prepare_cmd(pmpriv, HostCmd_CMD_802_11_SUBSCRIBE_EVENT, HostCmd_ACT_GEN_GET, 0, NULL, NULL, cmd);
        wifi_wait_for_cmdresp(sub_evt);
    }
    else
        return WM_E_INVAL;

    return wm_wifi.cmd_resp_status;
}

/**
 *  @brief This function get all subscribe event from firmware.
 *
 *  @param priv       A pointer to mlan_private structure.
 *  @sub_evt          subscribe event to submit.
 *
 *  @return           MLAN_STATUS_SUCCESS, WM_E_INVAL or MLAN_STATUS_FAILURE
 */
int wifi_get_subscribe_event(mlan_private *pmpriv, mlan_ds_subscribe_evt *sub_evt)
{
    if (!sub_evt)
        return WM_E_INVAL;
    if (!pmpriv)
        pmpriv = mlan_adap->priv[0];
    sub_evt->evt_action = HostCmd_ACT_GEN_GET;
    return wifi_subscribe_event_submit(pmpriv, sub_evt);
}

/**
 *  @brief This function disable the specific subscribe event.
 *
 *  @param priv       A pointer to mlan_private structure.
 *  @sbitmap          the specific subscribe event will be disabled.
 *
 *  @return           MLAN_STATUS_SUCCESS, WM_E_INVAL or MLAN_STATUS_FAILURE
 */
int wifi_clear_subscribe_event(mlan_private *pmpriv, int evt_bitmap)
{
    mlan_ds_subscribe_evt sub_evt;
    if (!pmpriv)
        pmpriv = mlan_adap->priv[0];
    memset((t_u8 *)&sub_evt, 0, sizeof(sub_evt));
    sub_evt.evt_action = SUBSCRIBE_EVT_ACT_BITWISE_CLR;
    sub_evt.evt_bitmap = evt_bitmap;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_rssi_low(mlan_private *pmpriv, unsigned int rssi_low, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action    = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap    = SUBSCRIBE_EVT_RSSI_LOW;
    sub_evt.low_rssi      = rssi_low;
    sub_evt.low_rssi_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}
int wifi_set_threshold_rssi_high(mlan_private *pmpriv, unsigned int rssi_high, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action     = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap     = SUBSCRIBE_EVT_RSSI_HIGH;
    sub_evt.high_rssi      = rssi_high;
    sub_evt.high_rssi_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}
int wifi_set_threshold_snr_low(mlan_private *pmpriv, unsigned int snr_low, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action   = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap   = SUBSCRIBE_EVT_SNR_LOW;
    sub_evt.low_snr      = snr_low;
    sub_evt.low_snr_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_snr_high(mlan_private *pmpriv, unsigned int snr_high, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action    = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap    = SUBSCRIBE_EVT_SNR_HIGH;
    sub_evt.high_snr      = snr_high;
    sub_evt.high_snr_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}
int wifi_set_threshold_max_fail(mlan_private *pmpriv, unsigned int max_fail, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action         = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap         = SUBSCRIBE_EVT_MAX_FAIL;
    sub_evt.failure_count      = max_fail;
    sub_evt.failure_count_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_beacon_miss(mlan_private *pmpriv, unsigned int beacon_miss, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action       = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap       = SUBSCRIBE_EVT_BEACON_MISSED;
    sub_evt.beacon_miss      = beacon_miss;
    sub_evt.beacon_miss_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_data_rssi_low(mlan_private *pmpriv, unsigned int data_rssi_low, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action         = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap         = SUBSCRIBE_EVT_DATA_RSSI_LOW;
    sub_evt.data_low_rssi      = data_rssi_low;
    sub_evt.data_low_rssi_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_data_rssi_high(mlan_private *pmpriv, unsigned int data_rssi_high, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action          = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap          = SUBSCRIBE_EVT_DATA_RSSI_HIGH;
    sub_evt.data_high_rssi      = data_rssi_high;
    sub_evt.data_high_rssi_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_data_snr_low(mlan_private *pmpriv, unsigned int data_snr_low, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action        = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap        = SUBSCRIBE_EVT_DATA_SNR_LOW;
    sub_evt.data_low_snr      = data_snr_low;
    sub_evt.data_low_snr_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_data_snr_high(mlan_private *pmpriv, unsigned int data_snr_high, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action         = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap         = SUBSCRIBE_EVT_DATA_SNR_HIGH;
    sub_evt.data_high_snr      = data_snr_high;
    sub_evt.data_high_snr_freq = freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_link_quality(mlan_private *pmpriv,
                                    unsigned int link_snr,
                                    unsigned int link_snr_freq,
                                    unsigned int link_rate,
                                    unsigned int link_rate_freq,
                                    unsigned int link_tx_latency,
                                    unsigned int link_tx_lantency_freq)
{
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action            = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap            = SUBSCRIBE_EVT_LINK_QUALITY;
    sub_evt.link_snr              = link_snr;
    sub_evt.link_snr_freq         = link_snr_freq;
    sub_evt.link_rate             = link_rate;
    sub_evt.link_rate_freq        = link_rate_freq;
    sub_evt.link_tx_latency       = link_tx_latency;
    sub_evt.link_tx_lantency_freq = link_tx_lantency_freq;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}

int wifi_set_threshold_pre_beacon_lost(mlan_private *pmpriv, unsigned int pre_beacon_lost, unsigned int freq)
{
    if (!pmpriv)
        return WM_E_INVAL;
    mlan_ds_subscribe_evt sub_evt;
    sub_evt.evt_action      = SUBSCRIBE_EVT_ACT_BITWISE_SET;
    sub_evt.evt_bitmap      = SUBSCRIBE_EVT_PRE_BEACON_LOST;
    sub_evt.pre_beacon_miss = pre_beacon_lost;
    return wifi_subscribe_event_submit(pmpriv, &sub_evt);
}
#endif

#if CONFIG_11MC
location_cfg_info_t g_ftm_location_cfg;
location_civic_rep_t g_ftm_civic_cfg;

void wlan_civic_ftm_cfg(location_civic_rep_t *ftm_civic_cfg)
{
    g_ftm_civic_cfg.civic_location_type = ftm_civic_cfg->civic_location_type;
    g_ftm_civic_cfg.civic_address_type  = ftm_civic_cfg->civic_address_type;
    g_ftm_civic_cfg.civic_req           = ftm_civic_cfg->civic_req;
    g_ftm_civic_cfg.country_code        = ftm_civic_cfg->country_code;
}

void wlan_location_ftm_cfg(location_cfg_info_t *ftm_location_cfg)
{
    g_ftm_location_cfg.lat_unc   = ftm_location_cfg->lat_unc;
    g_ftm_location_cfg.long_unc  = ftm_location_cfg->long_unc;
    g_ftm_location_cfg.alt_unc   = ftm_location_cfg->alt_unc;
    g_ftm_location_cfg.lci_req   = ftm_location_cfg->lci_req;
    g_ftm_location_cfg.longitude = ftm_location_cfg->longitude;
    g_ftm_location_cfg.latitude  = ftm_location_cfg->latitude;
    g_ftm_location_cfg.altitude  = ftm_location_cfg->altitude;
}

void wlan_dot11mc_ftm_cfg(void *p_buf, ftm_11mc_nego_cfg_t *ftm_11mc_nego_cfg)
{
    HostCmd_DS_COMMAND *cmd     = (HostCmd_DS_COMMAND *)p_buf;
    dot11mc_ftm_cfg_t *cfg_11mc = (dot11mc_ftm_cfg_t *)&cmd->params.ftm_session_cfg.tlv.cfg_11mc;

    cfg_11mc->sess_tlv.type                = wlan_cpu_to_le16(FTM_SESSION_CFG_INITATOR_TLV_ID);
    cfg_11mc->sess_tlv.len                 = wlan_cpu_to_le16(sizeof(ftm_session_cfg_t) + sizeof(t_u16));
    cfg_11mc->sess_tlv.val.burst_exponent  = ftm_11mc_nego_cfg->burst_exponent;
    cfg_11mc->sess_tlv.val.burst_duration  = ftm_11mc_nego_cfg->burst_duration;
    cfg_11mc->sess_tlv.val.min_delta_FTM   = ftm_11mc_nego_cfg->min_delta_FTM;
    cfg_11mc->sess_tlv.val.is_ASAP         = ftm_11mc_nego_cfg->is_ASAP;
    cfg_11mc->sess_tlv.val.per_burst_FTM   = ftm_11mc_nego_cfg->per_burst_FTM;
    cfg_11mc->sess_tlv.val.channel_spacing = ftm_11mc_nego_cfg->channel_spacing;
    cfg_11mc->sess_tlv.val.burst_period    = wlan_cpu_to_le16(ftm_11mc_nego_cfg->burst_period);
    cfg_11mc->sess_tlv.civic_req           = g_ftm_civic_cfg.civic_req;
    cfg_11mc->sess_tlv.lci_req             = g_ftm_location_cfg.lci_req;
    cmd->size += sizeof(ftm_session_cfg_tlv_t);

    if (cfg_11mc->sess_tlv.lci_req)
    {
        cfg_11mc->lci_tlv.type          = wlan_cpu_to_le16(FTM_SESSION_CFG_LCI_TLV_ID);
        cfg_11mc->lci_tlv.len           = wlan_cpu_to_le16(sizeof(lci_cfg_t));
        cfg_11mc->lci_tlv.val.altitude  = g_ftm_location_cfg.altitude;
        cfg_11mc->lci_tlv.val.alt_unc   = g_ftm_location_cfg.alt_unc;
        cfg_11mc->lci_tlv.val.latitude  = g_ftm_location_cfg.latitude;
        cfg_11mc->lci_tlv.val.lat_unc   = g_ftm_location_cfg.lat_unc;
        cfg_11mc->lci_tlv.val.longitude = g_ftm_location_cfg.longitude;
        cfg_11mc->lci_tlv.val.long_unc  = g_ftm_location_cfg.long_unc;
        cfg_11mc->lci_tlv.val.z_info    = Z_INFO;
        cmd->size += sizeof(lci_tlv_t);
    }

    if (cfg_11mc->sess_tlv.civic_req)
    {
        cfg_11mc->civic_tlv.type                     = wlan_cpu_to_le16(FTM_SESSION_CFG_LOCATION_CIVIC_TLV_ID);
        cfg_11mc->civic_tlv.len                      = wlan_cpu_to_le16(sizeof(civic_loc_cfg_t) -
                                                                        sizeof(cfg_11mc->civic_tlv.val.civic_address) + strlen(ftm_address));
        cfg_11mc->civic_tlv.val.civic_address_type   = g_ftm_civic_cfg.civic_address_type;
        cfg_11mc->civic_tlv.val.civic_location_type  = g_ftm_civic_cfg.civic_location_type;
        cfg_11mc->civic_tlv.val.country_code         = wlan_cpu_to_le16(g_ftm_civic_cfg.country_code);
        cfg_11mc->civic_tlv.val.civic_address_length = strlen(ftm_address);
        (void)memcpy(cfg_11mc->civic_tlv.val.civic_address, ftm_address, strlen(ftm_address));
        cmd->size += (cfg_11mc->civic_tlv.len + sizeof(t_u32)) + sizeof(t_u16);
    }

    cmd->size                  = wlan_cpu_to_le16(cmd->size);
    g_ftm_location_cfg.lci_req = 0;
    g_ftm_civic_cfg.civic_req  = 0;
}
#endif

#if CONFIG_11AZ
void wlan_dto11az_ranging_cfg(void *p_buf, const t_u8 protocl, HostCmd_FTM_SESSION_CFG *ftm_session_cfg)
{
    HostCmd_DS_COMMAND *cmd        = (HostCmd_DS_COMMAND *)p_buf;
    dot11az_ftm_cfg_t *cfg_11az    = (dot11az_ftm_cfg_t *)&cmd->params.ftm_session_cfg.tlv.cfg_11az;
    dot11az_ftm_cfg_t *in_cfg_11az = &ftm_session_cfg->tlv.cfg_11az;
    cfg_11az->range_tlv.len        = wlan_cpu_to_le16(sizeof(ranging_cfg_t));

    if (protocl == PROTO_DOT11AZ_TB)
        cfg_11az->range_tlv.type = wlan_cpu_to_le16(FTM_TB_RANGING_CFG_TLV_ID);
    else
        cfg_11az->range_tlv.type = wlan_cpu_to_le16(FTM_NTB_RANGING_CFG_TLV_ID);

    cfg_11az->range_tlv.val.format_bw                 = in_cfg_11az->range_tlv.val.format_bw;
    cfg_11az->range_tlv.val.max_i2r_sts_upto80        = in_cfg_11az->range_tlv.val.max_i2r_sts_upto80;
    cfg_11az->range_tlv.val.max_r2i_sts_upto80        = in_cfg_11az->range_tlv.val.max_r2i_sts_upto80;
    cfg_11az->range_tlv.val.az_measurement_freq       = in_cfg_11az->range_tlv.val.az_measurement_freq;
    cfg_11az->range_tlv.val.az_number_of_measurements = in_cfg_11az->range_tlv.val.az_number_of_measurements;
    cfg_11az->range_tlv.val.i2r_lmr_feedback          = in_cfg_11az->range_tlv.val.i2r_lmr_feedback;

    cfg_11az->range_tlv.val.civic_req = in_cfg_11az->range_tlv.val.civic_req;
    cfg_11az->range_tlv.val.lci_req   = in_cfg_11az->range_tlv.val.lci_req;

    cmd->size += (sizeof(dot11az_ftm_cfg_t));
}
#endif

#if (CONFIG_11MC) || (CONFIG_11AZ)
void wifi_ftm_process_cfg_resp(void *resp_buff)
{
    HostCmd_DS_COMMAND *resp = (HostCmd_DS_COMMAND *)resp_buff;

    if ((wlan_cpu_to_le16(resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.type) == FTM_NTB_RANGING_CFG_TLV_ID) ||
        (wlan_cpu_to_le16(resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.type) == FTM_TB_RANGING_CFG_TLV_ID))
    {
        if (resp->params.ftm_session_cfg.action == MLAN_ACT_SET)
            wifi_d("\n\nSet Ranging Parameters: \r\n");
        else
            wifi_d("\n\nGet Ranging Parameters: \r\n");
        wifi_d("---------------------------------\r\n");
        wifi_d("format_bw:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.val.format_bw);
        wifi_d("az_measurement_freq:%d \r\n",
               resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.val.az_measurement_freq);
        wifi_d("az_number_of_measurements:%d \r\n",
               resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.val.az_number_of_measurements);
        wifi_d("max_i2r_sts_upto80:%d \r\n",
               resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.val.max_i2r_sts_upto80);
        wifi_d("max_r2i_sts_upto80:%d \r\n",
               resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.val.max_r2i_sts_upto80);
        wifi_d("i2r_lmr_feedback:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11az.range_tlv.val.i2r_lmr_feedback);
    }
    else if (wlan_cpu_to_le16(resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.type) ==
             FTM_SESSION_CFG_INITATOR_TLV_ID)
    {
        if (resp->params.ftm_session_cfg.action == BG_SCAN_ACT_SET)
            wifi_d("\n\nSet dot11mc ftm session config: \r\n");
        else
            wifi_d("\n\nGet dot11mc ftm session config: \r\n");

        wifi_d("---------------------------------\r\n");
        wifi_d("burst_exponent:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.val.burst_exponent);
        wifi_d("burst_duration:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.val.burst_duration);
        wifi_d("min_delta_FTM:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.val.min_delta_FTM);
        wifi_d("is_ASAP:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.val.is_ASAP);
        wifi_d("per_burst_FTM:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.val.per_burst_FTM);
        wifi_d("channel_spacing:%d \r\n", resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.val.channel_spacing);
        wifi_d("burst_period:%d \n\n", resp->params.ftm_session_cfg.tlv.cfg_11mc.sess_tlv.val.burst_period);
    }
    else
        wifi_d("Invaild TLV return code \r\n");
}

void wifi_ftm_process_ctrl_resp(void *resp_buff)
{
    HostCmd_DS_COMMAND *resp = (HostCmd_DS_COMMAND *)resp_buff;

    if (resp->result == HostCmd_RESULT_OK)
    {
        if (resp->params.ftm_session_ctrl.action == FTM_ACTION_START)
        {
            wifi_d("[INFO] FTM Session Control Started on %d channel with Peer %02X:%02X:%02X:%02X:%02X:%02X \r\n",
                   resp->params.ftm_session_ctrl.chan, resp->params.ftm_session_ctrl.peer_mac[0],
                   resp->params.ftm_session_ctrl.peer_mac[1], resp->params.ftm_session_ctrl.peer_mac[2],
                   resp->params.ftm_session_ctrl.peer_mac[3], resp->params.ftm_session_ctrl.peer_mac[4],
                   resp->params.ftm_session_ctrl.peer_mac[5]);
        }
        else if (resp->params.ftm_session_ctrl.action == FTM_ACTION_STOP)
            wifi_d("[INFO] FTM Session Stopped\r\n");
        else
            wifi_d("[ERROR] Hostcmd action not supported\r\n");
    }
    else
    {
        wifi_d("[ERROR] Hostcmd failed: ReturnCode=%#04x, Result=%#04x\n", wlan_cpu_to_le16(resp->command),
               wlan_cpu_to_le16(resp->result));
    }
}

void wifi_ftm_process_event(void *p_data)
{
    wls_event_t *ftm_event = (wls_event_t *)p_data;
    double distance        = 0.0;
    t_u8 *MAC;

    PRINTF("[INFO] EventID: 0x%x SubeventID:%d \r\n", ftm_event->event_id, ftm_event->sub_event_id);

    switch (ftm_event->sub_event_id)
    {
        case WLS_SUB_EVENT_FTM_COMPLETE:
            PRINTF("\n\nFTM Session Complete:\r\n");
            PRINTF("=====================\r\n");
            PRINTF("Average RTT: %d ns\r\n", ftm_event->e.ftm_complete.avg_rtt);
            PRINTF("Average Clockoffset:%d ns\r\n", ftm_event->e.ftm_complete.avg_clk_offset);
            distance = ((ftm_event->e.ftm_complete.avg_clk_offset / 2) * (0.0003));
            PRINTF("Distance: %.6f meters\r\n\n", distance);
            wlan_wlcmgr_send_msg(WIFI_EVENT_FTM_COMPLETE, WIFI_EVENT_REASON_SUCCESS, NULL);
            break;
        case WLS_SUB_EVENT_RADIO_RECEIVED:
            wifi_d("WLS_SUB_EVENT_RADIO_RECEIVED\n");
            break;
        case WLS_SUB_EVENT_RADIO_RPT_RECEIVED:
            wifi_d("WLS_SUB_EVENT_RADIO_RPT_RECEIVED\n");
            break;
        case WLS_SUB_EVENT_ANQP_RESP_RECEIVED:
            wifi_d("WLS_SUB_EVENT_ANQP_RESP_RECEIVED\n");
            break;
        case WLS_SUB_EVENT_FTM_FAIL:
            wifi_d("WLS_SUB_EVENT_ANQP_RESP_RECEIVED\n");
            PRINTF("\nFTM Session Failed!\r\n");
            break;
        case WLS_SUB_EVENT_DISTANCE:
            distance = ftm_event->e.ftm_distance.distance / 256.0f;
            PRINTF("================================\r\n");
            PRINTF("FTM distance report (MAC %02X:%02X:%02X:%02X:%02X:%02X)\r\n", MAC[0], MAC[1], MAC[2], MAC[3],
                   MAC[4], MAC[5]);
            PRINTF("TSF: %x\r\n", ftm_event->e.ftm_distance.meas_start_tsf);
            PRINTF("distance: %.2f meters\r\n", distance);
            break;
        default:
            wifi_d("[ERROR] Unknown sub event\n");
            break;
    }
}
#endif

#ifdef RW610
uint32_t wifi_get_board_type()
{
    status_t status;
    static uint32_t wifi_rw610_package_type = 0xFFFFFFFF;

    if (0xFFFFFFFF == wifi_rw610_package_type)
    {
        OCOTP_OtpInit();
        status = OCOTP_ReadPackage(&wifi_rw610_package_type);
        if (status != kStatus_Success)
        {
            /*If status error, use BGA as default type*/
            wifi_rw610_package_type = RW610_PACKAGE_TYPE_BGA;
        }
        OCOTP_OtpDeinit();
    }

    return wifi_rw610_package_type;
}
#endif

#ifdef RW610
void wifi_cau_temperature_enable()
{
    t_u32 val;

    val = WIFI_REG32(WLAN_CAU_ENABLE_ADDR);
    val &= ~(0xC);
    val |= (2 << 2);
    WIFI_WRITE_REG32(WLAN_CAU_ENABLE_ADDR, val);
    OSA_TimeDelay(1);
}

int32_t wifi_get_temperature(void)
{
    int32_t val                   = 0;
    uint32_t reg_val              = 0;
    uint32_t temp_Cau_Raw_Reading = 0;
    uint32_t board_type           = 0;

    reg_val              = WIFI_REG32(WLAN_CAU_TEMPERATURE_ADDR);
    temp_Cau_Raw_Reading = ((reg_val & 0XFFC00) >> 10);
    board_type           = wifi_get_board_type();

    switch (board_type)
    {
        case RW610_PACKAGE_TYPE_QFN:
            val = (((((int32_t)(temp_Cau_Raw_Reading)) * 484260) - 220040600) / 1000000);
            break;

        case RW610_PACKAGE_TYPE_CSP:
            val = (((((int32_t)(temp_Cau_Raw_Reading)) * 480560) - 220707000) / 1000000);
            break;

        case RW610_PACKAGE_TYPE_BGA:
            val = (((((int32_t)(temp_Cau_Raw_Reading)) * 480561) - 220707400) / 1000000);
            break;

        default:
            PRINTF("Unknown board type, use BGA temperature \r\n");
            val = (((((int32_t)(temp_Cau_Raw_Reading)) * 480561) - 220707400) / 1000000);
            break;
    }

    return val;
}

int wifi_cau_temperature_write_to_firmware()
{
    int32_t val = 0;

    val = wifi_get_temperature();
    WIFI_WRITE_REG32(WLAN_CAU_TEMPERATURE_FW_ADDR, val);
    return val;
}

void wifi_pmip_v33_enable()
{
    uint32_t val;

    val = WIFI_REG32(WLAN_PMIP_TSEN_ADDR);
    val &= ~(0xE);
    val |= (5 << 1);
    WIFI_WRITE_REG32(WLAN_PMIP_TSEN_ADDR, val);

    val = WIFI_REG32(WLAN_V33_VSEN_ADDR);
    val &= ~(0xE);
    val |= (5 << 1);
    WIFI_WRITE_REG32(WLAN_V33_VSEN_ADDR, val);

    val = WIFI_REG32(WLAN_ADC_CTRL_ADDR);
    val |= 1 << 0;
    WIFI_WRITE_REG32(WLAN_ADC_CTRL_ADDR, val);

    val = WIFI_REG32(WLAN_ADC_CTRL_ADDR);
    val &= ~(1 << 0);
    WIFI_WRITE_REG32(WLAN_ADC_CTRL_ADDR, val);
}
#endif

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
int wifi_set_indrst_cfg(const wifi_indrst_cfg_t *indrst_cfg, mlan_bss_type bss_type)
{
    int ret;
    mlan_ds_misc_cfg misc;
    mlan_ioctl_req req;

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

    misc.param.ind_rst_cfg.ir_mode  = indrst_cfg->ir_mode;
    misc.param.ind_rst_cfg.gpio_pin = indrst_cfg->gpio_pin;

    misc.sub_command       = (t_u32)MLAN_OID_MISC_IND_RST_CFG;
    wm_wifi.cmd_resp_ioctl = &req;
    req.pbuf               = (t_u8 *)&misc;
    req.buf_len            = sizeof(mlan_ds_misc_cfg);
    req.req_id             = (t_u32)MLAN_IOCTL_MISC_CFG;
    req.action             = MLAN_ACT_SET;

    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        req.bss_index = (t_u32)MLAN_BSS_TYPE_UAP;
        ret           = (int)wlan_ops_uap_ioctl(mlan_adap, &req);
    }
    else
    {
        req.bss_index = (t_u32)MLAN_BSS_TYPE_STA;
        ret           = (int)wlan_ops_sta_ioctl(mlan_adap, &req);
    }

    wm_wifi.cmd_resp_ioctl = NULL;
    return ret;
}

int wifi_get_indrst_cfg(wifi_indrst_cfg_t *indrst_cfg, mlan_bss_type bss_type)
{
    int ret;
    mlan_ds_misc_cfg misc;
    mlan_ioctl_req req;
    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

    misc.sub_command = (t_u32)MLAN_OID_MISC_IND_RST_CFG;

    wm_wifi.cmd_resp_ioctl = &req;
    req.pbuf               = (t_u8 *)&misc;
    req.buf_len            = sizeof(mlan_ds_misc_cfg);
    req.req_id             = (t_u32)MLAN_IOCTL_MISC_CFG;
    req.action             = MLAN_ACT_GET;

    ret                    = (int)wlan_ops_sta_ioctl(mlan_adap, &req);
    wm_wifi.cmd_resp_ioctl = NULL;

    if (ret == WM_SUCCESS)
    {
        indrst_cfg->ir_mode  = misc.param.ind_rst_cfg.ir_mode;
        indrst_cfg->gpio_pin = misc.param.ind_rst_cfg.gpio_pin;
    }
    return ret;
}

int wifi_test_independent_reset()
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    HostCmd_DS_IND_RST ind;
    /** Action */
    ind.action = 0;
    /** CMD_SUBID */
    ind.sub_id = 0x117;
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_STA);
    cmd->result  = 0x0;
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_DBGS_CFG);
    cmd->size    = sizeof(HostCmd_DS_IND_RST) + S_DS_GEN;
    (void)memcpy(&cmd->params.ind_rst, &ind, sizeof(HostCmd_DS_IND_RST));

    wifi_wait_for_cmdresp(NULL);
    return WM_SUCCESS;
}

int wifi_trigger_oob_indrst()
{
    wlan_process_hang(FW_RELOAD_NO_EMULATION);

    return WM_SUCCESS;
}

#endif
