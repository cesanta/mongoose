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
 *  This header contains defines with respect to the IPv6 medium that are specific to
 *  the BLE implementation and the application use case.
 * @{
 */

#ifndef IPV6_MEDIUM_BLE_CONFIG_H__
#define IPV6_MEDIUM_BLE_CONFIG_H__

#include "app_util.h"

#define DEVICE_NAME                      "TCP_Server"                         /**< Device name used in BLE undirected advertisement. */
#define IS_SRVC_CHANGED_CHARACT_PRESENT  0

#define COMPANY_IDENTIFIER               0x0059                               /**< Company identifier for Nordic Semiconductor ASA as per www.bluetooth.org. */

#define APP_TIMER_PRESCALER              31                                   /**< Value of the RTC1 PRESCALER register. */
#define APP_ADV_ADV_INTERVAL             MSEC_TO_UNITS(333, UNIT_0_625_MS)    /**< The advertising interval. This value can vary between 100ms to 10.24s). */
#define APP_ADV_TIMEOUT                  0                                    /**< Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */

/*lint -emacro(524, MIN_CONN_INTERVAL) // Loss of precision */
#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(7.5, UNIT_1_25_MS)     /**< Minimum connection interval (7.5 ms) */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(30, UNIT_1_25_MS)      /**< Maximum connection interval (30 ms). */
#define SLAVE_LATENCY                    6                                    /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(430, UNIT_10_MS)       /**< Connection supervisory timeout (430 ms). */

#ifdef COMMISSIONING_ENABLED

#define CONFIG_MODE_DEVICE_NAME           "NodeCFG"                           /**< Name of device. Will be included in the advertising data. */

/*lint -emacro(524, MIN_CONN_INTERVAL) // Loss of precision */
#define CONFIG_MODE_MIN_CONN_INTERVAL     MSEC_TO_UNITS(400, UNIT_1_25_MS)    /**< Config mode: Minimum acceptable connection interval (0.4 seconds). */
#define CONFIG_MODE_MAX_CONN_INTERVAL     MSEC_TO_UNITS(650, UNIT_1_25_MS)    /**< Config mode: Maximum acceptable connection interval (0.65 second). */
#define CONFIG_MODE_SLAVE_LATENCY         0                                   /**< Config mode: Slave latency. */
#define CONFIG_MODE_CONN_SUP_TIMEOUT      MSEC_TO_UNITS(4000, UNIT_10_MS)     /**< Config mode: Connection supervisory timeout (4 seconds). */

#define CONFIG_MODE_ADV_ADV_INTERVAL      MSEC_TO_UNITS(333, UNIT_0_625_MS)   /**< Config mode: The advertising interval. This value can vary between 100ms to 10.24s). */
#define CONFIG_MODE_ADV_TIMEOUT           0                                   /**< Config mode: Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */

#define JOINING_MODE_MIN_CONN_INTERVAL    MSEC_TO_UNITS(400, UNIT_1_25_MS)    /**< Joining mode: Minimum acceptable connection interval (0.4 seconds). */
#define JOINING_MODE_MAX_CONN_INTERVAL    MSEC_TO_UNITS(650, UNIT_1_25_MS)    /**< Joining mode: Maximum acceptable connection interval (0.65 second). */
#define JOINING_MODE_SLAVE_LATENCY        0                                   /**< Joining mode: Slave latency. */
#define JOINING_MODE_CONN_SUP_TIMEOUT     MSEC_TO_UNITS(4000, UNIT_10_MS)     /**< Joining mode: Connection supervisory timeout (4 seconds). */

#define JOINING_MODE_ADV_ADV_INTERVAL     MSEC_TO_UNITS(333, UNIT_0_625_MS)   /**< Joining mode: The advertising interval. This value can vary between 100ms to 10.24s). */
#define JOINING_MODE_ADV_TIMEOUT          0                                   /**< Joining mode: Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */

#endif // COMMISSIONING_ENABLED

#endif // IPV6_MEDIUM_BLE_CONFIG_H__

/** @} */
/** @endcond */
