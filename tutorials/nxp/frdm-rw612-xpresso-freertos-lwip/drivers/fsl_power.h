/*
 * Copyright 2020-2023 NXP
 *  
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_POWER_H_
#define _FSL_POWER_H_

#include "fsl_common.h"

/*!
 * @addtogroup power
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief POWER driver version 2.5.0. */
#define FSL_POWER_DRIVER_VERSION (MAKE_VERSION(2, 5, 0))
/*@}*/

/*!
 * @brief Pin edge for wakeup.
 */
typedef enum _power_wakeup_edge
{
    kPOWER_WakeupEdgeLow  = 0U, /*!< Wakeup on pin low level. */
    kPOWER_WakeupEdgeHigh = 1U, /*!< Wakeup on pin high level. */
} power_wakeup_edge_t;

/*!
 * @brief Wakeup pin.
 */
typedef enum _power_wakeup_pin
{
    kPOWER_WakeupPin0 = 0U, /*!< Wakeup0 pin. */
    kPOWER_WakeupPin1 = 1U, /*!< Wakeup1 pin. */
} power_wakeup_pin_t;

/*!
 * @brief Reset cause.
 */
typedef enum _power_reset_cause
{
    kPOWER_ResetCauseSysResetReq = 1U << 0U, /*!< CM33 system soft reset request. */
    kPOWER_ResetCauseLockup      = 1U << 1U, /*!< CM33 locked up. */
    kPOWER_ResetCauseWdt         = 1U << 2U, /*!< Watchdog timer. */
    kPOWER_ResetCauseApResetReq  = 1U << 3U, /*!< Debug mailbox reset. */
    kPOWER_ResetCauseCodeWdt     = 1U << 4U, /*!< Code watchdog timer. */
    kPOWER_ResetCauseItrc        = 1U << 5U, /*!< ITRC_CHIP reset. */
    kPOWER_ResetCauseResetB      = 1U << 6U, /*!< sw_resetb_scantest reset. */
    kPOWER_ResetCauseAll         = 0x7FU,    /*!< All reset causes. Used in POWER_ClearResetCause(). */
} power_reset_cause_t;

/*!
 * @brief Reset source.
 */
typedef enum _power_reset_source
{
    kPOWER_ResetSourceSysResetReq = 1U << 0U, /*!< CM33 system soft reset request. */
    kPOWER_ResetSourceLockup      = 1U << 1U, /*!< CM33 locked up. */
    kPOWER_ResetSourceWdt         = 1U << 2U, /*!< Watchdog timer. */
    kPOWER_ResetSourceApResetReq  = 1U << 3U, /*!< Debug mailbox reset. */
    kPOWER_ResetSourceCodeWdt     = 1U << 4U, /*!< Code watchdog timer. */
    kPOWER_ResetSourceItrc        = 1U << 5U, /*!< ITRC_CHIP reset. */
    kPOWER_ResetSourceAll         = 0x3FU,    /*!< All reset sources. */
} power_reset_source_t;

/*!
 * @brief PM2 mem power up bits definition
 */
