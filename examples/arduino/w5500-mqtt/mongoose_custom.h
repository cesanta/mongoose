#pragma once

#include "Arduino.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#define MG_ARCH MG_ARCH_CUSTOM
#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_TCPIP 1
#define mkdir(a, b) (-1)
#define MG_IO_SIZE 512
//#define MG_ENABLE_LOG 0
