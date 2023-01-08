#ifndef _DRIVER_TEENSY41_H_
#define _DRIVER_TEENSY41_H_

//#include "mip.h"

#include "mongoose.h"

#if defined(MG_ENABLE_MIP) && (MG_ARCH == MG_ARCH_TEENSY41)

extern struct mip_driver mip_driver_teensy41;

#endif

#endif