enum _pm2_mem_pu_bits
{
    kPOWER_Pm2MemPuEnet    = (1UL << 28),
    kPOWER_Pm2MemPuSdio    = (1UL << 27),
    kPOWER_Pm2MemPuOtp     = (1UL << 26),
    kPOWER_Pm2MemPuRom     = (1UL << 25),
    kPOWER_Pm2MemPuFlexspi = (1UL << 24),
    kPOWER_Pm2MemPuPq      = (1UL << 23),
    kPOWER_Pm2MemPuPkc     = (1UL << 22),
    kPOWER_Pm2MemPuEls     = (1UL << 21),
    kPOWER_Pm2MemPuAon1    = (1UL << 20),
    kPOWER_Pm2MemPuAon0    = (1UL << 19),
    kPOWER_Pm2MemPuSram18  = (1UL << 18),
    kPOWER_Pm2MemPuSram17  = (1UL << 17),
    kPOWER_Pm2MemPuSram16  = (1UL << 16),
    kPOWER_Pm2MemPuSram15  = (1UL << 15),
    kPOWER_Pm2MemPuSram14  = (1UL << 14),
    kPOWER_Pm2MemPuSram13  = (1UL << 13),
    kPOWER_Pm2MemPuSram12  = (1UL << 12),
    kPOWER_Pm2MemPuSram11  = (1UL << 11),
    kPOWER_Pm2MemPuSram10  = (1UL << 10),
    kPOWER_Pm2MemPuSram9   = (1UL << 9),
    kPOWER_Pm2MemPuSram8   = (1UL << 8),
    kPOWER_Pm2MemPuSram7   = (1UL << 7),
    kPOWER_Pm2MemPuSram6   = (1UL << 6),
    kPOWER_Pm2MemPuSram5   = (1UL << 5),
    kPOWER_Pm2MemPuSram4   = (1UL << 4),
    kPOWER_Pm2MemPuSram3   = (1UL << 3),
    kPOWER_Pm2MemPuSram2   = (1UL << 2),
    kPOWER_Pm2MemPuSram1   = (1UL << 1),
    kPOWER_Pm2MemPuSram0   = (1UL << 0),
    kPOWER_Pm2MemPuAll     = (0x1FFFFFFFUL),
};

/*!
 * @brief PM2 ana power up bits definition
 */
enum _pm2_ana_pu_bits
{
    kPOWER_Pm2AnaPuT3      = (1UL << 6),
    kPOWER_Pm2AnaPuTcpuTop = (1UL << 5),
    kPOWER_Pm2AnaPuTddrTop = (1UL << 4),
    kPOWER_Pm2AnaPuAnaTop  = (1UL << 3),
    kPOWER_Pm2AnaPuGau     = (1UL << 2),
    kPOWER_Pm2AnaPuUsb     = (1UL << 1),
    kPOWER_Pm2AnaPuAvpll   = (1UL << 0),
    kPOWER_Pm2AnaPuAll     = (0x7FUL),
};

/*!
 * @brief clock gate bits definition
 */
enum _clk_gate_bits
{
    /* Only bit 2 is configuable, others contrlled by HW */
    kPOWER_ClkGateTddrMciEnet = (1UL << 2),
    kPOWER_ClkGateAll         = (1UL << 2),
};

/*!
 * @brief PM3 buck control bits definition
 */
enum _clk_pm3_buck_bits
{
    kPOWER_Pm3Buck18  = (1UL << 7), /*!< 1: Use normal buck18 level in PM3. 0: Use sleep buck18 level in PM3 */
    kPOWER_Pm3Buck11  = (1UL << 6), /*!< 1: Use normal buck11 level in PM3. 0: Use sleep buck11 level in PM3 */
    kPOWER_Pm3BuckAll = (0xC0UL),
};

/*!
 * @brief Capture slow pulse width
 */
typedef enum _capt_slow_pulse_width
{
    kPOWER_CaptSlowPulseWidth1 = 0U,
    kPOWER_CaptSlowPulseWidth2 = 1U,
    kPOWER_CaptSlowPulseWidth3 = 2U,
    kPOWER_CaptSlowPulseWidth4 = 3U,
    kPOWER_CaptSlowPulseWidth5 = 4U,
    kPOWER_CaptSlowPulseWidth6 = 5U,
    kPOWER_CaptSlowPulseWidth7 = 6U,
} capt_slow_pulse_width_t;

/*!
 * @brief Capture slow pulse edge
 */
typedef enum _capt_slow_pulse_edge
{
    kPOWER_CaptSlowPulseEdgeRising  = 0U,
    kPOWER_CaptSlowPulseEdgeFalling = 1U,
    kPOWER_CaptSlowPulseEdgeAny     = 2U,
} capt_slow_pulse_edge_t;

