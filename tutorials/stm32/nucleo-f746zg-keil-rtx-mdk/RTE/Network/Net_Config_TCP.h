/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Socket
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Net_Config_TCP.h
 * Purpose: Network Configuration for TCP Sockets
 * Rev.:    V7.1.1
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>TCP Sockets
#define TCP_ENABLE              1

//   <o>Number of TCP Sockets <1-20>
//   <i>Number of available TCP sockets
//   <i>Default: 6
#define TCP_NUM_SOCKS           10

//   <o>Number of Retries <0-20>
//   <i>How many times TCP module will try to retransmit data
//   <i>before giving up. Increase this value for high-latency
//   <i>and low throughput networks.
//   <i>Default: 5
#define TCP_MAX_RETRY           5

//   <o>Retry Timeout in seconds <1-10>
//   <i>If data frame not acknowledged within this time frame,
//   <i>TCP module will try to resend the data again.
//   <i>Default: 4
#define TCP_RETRY_TOUT          4

//   <o>Default Connect Timeout in seconds <1-65535>
//   <i>If no TCP data frame has been exchanged during this time,
//   <i>the TCP connection is either closed or a keep-alive frame
//   <i>is sent to verify that the connection still exists.
//   <i>Default: 120
#define TCP_DEFAULT_TOUT        120

//   <o>Maximum Segment Size <536-1440>
//   <i>The Maximum Segment Size specifies the maximum
//   <i>number of bytes in the TCP segment's Data field.
//   <i>Default: 1440
#define TCP_MAX_SEG_SIZE        1440

//   <o>Receive Window Size <536-65535>
//   <i>Receive Window Size specifies the size of data,
//   <i>that the socket is able to buffer in flow-control mode.
//   <i>Default: 4320
#define TCP_RECEIVE_WIN_SIZE    4320

// </h>

// TCP Initial Retransmit period in seconds
#define TCP_INITIAL_RETRY_TOUT  1

// TCP SYN frame retransmit period in seconds
#define TCP_SYN_RETRY_TOUT      2

// Number of retries to establish a connection
#define TCP_CONNECT_RETRY       7

// Dynamic port start (default 49152)
#define TCP_DYN_PORT_START      49152

// Dynamic port end (default 65535)
#define TCP_DYN_PORT_END        65535

//------------- <<< end of configuration section >>> ---------------------------
