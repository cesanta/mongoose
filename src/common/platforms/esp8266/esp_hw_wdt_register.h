/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CS_COMMON_PLATFORMS_ESP8266_ESP_HW_WDT_REGISTER_H_
#define CS_COMMON_PLATFORMS_ESP8266_ESP_HW_WDT_REGISTER_H_

#ifdef RTOS_SDK
#include <esp_common.h>
#else
#include <user_interface.h>
#endif

#define REG_WDT_BASE 0x60000900

#define WDT_CTL (REG_WDT_BASE + 0x0)
#define WDT_CTL_ENABLE (BIT(0))
#define WDT_CTL_STAGE1_NO_RESET (BIT(1))
#define WDT_CTL_STAGE1_DISABLE (BIT(2))
#define WDT_CTL_UNK3 (BIT(3))
#define WDT_CTL_UNK4 (BIT(4))
#define WDT_CTL_UNK5 (BIT(5))

/* Bits 3, 4, 5 - ???; set to 1 by ROM. */

#define WDT_RELOAD_STAGE0 (REG_WDT_BASE + 0x4)
#define WDT_RELOAD_STAGE0_V (0xf)
#define WDT_RELOAD_STAGE0_S (0)

#define WDT_RELOAD_STAGE1 (REG_WDT_BASE + 0x8)
#define WDT_RELOAD_STAGE1_V (0xf)
#define WDT_RELOAD_STAGE1_S (0)

#define WDT_COUNT (REG_WDT_BASE + 0xc) /* Counts at CPU_CLK (80 MHz) */
#define WDT_COUNT_V (0xffffffff)
#define WDT_COUNT_S (0)

#define WDT_STAGE (REG_WDT_BASE + 0x10)
#define WDT_STAGE_V (1)
#define WDT_STAGE_S (0)

#define WDT_RESET (REG_WDT_BASE + 0x14)
#define WDT_RESET_V (0xff)
#define WDT_RESET_S (0)

#define WDT_RESET_STAGE (REG_WDT_BASE + 0x18)
#define WDT_RESET_STAGE_V (0xff)
#define WDT_RESET_STAGE_S (0)

#define WDT_RESET_VALUE 0x73

#endif /* CS_COMMON_PLATFORMS_ESP8266_ESP_HW_WDT_REGISTER_H_ */
