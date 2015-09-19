/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * This is the cloud endpoint of the Raspberry Pi camera/LED example
 * of the Mongoose networking library.
 * It is a simple web server, serving both static files, a REST API handler,
 * and a WebSocket handler.
 */
#include "mongoose.h"

static struct mg_serve_http_opts web_root_opts;

/*
 * Forwards the jpeg frame data to all open mjpeg connections.
 *
 * Incoming messages follow a very simple binary frame format:
 * 4 bytes: timestamp (in network byte order)
 * n bytes: jpeg payload
 *
 * The timestamp is used to compute a lag.
 * It's done in a quite stupid way as it requires the device clock
 * to be synchronized with the cloud endpoint.
 */
static void push_frame_to_clients(struct mg_mgr *mgr,
                                  const struct websocket_message *wm) {
  struct mg_connection *nc;
  /*
   * mjpeg connections are tagged with the MG_F_USER_2 flag so we can find them
   * my scanning the connection list provided by the mongoose manager.
   */
  for (nc = mg_next(mgr, NULL); nc != NULL; nc = mg_next(mgr, nc)) {
    if (!(nc->flags & MG_F_USER_2)) continue;  // Ignore un-marked requests

    mg_printf(nc, "--w00t\r\nContent-Type: image/jpeg\r\n"
              "Content-Length: %lu\r\n\r\n", (unsigned long) wm->size);
    mg_send(nc, wm->data, wm->size);
    mg_send(nc, "\r\n", 2);
    printf("Image pushed to %p\n", nc);
  }
}

/*
 * Forwards API payload to the device, by scanning through
 * all the connections to find those that are tagged as WebSocket.
 */
static void send_command_to_the_device(struct mg_mgr *mgr,
                                       const struct mg_str *cmd) {
  struct mg_connection *nc;
  for (nc = mg_next(mgr, NULL); nc != NULL; nc = mg_next(mgr, nc)) {
    if (!(nc->flags & MG_F_IS_WEBSOCKET)) continue;  // Ignore non-websocket requests

    mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, cmd->p, cmd->len);
    printf("Sent API command [%.*s] to %p\n", (int) cmd->len, cmd->p, nc);
  }
}

/*
 * Main event handler. Receives data events and dispatches to
 * the appropriate handler function.
 *
 * 1. RESTful API requests are handled by send_command_to_the_device.
 * 2. requests to /mpeg are established and left open waiting for data to arrive
 *    from WebSocket.
 * 3. WebSocket frames are handled by push_frame_to_clients.
 * 4. All other connections are passed to the mg_serve_http handler
 *    which serves static files.
 */
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (mg_vcmp(&hm->uri, "/mjpg") == 0) {
        nc->flags |= MG_F_USER_2;   /* Set a mark on image requests */
        mg_printf(nc, "%s",
                "HTTP/1.0 200 OK\r\n"
                "Cache-Control: no-cache\r\n"
                "Pragma: no-cache\r\n"
                "Expires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
                "Connection: close\r\n"
                "Content-Type: multipart/x-mixed-replace; "
                "boundary=--w00t\r\n\r\n");
      } else if (mg_vcmp(&hm->uri, "/api") == 0 && hm->body.len > 0) {
        /*
         * RESTful API call. HTTP message body should be a JSON message.
         * We should parse it and take appropriate action.
         * In our case, simply forward that call to the device.
         */
        printf("API CALL: [%.*s] [%.*s]\n", (int) hm->method.len, hm->method.p,
               (int) hm->body.len, hm->body.p);
        send_command_to_the_device(nc->mgr, &hm->body);
        mg_printf(nc, "HTTP/1.0 200 OK\nContent-Length: 0\n\n");
      } else {
        /* Delegate to the static web server handler for all other paths. */
        mg_serve_http(nc, hm, web_root_opts);
      }
      break;
    case MG_EV_WEBSOCKET_FRAME:
      printf("Got websocket frame, size %lu\n", (unsigned long) wm->size);
      push_frame_to_clients(nc->mgr, wm);
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <listening_addr>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  printf("Listening on: [%s]\n", argv[1]);
  mg_mgr_init(&mgr, NULL);

  /*
   * mg_bind() creates a listening connection on a given ip:port and
   * with an attached event handler.
   * The event handler will only trigger TCP events until the http
   * protocol handler is installed.
   */
  if ((nc = mg_bind(&mgr, argv[1], ev_handler)) == NULL) {
    fprintf(stderr, "Error binding to %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  mg_set_protocol_http_websocket(nc);
  web_root_opts.document_root =  "./web_root";

  /*
   * We explicitly hand over control to the Mongoose manager
   * in this event loop and we can easily multiplex other activities.
   */
  for(;;) {
    mg_mgr_poll(&mgr, 1000);
  }

  return EXIT_SUCCESS;
}
