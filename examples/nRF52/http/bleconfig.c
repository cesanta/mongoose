
/* clang-format off */

#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "sdk_config.h"
#include "app_timer_appsh.h"
#include "app_scheduler.h"
#include "app_button.h"
#include "nordic_common.h"
#include "softdevice_handler_appsh.h"
#include "ble_advdata.h"
#include "ble_srv_common.h"
#include "ble_ipsp.h"
#include "ble_6lowpan.h"
#include "mem_manager.h"
#include "app_trace.h"

/*
 * arm-none-eabi-gcc has BYTE_ORDER already defined, so in order to avoid
 * warnings in lwip, we have to undefine it
 *
 * TODO: Check if in the future versions of nRF5 SDK that changes.
 *       Current version of nRF51 SDK: 0.8.0
 *                          nRF5 SDK:  0.9.0
 */
#undef BYTE_ORDER
#include "lwip/init.h"
#include "lwip/inet6.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/netif.h"
/*lint -save -e607 */
#include "lwip/tcp.h"
/*lint -restore */
#include "lwip/timers.h"
#include "nrf_platform_port.h"
#include "app_util_platform.h"
#include "iot_timer.h"
#include "ipv6_medium.h"
#include "SEGGER_RTT.h"
#include "myboard.h"

#define SCHED_MAX_EVENT_DATA_SIZE           128                                                     /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE                    12                                                      /**< Maximum number of events in the scheduler queue. */

#ifdef COMMISSIONING_ENABLED
#define ERASE_BUTTON_PIN_NO                 BSP_BUTTON_3                                            /**< Button used to erase commissioning settings. */
#endif // COMMISSIONING_ENABLED

#define APP_TIMER_OP_QUEUE_SIZE             5
#define LWIP_SYS_TICK_MS                    100                                                     /**< Interval for timer used as trigger to send. */
#define LED_BLINK_INTERVAL_MS               300                                                     /**< LED blinking interval. */

#define MAX_LENGTH_FILENAME                 128                                                     /**< Max length of filename to copy for the debug error handler. */

#define APPL_LOG                            app_trace_log                                           /**< Macro for logging application messages on UART, in case ENABLE_DEBUG_LOG_SUPPORT is not defined, no logging occurs. */
#define APPL_DUMP                           app_trace_dump                                          /**< Macro for dumping application data on UART, in case ENABLE_DEBUG_LOG_SUPPORT is not defined, no logging occurs. */

#define TCP_SERVER_PORT                     9000                                                    /**< TCP server listen port number. */
#define TCP_DATA_SIZE                       8                                                       /**< UDP Data size sent on remote. */

typedef enum
{
  TCP_STATE_IDLE,
  TCP_STATE_REQUEST_CONNECTION,
  TCP_STATE_CONNECTED,
  TCP_STATE_DATA_TX_IN_PROGRESS,
  TCP_STATE_DISCONNECTED
}tcp_state_t;

APP_TIMER_DEF(m_iot_timer_tick_src_id);                                                             /**< System Timer used to service CoAP and LWIP periodically. */
eui64_t                                     eui64_local_iid;                                        /**< Local EUI64 value that is used as the IID for*/
static ipv6_medium_instance_t               m_ipv6_medium;
static tcp_state_t                          m_tcp_state;                                            /**< TCP State information. */

#ifdef COMMISSIONING_ENABLED
static bool                                 m_power_off_on_failure = false;
static bool                                 m_identity_mode_active;
#endif // COMMISSIONING_ENABLED

/**@brief Function to handle interface up event. */
void nrf_driver_interface_up(void)
{
#ifdef COMMISSIONING_ENABLED
  commissioning_joining_mode_timer_ctrl(JOINING_MODE_TIMER_STOP_RESET);
#endif // COMMISSIONING_ENABLED

  APPL_LOG ("[APPL]: IPv6 interface up.\r\n");

  sys_check_timeouts();

  m_tcp_state = TCP_STATE_REQUEST_CONNECTION;

  LEDS_OFF(LED_ONE);
  LEDS_ON(LED_TWO);
}


/**@brief Function to handle interface down event. */
void nrf_driver_interface_down(void)
{
#ifdef COMMISSIONING_ENABLED
  commissioning_joining_mode_timer_ctrl(JOINING_MODE_TIMER_START);
#endif // COMMISSIONING_ENABLED

  APPL_LOG ("[APPL]: IPv6 interface down.\r\n");

  LEDS_OFF((DISPLAY_LED_0 | DISPLAY_LED_1 | DISPLAY_LED_2 | DISPLAY_LED_3));
  LEDS_ON(LED_ONE);

  m_tcp_state = TCP_STATE_DISCONNECTED;
}

