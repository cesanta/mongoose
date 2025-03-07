/*
 * Copyright 2023 NXP
 *  
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_I2S_BRIDGE_H_
#define _FSL_I2S_BRIDGE_H_

#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @addtogroup i2s_bridge
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief Group I2S Bridge driver version for SDK */
#define FSL_I2S_BRIDGE_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */
/*@}*/

/*! @brief I2S Bridge share set */
enum _i2s_bridge_share_set_index
{
    kI2S_BRIDGE_OriginalSignal = 0U, /*!< Original FLEXCOMM I2S signals */
    kI2S_BRIDGE_ShareSet0      = 1U, /*!< share set 0 signals */
    kI2S_BRIDGE_ShareSet1      = 2U, /*!< share set 1 signals */
};

/*! @brief I2S signal */
typedef enum _i2s_bridge_signal
{
    kI2S_BRIDGE_SignalSCK     = 0U, /*!< SCK signal */
    kI2S_BRIDGE_SignalWS      = 1U, /*!< WS signal */
    kI2S_BRIDGE_SignalDataIn  = 2U, /*!< Data in signal */
    kI2S_BRIDGE_SignalDataOut = 3U, /*!< Data out signal */
} i2s_bridge_signal_t;

/*! @brief I2S signal source */
enum _i2s_bridge_share_src
{
    kI2S_BRIDGE_Flexcomm0 = 0U, /*!< Shared signal comes from FLEXCOMM0 */
    kI2S_BRIDGE_Flexcomm1 = 1U, /*!< Shared signal comes from FLEXCOMM1 */
    kI2S_BRIDGE_Flexcomm2 = 2U, /*!< Shared signal comes from FLEXCOMM2 */
    kI2S_BRIDGE_Flexcomm3 = 3U, /*!< Shared signal comes from FLEXCOMM3 */
};

/*! @brief I2S Bridge shared data out mask */
enum _i2s_bridge_dataout_mask
{
    kI2S_BRIDGE_Flexcomm0DataOut = SYSCTL1_SHAREDCTRLSET_FC0DATAOUTEN_MASK, /*!< FLEXCOMM0 DATAOUT Output Enable */
    kI2S_BRIDGE_Flexcomm1DataOut = SYSCTL1_SHAREDCTRLSET_FC1DATAOUTEN_MASK, /*!< FLEXCOMM1 DATAOUT Output Enable */
    kI2S_BRIDGE_Flexcomm2DataOut = SYSCTL1_SHAREDCTRLSET_FC2DATAOUTEN_MASK, /*!< FLEXCOMM2 DATAOUT Output Enable */
    kI2S_BRIDGE_Flexcomm3DataOut = SYSCTL1_SHAREDCTRLSET_FC3DATAOUTEN_MASK, /*!< FLEXCOMM3 DATAOUT Output Enable */
};
/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name I2S Bridge share signal configure
 * @{
 */

/*!
 * @brief I2S Bridge share set selection for flexcomm instance.
 *

 * @param flexCommIndex index of flexcomm, refer to RM for supported FLEXCOMM instances.
 * @param sckSet share set for sck, refer to _i2s_bridge_share_set_index
 * @param wsSet share set for ws, refer to _i2s_bridge_share_set_index
 * @param dataInSet share set for data in, refer to _i2s_bridge_share_set_index
 * @param dataOutSet share set for data out, refer to _i2s_bridge_share_set_index
 *
 */
void I2S_BRIDGE_SetFlexcommShareSet(
    uint32_t flexCommIndex, uint32_t sckSet, uint32_t wsSet, uint32_t dataInSet, uint32_t dataOutSet);

/*!
 * @brief I2S Bridge share set selection for a separate signal
 *
 * @param flexCommIndex index of flexcomm, refer to RM for supported FLEXCOMM instances.
 * @param signal The signal need to be configured.
 * @param set share set for the signal, refer to  #_i2s_bridge_share_set_index
 *
 */
void I2S_BRIDGE_SetFlexcommSignalShareSet(uint32_t flexCommIndex, i2s_bridge_signal_t signal, uint32_t set);

/*!
 * @brief I2S Bridge share set source configure
 *
 * @param setIndex index of share set, refer #_i2s_bridge_share_set_index
 * @param sckShareSrc sck source for this share set, refer to _i2s_bridge_share_src
 * @param wsShareSrc ws source for this share set, refer to _i2s_bridge_share_src
 * @param dataInShareSrc data in source for this share set, refer to _i2s_bridge_share_src
 * @param dataOutShareSrc data out source for this share set, refer to _i2s_bridge_dataout_mask
 *
 */
void I2S_BRIDGE_SetShareSetSrc(
    uint32_t setIndex, uint32_t sckShareSrc, uint32_t wsShareSrc, uint32_t dataInShareSrc, uint32_t dataOutShareSrc);

/*!
 * @brief I2S Bridge shared signal source selection for a share set.
 *
 * @param setIndex index of share set, refer to _i2s_bridge_share_set_index
 * @param signal the shared signal to be configured
 * @param shareSrc the signal selection, refer to _i2s_bridge_share_src.
 *
 */
void I2S_BRIDGE_SetShareSignalSrc(uint32_t setIndex, i2s_bridge_signal_t signal, uint32_t shareSrc);

/* @} */

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* _FSL_I2S_BRIDGE_H_ */
