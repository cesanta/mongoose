/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if (CS_PLATFORM == CS_P_NRF51 || CS_PLATFORM == CS_P_NRF52) && \
    defined(__ARMCC_VERSION)
int gettimeofday(struct timeval *tp, void *tzp) {
  /* TODO */
  tp->tv_sec = 0;
  tp->tv_usec = 0;
  return 0;
}
#endif
