/*
 * C CGI Library version 1.2
 *
 * Author:  Stephen C. Losen,  University of Virginia
 *
 * Copyright 2015 Stephen C. Losen.  Distributed under the terms
 * of the GNU Lesser General Public License (LGPL 2.1)
 *
 * CGI_prefork_server() is a pre forking SCGI server that handles web
 * requests by calling a callback function provided by the user.
 * The parent process forks a configurable number of child processes
 * to accept web requests.
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <syslog.h>
#include <stdio.h>

#define SCORE_MAX_PROC 10000
#define SCORE_IDLE 1
#define SCORE_BUSY 2

/* scoreboard structs to keep track of child process state */

struct score_state {
    pid_t pid;
    int   state;
};

struct score_board {
    int numproc;
    int numidle;
    int minidle;
    int maxidle;
    int maxproc;
    struct score_state proc[1];
};

/* score_new() creates new scoreboard */

static struct score_board *
score_new(int maxproc, int minidle, int maxidle) {
    struct score_board *scb;
    int size = sizeof(*scb) + maxproc * sizeof(scb->proc);
    scb = (struct score_board *) malloc(size);
    memset(scb, 0, size);
    scb->maxproc = maxproc;
    scb->minidle = minidle;
    scb->maxidle = maxidle;
    return scb;
}

/* score_find() finds process entry and returns its index */

static int
score_find(struct score_board *scb, pid_t pid) {
    int i;
    for (i = 0; i < scb->maxproc; i++) {
        if (scb->proc[i].pid == pid) {
            return i;
        }
    }
    return -1;
}

/* score_add() adds a new pid to the scoreboard */

static void
score_add(struct score_board *scb, pid_t pid) {
    int i = score_find(scb, 0);
    if (i >= 0) {
        scb->proc[i].pid = pid;
        scb->proc[i].state = SCORE_IDLE;
        scb->numproc++;
        scb->numidle++;
    }
}

/* score_remove() removes a pid from the scoreboard */

static void
score_remove(struct score_board *scb, pid_t pid) {
    int i = score_find(scb, pid);
    if (i >= 0) {
        scb->proc[i].pid = 0;
        scb->numproc--;
        if (scb->proc[i].state == SCORE_IDLE) {
            scb->numidle--;
        }
    }
}

/* score_update() changes pid state to busy or idle */

static void
score_update(struct score_board *scb, struct score_state *st) {
    int i = score_find(scb, st->pid);
    if (i >= 0 && scb->proc[i].state != st->state) {
        scb->proc[i].state = st->state;
        if (st->state == SCORE_IDLE) {
            scb->numidle++;
        }
        else {
            scb->numidle--;
        }
    }
}

/* score_kill() finds an idle process and kills it */

static void
score_kill(struct score_board *scb) {
    int i;
    for (i = 0; i < scb->maxproc; i++) {
        if (scb->proc[i].pid != 0 && scb->proc[i].state == SCORE_IDLE) {
            kill(scb->proc[i].pid, SIGTERM);
            return;
        }
    }
}

/*
 * setup_sock() opens a TCP socket, binds it to a local address
 * specified by host and port, calls listen(), and returns the
 * socket.
 */

static int
setup_sock(const char *host, int port) {
    struct sockaddr_in local;
    struct hostent *h;
    int sock;

    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    if (host == 0 || *host == 0) {
        local.sin_addr.s_addr = INADDR_ANY;
    }
    else if (!isdigit(host[0]) ||
        inet_aton(host, &local.sin_addr) == 0)
    {
        if ((h = gethostbyname(host)) != 0) {
            memcpy(&local.sin_addr, h->h_addr_list[0], h->h_length);
        }
        else {
            local.sin_addr.s_addr = INADDR_ANY;
        }
    }
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return sock;
    }
    if (bind(sock, (struct sockaddr *) &local, sizeof(local)) < 0 ||
        listen(sock, 5) < 0)
    {
        close(sock);
        return -1;
    }
    return sock;
}