/**@brief Timer callback used for periodic servicing of LwIP protocol timers.
 *
 * @details Timer callback used for periodic servicing of LwIP protocol timers.
 *
 * @param[in]   p_context   Pointer used for passing context. No context used in this application.
 */
static void system_timer_callback(iot_timer_time_in_ms_t wall_clock_value)
{
  UNUSED_VARIABLE(wall_clock_value);
  sys_check_timeouts();
}

/**@brief Function for starting connectable mode.
*/
static void connectable_mode_enter(void)
{
  uint32_t err_code = ipv6_medium_connectable_mode_enter(m_ipv6_medium.ipv6_medium_instance_id);
  APP_ERROR_CHECK(err_code);

  APPL_LOG("[APPL]: Physical layer in connectable mode.\r\n");
  LEDS_OFF(LED_TWO);
  LEDS_ON(LED_ONE);
}

/**@brief Function for updating the wall clock of the IoT Timer module.
*/
static void iot_timer_tick_callback(void * p_context)
{
  UNUSED_VARIABLE(p_context);

  uint32_t err_code = iot_timer_update();
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for the LEDs initialization.
 *
 * @details Initializes all LEDs used by this application.
 */
static void leds_init(void)
{
  // Configure application LED pins.
  LEDS_CONFIGURE(ALL_APP_LED);

  // Turn off all LED on initialization.
  LEDS_OFF(ALL_APP_LED);
}

#ifdef COMMISSIONING_ENABLED
/**@brief Timer callback used for controlling board LEDs to represent application state.
 *
 */
static void blink_timeout_handler(iot_timer_time_in_ms_t wall_clock_value)
{
  UNUSED_PARAMETER(wall_clock_value);
  static bool id_mode_previously_enabled;

  if ((id_mode_previously_enabled == false) && (m_identity_mode_active == true))
  {
    LEDS_OFF(LED_THREE | LED_FOUR);
  }

  if ((id_mode_previously_enabled == true) && (m_identity_mode_active == true))
  {
    LEDS_INVERT(LED_THREE | LED_FOUR);
  }

  if ((id_mode_previously_enabled == true) && (m_identity_mode_active == false))
  {
    LEDS_OFF(LED_THREE | LED_FOUR);
  }

  id_mode_previously_enabled = m_identity_mode_active;
}
#endif // COMMISSIONING_ENABLED

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{
  uint32_t err_code;

  // Initialize timer module.
  APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

  // Create a sys timer.
  err_code = app_timer_create(&m_iot_timer_tick_src_id,
      APP_TIMER_MODE_REPEATED,
      iot_timer_tick_callback);
  APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the IoT Timer. */
static void iot_timer_init(void)
{
  uint32_t err_code;

  static const iot_timer_client_t list_of_clients[] =
  {
    {system_timer_callback,   LWIP_SYS_TICK_MS},
#ifdef COMMISSIONING_ENABLED
    {blink_timeout_handler,   LED_BLINK_INTERVAL_MS},
    {commissioning_time_tick, SEC_TO_MILLISEC(COMMISSIONING_TICK_INTERVAL_SEC)}
#endif // COMMISSIONING_ENABLED
  };

  // The list of IoT Timer clients is declared as a constant.
  static const iot_timer_clients_list_t iot_timer_clients =
  {
    (sizeof(list_of_clients) / sizeof(iot_timer_client_t)),
    &(list_of_clients[0]),
  };

  // Passing the list of clients to the IoT Timer module.
  err_code = iot_timer_client_list_set(&iot_timer_clients);
  APP_ERROR_CHECK(err_code);

  // Starting the app timer instance that is the tick source for the IoT Timer.
  err_code = app_timer_start(m_iot_timer_tick_src_id, \
      APP_TIMER_TICKS(IOT_TIMER_RESOLUTION_IN_MS, APP_TIMER_PRESCALER), \
      NULL);
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing IP stack.
 *
 * @details Initialize the IP Stack and its driver.
 */
static void ip_stack_init(void)
{
  uint32_t err_code;

  err_code = ipv6_medium_eui64_get(m_ipv6_medium.ipv6_medium_instance_id, \
      &eui64_local_iid);
  APP_ERROR_CHECK(err_code);

  err_code = nrf_mem_init();
  APP_ERROR_CHECK(err_code);

  //Initialize LwIP stack.
  lwip_init();

  //Initialize LwIP stack driver.
  err_code = nrf_driver_init();
  APP_ERROR_CHECK(err_code);
}

#ifdef COMMISSIONING_ENABLED
/**@brief Function for handling button events.
 *
 * @param[in]   pin_no         The pin number of the button pressed.
 * @param[in]   button_action  The action performed on button.
 */
static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
  if ((button_action == APP_BUTTON_PUSH) && (pin_no == ERASE_BUTTON_PIN_NO))
  {
    APPL_LOG("[APPL]: Erasing all commissioning settings from persistent storage... \r\n");
    commissioning_settings_clear();
    return;
  }
  return;
}


/**@brief Function for the Button initialization.
 *
 * @details Initializes all Buttons used by this application.
 */
static void buttons_init(void)
{
  uint32_t err_code;

  static app_button_cfg_t buttons[] =
  {
#ifdef COMMISSIONING_ENABLED
    {ERASE_BUTTON_PIN_NO, false, BUTTON_PULL, button_event_handler}
#endif // COMMISSIONING_ENABLED
  };

#define BUTTON_DETECTION_DELAY APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)

  err_code = app_button_init(buttons, \
      sizeof(buttons) / sizeof(buttons[0]), \
      BUTTON_DETECTION_DELAY);
  APP_ERROR_CHECK(err_code);

  err_code = app_button_enable();
  APP_ERROR_CHECK(err_code);
}
#endif // COMMISSIONING_ENABLED

/**@brief Function for the Event Scheduler initialization.
*/
static void scheduler_init(void)
{
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void on_ipv6_medium_evt(ipv6_medium_evt_t * p_ipv6_medium_evt)
{
  switch (p_ipv6_medium_evt->ipv6_medium_evt_id)
  {
    case IPV6_MEDIUM_EVT_CONN_UP:
      {
        APPL_LOG("[APPL]: Physical layer: connected.\r\n");
        LEDS_OFF(LED_ONE);
        LEDS_ON(LED_TWO);
        break;
      }
    case IPV6_MEDIUM_EVT_CONN_DOWN:
      {
        APPL_LOG("[APPL]: Physical layer: disconnected.\r\n");
        connectable_mode_enter();
        break;
      }
    default:
      {
        break;
      }
  }
}


static void on_ipv6_medium_error(ipv6_medium_error_t * p_ipv6_medium_error)
{
  // Do something.
}

#ifdef COMMISSIONING_ENABLED
void commissioning_id_mode_cb(mode_control_cmd_t control_command)
{
  switch (control_command)
  {
    case CMD_IDENTITY_MODE_ENTER:
      {
        LEDS_OFF(LED_THREE | LED_FOUR);
        m_identity_mode_active = true;

        break;
      }
    case CMD_IDENTITY_MODE_EXIT:
      {
        m_identity_mode_active = false;
        LEDS_OFF((LED_THREE | LED_FOUR));

        break;
      }
    default:
      {

        break;
      }
  }
}


void commissioning_power_off_cb(bool power_off_on_failure)
{
  m_power_off_on_failure = power_off_on_failure;

  APPL_LOG("[APPL]: Commissioning: do power_off on failure: %s.\r\n", \
      m_power_off_on_failure ? "true" : "false");
}
#endif // COMMISSIONING_ENABLED

void bleconfig_init(void) {
  uint32_t err_code;

  //Initialize.
  app_trace_init();
  leds_init();
  timers_init();
  iot_timer_init();

#ifdef COMMISSIONING_ENABLED
  err_code = pstorage_init();
  APP_ERROR_CHECK(err_code);

  buttons_init();
#endif // COMMISSIONING_ENABLED

  static ipv6_medium_init_params_t ipv6_medium_init_params;
  memset(&ipv6_medium_init_params, 0x00, sizeof(ipv6_medium_init_params));
  ipv6_medium_init_params.ipv6_medium_evt_handler    = on_ipv6_medium_evt;
  ipv6_medium_init_params.ipv6_medium_error_handler  = on_ipv6_medium_error;
  ipv6_medium_init_params.use_scheduler              = true;
#ifdef COMMISSIONING_ENABLED
  ipv6_medium_init_params.commissioning_id_mode_cb   = commissioning_id_mode_cb;
  ipv6_medium_init_params.commissioning_power_off_cb = commissioning_power_off_cb;
#endif // COMMISSIONING_ENABLED

  err_code = ipv6_medium_init(&ipv6_medium_init_params, \
      IPV6_MEDIUM_ID_BLE,       \
      &m_ipv6_medium);
  APP_ERROR_CHECK(err_code);

  eui48_t ipv6_medium_eui48;
  err_code = ipv6_medium_eui48_get(m_ipv6_medium.ipv6_medium_instance_id, \
      &ipv6_medium_eui48);

  ipv6_medium_eui48.identifier[EUI_48_SIZE - 1] = 0x00;

  err_code = ipv6_medium_eui48_set(m_ipv6_medium.ipv6_medium_instance_id, \
      &ipv6_medium_eui48);
  APP_ERROR_CHECK(err_code);

  ip_stack_init();
  scheduler_init();

  //Start execution.
  connectable_mode_enter();

  APPL_LOG("BLE init done\n");
}

void bleconfig_poll(void) {
  //Execute event schedule.
  app_sched_execute();
  sys_check_timeouts();
}

