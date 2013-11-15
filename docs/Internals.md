# Mongoose Internals

Mongoose is multithreaded web server. `mg_start()` function allocates
web server context (`struct mg_context`), which holds all information
about web server instance:

- configuration options. Note that mongoose makes internal copies of
  passed options.
- SSL context, if any
- user-defined callbacks
- opened listening sockets
- a queue for accepted sockets
- mutexes and condition variables for inter-thread synchronization

When `mg_start()` returns, all initialization is quaranteed to be complete
(e.g. listening ports are opened, SSL is initialized, etc). `mg_start()` starts
two threads: a master thread, that accepts new connections, and several
worker threads, that process accepted connections. The number of worker threads
is configurable via `num_threads` configuration option. That number puts a
limit on number of simultaneous requests that can be handled by mongoose.

When master thread accepts new connection, a new accepted socket (described by
`struct socket`) it placed into the accepted sockets queue,
which has size of 20 (see [code](https://github.com/cesanta/mongoose/blob/3892e0199e6ca9613b160535d9d107ede09daa43/mongoose.c#L486)). Any idle worker thread
can grab accepted sockets from that queue. If all worker threads are busy,
master thread can accept and queue up to 20 more TCP connections,
filling up the queue.
In the attempt to queue next accepted connection, master thread blocks
until there is space in a queue. When master thread is blocked on a
full queue, TCP layer in OS can also queue incoming connection.
The number is limited by the `listen()` call parameter on listening socket,
which is `SOMAXCONN` in case of Mongoose, and depends on a platform.

Worker threads are running in an infinite loop, which in simplified form
looks something like this:

    static void *worker_thread() {
      while (consume_socket()) {
        process_new_connection();
      }
    }

Function `consume_socket()` gets new accepted socket from the mongoose socket
queue, atomically removing it from the queue. If the queue is empty,
`consume_socket()` blocks and waits until new sockets are placed in a queue
by the master thread. `process_new_connection()` actually processes the
connection, i.e. reads the request, parses it, and performs appropriate action
depending on a parsed request.

Master thread uses `poll()` and `accept()` to accept new connections on
listening sockets. `poll()` is used to avoid `FD_SETSIZE` limitation of
`select()`. Since there are only a few listening sockets, there is no reason
to use hi-performance alternatives like `epoll()` or `kqueue()`. Worker
threads use blocking IO on accepted sockets for reading and writing data.
All accepted sockets have `SO_RCVTIMEO` and `SO_SNDTIMEO` socket options set
(controlled by `request_timeout_ms` mongoose option, 30 seconds default) which
specify read/write timeout on client connection.


