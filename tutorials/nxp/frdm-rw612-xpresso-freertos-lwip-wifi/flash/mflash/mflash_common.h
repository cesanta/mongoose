/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MFLASH_COMMON_H__
#define __MFLASH_COMMON_H__

#include <stdint.h>
#include <stdbool.h>

#include "fsl_common.h"

/*******************************************************************************
 * Common definitions
 ******************************************************************************/

#define MFLASH_INVALID_ADDRESS (UINT32_MAX)

#define mflash_drv_is_page_aligned(x)   (((x) % (MFLASH_PAGE_SIZE)) == 0U)
#define mflash_drv_is_sector_aligned(x) (((x) % (MFLASH_SECTOR_SIZE)) == 0U)

/*
 * The addresses of FLASH locations used by APIs below may not correspond with the addresses space, especially when
 * FLASH remapping is being used. Use mflash_drv_phys2log/log2phys API to obtain actual pointer or physical address.
 */

/*******************************************************************************
 * APIs provided by low level driver
 ******************************************************************************/

/*! @brief Initializes mflash driver */
int32_t mflash_drv_init(void);

/*! @brief Erases single sector */
int32_t mflash_drv_sector_erase(uint32_t sector_addr);

/*! @brief Writes single page */
int32_t mflash_drv_page_program(uint32_t page_addr, uint32_t *data);

/*! @brief Reads data of arbitrary length */
int32_t mflash_drv_read(uint32_t addr, uint32_t *buffer, uint32_t len);

/*! @brief Returns pointer to memory area where the specified region of FLASH is mapped, NULL on failure (could not map
 * continuous block) */
void *mflash_drv_phys2log(uint32_t addr, uint32_t len);

/*! @brief Returns address of physical memory where the area accessible by given pointer is actually stored, UINT32_MAX
 * on failure (could not map as continuous block) */
uint32_t mflash_drv_log2phys(void *ptr, uint32_t len);

#endif
