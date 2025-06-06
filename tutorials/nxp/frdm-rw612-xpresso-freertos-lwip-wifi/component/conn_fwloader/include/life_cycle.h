/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __LIFE_CYCLE_H__
#define __LIFE_CYCLE_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Life cycle definitions. */
#define LIFECYCLE_NXP_BLANK (0x0u)

#define LIFECYCLE_NXP_DEV_NON_SEC (0xFDu)
#define LIFECYCLE_NXP_DEV_SEC     (0xFEu)

#define LIFECYCLE_NXP_PROVISIONED  (0x1u)
#define LIFECYCLE_OEM_OPEN         (0x3u)
#define LIFECYCLE_OEM_SECURE       (0x7u)
#define LIFECYCLE_OEM_CLOSED       (0xFu)
#define LIFECYCLE_OEM_FIELD_RETURN (0x1Fu)
#define LIFECYCLE_NXP_FIELD_RETURN (0x3Fu)

#define LIFECYCLE_OEM_LOCKED (0xCFu)
#define LIFECYCLE_SHREDDED   (0xFFu)

#define DIS_ROM_HIDIND_MASK  (0xF00000u)
#define DIS_ROM_HIDIND_SHIFT (20u)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

uint32_t get_lifecycle_state(void);

#endif
