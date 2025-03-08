/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Comments:
 *
 *   This file contains the functions that are used to initialize FS
 *   It also contains the FS driver functions.
 */
#include "httpsrv_fs.h"
#include "httpsrv_port.h"

static int32_t httpsrv_fs_cmp(char *, char *);
static const HTTPSRV_FS_DIR_ENTRY *httpsrv_fs_open_file(char *, int32_t *);
static uint32_t httpsrv_fs_read(HTTPSRV_FS_FILE_PTR, uint32_t, char *, int32_t *);
static uint32_t httpsrv_fs_move_file_pointer(HTTPSRV_FS_FILE_PTR, int32_t *);

const HTTPSRV_FS_DIR_ENTRY *ROOT;

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : HTTPSRV_FS_init
 * Returned Value   : HTTPSRV_FS error code.
 * Comments         : Initialize the Trivial File System.
 *
 *END*---------------------------------------------------------------------*/

void HTTPSRV_FS_init(
    /*[IN] pointer to the first entry of the root HTTPSRV_FS directory  */
    const HTTPSRV_FS_DIR_ENTRY *root)
{
    ROOT = root;
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : HTTPSRV_FS_open
 * Returned Value   : HTTPSRV_FS error code.
 * Comments         : Opens HTTPSRV_FS driver and initializes given file descriptor.
 *
 *END*----------------------------------------------------------------------*/

HTTPSRV_FS_FILE_PTR HTTPSRV_FS_open(
    /* [IN] the remaining portion of the name of the device */
    char *open_name_ptr)
{
    int32_t error_code                = HTTPSRV_FS_OK;
    HTTPSRV_FS_FILE_PTR fd_ptr        = NULL; /* the file handle for the device being opened */
    const HTTPSRV_FS_DIR_ENTRY *entry = NULL;

    if (open_name_ptr && (*open_name_ptr != '\0'))
    {
        entry = httpsrv_fs_open_file(open_name_ptr, &error_code);
        if (entry && (error_code == HTTPSRV_FS_OK))
        {
            fd_ptr = httpsrv_mem_alloc(sizeof(HTTPSRV_FS_FILE));

            if (fd_ptr)
            {
                /* Initialise the file information fields */
                fd_ptr->DEV_DATA_PTR = entry;
                fd_ptr->LOCATION     = 0;
                fd_ptr->ERROR        = 0;
                fd_ptr->SIZE         = ((HTTPSRV_FS_DIR_ENTRY_PTR)fd_ptr->DEV_DATA_PTR)->SIZE;
            }
        }
    }
    return fd_ptr;
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : HTTPSRV_FS_close
 * Returned Value   : HTTPSRV_FS error code.
 * Comments         : Closes given file descriptor.
 *
 *END*----------------------------------------------------------------------*/

void HTTPSRV_FS_close(
    /* [IN/OUT] the file handle for the device being closed */
    HTTPSRV_FS_FILE_PTR fd_ptr)
{
    if (fd_ptr)
    {
        fd_ptr->DEV_DATA_PTR = NULL;
        httpsrv_mem_free(fd_ptr);
    }
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : HTTPSRV_FS_read
 * Returned Value   : Number of characters read.
 * Comments         : Reads data from given file.
 *
 *END*----------------------------------------------------------------------*/

int32_t HTTPSRV_FS_read(
    /* [IN/OUT] the stream to perform the operation on */
    HTTPSRV_FS_FILE_PTR file_ptr,

    /* [IN] the data location to write to */
    char *data_ptr,

    /* [IN] the number of bytes to read */
    int32_t num)
{
    uint32_t bytes_read;
    int32_t error_code = HTTPSRV_FS_OK;
    int32_t ret;

    bytes_read = httpsrv_fs_read(file_ptr, num, data_ptr, &error_code);

    if (error_code != HTTPSRV_FS_OK)
    {
        file_ptr->ERROR = error_code;
        ret             = error_code;
    }
    else
    {
        ret = (int32_t)bytes_read;
    }
    return (ret);
}

int32_t HTTPSRV_FS_fseek(HTTPSRV_FS_FILE_PTR file_ptr, int32_t offset, uint32_t mode)
{
    int32_t result;
    int32_t location;

    if (file_ptr == NULL)
    {
        return (HTTPSRV_FS_ERROR);
    }

    /* Otherwise use common implementation of seek */
    switch (mode)
    {
        case HTTPSRV_FS_IO_SEEK_SET:
            if (0 > offset)
            {
                location = -1;
            }
            else
            {
                location = offset;
            }
            break;
        case HTTPSRV_FS_IO_SEEK_CUR:
            if ((offset >= 0 && ((UINT64_MAX - file_ptr->LOCATION) < offset)) ||
                (offset < 0 && (file_ptr->LOCATION < -offset)))
            {
                location = -1;
            }
            else
            {
                location = file_ptr->LOCATION + offset;
            }
            break;
        case HTTPSRV_FS_IO_SEEK_END:
            if ((offset >= 0 && ((UINT64_MAX - file_ptr->SIZE) < offset)) || (offset < 0 && (file_ptr->SIZE < -offset)))
            {
                location = -1;
            }
            else
            {
                location = file_ptr->SIZE + offset;
            }
            break;
        default:
            location = -1; /* Set location to negative value to induce IO_ERROR */
    }

    /* Check for overflow/underflow */
    if (location < 0)
    {
        result = HTTPSRV_FS_ERROR;
    }
    else
    {
        file_ptr->LOCATION = location;

        httpsrv_fs_move_file_pointer(file_ptr, &result);

        result = HTTPSRV_FS_OK;
    }

    return result;
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : HTTPSRV_FS_ioctl
 * Returned Value   : HTTPSRV_FS error code.
 * Comments         : Performs specified operation related to given file.
 *
 *END*----------------------------------------------------------------------*/

int32_t HTTPSRV_FS_ioctl(
    /* [IN] the stream to perform the operation on */
    HTTPSRV_FS_FILE_PTR file_ptr,

    /* [IN] the ioctl command */
    uint32_t cmd,

    /* [IN/OUT] the ioctl parameters */
    void *param_ptr)
{
    int32_t error_code = HTTPSRV_FS_OK;

    switch (cmd)
    {
        case IO_IOCTL_HTTPSRV_FS_GET_NAME:
            if (file_ptr->DEV_DATA_PTR == NULL)
            {
                error_code = HTTPSRV_FS_ERROR_INVALID_FILE_HANDLE;
            }
            else
            {
                *((char **)param_ptr) = ((HTTPSRV_FS_DIR_ENTRY_PTR)file_ptr->DEV_DATA_PTR)->NAME;
            }
            break;
        case IO_IOCTL_HTTPSRV_FS_GET_ATTRIBUTES:
            if (file_ptr->DEV_DATA_PTR == NULL)
            {
                error_code = HTTPSRV_FS_ERROR_INVALID_FILE_HANDLE;
            }
            else
            {
                *((uint32_t *)param_ptr) = ((HTTPSRV_FS_DIR_ENTRY_PTR)file_ptr->DEV_DATA_PTR)->FLAGS;
            }
            break;
        case IO_IOCTL_HTTPSRV_FS_GET_LENGTH:
            if (file_ptr->DEV_DATA_PTR == NULL)
            {
                error_code = HTTPSRV_FS_ERROR_INVALID_FILE_HANDLE;
            }
            else
            {
                *((uint32_t *)param_ptr) = file_ptr->SIZE;
            }
            break;
        case IO_IOCTL_HTTPSRV_FS_GET_CURRENT_DATA_PTR:
            if (file_ptr->DEV_DATA_PTR == NULL)
            {
                error_code = HTTPSRV_FS_ERROR_INVALID_FILE_HANDLE;
            }
            else
            {
                *((unsigned char **)param_ptr) =
                    ((HTTPSRV_FS_DIR_ENTRY_PTR)file_ptr->DEV_DATA_PTR)->DATA + file_ptr->LOCATION;
            }
            break;
        case IO_IOCTL_HTTPSRV_FS_GET_LAST_ERROR:
            error_code = file_ptr->ERROR;
            break;
        default:
            error_code = HTTPSRV_FS_ERROR_INVALID_IOCTL_CMD;
            break;
    }

    return (error_code);
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : httpsrv_fs_cmp
 * Returned Value   : -1, 0, 1 depending on path1 <, ==, > path2.
 * Comments         : Compares file paths. Not case sensitive. Both delimiters
 *                    '/' and '\' supported.
 *
 *END*---------------------------------------------------------------------*/

static int32_t httpsrv_fs_cmp(
    /* [IN] first file path to compare */
    char *path1,

    /* [IN] second file path to compare */
    char *path2)
{ /* Body */
    uint32_t ch1, ch2;

    if (path1 == path2)
        return 0;
    if (path1 == NULL)
        return -1;
    if (path2 == NULL)
        return 1;
    do
    {
        ch1 = *path1++;
        ch2 = *path2++;
        if (ch1 == '\\')
            ch1 = '/';
        if ((uint32_t)(ch1 - 'a') <= (uint32_t)('z' - 'a'))
            ch1 = ch1 - 'a' + 'A';
        if (ch2 == '\\')
            ch2 = '/';
        if ((uint32_t)(ch2 - 'a') <= (uint32_t)('z' - 'a'))
            ch2 = ch2 - 'a' + 'A';
        if ((ch1 == '\0') || (ch2 == '\0'))
            return (int32_t)(ch1 - ch2);
    } while (ch1 == ch2);
    return (int32_t)(ch1 - ch2);
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : httpsrv_fs_open_file
 * Returned Value   : Pointer to HTTPSRV_FS directory entry or NULL.
 * Comments         : Searches for specified file and returns directory entry.
 *
 *END*---------------------------------------------------------------------*/

static const HTTPSRV_FS_DIR_ENTRY *httpsrv_fs_open_file(
    /* [IN] HTTPSRV_FS IO drive information */
    /*HTTPSRV_FS_DRIVE_STRUCT_PTR   drive_ptr,*/

    /* [IN] file path */
    char *pathname,

    /* [OUT] error result */
    int32_t *error_ptr)
{
    const HTTPSRV_FS_DIR_ENTRY *entry;

    if (/*(drive_ptr == NULL) ||*/ (pathname == NULL) || (*pathname == '\0'))
    {
        *error_ptr = HTTPSRV_FS_INVALID_PARAMETER;
        return NULL;
    }
    *error_ptr = HTTPSRV_FS_FILE_NOT_FOUND;
    entry      = /* (HTTPSRV_FS_DIR_ENTRY_PTR)drive_ptr-> */ ROOT;
    while (entry->NAME != NULL)
    {
        if (httpsrv_fs_cmp(entry->NAME, pathname) == 0)
        {
            *error_ptr = HTTPSRV_FS_OK;
            return entry;
        }
        entry++;
    }
    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : httpsrv_fs_read
 * Returned Value   : Number of bytes actually read.
 * Comments         : Reads given file and stores data into the given buffer.
 *
 *END*---------------------------------------------------------------------*/

static uint32_t httpsrv_fs_read(
    /* [IN/OUT] file to read from */
    HTTPSRV_FS_FILE_PTR file_fd_ptr,

    /* [IN] number of bytes to read */
    uint32_t num_bytes,

    /* [OUT] buffer to write the data to */
    char *buffer_address,

    /* [OUT] error information */
    int32_t *error_ptr)
{
    uint32_t i;
    unsigned char *data;

    *error_ptr = HTTPSRV_FS_OK;
    if (file_fd_ptr->DEV_DATA_PTR == NULL)
    {
        *error_ptr = HTTPSRV_FS_ERROR_INVALID_FILE_HANDLE;
        return 0;
    }
    if (file_fd_ptr->LOCATION >= file_fd_ptr->SIZE)
    {
        *error_ptr = HTTPSRV_FS_EOF;
        return 0;
    }
    if (num_bytes > file_fd_ptr->SIZE - file_fd_ptr->LOCATION)
    {
        num_bytes = file_fd_ptr->SIZE - file_fd_ptr->LOCATION;
    }
    data = ((HTTPSRV_FS_DIR_ENTRY_PTR)file_fd_ptr->DEV_DATA_PTR)->DATA + file_fd_ptr->LOCATION;
    for (i = num_bytes; i != 0; i--)
        *buffer_address++ = *data++;
    file_fd_ptr->LOCATION += num_bytes;
    return num_bytes;
}

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : httpsrv_fs_move_file_pointer
 * Returned Value   : New file location.
 * Comments         : Performs seek within given file.
 *
 *END*---------------------------------------------------------------------*/

static uint32_t httpsrv_fs_move_file_pointer(
    /* [IN/OUT] file to seek within */
    HTTPSRV_FS_FILE_PTR file_fd_ptr,

    /* [OUT] error information */
    int32_t *error_ptr)
{
    *error_ptr = HTTPSRV_FS_OK;
    if (file_fd_ptr->DEV_DATA_PTR == NULL)
    {
        *error_ptr = HTTPSRV_FS_ERROR_INVALID_FILE_HANDLE;
        return 0;
    }
    if (file_fd_ptr->LOCATION > file_fd_ptr->SIZE)
    {
        *error_ptr            = HTTPSRV_FS_EOF;
        file_fd_ptr->LOCATION = file_fd_ptr->SIZE;
    }
    return (file_fd_ptr->LOCATION);
}

size_t HTTPSRV_FS_size(HTTPSRV_FS_FILE_PTR file_fd_ptr)
{
    size_t size = 0;
    if (file_fd_ptr)
    {
        size = file_fd_ptr->SIZE;
    }
    return size;
}
