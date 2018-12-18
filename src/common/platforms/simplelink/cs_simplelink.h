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

#ifndef CS_COMMON_PLATFORMS_SIMPLELINK_CS_SIMPLELINK_H_
#define CS_COMMON_PLATFORMS_SIMPLELINK_CS_SIMPLELINK_H_

#if defined(MG_NET_IF) && MG_NET_IF == MG_NET_IF_SIMPLELINK

/* If simplelink.h is already included, all bets are off. */
#if !defined(__SIMPLELINK_H__)

#include <stdbool.h>

#ifndef __TI_COMPILER_VERSION__
#undef __CONCAT
#undef FD_CLR
#undef FD_ISSET
#undef FD_SET
#undef FD_SETSIZE
#undef FD_ZERO
#undef fd_set
#endif

#if CS_PLATFORM == CS_P_CC3220
#include <ti/drivers/net/wifi/porting/user.h>
#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/sl_socket.h>
#include <ti/drivers/net/wifi/netapp.h>
#else
/* We want to disable SL_INC_STD_BSD_API_NAMING, so we include user.h ourselves
 * and undef it. */
#define PROVISIONING_API_H_
#include <simplelink/user.h>
#undef PROVISIONING_API_H_
#undef SL_INC_STD_BSD_API_NAMING

#include <simplelink/include/simplelink.h>
#include <simplelink/include/netapp.h>
#endif /* CS_PLATFORM == CS_P_CC3220 */

/* Now define only the subset of the BSD API that we use.
 * Notably, close(), read() and write() are not defined. */
#define AF_INET SL_AF_INET

#define socklen_t SlSocklen_t
#define sockaddr SlSockAddr_t
#define sockaddr_in SlSockAddrIn_t
#define in_addr SlInAddr_t

#define SOCK_STREAM SL_SOCK_STREAM
#define SOCK_DGRAM SL_SOCK_DGRAM

#define htonl sl_Htonl
#define ntohl sl_Ntohl
#define htons sl_Htons
#define ntohs sl_Ntohs

#ifndef EACCES
#define EACCES SL_EACCES
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT SL_EAFNOSUPPORT
#endif
#ifndef EAGAIN
#define EAGAIN SL_EAGAIN
#endif
#ifndef EBADF
#define EBADF SL_EBADF
#endif
#ifndef EINVAL
#define EINVAL SL_EINVAL
#endif
#ifndef ENOMEM
#define ENOMEM SL_ENOMEM
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK SL_EWOULDBLOCK
#endif

#define SOMAXCONN 8

#ifdef __cplusplus
extern "C" {
#endif

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
char *inet_ntoa(struct in_addr in);
int inet_pton(int af, const char *src, void *dst);

struct mg_mgr;
struct mg_connection;

typedef void (*mg_init_cb)(struct mg_mgr *mgr);
bool mg_start_task(int priority, int stack_size, mg_init_cb mg_init);

void mg_run_in_task(void (*cb)(struct mg_mgr *mgr, void *arg), void *cb_arg);

int sl_fs_init(void);

void sl_restart_cb(struct mg_mgr *mgr);

int sl_set_ssl_opts(int sock, struct mg_connection *nc);

#ifdef __cplusplus
}
#endif

#endif /* !defined(__SIMPLELINK_H__) */

/* Compatibility with older versions of SimpleLink */
#if SL_MAJOR_VERSION_NUM < 2

#define SL_ERROR_BSD_EAGAIN SL_EAGAIN
#define SL_ERROR_BSD_EALREADY SL_EALREADY
#define SL_ERROR_BSD_ENOPROTOOPT SL_ENOPROTOOPT
#define SL_ERROR_BSD_ESECDATEERROR SL_ESECDATEERROR
#define SL_ERROR_BSD_ESECSNOVERIFY SL_ESECSNOVERIFY
#define SL_ERROR_FS_FAILED_TO_ALLOCATE_MEM SL_FS_ERR_FAILED_TO_ALLOCATE_MEM
#define SL_ERROR_FS_FILE_HAS_NOT_BEEN_CLOSE_CORRECTLY \
  SL_FS_FILE_HAS_NOT_BEEN_CLOSE_CORRECTLY
#define SL_ERROR_FS_FILE_NAME_EXIST SL_FS_FILE_NAME_EXIST
#define SL_ERROR_FS_FILE_NOT_EXISTS SL_FS_ERR_FILE_NOT_EXISTS
#define SL_ERROR_FS_NO_AVAILABLE_NV_INDEX SL_FS_ERR_NO_AVAILABLE_NV_INDEX
#define SL_ERROR_FS_NOT_ENOUGH_STORAGE_SPACE SL_FS_ERR_NO_AVAILABLE_BLOCKS
#define SL_ERROR_FS_NOT_SUPPORTED SL_FS_ERR_NOT_SUPPORTED
#define SL_ERROR_FS_WRONG_FILE_NAME SL_FS_WRONG_FILE_NAME
#define SL_ERROR_FS_INVALID_HANDLE SL_FS_ERR_INVALID_HANDLE
#define SL_NETCFG_MAC_ADDRESS_GET SL_MAC_ADDRESS_GET
#define SL_SOCKET_FD_ZERO SL_FD_ZERO
#define SL_SOCKET_FD_SET SL_FD_SET
#define SL_SOCKET_FD_ISSET SL_FD_ISSET
#define SL_SO_SECURE_DOMAIN_NAME_VERIFICATION SO_SECURE_DOMAIN_NAME_VERIFICATION

#define SL_FS_READ FS_MODE_OPEN_READ
#define SL_FS_WRITE FS_MODE_OPEN_WRITE

#define SL_FI_FILE_SIZE(fi) ((fi).FileLen)
#define SL_FI_FILE_MAX_SIZE(fi) ((fi).AllocatedLen)

#define SlDeviceVersion_t SlVersionFull
#define sl_DeviceGet sl_DevGet
#define SL_DEVICE_GENERAL SL_DEVICE_GENERAL_CONFIGURATION
#define SL_LEN_TYPE _u8
#define SL_OPT_TYPE _u8

#else /* SL_MAJOR_VERSION_NUM >= 2 */

#define FS_MODE_OPEN_CREATE(max_size, flag) \
  (SL_FS_CREATE | SL_FS_CREATE_MAX_SIZE(max_size))
#define SL_FI_FILE_SIZE(fi) ((fi).Len)
#define SL_FI_FILE_MAX_SIZE(fi) ((fi).MaxSize)

#define SL_LEN_TYPE _u16
#define SL_OPT_TYPE _u16

#endif /* SL_MAJOR_VERSION_NUM < 2 */

int slfs_open(const unsigned char *fname, uint32_t flags, uint32_t *token);

#endif /* MG_NET_IF == MG_NET_IF_SIMPLELINK */

#endif /* CS_COMMON_PLATFORMS_SIMPLELINK_CS_SIMPLELINK_H_ */
