#pragma once

#if MG_ARCH == MG_ARCH_RTTHREAD

#include <rtthread.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 1460
#endif

#endif // MG_ARCH == MG_ARCH_RTTHREAD