/*!
 * @brief Capture timer callback function
 * @param param : User parameter for callback.
 */
typedef void (*capt_pulse_timer_callback_t)(void *param);

/*!
 * @brief Power mode switch callback function
 * @param mode : Power mode to switch.
 * @param param : User parameter for callback.
 */
typedef void (*power_switch_callback_t)(uint32_t mode, void *param);

/*!
 * @brief Init configuration.
 */
typedef struct _power_init_config
{
    bool iBuck;         /*!< true: VCORE and AVDD18 supplied from iBuck; false: supplied from external DCDC. */
    bool gateCauRefClk; /*!< true: CAU_SOC_SLP_REF_GEN_CLK gated; false: CAU_SOC_SLP_REF_GEN_CLK on. */
} power_init_config_t;

/*!
 * @brief Sleep configuration.
 */
typedef struct _power_sleep_config
{
    uint32_t
        pm2MemPuCfg; /*!< Modules to keep powered on in PM2 mode. Logical OR of the enums in @ref _pm2_mem_pu_bits. */
    uint32_t pm2AnaPuCfg; /*!< Ana to keep powered on in PM2 mode. Logical OR of the enums in @ref _pm2_ana_pu_bits. */
    uint32_t clkGate;     /*!< Source clock gate control. Logical OR of the enums in @ref _clk_gate_bits. */
    uint32_t memPdCfg;    /*!< PMU MEM_CFG: Power Down memory configuration. Bit0-5 for PM3, bit8 for PM4.
                                            bit0: ram0-5 384KB
                                            bit1: ram6 64KB
                                            bit2: ram7 64KB
                                            bit3: ram8-9 128KB
                                            bit4: ram10-13 256KB
                                            bit5: ram14-18 320KB.
                                            bit8: aon mem higher 8KB */
    uint32_t pm3BuckCfg;  /*!< PMIP BUCK control in PM3 mode. Logical OR of the enums in @ref _clk_pm3_buck_bits. */
} power_sleep_config_t;

/*!
 * @brief Glitch detector configuration.
 */
typedef struct _power_gdet_data
{
    uint32_t CFG[6];
    uint32_t TRIM0;
} power_gdet_data_t;

/*!
 * @brief Glitch detector configuration load function.
 */
typedef bool (*power_load_gdet_cfg)(power_gdet_data_t *data);

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief   Enable system reset source
 * @param   source   : A bitmask of of @ref power_reset_source_t
 */
__STATIC_INLINE void POWER_EnableResetSource(uint32_t source)
{
    assert((source & ~(uint32_t)kPOWER_ResetSourceAll) == 0U);

    PMU->SYS_RST_EN |= source;
}

/**
 * @brief   Disable system reset source
 * @param   source   : A bitmask of of @ref power_reset_source_t
 */
__STATIC_INLINE void POWER_DisableResetSource(uint32_t source)
{
    assert((source & ~(uint32_t)kPOWER_ResetSourceAll) == 0U);

    PMU->SYS_RST_EN &= ~source;
}

/**
 * @brief   Get last reset cause
 * @return  Or'ed cause of @ref power_reset_cause_t
 */
__STATIC_INLINE uint32_t POWER_GetResetCause(void)
{
    /* On reset, PMU->SYS_RST_STATUS is backed up in RF_SYSCON->WO_SCRATCH_REG[3]
       and cleared by ROM */
    return RF_SYSCON->WO_SCRATCH_REG[3] & (uint32_t)kPOWER_ResetCauseAll;
}

/**
 * @brief   Clear last reset cause
 * @param   cause   : A bitmask of of @ref power_reset_cause_t
 */
__STATIC_INLINE void POWER_ClearResetCause(uint32_t cause)
{
    assert((cause & ~(uint32_t)kPOWER_ResetCauseAll) == 0U);

    PMU->SYS_RST_CLR = cause;
}

