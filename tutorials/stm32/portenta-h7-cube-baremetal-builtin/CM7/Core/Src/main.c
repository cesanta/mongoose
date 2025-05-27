/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef enum {RESOURCE_IN_MEMORY,RESOURCE_IN_EXTERNAL_STORAGE} resource_location_t;
typedef struct {resource_location_t l;unsigned long sz;union{struct {unsigned long o;const char *f;}fs;struct {const char *m;}mem;}val;} resource_hnd_t;
#include "resources/firmware/COMPONENT_4343W_FS/4343WA1_clm_blob.c"
#include "resources/firmware/COMPONENT_4343W_FS/4343WA1_bin.c"
#include "resources/nvram/wifi_nvram_image.h"

#include "mongoose.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define WIFI_SSID "YOUR_WIFI_NETWORK_NAME"  // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"      // SET THIS!


#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

RNG_HandleTypeDef hrng;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_RNG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r;
    HAL_RNG_GenerateRandomNumber(&hrng, &r);
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true; // TODO(): ensure successful RNG init, then return on false above
}

uint64_t mg_millis(void) {
  return HAL_GetTick();
}

int _write(int fd, unsigned char *buf, int len) {
  if (fd == 1 || fd == 2) {                     // stdout or stderr ?
    HAL_UART_Transmit(&huart1, buf, len, 999);  // Print to the UART
  }
  return len;
}


// - 58.4 Table 473
// - 58.5.8 AHB bw >= 3x SDMMC bus bw; Table 492; SD HS => AHB >= ~20MHz
// - SDIO 2.0 section 3.1
static inline void sdmmc_set_ds(void) { // NO FLOW CONTROL ??? REALLY ??? *******************************
  MG_SET_BITS(SDMMC1->CLKCR, (1 << 17) | 0x3ff, (1 << 14) | ((200000000 / (2 * 25000000)) & 0x3ff));  // 4-bit bus, 25MHz
}
static inline void sdmmc_set_hs(void) { // mbed enables flow control... AIROC does not
  MG_SET_BITS(SDMMC1->CLKCR, 0x3ff, (1 << 17) | (1 << 14) | ((200000000 / (2 * 50000000)) & 0x3ff));  // hw flow control, 4-bit bus, 50MHz
}
static inline void sdmmc_init(void) {
  RCC->AHB3RSTR |= MG_BIT(16);  // Reset SDIO block
  RCC->AHB3RSTR &= ~MG_BIT(16);
  RCC->D1CCIPR &= ~MG_BIT(16); // 9.7.18, use PLL1Q
  RCC->AHB3ENR |= MG_BIT(16);  // Enable SDIO clock
  // SDIO pins initialized by Cube; Portenta H7 doesn't pull-up the SDIO bus
  // 1-bit bus, rising edge, clock always on, no flow control, 400KHz
  SDMMC1->CLKCR = (200000000 / (2 * 400000)) & 0x3ff;
  SDMMC1->POWER |= 3 << 0; // SDMMC_POWER_PWRCTRL
  SDMMC1->DCTRL = MG_BIT(10); // Read Wait control by stopping SDMMCCLK
  // wait 74 clocks (~200us) before accessing the "card"
}

static inline bool sdmmc_transact(uint8_t cmd, uint32_t arg, uint32_t *resp) {
  unsigned int wresp = (resp != NULL) ? 1 : 0; // 58.10.4: 2-bit; cmds used
  uint32_t cmdr = MG_BIT(12) | (wresp << 8) | ((cmd & 0x3f) << 0); // CPSMEN
  SDMMC1->ARG = arg;
  MG_SET_BITS(SDMMC1->CMD, 0x00011f3f /* CMDSUSPEND CPSMEN WAITPEND WAITINT WAITRESP CMD */, cmdr);
  if (wresp == 0) {
    while ((SDMMC1->STA & SDMMC_STA_CMDSENT) == 0) (void)0;
    SDMMC1->ICR = 0x002000C5; // BUSYD0END CMDSENT CMDREND CTIMEOUT CCRCFAIL
    return true;
  }
  //while ((SDMMC1->STA & 0x00200045 /*BUSYD0END CMDREND CTIMEOUT CCRCFAIL*/) == 0 || SDMMC1->STA & SDMMC_STA_CPSMACT) spin(1);
  while (SDMMC1->STA & SDMMC_STA_CPSMACT) (void)0;
  bool res = (SDMMC1->STA & 0x00200005 /*BUSYD0END CTIMEOUT CCRCFAIL*/) == 0;
  SDMMC1->ICR = 0x002000C5; // BUSYD0END CMDSENT CMDREND CTIMEOUT CCRCFAIL
  *resp = SDMMC1->RESP1;
  if (!res || SDMMC1->RESPCMD != cmd) return false;
  return true;
}

