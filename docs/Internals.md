# Mongoose Internals

Mongoose has single-threaded, event-driven, asynchronous, non-blocking core.
When mongoose server instance is created, it contains an information about
the configuration and the state of each connection:

    struct mg_server {
      sock_t listening_sock;          // Listening socket
      union socket_address lsa;       // Listening socket address
      struct linked_list_link active_connections;
      struct linked_list_link uri_handlers;
      char *config_options[NUM_OPTIONS];
      ...
    };

A server instance is capable on listening on only one port. After creation,
a server instance has a list
of active connections, initially empty. It has a list of URI handlers,
initially empty, and configuration parameters. Configuration can be
altered by `mg_set_option()`, and new URI handlers could be added by
`mg_add_uri_handler()`.