/**
 * @brief   Configure pin edge for wakeup
 * @param   pin     : Wakeup pin
 * @param   edge    : Pin level for wakeup
 */
__STATIC_INLINE void POWER_ConfigWakeupPin(power_wakeup_pin_t pin, power_wakeup_edge_t edge)
{
    PMU->WAKEUP_LEVEL = (PMU->WAKEUP_LEVEL & ~(1UL << (uint8_t)pin)) | ((uint32_t)edge << (uint8_t)pin);
}

/**
 * @brief   Check if IRQ is the wakeup source
 * @param   irq   : IRQ number
 * @return  true if IRQ is the wakeup source, false otherwise.
 */
bool POWER_GetWakeupStatus(IRQn_Type irq);

/**
 * @brief   Clear wakeup status
 * @param   irq   : IRQ number
 */
void POWER_ClearWakeupStatus(IRQn_Type irq);

/**
 * @brief   Enable the Wakeup interrupt.
 * @param   irq   : IRQ number
 */
void POWER_EnableWakeup(IRQn_Type irq);

/**
 * @brief   Disable the Wakeup interrupts.
 * @param   irq   : IRQ number
 */
void POWER_DisableWakeup(IRQn_Type irq);

/**
 * @brief   Set power mode on idle.
 * @param   mode : 0 ~ 4 stands for PM0 ~ PM4.
 */
AT_QUICKACCESS_SECTION_CODE(void POWER_SetSleepMode(uint32_t mode));

/**
 * @brief   Get power mode waken up from.
 * @return  Power mode.
 */
__STATIC_INLINE uint32_t POWER_GetWakenMode(void)
{
    return (PMU->PWR_MODE_STATUS & PMU_PWR_MODE_STATUS_PWR_MODE_STATUS_MASK) + 1U;
}

/**
 * @brief   Get current sleep configuration.
 * @param   config : Pointer to config structure to save current config.
 */
void POWER_GetCurrentSleepConfig(power_sleep_config_t *config);

/**
 * @brief   Initialize power configuration.
 * @param   config : Pointer to init config structure.
 */
void POWER_InitPowerConfig(const power_init_config_t *config);

/**
 * @brief   Configure CAU_SOC_SLP_REF_GEN_CLK on/off status in SoC sleep mode.
 * @param   pdCau : true for clock off; false for clock on.
 */
void POWER_ConfigCauInSleep(bool pdCau);

/**
 * @brief   Set power mode switch callback. The callbacks are called with interrupt disabled.
 * @param   pre : Function called before power mode switch
 * @param   preParam : User parameter for pre callback
 * @param   post : Function called after power mode switch
 * @param   postParam : User parameter for post callback
 */
void POWER_SetPowerSwitchCallback(power_switch_callback_t pre,
                                  void *preParam,
                                  power_switch_callback_t post,
                                  void *postParam);

/**
 * @brief   Switch system into certain power mode.
 * @param   mode : 0 ~ 4 stands for PM0 ~ PM4.
 * @param   config : Sleep configuration on PM2-PM4.
 * @return  True for success, else failure.
 */
AT_QUICKACCESS_SECTION_CODE(bool POWER_EnterPowerMode(uint32_t mode, const power_sleep_config_t *config));

/**
 * @brief   Power on WLAN.
 */
void POWER_PowerOnWlan(void);

/**
 * @brief   Power off WLAN.
 */
void POWER_PowerOffWlan(void);

/**
 * @brief   Enable MCI wakeup WLAN
 * @param   wlWakeup : 8 bits wakeup mask
 */
__STATIC_INLINE void PMU_EnableWlanWakeup(uint8_t wlWakeup)
{
    PMU->WLAN_CTRL |= PMU_WLAN_CTRL_WL_WAKEUP(wlWakeup);
}

/**
 * @brief   Disable MCI wakeup WLAN
 * @param   wlWakeup : 8 bits wakeup mask
 */