/*
 * read_env() reads the environment from the SCGI client (httpd).
 * We read the length of the environment, which is an ASCII decimal
 * string terminated by ':'.  We read the environment, which
 * is a sequence of null terminated strings where each pair of strings
 * is an environment variable name and value.  After the environment
 * is a comma. (Any remaining input is POST data, which we leave
 * unread.)  We replace the null between each variable name and value
 * with '=' and we return an array of char * pointers to the
 * environment variable strings.
 */

static char **
read_env(void) {
    char **env;
    char *buf, *p;
    int i, k, len;

    /* read length of environment data, eg, "175:" */

    for (len = 0; isdigit(k = getc(stdin)); ) {
        len = len * 10 + k - '0';
    }
    if (k != ':' || len < 4) {
        return 0;
    }

    /* read the environment strings */

    buf = (char *) malloc(len);
    if (fread(buf, 1, len, stdin) != len || getc(stdin) != ',') {
        free(buf);
        return 0;
    }

    /* replace null between each name and value with '=' */

    for (i = k = 0; i < len; i++) {
        if (buf[i] == 0 && (++k & 1)) {
            buf[i] = '=';
        }
    }

    /* build environment array */

    k = k / 2 + 1;
    env = (char **) malloc(k * sizeof(*env));
    p = buf;
    for (i = k = 0; i < len; i++) {
        if (buf[i] == 0) {
            env[k++] = p;
            p = buf + i + 1;
        }
    }
    env[k] = 0;
    return env;
}

/* terminate() sets a flag when we get SIGTERM */

static int terminate_flag = 0;

static void
terminate(int sig) {
    terminate_flag = 1;
}

/* child_handler() sets a flag when a child exits */

static int child_exited = 0;

static void
child_handler(int sig) {
    child_exited = 1;
}

/*
 * set_handler() sets a signal handler.  We use this instead of
 * signal() because it is more portable and system calls tend to
 * be restarted when signal() is used.  In particular we need
 * read() to return if we catch a signal.
 */

static int
set_handler(int sig, void (*handler)(int sig)) {
    struct sigaction action;
    action.sa_handler = handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    return sigaction(sig, &action, 0);
}

/*
 * CGI_prefork_server() pre forks child processes to handle SCGI requests.
 * The parent process opens a TCP socket that listens on a local
 * address specified by "host" and "port".  Each child handles "maxreq"
 * requests and exits.  If maxreq <= 0 then maxreq is unlimited.  The
 * parent process creates or destroys child processes to maintain between
 * "minidle" and "maxidle" idle children, and no more than "maxproc"
 * total children.  To handle a request, a child 1) accepts a connection
 * on the listen socket from the SCGI client, 2) redirects stdin and
 * stdout to the socket, 3) reads the environment from the SCGI client,
 * 4) calls the "callback" function, 5) closes the  socket and restores
 * the environment.  Child processes notify the parent when they
 * become busy or idle by writing messages to a pipe.
 */

