/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __httpsrv_fs_h__
#define __httpsrv_fs_h__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*
** Defines specific to HTTPSRV_FS
*/

#define HTTPSRV_FS_VERSION 1

#define HTTPSRV_FS_FLAG_INDEX 1
#define HTTPSRV_FS_FLAG_AUTH  2

/*
** error codes
*/
#define HTTPSRV_FS_OK    0
#define HTTPSRV_FS_ERROR -1

#define HTTPSRV_FS_INVALID_FUNCTION_CODE            -1
#define HTTPSRV_FS_FILE_NOT_FOUND                   -2
#define HTTPSRV_FS_PATH_NOT_FOUND                   -3
#define HTTPSRV_FS_ACCESS_DENIED                    -4
#define HTTPSRV_FS_INVALID_HANDLE                   -5
#define HTTPSRV_FS_INSUFFICIENT_MEMORY              -6
#define HTTPSRV_FS_INVALID_MEMORY_BLOCK_ADDRESS     -7
#define HTTPSRV_FS_ATTEMPT_TO_REMOVE_CURRENT_DIR    -8
#define HTTPSRV_FS_DISK_IS_WRITE_PROTECTED          -9
#define HTTPSRV_FS_BAD_DISK_UNIT                    -10
#define HTTPSRV_FS_INVALID_LENGTH_IN_DISK_OPERATION -11
#define HTTPSRV_FS_NOT_A_DOS_DISK                   -12
#define HTTPSRV_FS_SECTOR_NOT_FOUND                 -13
#define HTTPSRV_FS_WRITE_FAULT                      -14
#define HTTPSRV_FS_READ_FAULT                       -15
#define HTTPSRV_FS_SHARING_VIOLATION                -16
#define HTTPSRV_FS_FILE_EXISTS                      -17
#define HTTPSRV_FS_ALREADY_ASSIGNED                 -18
#define HTTPSRV_FS_INVALID_PARAMETER                -16
#define HTTPSRV_FS_DISK_FULL                        -17
#define HTTPSRV_FS_ROOT_DIR_FULL                    -18
#define HTTPSRV_FS_EOF                              -19
#define HTTPSRV_FS_CANNOT_CREATE_DIRECTORY          -20
#define HTTPSRV_FS_NOT_INITIALIZED                  -21
#define HTTPSRV_FS_OPERATION_NOT_ALLOWED            -22
#define HTTPSRV_FS_ERROR_INVALID_DRIVE_HANDLE       -23
#define HTTPSRV_FS_ERROR_INVALID_FILE_HANDLE        -24
#define HTTPSRV_FS_ERROR_UNKNOWN_FS_VERSION         -25
#define HTTPSRV_FS_LOST_CHAIN                       -26
#define HTTPSRV_FS_INVALID_DEVICE                   -27
#define HTTPSRV_FS_INVALID_CLUSTER_NUMBER           -28
#define HTTPSRV_FS_FAILED_TO_DELETE_LFN             -29
#define HTTPSRV_FS_BAD_LFN_ENTRY                    -30
#define HTTPSRV_FS_ERROR_INVALID_IOCTL_CMD          -31
/*
** Extra IO_IOCTL codes
*/
#define IO_IOCTL_HTTPSRV_FS_GET_NAME             (0x01)
#define IO_IOCTL_HTTPSRV_FS_GET_ATTRIBUTES       (0x02)
#define IO_IOCTL_HTTPSRV_FS_GET_LENGTH           (0x03)
#define IO_IOCTL_HTTPSRV_FS_GET_LAST_ERROR       (0x04)
#define IO_IOCTL_HTTPSRV_FS_GET_CURRENT_DATA_PTR (0x05)

#define IO_IOCTL_HTTPSRV_FS_SEEK (0x06)

/*
 * Seek parameters
 */
#define HTTPSRV_FS_IO_SEEK_SET (1) /* Seek from start */
#define HTTPSRV_FS_IO_SEEK_CUR (2) /* Seek from current location */
#define HTTPSRV_FS_IO_SEEK_END (3) /* Seek from end */

/*
** HTTP_SRV directory entry information
*/
typedef struct httpsrv_fs_dir_entry
{
    char *NAME;
    uint32_t FLAGS;
    unsigned char *DATA;
    uint32_t SIZE;
} HTTPSRV_FS_DIR_ENTRY, *HTTPSRV_FS_DIR_ENTRY_PTR;

/* FILE STRUCTURE */

/*!
 * \brief This structure defines the information kept in order to implement ANSI
 * 'C' standard I/O stream.
 */
typedef struct httpsrv_fs_file
{
    /*! \brief Device Driver specific information. */
    const HTTPSRV_FS_DIR_ENTRY *DEV_DATA_PTR;

    /*! \brief The current error for this stream. */
    int32_t ERROR;

    /*! \brief The current position in the stream. */
    size_t LOCATION;

    /*! \brief The current size of the file. */
    size_t SIZE;
} HTTPSRV_FS_FILE, *HTTPSRV_FS_FILE_PTR;

#ifdef __cplusplus
extern "C" {
#endif

void HTTPSRV_FS_init(const HTTPSRV_FS_DIR_ENTRY *);
HTTPSRV_FS_FILE_PTR HTTPSRV_FS_open(char *);
void HTTPSRV_FS_close(HTTPSRV_FS_FILE_PTR);
int32_t HTTPSRV_FS_read(HTTPSRV_FS_FILE_PTR, char *, int32_t);
size_t HTTPSRV_FS_size(HTTPSRV_FS_FILE_PTR);
int32_t HTTPSRV_FS_ioctl(HTTPSRV_FS_FILE_PTR, uint32_t, void *);
/*!
 * \brief This function sets the current file position.
 *
 * \param[in] file_ptr The stream to use.
 * \param[in] offset   The offset for the seek.
 * \param[in] mode     Mode to determine where to start the seek from.
 *
 * \return HTTPSRV_FS_OK
 * \return HTTPSRV_FS_ERROR (Failure.)
 */
int32_t HTTPSRV_FS_fseek(HTTPSRV_FS_FILE_PTR file_ptr, int32_t offset, uint32_t mode);

#ifdef __cplusplus
}
#endif

#endif
