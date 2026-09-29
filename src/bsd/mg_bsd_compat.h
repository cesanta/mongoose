#pragma once

#ifndef MG_BSD_COMPAT_H
#define MG_BSD_COMPAT_H

#include <stdint.h>

typedef unsigned int socklen_t;
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
#define h_addr h_addr_list[0]
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
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT 93
#endif

uint16_t mg_ntohs(uint16_t);
uint32_t mg_ntohl(uint32_t);
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
struct hostent *gethostbyname(const char *);
int getaddrinfo(const char *, const char *, const struct addrinfo *,
                struct addrinfo **);
void freeaddrinfo(struct addrinfo *);
int inet_pton(int, const char *, void *);
const char *inet_ntop(int, const void *, char *, socklen_t);
in_addr_t inet_addr(const char *);
char *inet_ntoa(struct in_addr);

#define closesocket(a) close(a)

#include <poll.h>

#endif  // MG_BSD_COMPAT_H
