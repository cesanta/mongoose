/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Socket
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Net_Config_UDP.h
 * Purpose: Network Configuration for UDP Sockets
 * Rev.:    V5.1.0
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>UDP Sockets
#define UDP_ENABLE              1

//   <o>Number of UDP Sockets <1-20>
//   <i>Number of available UDP sockets
//   <i>Default: 5
#define UDP_NUM_SOCKS           11

// </h>

// Dynamic port start (default 49152)
#define UDP_DYN_PORT_START      49152

// Dynamic port end (default 65535)
#define UDP_DYN_PORT_END        65535

//------------- <<< end of configuration section >>> ---------------------------
