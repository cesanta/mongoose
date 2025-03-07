/*
 *     Copyright 2023 NXP
 *      
 *
 *     SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_IPED_H_
#define _FSL_IPED_H_

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.iped"
#endif

#include <stdint.h>
#include <stddef.h>

#include "fsl_common.h"

/*!
 * @addtogroup iped
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @name Driver version */
/*@{*/
/*! @brief IPED driver version for RW61x. Version 1.0.0.
 *
 * - Version 1.0.0
 *   - Initial version
 */
#define FSL_IPED_DRIVER_VERSION (MAKE_VERSION(1, 0, 0))
/*@}*/

#define kIPED_Region0  (0U)  /*!< IPED region  0 */
#define kIPED_Region1  (1U)  /*!< IPED region  1 */
#define kIPED_Region2  (2U)  /*!< IPED region  2 */
#define kIPED_Region3  (3U)  /*!< IPED region  3 */
#define kIPED_Region4  (4U)  /*!< IPED region  4 */
#define kIPED_Region5  (5U)  /*!< IPED region  5 */
#define kIPED_Region6  (6U)  /*!< IPED region  6 */
#define kIPED_Region7  (7U)  /*!< IPED region  7 */
#define kIPED_Region8  (8U)  /*!< IPED region  8 */
#define kIPED_Region9  (9U)  /*!< IPED region  9 */
#define kIPED_Region10 (10U) /*!< IPED region 10 */
#define kIPED_Region11 (11U) /*!< IPED region 11 */
#define kIPED_Region12 (12U) /*!< IPED region 12 */
#define kIPED_Region13 (13U) /*!< IPED region 13 */
#define kIPED_Region14 (14U) /*!< IPED region 14 */
#define kIPED_Region15 (15U) /*!< IPED region 15 */
typedef uint32_t iped_region_t;

#define kIPED_PrinceRounds12 (0U)
#define kIPED_PrinceRounds22 (1U)
typedef uint32_t iped_prince_rounds_t;

/*! @brief IPED region count */
#define IPED_REGION_COUNT 16U

#define IPED_RW_ENABLE_VAL  0x2U
#define IPED_RW_DISABLE_VAL 0x1U

// The IPED context registers in the register map are spaced this distance apart.
#define IPED_CTX_REG_OFFSET 0x20U

enum _iped_status
{
    kStatus_IPED_RegionIsLocked = MAKE_STATUS(kStatusGroup_IPED, 0x1),
};

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Enable data encryption.
 *
 * This function enables IPED on-the-fly data encryption.
 *
 * @param base IPED peripheral address.
 */
static inline void IPED_EncryptEnable(FLEXSPI_Type *base)
{
    base->IPEDCTRL |= FLEXSPI_IPEDCTRL_IPED_EN_MASK | FLEXSPI_IPEDCTRL_AHBWR_EN_MASK | FLEXSPI_IPEDCTRL_AHBRD_EN_MASK |
                      FLEXSPI_IPEDCTRL_AHGCMWR_MASK | FLEXSPI_IPEDCTRL_AHBGCMRD_MASK;
    base->AHBCR |= FLEXSPI_AHBCR_READADDROPT_MASK | FLEXSPI_AHBCR_PREFETCHEN_MASK;
}

/*!
 * @brief Disable data encryption.
 *
 * This function disables IPED on-the-fly data encryption.
 *
 * @param base IPED peripheral address.
 */
static inline void IPED_EncryptDisable(FLEXSPI_Type *base)
{
    base->IPEDCTRL &=
        ~(FLEXSPI_IPEDCTRL_IPED_EN_MASK | FLEXSPI_IPEDCTRL_AHBWR_EN_MASK | FLEXSPI_IPEDCTRL_AHBRD_EN_MASK |
          FLEXSPI_IPEDCTRL_AHGCMWR_MASK | FLEXSPI_IPEDCTRL_AHBGCMRD_MASK);
}

/*!
 * @brief Locks access for specified region registers or data mask register.
 *
 * This function sets lock on specified region.
 *
 * @param base IPED peripheral address.
 * @param region number to lock
 */
static inline void IPED_SetLock(FLEXSPI_Type *base, iped_region_t region)
{
    /* Unlock soft-lock first */
    base->IPEDCTXCTRL[0] = (base->IPEDCTXCTRL[0] & ~(FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK << (region * 2UL))) |
                           (IPED_RW_ENABLE_VAL << (region * 2UL));
    /* Lock region settings */
    base->IPEDCTXCTRL[1] = (base->IPEDCTXCTRL[1] & ~(FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE1_MASK << (region * 2UL))) |
                           (IPED_RW_DISABLE_VAL << (region * 2UL));
    /* Re-enable soft-lock */
    base->IPEDCTXCTRL[0] = (base->IPEDCTXCTRL[0] & ~(FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK << (region * 2UL))) |
                           (IPED_RW_DISABLE_VAL << (region * 2UL));
}

