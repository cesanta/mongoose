/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_NIC301_H_
#define _FSL_NIC301_H_

#include "fsl_common.h"

/*!
 * @addtogroup nic301
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.nic301"
#endif

/*! @name Driver version */
/*@{*/
/*! @brief NIC301 driver version 2.0.1. */
#define FSL_NIC301_DRIVER_VERSION (MAKE_VERSION(2U, 0U, 1U))
/*@}*/

#define GPV0_BASE (0x41000000UL)
#define GPV1_BASE (0x41100000UL)
#define GPV4_BASE (0x41400000UL)

#define NIC_FN_MOD2_OFFSET     (0x024UL)
#define NIC_FN_MOD_AHB_OFFSET  (0x028UL)
#define NIC_WR_TIDEMARK_OFFSET (0x040UL)
#define NIC_READ_QOS_OFFSET    (0x100UL)
#define NIC_WRITE_QOS_OFFSET   (0x104UL)
#define NIC_FN_MOD_OFFSET      (0x108UL)

#define NIC_LCD_BASE (GPV0_BASE + 0x44000)
#define NIC_CSI_BASE (GPV0_BASE + 0x45000)
#define NIC_PXP_BASE (GPV0_BASE + 0x46000)

#define NIC_DCP_BASE      (GPV1_BASE + 0x42000)
#define NIC_ENET_BASE     (GPV1_BASE + 0x43000)
#define NIC_USBO2_BASE    (GPV1_BASE + 0x44000)
#define NIC_USDHC1_BASE   (GPV1_BASE + 0x45000)
#define NIC_USDHC2_BASE   (GPV1_BASE + 0x46000)
#define NIC_TestPort_BASE (GPV1_BASE + 0x47000)
#define NIC_ENET2_BASE    (GPV1_BASE + 0x48000)

#define NIC_CM7_BASE (GPV4_BASE + 0x42000)
#define NIC_DMA_BASE (GPV4_BASE + 0x43000)

#define NIC_QOS_MASK         (0xF)
#define NIC_WR_TIDEMARK_MASK (0x7)
#define NIC_FN_MOD_AHB_MASK  (0x7)
#define NIC_FN_MOD_MASK      (0x1)
#define NIC_FN_MOD2_MASK     (0x1)

typedef enum _nic_reg
{
    /* read_qos */
    kNIC_REG_READ_QOS_LCD      = NIC_LCD_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_CSI      = NIC_CSI_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_PXP      = NIC_PXP_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_DCP      = NIC_DCP_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_ENET     = NIC_ENET_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_USBO2    = NIC_USBO2_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_USDHC1   = NIC_USDHC1_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_USDHC2   = NIC_USDHC2_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_TestPort = NIC_TestPort_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_ENET2    = NIC_ENET2_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_CM7      = NIC_CM7_BASE + NIC_READ_QOS_OFFSET,
    kNIC_REG_READ_QOS_DMA      = NIC_DMA_BASE + NIC_READ_QOS_OFFSET,

    /* write_qos */
    kNIC_REG_WRITE_QOS_LCD      = NIC_LCD_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_CSI      = NIC_CSI_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_PXP      = NIC_PXP_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_DCP      = NIC_DCP_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_ENET     = NIC_ENET_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_USBO2    = NIC_USBO2_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_USDHC1   = NIC_USDHC1_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_USDHC2   = NIC_USDHC2_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_TestPort = NIC_TestPort_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_ENET2    = NIC_ENET2_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_CM7      = NIC_CM7_BASE + NIC_WRITE_QOS_OFFSET,
    kNIC_REG_WRITE_QOS_DMA      = NIC_DMA_BASE + NIC_WRITE_QOS_OFFSET,

    /* fn_mod */
    kNIC_REG_FN_MOD_LCD      = NIC_LCD_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_CSI      = NIC_CSI_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_PXP      = NIC_PXP_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_DCP      = NIC_DCP_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_ENET     = NIC_ENET_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_USBO2    = NIC_USBO2_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_USDHC1   = NIC_USDHC1_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_USDHC2   = NIC_USDHC2_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_TestPort = NIC_TestPort_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_ENET2    = NIC_ENET2_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_CM7      = NIC_CM7_BASE + NIC_FN_MOD_OFFSET,
    kNIC_REG_FN_MOD_DMA      = NIC_DMA_BASE + NIC_FN_MOD_OFFSET,

    /* fn_mod2 */
    kNIC_REG_FN_MOD2_DCP = NIC_DCP_BASE + NIC_FN_MOD2_OFFSET,

    /* fn_mod_ahb */
    kNIC_REG_FN_MOD_AHB_ENET     = NIC_ENET_BASE + NIC_FN_MOD_AHB_OFFSET,
    kNIC_REG_FN_MOD_AHB_TestPort = NIC_TestPort_BASE + NIC_FN_MOD_AHB_OFFSET,
    kNIC_REG_FN_MOD_AHB_ENET2    = NIC_ENET2_BASE + NIC_FN_MOD_AHB_OFFSET,
    kNIC_REG_FN_MOD_AHB_DMA      = NIC_DMA_BASE + NIC_FN_MOD_AHB_OFFSET,

    /* wr_tidemark */
    kNIC_REG_WR_TIDEMARK_CM7 = NIC_CM7_BASE + NIC_WR_TIDEMARK_OFFSET,
} nic_reg_t;