__STATIC_INLINE void PMU_DisableWlanWakeup(uint8_t wlWakeup)
{
    PMU->WLAN_CTRL &= ~PMU_WLAN_CTRL_WL_WAKEUP(wlWakeup);
}

/**
 * @brief   Power on BLE.
 */
void POWER_PowerOnBle(void);

/**
 * @brief   Power off BLE.
 */
void POWER_PowerOffBle(void);

/**
 * @brief   Enable MCI wakeup BLE
 * @param   bleWakeup : 8 bits wakeup mask
 */
__STATIC_INLINE void PMU_EnableBleWakeup(uint8_t bleWakeup)
{
    PMU->BLE_CTRL |= PMU_BLE_CTRL_BLE_WAKEUP(bleWakeup);
}

/**
 * @brief   Disable MCI wakeup BLE
 * @param   bleWakeup : 8 bits wakeup mask
 */
__STATIC_INLINE void PMU_DisableBleWakeup(uint8_t bleWakeup)
{
    PMU->BLE_CTRL &= ~PMU_BLE_CTRL_BLE_WAKEUP(bleWakeup);
}

/**
 * @brief   Power on GAU.
 */
void POWER_PowerOnGau(void);

/**
 * @brief   Power off GAU.
 */
void POWER_PowerOffGau(void);

/**
 * @brief   Enable capture slow pulse timer with 32768Hz clock source
 * @param   width    : input capture filter width in cycles
 * @param   edge     : trigger condition of counter
 * @param   timeout  : timer expire counter which will trigger callback
 * @param   callback : callback function on timer expire
 * @param   param    : callback parameter
 */
void POWER_EnableCaptSlowPulseTimer(capt_slow_pulse_width_t width,
                                    capt_slow_pulse_edge_t edge,
                                    uint32_t timeout,
                                    capt_pulse_timer_callback_t cb,
                                    void *param);

/**
 * @brief   Enable capture fast pulse timer with 3.84/4MHz clock source
 * @param   timeout  : timer expire counter which will trigger callback
 * @param   callback : callback function on timer expire
 * @param   param    : callback parameter
 */
void POWER_EnableCaptFastPulseTimer(uint32_t timeout, capt_pulse_timer_callback_t cb, void *param);

/**
 * @brief   Disable capture pulse timer
 */
void POWER_DisableCaptPulseTimer(void);

/**
 * @brief   Configure power rail voltage and LVD/HVD threshold.
 * @param   dro  : trim value from fuse.
 * @param   pack : Device package type: 0 - QFN, 1 - CSP, 2 - BGA
 */
void POWER_InitVoltage(uint32_t dro, uint32_t pack);

/**
 * @brief   Initialize glitch detector configuration.
 * @param   loadFunc : function pointer to the GDET load configuration.
 * @param   data     : GDET config data loaded from fuse.
 * @param   pack     : Device package type: 0 - QFN, 1 - CSP, 2 - BGA
 */
void Power_InitLoadGdetCfg(power_load_gdet_cfg loadFunc, const power_gdet_data_t *data, uint32_t pack);

/**
 * @brief   Disable GDET and VSensors
 */
AT_QUICKACCESS_SECTION_CODE(void POWER_DisableGDetVSensors(void));

/**
 * @brief   Enable GDET and VSensors
 * @return  True for success, else failure.
 */
AT_QUICKACCESS_SECTION_CODE(bool POWER_EnableGDetVSensors(void));

/**
 * @brief   Apply SVC GDC equation and get the SVC trim configuration
 * @param   gdetTrim : GDET trim value from fuse.
 * @param   pack     : Device package type: 0 - QFN, 1 - CSP, 2 - BGA
 */
uint32_t POWER_TrimSvc(uint32_t gdetTrim, uint32_t pack);

#if defined(__cplusplus)
}
#endif /*_cplusplus */

/*! @}*/

#endif /* _FSL_POWER_H_ */
