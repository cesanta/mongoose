/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/* clang-format off */

#include "boards.h"

#define LED_ONE                             BSP_LED_0_MASK                                          /**< Is on when device is advertising. */
#define LED_TWO                             BSP_LED_1_MASK                                          /**< Is on when device is connected. */
#define LED_THREE                           BSP_LED_2_MASK
#define LED_FOUR                            BSP_LED_3_MASK
#define TCP_CONNECTED_LED                   BSP_LED_2_MASK                                          /**< Is on when a TCP connection is established. */
#define DISPLAY_LED_0                       BSP_LED_0_MASK                                          /**< LED used for displaying mod 4 of data payload received on UDP port. */
#define DISPLAY_LED_1                       BSP_LED_1_MASK                                          /**< LED used for displaying mod 4 of data payload received on UDP port. */
#define DISPLAY_LED_2                       BSP_LED_2_MASK                                          /**< LED used for displaying mod 4 of data payload received on UDP port. */
#define DISPLAY_LED_3                       BSP_LED_3_MASK                                          /**< LED used for displaying mod 4 of data payload received on UDP port. */
#define ALL_APP_LED                        (BSP_LED_0_MASK | BSP_LED_1_MASK | \
    BSP_LED_2_MASK | BSP_LED_3_MASK)                        /**< Define used for simultaneous operation of all application LEDs. */