static inline unsigned int _log2(uint16_t x) {
  unsigned int i = 0;
  while (x != 0) x = ((uint16_t)x) >> 1, ++i;
  return i - 1;
}

static inline bool sdmmc_transfer(bool write, uint32_t arg, uint16_t blksz, uint32_t *data, uint32_t length, uint32_t *resp) {
  uint32_t cmdr = MG_BIT(12) | (1 << 8) | (53 << 0); // CPSMEN WRESP CMD=53
  bool bmode = (blksz != 0);
  uint8_t dblksz = bmode ? (uint8_t)_log2(blksz) : 0;
  uint32_t dctrlr = (dblksz << 4) | (bmode ? (0 << 2) : (1 << 2)) | (write ? 0 : MG_BIT(1)); // DBLOCKSIZE=f(blksz) DTMODE=f(bmode) DTDIR=!write !DTEN
  SDMMC1->DCTRL = SDMMC_DCTRL_RWMOD;
  SDMMC1->IDMACTRL = SDMMC_IDMA_IDMAEN; // single buffer
  SDMMC1->IDMABASE0 = (uint32_t)data;
  SDMMC1->DTIMER = (uint32_t) 50000000; // 58.10.7, set for 1 sec @50MHz (2 secs @25MHz)
  SDMMC1->DLEN = length;  // 58.10.8 call with a multiple of blksz (we do)
  MG_SET_BITS(SDMMC1->DCTRL, 0xff /* DBLOCKSIZE DTMODE DTDIR DTEN */, dctrlr);
  SDMMC1->CMD = SDMMC_CMD_CMDTRANS;  // CPSM will perform a data transfer
  SDMMC1->ARG = arg;
  SDMMC1->CMD |= cmdr; // other flags cleared above
  while ((SDMMC1->STA & 0x0020003f) == 0 && SDMMC1->STA & SDMMC_STA_DPSMACT)
    (void) 0; // incorrect "card" usage will stall here
  bool res = (SDMMC1->STA & 0x0020003f /* BUSYD0END RXOVERR TXUNDERR DTIMEOUT CTIMEOUT DCRCFAIL CCRCFAIL */) == 0;
  SDMMC1->CMD = 0;
  SDMMC1->DLEN = 0;
  SDMMC1->IDMACTRL = 0;
  SDMMC1->ICR = (uint32_t) ~0; // clear all flags
  *resp = SDMMC1->RESP1;
  SDMMC1->DCTRL = SDMMC_DCTRL_RWMOD;
  if (!res || SDMMC1->RESPCMD != 53) return false;
  return true;
}


static void hwspecific_sdio_init(void) {
  HAL_GPIO_WritePin(PWR_GPIO_Port, PWR_Pin, 0);
  mg_delayms(100);
  sdmmc_init();
  mg_delayms(1);
  HAL_GPIO_WritePin(PWR_GPIO_Port, PWR_Pin, 1);
  mg_delayms(50);
}

void hwspecific_sdio_config(void *sdio, uint8_t cfg) {
  (void) sdio;
  if (cfg == 0) return sdmmc_set_ds();
  if (cfg == 1) return sdmmc_set_hs();
}
bool hwspecific_sdio_txn(void *sdio, uint8_t cmd, uint32_t arg, uint32_t *resp) {
  (void) sdio;
  return sdmmc_transact(cmd, arg, resp);
}
bool hwspecific_sdio_xfr(void *sdio, bool write, uint32_t arg, uint16_t blksz, uint32_t *data, uint32_t len, uint32_t *resp){
  (void) sdio;
  return sdmmc_transfer(write, arg, blksz, data, len, resp);
}

static const struct mg_tcpip_sdio sdio = {NULL, hwspecific_sdio_config, hwspecific_sdio_txn, hwspecific_sdio_xfr};

static const struct mg_tcpip_driver_cyw_firmware fw = {
  (const uint8_t *)wifi_firmware_image_data, (size_t)sizeof(wifi_firmware_image_data),
  (const uint8_t *)wifi_nvram_image, (size_t)sizeof(wifi_nvram_image),
  (const uint8_t *)wifi_firmware_clm_blob_image_data, (size_t)sizeof(wifi_firmware_clm_blob_image_data)};

// mif user states
enum {AP, SCANNING, STOPPING_AP, CONNECTING, READY};
static unsigned int state;
static uint32_t s_ip, s_mask;


