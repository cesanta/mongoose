/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#include "app.h"
#include "system/debug/sys_debug.h"
#include "./../../../../../mongoose.h"
#include "user_params.h"

static struct mg_mgr mgr;
static int msg_id;
struct mg_mqtt_topic_expression topic_expressions[] = {{"/test", 0}};

APP_DATA appData;

void APP_Initialize(void) {
  appData.state = APP_STATE_INIT;
  DRV_RTCC_Start();
  mg_mgr_init(&mgr, NULL);
}

static void ev_handler(struct mg_connection *nc, int ev, void *p);

void APP_Tasks(void) {
  /*
   * Processing CC3100 Tasks
   * It looks like SYS task and might be processed in SYS_Tasks
   * But for this demo it is here to make this call
   * visible
   */
  _SlNonOsMainLoopTask();

  /* Check the application's current state. */
  switch (appData.state) {
    /* Application's initial state. */
    case APP_STATE_INIT: {
      SYS_PRINT("\n\r*** PIC32 MQTT CLIENT ***\n\r");

      SYS_PRINT("\n\rInitializing CC3100\n\r");
      int res = sl_Start(NULL, NULL, NULL);
      if (res != 0) {
        SYS_PRINT("FAILED\n\r");
        appData.state = APP_STATE_DONE;
        break;
      }

      SlSecParams_t sec_params;
      memset(&sec_params, 0, sizeof(sec_params));
      sec_params.Key = NET_PWD;
      sec_params.KeyLen = sizeof(NET_PWD) - 1;
      sec_params.Type = NET_SECURITY;

      SYS_PRINT("Connecting to WiFi\n\r");

      sl_WlanConnect(NET_SSID, sizeof(NET_SSID) - 1, 0, &sec_params, NULL);

      SYS_PRINT("Initialization done\n\r");

      appData.state = APP_STATE_SERVICE_TASKS;

      break;
    }

    case APP_STATE_CONNECT_BROKER: {
      if (mg_connect(&mgr, MQTT_BROKER_ADDRESS, ev_handler) == NULL) {
        SYS_PRINT("Failed to create connection\n\r");
        appData.state = APP_STATE_DONE;
      } else {
        appData.state = APP_STATE_SERVICE_TASKS;
      }
      break;
    }
    case APP_STATE_SERVICE_TASKS: {
      static uint32_t prev_poll_time = 0;
      uint32_t now = DRV_RTCC_TimeGet();
      if (now - prev_poll_time > 100) {
        /*
         * We cannot call mg_mgr_poll every cycle
         * it leads to SPI overload (internaly mg_mgr_poll calls
         * CC3100 via SPI
         */
        mg_mgr_poll(&mgr, 1);
        prev_poll_time = now;
      }
      break;
    }

    case APP_STATE_DONE: {
      /* Do nothing here */
      break;
    }
    default: {
      /* TODO: Handle error in application's state machine. */
      break;
    }
  }
}

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent) {
  switch (pWlanEvent->Event) {
    case SL_WLAN_CONNECT_EVENT:
      SYS_PRINT("Connected to WiFi\r\n");
      break;
    case SL_WLAN_DISCONNECT_EVENT:
      SYS_PRINT("Disconnected WiFi\r\n");
      break;
    default:
      SYS_PRINT("Got Wlan event %d\r\n", pWlanEvent->Event);
      break;
  }
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {
  switch (pNetAppEvent->Event) {
    case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
      SYS_PRINT("Got IP\r\n");
      appData.state = APP_STATE_CONNECT_BROKER;
      break;
    default:
      SYS_PRINT("Got NetApp Event: %d\r\n", pNetAppEvent->Event);
      break;
  }
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  struct mg_mqtt_message *msg = (struct mg_mqtt_message *) p;
  static int count1 = 0;
  switch (ev) {
    case MG_EV_CONNECT:
      if (*(int *) p != 0) {
        SYS_PRINT("Failed to connect to %s\r\n", MQTT_BROKER_ADDRESS);
      } else {
        SYS_PRINT("Connected to %s\r\n", MQTT_BROKER_ADDRESS);
      }
      struct mg_send_mqtt_handshake_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.user_name = MQTT_USER_NAME;
      opts.password = MQTT_USER_PWD;
      mg_set_protocol_mqtt(nc);
      mg_send_mqtt_handshake_opt(nc, "PIC32", opts);
      break;
    case MG_EV_MQTT_CONNACK:
      if (msg->connack_ret_code != MG_EV_MQTT_CONNACK_ACCEPTED) {
        SYS_PRINT("Got mqtt connection error %d\n\r", msg->connack_ret_code);
      } else {
        SYS_PRINT("Connected to broker\n\r");
      }
      SYS_PRINT("Subscribing to /test\n\r");
      mg_mqtt_subscribe(nc, topic_expressions,
                        sizeof(topic_expressions) / sizeof(*topic_expressions),
                        ++msg_id);
      nc->flags |= MG_F_USER_1;
      break;
    case MG_EV_MQTT_SUBACK:
      SYS_PRINT("Subscription acknowledged\r\n");
      break;
    case MG_EV_MQTT_PUBLISH:
      SYS_PRINT("Got incoming message %s: %.*s\r\n", msg->topic,
                (int) msg->payload.len, msg->payload.p);
      break;
    case MG_EV_POLL: {
      if (nc->flags & MG_F_USER_1) {
        char msg[100];
        static uint32_t prev_send = 0;
        uint32_t now = DRV_RTCC_TimeGet();
        if (now - prev_send > 1000) {
          int len = snprintf(msg, sizeof(msg), "Current RTC value: %u", now);
          SYS_PRINT("Publishing message with RTC value=%u\r\n", now);
          mg_mqtt_publish(nc, "/stuff", ++msg_id, MG_MQTT_QOS(0), msg, len);
          prev_send = now;
        }
      }
      break;
    }
    case MG_EV_CLOSE:
      SYS_PRINT("Connection to broker is closed\r\n");
      appData.state = APP_STATE_DONE;
      break;
    default:
      break;
  }
}
