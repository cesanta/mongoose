---
title: "struct mg_connection"
decl_name: "struct mg_connection"
symbol_kind: "struct"
signature: |
  struct mg_connection {
    struct mg_connection *next, *prev; /* mg_mgr::active_connections linkage */
    struct mg_connection *listener;    /* Set only for accept()-ed connections */
    struct mg_mgr *mgr;                /* Pointer to containing manager */
  
    sock_t sock; /* Socket to the remote peer */
    int err;
    union socket_address sa; /* Remote peer address */
    size_t recv_mbuf_limit;  /* Max size of recv buffer */
    struct mbuf recv_mbuf;   /* Received data */
    struct mbuf send_mbuf;   /* Data scheduled for sending */
    time_t last_io_time;     /* Timestamp of the last socket IO */
    double ev_timer_time;    /* Timestamp of the future MG_EV_TIMER */
  #if MG_ENABLE_SSL
    void *ssl_if_data; /* SSL library data. */
  #endif
    mg_event_handler_t proto_handler; /* Protocol-specific event handler */
    void *proto_data;                 /* Protocol-specific data */
    void (*proto_data_destructor)(void *proto_data);
    mg_event_handler_t handler; /* Event handler function */
    void *user_data;            /* User-specific data */
    union {
      void *v;
      /*
       * the C standard is fussy about fitting function pointers into
       * void pointers, since some archs might have fat pointers for functions.
       */
      mg_event_handler_t f;
    } priv_1;
    void *priv_2;
    void *mgr_data; /* Implementation-specific event manager's data. */
    struct mg_iface *iface;
    unsigned long flags;
  /* Flags set by Mongoose */
  #define MG_F_LISTENING (1 << 0)          /* This connection is listening */
  #define MG_F_UDP (1 << 1)                /* This connection is UDP */
  #define MG_F_RESOLVING (1 << 2)          /* Waiting for async resolver */
  #define MG_F_CONNECTING (1 << 3)         /* connect() call in progress */
  #define MG_F_SSL (1 << 4)                /* SSL is enabled on the connection */
  #define MG_F_SSL_HANDSHAKE_DONE (1 << 5) /* SSL hanshake has completed */
  #define MG_F_WANT_READ (1 << 6)          /* SSL specific */
  #define MG_F_WANT_WRITE (1 << 7)         /* SSL specific */
  #define MG_F_IS_WEBSOCKET (1 << 8)       /* Websocket specific */
  
  /* Flags that are settable by user */
  #define MG_F_SEND_AND_CLOSE (1 << 10)      /* Push remaining data and close  */
  #define MG_F_CLOSE_IMMEDIATELY (1 << 11)   /* Disconnect */
  #define MG_F_WEBSOCKET_NO_DEFRAG (1 << 12) /* Websocket specific */
  #define MG_F_DELETE_CHUNK (1 << 13)        /* HTTP specific */
  #define MG_F_ENABLE_BROADCAST (1 << 14)    /* Allow broadcast address usage */
  
  #define MG_F_USER_1 (1 << 20) /* Flags left for application */
  #define MG_F_USER_2 (1 << 21)
  #define MG_F_USER_3 (1 << 22)
  #define MG_F_USER_4 (1 << 23)
  #define MG_F_USER_5 (1 << 24)
  #define MG_F_USER_6 (1 << 25)
  };
---

Mongoose connection. 