static void mif_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  // TODO(): should we include this inside ifp ? add an fn_data ?
  if (ev == MG_TCPIP_EV_ST_CHG) {
    MG_INFO(("State change: %u", *(uint8_t *) ev_data));
  }
  switch(state) {
    case AP: // we are in AP mode, wait for a user connection to trigger a scan or a connection to a network
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_UP) {
        MG_INFO(("Access Point started"));
        s_ip = ifp->ip, ifp->ip = MG_IPV4(192, 168, 169, 1);
        s_mask = ifp->mask, ifp->mask = MG_IPV4(255, 255, 255, 0);
        ifp->enable_dhcp_client = false;
        ifp->enable_dhcp_server = true;
      } else if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("Access Point READY !"));

        // simulate user request to scan for networks
        bool res = mg_wifi_scan();
        MG_INFO(("Starting scan: %s", res ? "OK":"FAIL"));
        if (res) state = SCANNING;
      }
      break;
    case SCANNING:
      if (ev == MG_TCPIP_EV_WIFI_SCAN_RESULT) {
        struct mg_wifi_scan_bss_data *bss = (struct mg_wifi_scan_bss_data *) ev_data;
        MG_INFO(("BSS: %.*s (%u) (%M) %d dBm %u", bss->SSID.len, bss->SSID.buf, bss->channel, mg_print_mac, bss->BSSID, (int) bss->RSSI, bss->security));
      } else if (ev == MG_TCPIP_EV_WIFI_SCAN_END) {
        // struct mg_tcpip_driver_cyw_data *d = (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
        MG_INFO(("Wi-Fi scan finished"));

        // simulate user selection of a network (1/2: stop AP)
        bool res = mg_wifi_ap_stop();
        MG_INFO(("Manually stopping AP: %s", res ? "OK":"FAIL"));
        if (res) state = STOPPING_AP;
        // else we have a hw/fw problem
      }
      break;
    case STOPPING_AP:
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_tcpip_driver_cyw_data *d = (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
        d->apmode = false;

        // simulate user selection of a network (2/2: actual connect)
        bool res = mg_wifi_connect(d->ssid, d->pass);
        MG_INFO(("Manually connecting: %s", res ? "OK":"FAIL"));
        if (res) {
          state = CONNECTING;
          ifp->ip = s_ip;
          ifp->mask = s_mask;
          if (ifp->ip == 0) ifp->enable_dhcp_client = true;
          ifp->enable_dhcp_server = false;
        } // else manually start AP as below
      }
      break;
    case CONNECTING:
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("READY!"));
        state = READY;

        // simulate user code disconnection and go back to AP mode (1/2: disconnect)
        bool res = mg_wifi_disconnect();
        MG_INFO(("Manually disconnecting: %s", res ? "OK":"FAIL"));
      } else if (ev == MG_TCPIP_EV_WIFI_CONNECT_ERR) {
        MG_ERROR(("Wi-Fi connect failed"));
        // manually start AP as below
      }
      break;
    case READY:
      // go back to AP mode after a disconnection (simulation 2/2), you could retry
      if (ev == MG_TCPIP_EV_ST_CHG && *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_tcpip_driver_cyw_data *d = (struct mg_tcpip_driver_cyw_data *) ifp->driver_data;
        bool res = mg_wifi_ap_start(d->apssid, d->appass, d->apchannel);
        MG_INFO(("Disconnected"));
        MG_INFO(("Manually starting AP: %s", res ? "OK":"FAIL"));
        if (res) {
          state = AP;
          d->apmode = true;
        }
      }
      break;
  }
}


static struct mg_tcpip_driver_cyw_data d = {
  (void *)&sdio, (struct mg_tcpip_driver_cyw_firmware *)&fw, WIFI_SSID, WIFI_PASS, "mongoose", "mongoose", 0, 0, 10, true, false};


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
  hwspecific_sdio_init();

  state = d.apmode ? AP : CONNECTING;

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  // Either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  struct mg_tcpip_if mif = {
      .ip = 0,
      .driver = (struct mg_tcpip_driver *)&mg_tcpip_driver_cyw,
      .driver_data = (struct mg_tcpip_driver_cyw_data*)&d,
      .fn = mif_fn,
//      .recv_queue.size = 8192
  };

  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    mg_mgr_poll(&mgr, 0);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOK, LED_R_Pin|LED_G_Pin|LED_B_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PWR_GPIO_Port, PWR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_R_Pin LED_G_Pin LED_B_Pin */
  GPIO_InitStruct.Pin = LED_R_Pin|LED_G_Pin|LED_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC11 PC12 PC8
                           PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PWR_Pin */
  GPIO_InitStruct.Pin = PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WAKE_Pin */
  GPIO_InitStruct.Pin = WAKE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(WAKE_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
