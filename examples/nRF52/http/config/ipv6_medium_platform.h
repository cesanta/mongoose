/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */

/** @cond To make doxygen skip this file */

/** @file
 *  This header needs to include the desired IPv6 medium implementation(s). 
 *  Based on the chosen implementation, it also definitions that are specific to
 *  the IPv6 medium implementation(s) and the application use case.
 * @{
 */

#ifndef IPV6_MEDIUM_PLATFORM_H__
#define IPV6_MEDIUM_PLATFORM_H__

#include "ipv6_medium_ble.h"
#ifdef COMMISSIONING_ENABLED
#include "commissioning.h"
#endif // COMMISSIONING_ENABLED

typedef union
{
    ipv6_medium_ble_cb_params_t ble;
#ifdef COMMISSIONING_ENABLED
    commissioning_evt_t         commissioning;
#endif // COMMISSIONING_ENABLED
} ipv6_medium_cb_params_union_t;

typedef union
{
    ipv6_medium_ble_error_params_t ble;
#ifdef COMMISSIONING_ENABLED
    commissioning_evt_t            commissioning;
#endif // COMMISSIONING_ENABLED
} ipv6_medium_err_params_union_t;

#endif // IPV6_MEDIUM_PLATFORM_H__

/** @} */
/** @endcond */
