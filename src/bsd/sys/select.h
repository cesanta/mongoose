#pragma once

#ifndef MG_BSD_SELECT_H
#define MG_BSD_SELECT_H

#ifndef FD_SETSIZE
#define FD_SETSIZE 64
#endif

typedef unsigned long __fd_mask;
#define _NFDBITS ((int) sizeof(__fd_mask) * 8)
#define _howmany(x, y) (((x) + ((y) - 1)) / (y))
typedef struct fd_set {
  __fd_mask __fds_bits[_howmany(FD_SETSIZE, _NFDBITS)];
} fd_set;

#define __fdset_mask(n) ((__fd_mask) 1 << ((n) % _NFDBITS))
#define FD_CLR(n, p) ((p)->__fds_bits[(n) / _NFDBITS] &= ~__fdset_mask(n))
#define FD_ISSET(n, p) \
  (((p)->__fds_bits[(n) / _NFDBITS] & __fdset_mask(n)) != 0)
#define FD_SET(n, p) ((p)->__fds_bits[(n) / _NFDBITS] |= __fdset_mask(n))
#define FD_ZERO(p)                                                     \
  do {                                                                 \
    fd_set *_p = (p);                                                  \
    size_t _n = _howmany(FD_SETSIZE, _NFDBITS);                        \
    while (_n > 0) _p->__fds_bits[--_n] = 0;                           \
  } while (0)

struct timeval;
int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);

#endif  // MG_BSD_SELECT_H