/* fn_mod2 */
typedef enum _nic_fn_mod2
{
    kNIC_FN_MOD2_ENABLE = 0,
    kNIC_FN_MOD2_BYPASS,
} nic_fn_mod2_t;

/* fn_mod_ahb */
typedef enum _nic_fn_mod_ahb
{
    kNIC_FN_MOD_AHB_RD_INCR_OVERRIDE = 0,
    kNIC_FN_MOD_AHB_WR_INCR_OVERRIDE,
    kNIC_FN_MOD_AHB_LOCK_OVERRIDE,
} nic_fn_mod_ahb_t;

/* fn_mod */
typedef enum _nic_fn_mod
{
    kNIC_FN_MOD_ReadIssue = 0,
    kNIC_FN_MOD_WriteIssue,
} nic_fn_mod_t;

/* read_qos/write_qos */
typedef enum _nic_qos
{
    kNIC_QOS_0 = 0,
    kNIC_QOS_1,
    kNIC_QOS_2,
    kNIC_QOS_3,
    kNIC_QOS_4,
    kNIC_QOS_5,
    kNIC_QOS_6,
    kNIC_QOS_7,
    kNIC_QOS_8,
    kNIC_QOS_9,
    kNIC_QOS_10,
    kNIC_QOS_11,
    kNIC_QOS_12,
    kNIC_QOS_13,
    kNIC_QOS_14,
    kNIC_QOS_15,
} nic_qos_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Set read_qos Value
 *
 * @param base Base address of GPV address
 * @param value Target value (0 - 15)
 */
static inline void NIC_SetReadQos(nic_reg_t base, nic_qos_t value)
{
    *(volatile uint32_t *)(base) = (value & NIC_QOS_MASK);
    __DSB();
}

/*!
 * @brief Get read_qos Value
 *
 * @param base Base address of GPV address
 * @return Current value configured
 */
static inline nic_qos_t NIC_GetReadQos(nic_reg_t base)
{
    return (nic_qos_t)((*(volatile uint32_t *)(base)) & NIC_QOS_MASK);
}

/*!
 * @brief Set write_qos Value
 *
 * @param base Base address of GPV address
 * @param value Target value (0 - 15)
 */
static void inline NIC_SetWriteQos(nic_reg_t base, nic_qos_t value)
{
    *(volatile uint32_t *)(base) = (value & NIC_QOS_MASK);
    __DSB();
}

/*!
 * @brief Get write_qos Value
 *
 * @param base Base address of GPV address
 * @return Current value configured
 */
static inline nic_qos_t NIC_GetWriteQos(nic_reg_t base)
{
    return (nic_qos_t)((*(volatile uint32_t *)(base)) & NIC_QOS_MASK);
}

/*!
 * @brief Set fn_mod_ahb Value
 *
 * @param base Base address of GPV address
 * @param value Target value
 */
static inline void NIC_SetFnModAhb(nic_reg_t base, nic_fn_mod_ahb_t v)
{
    *(volatile uint32_t *)(base) = v;
    __DSB();
}

/*!
 * @brief Get fn_mod_ahb Value
 *
 * @param base Base address of GPV address
 * @return Current value configured
 */
static inline nic_fn_mod_ahb_t NIC_GetFnModAhb(nic_reg_t base)
{
    return (nic_fn_mod_ahb_t)((*(volatile uint32_t *)(base)) & NIC_FN_MOD_AHB_MASK);
}

/*!
 * @brief Set wr_tidemark Value
 *
 * @param base Base address of GPV address
 * @param value Target value (0 - 7)
 */
static inline void NIC_SetWrTideMark(nic_reg_t base, uint8_t value)
{
    *(volatile uint32_t *)(base) = (value & NIC_WR_TIDEMARK_MASK);
    __DSB();
}

/*!
 * @brief Get wr_tidemark Value
 *
 * @param base Base address of GPV address
 * @return Current value configured
 */
static inline uint8_t NIC_GetWrTideMark(nic_reg_t base)
{
    return (uint8_t)((*(volatile uint32_t *)(base)) & NIC_WR_TIDEMARK_MASK);
}

/*!
 * @brief Set fn_mod Value
 *
 * @param base Base address of GPV address
 * @param value Target value
 */
static inline void NIC_SetFnMod(nic_reg_t base, nic_fn_mod_t value)
{
    *(volatile uint32_t *)(base) = value;
    __DSB();
}

/*!
 * @brief Get fn_mod Value
 *
 * @param base Base address of GPV address
 * @return Current value configured
 */
static inline nic_fn_mod_t NIC_GetFnMod(nic_reg_t base)
{
    return (nic_fn_mod_t)((*(volatile uint32_t *)(base)) & NIC_FN_MOD_MASK);
}

/*!
 * @brief Set fn_mod2 Value
 *
 * @param base Base address of GPV address
 * @param value Target value
 */
static inline void NIC_SetFnMod2(nic_reg_t base, nic_fn_mod_t value)
{
    *(volatile uint32_t *)(base) = value;
    __DSB();
}

/*!
 * @brief Get fn_mod2 Value
 *
 * @param base Base address of GPV address
 * @return Current value configured
 */
static inline nic_fn_mod2_t NIC_GetFnMod2(nic_reg_t base)
{
    return (nic_fn_mod2_t)((*(volatile uint32_t *)(base)) & NIC_FN_MOD2_MASK);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _FSL_NIC301_H_ */