/*!
 * @brief Gets info whether IPED region is locked.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be queried.
 */
static inline bool IPED_IsRegionLocked(FLEXSPI_Type *base, iped_region_t region)
{
    uint32_t freeze_mask    = (FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE1_MASK << (region * 2UL));
    uint32_t rw_enable_mask = (IPED_RW_ENABLE_VAL << (region * 2UL));
    return ((base->IPEDCTXCTRL[1] & freeze_mask) != (rw_enable_mask));
}

/*!
 * @brief Enable encryption for a specific IPED region encryption.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be enabled.
 */
status_t IPED_SetRegionEnable(FLEXSPI_Type *base, iped_region_t region, bool enable);

/*!
 * @brief Gets info whether IPED region encryption is enabled.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be queried.
 */
static inline bool IPED_IsRegionEnabled(FLEXSPI_Type *base, iped_region_t region)
{
    __IO uint32_t *reg_start = (__IO uint32_t *)(((uint32_t) & (base->IPEDCTX0START)) + (IPED_CTX_REG_OFFSET * region));
    return (*reg_start & FLEXSPI_IPEDCTX0START_GCM_MASK) != 0u;
}

/*!
 * @brief Sets IPED region address range.
 *
 * This function configures IPED region address range.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be configured.
 * @param start_address Start address for region.
 * @param end_address End address for region.
 */
status_t IPED_SetRegionAddressRange(FLEXSPI_Type *base,
                                    iped_region_t region,
                                    uint32_t start_address,
                                    uint32_t end_address);

/*!
 * @brief Gets IPED region base address.
 *
 * This function reads current start and end address settings for selected region.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be configured.
 * @param start_address Start address for region.
 * @param end_address End address for region.
 */
void IPED_GetRegionAddressRange(FLEXSPI_Type *base,
                                iped_region_t region,
                                uint32_t *start_address,
                                uint32_t *end_address);

/*!
 * @brief Sets the IPED region IV.
 *
 * This function sets specified AES IV for the given region.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be configured.
 * @param iv 64-bit AES IV in little-endian byte order.
 */
void IPED_SetRegionIV(FLEXSPI_Type *base, iped_region_t region, const uint8_t iv[8]);

/*!
 * @brief Gets the IPED region IV.
 *
 * This function gets specified AES IV for the given region.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be configured.
 * @param iv 64-bit AES IV in little-endian byte order.
 */
void IPED_GetRegionIV(FLEXSPI_Type *base, iped_region_t region, uint8_t iv[8]);

/*!
 * @brief Sets the IPED region AAD.
 *
 * This function sets specified AES AAD for the given region.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be configured.
 * @param iv 64-bit AES AAD in little-endian byte order.
 */
void IPED_SetRegionAAD(FLEXSPI_Type *base, iped_region_t region, const uint8_t aad[8]);

/*!
 * @brief Gets the IPED region AAD.
 *
 * This function gets specified AES AAD for the given region.
 *
 * @param base IPED peripheral address.
 * @param region Selection of the IPED region to be configured.
 * @param iv 64-bit AES AAD in little-endian byte order.
 */
void IPED_GetRegionAAD(FLEXSPI_Type *base, iped_region_t region, uint8_t aad[8]);

/*!
 * @brief Sets the number of rounds used for PRINCE.
 *
 * @param base IPED peripheral address.
 * @param rounds Number of PRINCE rounds used during encryption/decryption
 */
static inline void IPED_SetPrinceRounds(FLEXSPI_Type *base, iped_prince_rounds_t rounds)
{
    if (rounds == kIPED_PrinceRounds12)
    {
        base->IPEDCTRL &= ~FLEXSPI_IPEDCTRL_CONFIG_MASK;
    }
    else
    {
        base->IPEDCTRL |= FLEXSPI_IPEDCTRL_CONFIG_MASK;
    }
}

/*!
 * @brief Gets the number of rounds used for PRINCE.
 *
 * @param base IPED peripheral address.
 * @param rounds Number of PRINCE rounds used during encryption/decryption
 */
static inline iped_prince_rounds_t IPED_GetPrinceRounds(FLEXSPI_Type *base)
{
    if ((base->IPEDCTRL & FLEXSPI_IPEDCTRL_CONFIG_MASK) == FLEXSPI_IPEDCTRL_CONFIG_MASK)
    {
        return kIPED_PrinceRounds22;
    }
    else
    {
        return kIPED_PrinceRounds12;
    }
}

#if defined(__cplusplus)
}
#endif

/*!
 *@}
 */

#endif /* _FSL_IPED_H_ */
