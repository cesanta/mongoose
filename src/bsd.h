#pragma once

#include "config.h"
#include "net_builtin.h"

#if MG_ENABLE_BSD_SOCKETS
void mg_bsd_init(struct mg_mgr *);
void mg_bsd_poll(struct mg_mgr *);

#include <errno.h>
#include <fcntl.h>

#ifndef EAGAIN
#define EAGAIN 11
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif
#ifndef EINPROGRESS
#define EINPROGRESS 36
#endif
#ifndef ENOTCONN
#define ENOTCONN 107
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED 111
#endif
#ifndef ECONNRESET
#define ECONNRESET 104
#endif
#ifndef EADDRINUSE
#define EADDRINUSE 98
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif
#ifndef EBADF
#define EBADF 9
#endif
#ifndef EMFILE
#define EMFILE 24
#endif
#ifndef ENOMEM
#define ENOMEM 12
#endif
#ifndef EIO
#define EIO 5
#endif
#ifndef EINVAL
#define EINVAL 22
#endif
#ifndef EPERM
#define EPERM 1
#endif
#ifndef EPIPE
#define EPIPE 32
#endif
#ifndef EOPNOTSUPP
#define EOPNOTSUPP 95
#endif
#ifndef EISCONN
#define EISCONN 106
#endif
#ifndef EDESTADDRREQ
#define EDESTADDRREQ 121
#endif
#ifndef EMSGSIZE
#define EMSGSIZE 90
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT 93
#endif

#if defined(__DCC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif
struct mg_bsd_udp_frame {
  uint32_t ip;
  uint16_t port, len;
};
#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
#endif

#if MG_ENABLE_BSD_PROTOTYPES
typedef unsigned int socklen_t;
typedef unsigned long nfds_t;
typedef uint32_t in_addr_t;
struct in_addr { in_addr_t s_addr; };
struct in6_addr { uint8_t s6_addr[16]; };
struct sockaddr { uint16_t sa_family; char sa_data[14]; };
struct sockaddr_in {
  uint16_t sin_family;
  uint16_t sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};
struct sockaddr_in6 {
  uint16_t sin6_family;
  uint16_t sin6_port;
  uint32_t sin6_flowinfo;
  struct in6_addr sin6_addr;
  uint32_t sin6_scope_id;
};
struct sockaddr_storage { uint16_t ss_family; char __ss_pad[126]; };
struct hostent {
  char *h_name;
  char **h_aliases;
  int h_addrtype;
  int h_length;
  char **h_addr_list;
};
struct addrinfo {
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  socklen_t ai_addrlen;
  struct sockaddr *ai_addr;
  char *ai_canonname;
  struct addrinfo *ai_next;
};
struct pollfd { int fd; short events; short revents; };

#if !defined(FD_SET) && !defined(FD_ZERO) && !defined(FD_ISSET)
#define MG_BSD_SELECT_H
typedef unsigned long __fd_mask;
#ifndef FD_SETSIZE
#define FD_SETSIZE 64
#endif
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
#endif

#define h_addr h_addr_list[0]
#define AF_INET 2
#define AF_INET6 10
#define AF_UNSPEC 0
#define PF_INET AF_INET
#define PF_INET6 AF_INET6
#define PF_UNSPEC AF_UNSPEC
#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define INADDR_ANY 0
#define INADDR_LOOPBACK 0x7f000001
#define INET_ADDRSTRLEN 16
#define SOL_SOCKET 0xffff
#define SO_REUSEADDR 2
#define SO_REUSEPORT 15
#define SO_KEEPALIVE 9
#define SO_ERROR 4
#define SO_BROADCAST 6
#define SO_RCVBUF 8
#define SO_SNDBUF 7
#define SO_NOSIGPIPE 0x1022
#define TCP_NODELAY 1
#define MSG_DONTWAIT 0x40
#define MSG_NOSIGNAL 0x4000
#define MSG_PEEK 0x02
#ifndef F_GETFL
#define F_GETFL 3
#endif
#ifndef F_SETFL
#define F_SETFL 4
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK 0x4000
#endif
#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
#define AI_PASSIVE 0x0001
#define NI_MAXHOST 1025
#define NI_MAXSERV 32
#define POLLIN 0x001
#define POLLOUT 0x004
#define POLLERR 0x008
#define POLLHUP 0x010
#define POLLNVAL 0x020

#define htons(x) mg_ntohs(x)
#define ntohs(x) mg_ntohs(x)
#define htonl(x) mg_ntohl(x)
#define ntohl(x) mg_ntohl(x)

int socket(int, int, int);
int bind(int, const struct sockaddr *, socklen_t);
int listen(int, int);
int accept(int, struct sockaddr *, socklen_t *);
int connect(int, const struct sockaddr *, socklen_t);
ssize_t send(int, const void *, size_t, int);
ssize_t recv(int, void *, size_t, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *,
               socklen_t);
ssize_t recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
int close(int);
int shutdown(int, int);
int setsockopt(int, int, int, const void *, socklen_t);
int getsockopt(int, int, int, void *, socklen_t *);
int getsockname(int, struct sockaddr *, socklen_t *);
int getpeername(int, struct sockaddr *, socklen_t *);
int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int poll(struct pollfd *, nfds_t, int);
struct hostent *gethostbyname(const char *);
int getaddrinfo(const char *, const char *, const struct addrinfo *,
                struct addrinfo **);
void freeaddrinfo(struct addrinfo *);
int inet_pton(int, const char *, void *);
const char *inet_ntop(int, const void *, char *, socklen_t);
in_addr_t inet_addr(const char *);
char *inet_ntoa(struct in_addr);
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#endif  // MG_ENABLE_BSD_PROTOTYPES

#include <sys/time.h>

#else
#define mg_bsd_init(mgr)
#define mg_bsd_poll(mgr)
#endif  // MG_ENABLE_BSD_SOCKETS
