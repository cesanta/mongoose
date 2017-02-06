/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "./../../mongoose.h"
#include "stm32f4xx_hal_msp.h"
#include "stm32f4xx.h"
#include "user_params.h"

enum SampleState { ssIniting, ssConnectedWLAN, ssWorking, ssStopped };

static enum SampleState state = ssIniting;
static struct mg_mgr mgr;
static int msg_id;

void Error_Handler() {
  /* Turn LED4 (red) on/off */
  while (1) {
    BSP_LED_Toggle(LED4);
    Delay(100);
  }
}

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent) {
  switch (pWlanEvent->Event) {
    case SL_WLAN_CONNECT_EVENT:
      CLI_Write("Connected to WiFi\r\n");
      break;
    case SL_WLAN_DISCONNECT_EVENT:
      CLI_Write("Disconnected WiFi\r\n");
      break;
    default:
      CLI_Write("Got Wlan event %d\r\n", pWlanEvent->Event);
      break;
  }
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {
  switch (pNetAppEvent->Event) {
    case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
      CLI_Write("Got IP\r\n");
      state = ssConnectedWLAN;
      break;
    default:
      CLI_Write("Got NetApp Event: %d\r\n", pNetAppEvent->Event);
      break;
  }
}

struct mg_mqtt_topic_expression topic_expressions[] = {{"/test", 0}};

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  struct mg_mqtt_message *msg = (struct mg_mqtt_message *) p;

  switch (ev) {
    case MG_EV_CONNECT:
      if (*(int *) p != 0) {
        CLI_Write("Failed to connect to %s\r\n", MQTT_BROKER_ADDRESS);
      } else {
        CLI_Write("Connected to %s\r\n", MQTT_BROKER_ADDRESS);
      }
      struct mg_send_mqtt_handshake_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.user_name = MQTT_USER_NAME;
      opts.password = MQTT_USER_PWD;
      mg_set_protocol_mqtt(nc);
      mg_send_mqtt_handshake_opt(nc, "STM32", opts);
      break;
    case MG_EV_MQTT_CONNACK:
      if (msg->connack_ret_code != MG_EV_MQTT_CONNACK_ACCEPTED) {
        CLI_Write("Got mqtt connection error %d\n\r", msg->connack_ret_code);
      } else {
        CLI_Write("Connected to broker\n\r");
      }
      CLI_Write("Subscribing to /test\n\r");
      mg_mqtt_subscribe(nc, topic_expressions,
                        sizeof(topic_expressions) / sizeof(*topic_expressions),
                        ++msg_id);
      break;
    case MG_EV_MQTT_SUBACK:
      CLI_Write("Subscription acknowledged\r\n");
      state = ssWorking;
      break;
    case MG_EV_MQTT_PUBLISH:
      CLI_Write("Got incoming message %s: %.*s\r\n", msg->topic,
                (int) msg->payload.len, msg->payload.p);
      break;
    case MG_EV_POLL:
      if (state == ssWorking) {
        char msg[100];
        uint32_t tick = HAL_GetTick();
        int len = snprintf(msg, sizeof(msg), "Current tick: %u", tick);
        CLI_Write("Publishing message with tick=%u\r\n", tick);
        mg_mqtt_publish(nc, "/stuff", ++msg_id, MG_MQTT_QOS(0), msg, len);
      }
      break;
    case MG_EV_CLOSE:
      CLI_Write("Connection to broker is closed\r\n");
      state = ssStopped;
      break;
    default:
      break;
  }
}

int main(void) {
  stopWDT();
  initClk();

  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  CLI_Configure();
  CLI_Write("\n\n\n**** Hello ****\r\n");
  CLI_Write("Initializing CC3100 on SPI%d\r\n", SPIx_NUMBER);

  int ret = sl_Start(NULL, NULL, NULL);

  SlSecParams_t sec_params;
  memset(&sec_params, 0, sizeof(sec_params));
  sec_params.Key = NET_PWD;
  sec_params.KeyLen = sizeof(NET_PWD) - 1;
  sec_params.Type = NET_SECURITY;

  CLI_Write("Starting WiFi connect\r\n");

  ret = sl_WlanConnect(NET_SSID, sizeof(NET_SSID) - 1, 0, &sec_params, NULL);

  CLI_Write("Done, waiting for events\n\r");

  while (1) {
    BSP_LED_Toggle(LED3);
    _SlNonOsMainLoopTask();
    if (state == 1) {
      mg_mgr_init(&mgr, NULL);
      if (mg_connect(&mgr, MQTT_BROKER_ADDRESS, ev_handler) == NULL) {
        CLI_Write("Failed to create connection\n\r");
      }
      state = 2;
    }
    mg_mgr_poll(&mgr, 1000);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
  /* Turn LED4 (red) on: Transfer error in reception/transmission process */
  BSP_LED_On(LED4);
}
