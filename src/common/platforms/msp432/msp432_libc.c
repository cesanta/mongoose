
#if CS_PLATFORM == CS_P_MSP432

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

int gettimeofday(struct timeval *tp, void *tzp) {
  uint32_t ticks = Clock_getTicks();
  tp->tv_sec = ticks / 1000;
  tp->tv_usec = (ticks % 1000) * 1000;
  return 0;
}

#endif /* CS_PLATFORM == CS_P_MSP432 */
