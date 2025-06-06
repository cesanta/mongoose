/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "life_cycle.h"
#include "fusemap.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototype
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Codes
 ******************************************************************************/
uint32_t get_lifecycle_state(void)
{
    uint32_t lifeCycleStateOtpShadow          = OTP_LCS_FUSE_VALUE();
    uint32_t lifeCycleStateRedundantOtpShadow = OTP_LCS_REDUNDANT_FUSE_VALUE();
    if (lifeCycleStateOtpShadow != lifeCycleStateRedundantOtpShadow)
    {
        while (true)
        {
            ; /* No necessary actions. */
        }
    }

    return ((lifeCycleStateOtpShadow & OTP_LCS_FUSE_MASK) >> OTP_LCS_FUSE_SHIFT);
}