void
CGI_prefork_server(const char *host, int port, const char *pidfile,
    int maxproc, int minidle, int maxidle, int maxreq,
    void (*callback)(void))
{
    int i, sock, fd;
    struct score_state message;
    struct score_board *scb;
    pid_t pid;
    FILE *fp;
    int pfd[2];
    char **realenv, **tmpenv;
    char *tmpbuf;
    extern char **environ;

    /* sanity check arguments */

    if (callback == 0) {
        syslog(LOG_ERR, "CGI_prefork_server(): null callback "
            "function pointer");
        return;
    }

    if (minidle <= 0) {
        minidle = 2;
    }
    if (maxidle <= minidle) {
        maxidle = minidle + 2;
    }
    if (maxproc <= 0) {
        maxproc = maxidle;
    }
    if (maxproc > SCORE_MAX_PROC) {
        maxproc = SCORE_MAX_PROC;
    }
    syslog(LOG_INFO, "CGI_prefork_server(): maxproc = %d, minidle = %d, "
        "maxidle = %d, maxreq = %d", maxproc, minidle, maxidle, maxreq);

    /* parent puts self into the background */

    if (fork() != 0) {
        _exit(0);
    }
    setsid();
    set_handler(SIGTERM, terminate);
    set_handler(SIGCHLD, child_handler);
    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);

    /* write our pid to pidfile */

    if (pidfile != 0 && *pidfile != 0 &&
        (fp = fopen(pidfile, "w")) != 0)
    {
        fprintf(fp, "%d\n", getpid());
        fclose(fp);
    }

    /* create child process scoreboard */

    scb = score_new(maxproc, minidle, maxidle);

    /* parent opens the listen socket, children accept() connections */

    if ((sock = setup_sock(host, port)) < 0) {
        syslog(LOG_ERR, "CGI_prefork_server(): setup_sock() failed: %m");
        return;
    }

    /* open pipe to receive messages from child processes */

    pipe(pfd);

    /* parent manages child processes */

    for (;;) {

        /* fork child if necessary */

        if (scb->numidle < scb->minidle && scb->numproc < scb->maxproc) {
            if ((pid = fork()) == 0) {
                break;
            }
            else if (pid > 0) {
                score_add(scb, pid);
                continue;
            }
            else {
                syslog(LOG_ERR, "CGI_prefork_server(): fork() failed: %m");
                if (scb->numproc == 0) {
                    return;
                }
            }
        }

        /*
         * read status message from child.  The read() call returns with
         * an error if we catch SIGCHLD or SIGTERM.
         */

        if (child_exited == 0 && terminate_flag == 0 &&
            read(pfd[0], &message, sizeof(message)) == sizeof(message))
        {
            score_update(scb, &message);
        }

        /* kill everything and exit if we got SIGTERM */

        if (terminate_flag != 0) {
            set_handler(SIGTERM, SIG_IGN);
            kill(0, SIGTERM);        /* kill process group */
            while(wait(0) >= 0)
                ;
            exit(0);
        }

        /* kill idle child if necessary */

        if (scb->numidle > scb->maxidle) {
            score_kill(scb);
        }

        /* wait for exited children */

        child_exited = 0;
        while ((pid = waitpid(-1, 0, WNOHANG)) > 0) {
            score_remove(scb, pid);
        }
    }

    /* child handles maxreq requests and exits */

    set_handler(SIGTERM, SIG_DFL);
    set_handler(SIGCHLD, SIG_DFL);
    close(pfd[0]);
    message.pid = getpid();
    realenv = environ;

    for (i = 0; i < maxreq || maxreq <= 0; i++) {

        /* accept connection from SCGI client (httpd) */

        if ((fd = accept(sock, 0, 0)) < 0) {
            syslog(LOG_ERR, "CGI_prefork_server(): accept() failed: %m");
            break;
        }

        /* notify parent we are busy */

        message.state = SCORE_BUSY;
        write(pfd[1], &message, sizeof(message));

        /* redirect stdin and stdout to socket */

        dup2(fd, 0);
        dup2(fd, 1);
        close(fd);

        /* read environment and call callback */

        if ((tmpenv = read_env()) != 0) {
            tmpbuf = tmpenv[0];
            environ = tmpenv;
            callback();
        }
        else {
            fputs("Content-type: text/plain\r\n\r\n"
                "CGI_prefork_server() could not read environment.\r\n",
                stdout);
            syslog(LOG_ERR, "CGI_prefork_server(): could not read "
                "environment");
        }

        /* close socket and restore environment */

        freopen("/dev/null", "r", stdin);  /* closes socket */
        freopen("/dev/null", "w", stdout);
        environ = realenv;
        if (tmpenv != 0) {
            free(tmpbuf);
            free(tmpenv);
        }

        /* notify parent we are idle */

        message.state = SCORE_IDLE;
        write(pfd[1], &message, sizeof(message));
    }
    _exit(0);
}
