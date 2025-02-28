/*
 * Copyright (c) 2019 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ETHIF_CONFIG_H
#define ETHIF_CONFIG_H

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <o>Connect to hardware via Driver_ETH# <0-255>
// <i>Select driver control block for MAC and PHY interface
#define ETH_DRV_NUM         0

// <h>Ethernet MAC Address
//   <o>MAC Address byte 1 <0x00-0xff>
#define MAC_ADDR0           0x02

//   <o>MAC Address byte 2 <0x00-0xff>
#define MAC_ADDR1           0x00

//   <o>MAC Address byte 3 <0x00-0xff>
#define MAC_ADDR2           0x00

//   <o>MAC Address byte 4 <0x00-0xff>
#define MAC_ADDR3           0x00

//   <o>MAC Address byte 4 <0x00-0xff>
#define MAC_ADDR4           0x00

//   <o>MAC Address byte 4 <0x00-0xff>
#define MAC_ADDR5           0x00
// </h>

//------------- <<< end of configuration section >>> ---------------------------

#endif
