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
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT 93
#endif

#if MG_ENABLE_BSD_PROTOTYPES
#include <sys/select.h>
#ifndef MG_BSD_SELECT_H
#error "Add the Mongoose BSD compatibility directory to the include path"
#else
#include <mg_bsd_compat.h>
#endif
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
