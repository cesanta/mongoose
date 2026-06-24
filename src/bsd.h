#pragma once

#include "config.h"
#include "net_builtin.h"

#if MG_ENABLE_BSD_SOCKETS

#ifndef MG_ENABLE_BSD_PROTOTYPES
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#else
#include <errno.h>
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
struct pollfd { int fd; short events; short revents; };
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
#define SOL_SOCKET 0xffff
#define SO_REUSEADDR 2
#define SO_REUSEPORT 15
#define SO_KEEPALIVE 9
#define SO_ERROR 4
#define SO_BROADCAST 6
#define SO_RCVBUF 8
#define SO_SNDBUF 7
#define TCP_NODELAY 1
#define MSG_DONTWAIT 0x40
#define MSG_NOSIGNAL 0x4000
#define MSG_PEEK 0x02
#define F_GETFL 3
#define F_SETFL 4
#define O_NONBLOCK 0x0004
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
#define POLLIN 0x001
#define POLLOUT 0x004
#define POLLERR 0x008
#define POLLHUP 0x010
#define POLLNVAL 0x020
uint16_t htons(uint16_t);
uint16_t ntohs(uint16_t);
uint32_t htonl(uint32_t);
uint32_t ntohl(uint32_t);
#endif  // MG_ENABLE_BSD_PROTOTYPES

#define closesocket(a) close(a)

#ifndef MG_BSD_BACKLOG
#define MG_BSD_BACKLOG 5
#endif

#ifndef MG_BSD_CONNECT_TIMEOUT_MS
#define MG_BSD_CONNECT_TIMEOUT_MS 10000
#endif

// Mongoose-specific API
void mg_bsd_init(void);  // must be called before socket()
void mg_bsd_poll(struct mg_mgr *);  // process pending BSD commands

// Transport backend: implement these OR define MG_ENABLE_FREERTOS for the built-in backend
void *mg_bsd_transport_new(int domain, int type, int proto);
void  mg_bsd_transport_free(void *t);
int   mg_bsd_transport_listen(void *t, const struct sockaddr_in *addr);
void *mg_bsd_transport_accept(void *t, struct sockaddr_in *peer, bool nonblock);
ssize_t mg_bsd_transport_recv(void *t, void *buf, size_t len, bool nonblock);
ssize_t mg_bsd_transport_send(void *t, const void *buf, size_t len, bool nonblock);
int   mg_bsd_transport_connect(void *t, const struct sockaddr_in *addr, bool nonblock);
void  mg_bsd_transport_close(void *t);

// Standard BSD socket API
int socket(int, int, int);
int bind(int, const struct sockaddr *, socklen_t);
int listen(int, int);
int accept(int, struct sockaddr *, socklen_t *);
int connect(int, const struct sockaddr *, socklen_t);
ssize_t send(int, const void *, size_t, int);
ssize_t recv(int, void *, size_t, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
ssize_t recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
int close(int);
int shutdown(int, int);
int fcntl(int, int, int);
int setsockopt(int, int, int, const void *, socklen_t);
int getsockopt(int, int, int, void *, socklen_t *);
int getsockname(int, struct sockaddr *, socklen_t *);
int getpeername(int, struct sockaddr *, socklen_t *);
int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int poll(struct pollfd *, unsigned int, int);
struct hostent *gethostbyname(const char *);
int getaddrinfo(const char *, const char *, const struct addrinfo *, struct addrinfo **);
void freeaddrinfo(struct addrinfo *);
int inet_pton(int, const char *, void *);
const char *inet_ntop(int, const void *, char *, socklen_t);
in_addr_t inet_addr(const char *);
char *inet_ntoa(struct in_addr);

#endif  // MG_ENABLE_BSD_SOCKETS
