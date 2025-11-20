/**
  ******************************************************************************
  * @file    blt_at_client.c
  * @brief   BLE AT Client application
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ble_at_client.h"
#include "b-u585i-iot02a-bsp/b_u585i_iot02a.h"
#include "config.h"
#include "wb-at-client/stm32wb_at.h"
#include "wb-at-client/stm32wb_at_ble.h"
#include "wb-at-client/stm32wb_at_client.h"
#include "High_Level/uart.h"
#include "flash_error_log.h"
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t at_buffer[64];
uint8_t global_svc_index = 0;
volatile bool global_device_connected = false; // A Device is connected
volatile bool global_device_has_connected = false; // A device has connected at least once

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int8_t ble_at_client_notify() {
  /* Send a BLE_NOTIF_VAL command to notify BLE application */
  stm32wb_at_BLE_NOTIF_VAL_t param_BLE_NOTIF_VAL;
  if(global_svc_index == 1)
  {
    param_BLE_NOTIF_VAL.svc_index = 1;
    param_BLE_NOTIF_VAL.char_index = 2;
    param_BLE_NOTIF_VAL.val_tab[0] = 1;
    param_BLE_NOTIF_VAL.val_tab_len = 1;
  }
  else
  {
    param_BLE_NOTIF_VAL.svc_index = 2;
    param_BLE_NOTIF_VAL.char_index = 1;
    param_BLE_NOTIF_VAL.val_tab[0] = 80 + (HAL_GetTick() % 60);
    param_BLE_NOTIF_VAL.val_tab_len = 1;
  }

  return stm32wb_at_client_Set(BLE_NOTIF_VAL, &param_BLE_NOTIF_VAL);
}

int8_t ble_at_client_set_service(uint8_t svc_index) {
  /* Send a BLE_SVC command to select the service running */

  if (svc_index != 1 && svc_index != 2) {
    return -1;
  }

  stm32wb_at_BLE_SVC_t param_BLE_SVC;
  param_BLE_SVC.index = svc_index;
  int8_t status = stm32wb_at_client_Set(BLE_SVC, &param_BLE_SVC);

  global_svc_index = svc_index;

  return status;
}

uint8_t stm32wb_at_BLE_SVC_cb(stm32wb_at_BLE_SVC_t *param)
{
  global_svc_index = param->index;

  return 0;
}

uint8_t stm32wb_at_BLE_ADV_cb(stm32wb_at_BLE_ADV_t *param)
{
  return 0;
}

uint8_t stm32wb_at_BLE_EVT_WRITE_cb(stm32wb_at_BLE_EVT_WRITE_t *param)
{
  if( (param->svc_index == 1) && (param->char_index == 2) )
  {
    if(param->val_tab[0] == 0)
    {
      printf("Logging disabled\n");
      config_set_logging_enabled(false);
      BSP_LED_Off(LED_GREEN);
    }
    else
    {
      printf("Logging enabled\n");
      config_set_logging_enabled(true);
      BSP_LED_On(LED_GREEN);
    }
  }

  return 0;
}

uint8_t stm32wb_at_BLE_EVT_CONN_cb(stm32wb_at_BLE_EVT_CONN_t *param)
{
  if(param->status != 0)
  {
    printf("A remote device is  now connected\n");
    global_device_connected = true;
    global_device_has_connected = true;
  }
  else
  {
    printf("The remote device is  now disconnected\n");
    global_device_connected = false;
  }
  return 0;
}

/**
 * @brief  Triggers the reception of a byte
 * @param  pBuff pointer to the first byte to transmit
 * @param  Size byte count to transfer
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t stm32wb_at_ll_Transmit(uint8_t *pBuff, uint16_t Size)
{
  bool success;

  success = uart_ble_send_data(pBuff, Size);
  /*printf("  TX: %s",pBuff);*/

  if (success)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

/**
 * @brief  Triggers the reception of a byte
 * @param  new_frame contains the length of a new frame received in rx buffer, else 0
 * @retval None
 */
void stm32wb_at_ll_Async_receive(uint8_t new_frame)
{
  /*if(new_frame != 0)*/
  /*{*/
    /*printf("  RX: %s\r\n",at_buffer);*/
  /*}*/

  uart_ble_rx_start();

  return;
}

void uart_ble_at_client_rx_cb(uint8_t data) {
  stm32wb_at_Received(data);
}

void ble_at_client_init() {
  global_device_connected = false;
  global_device_has_connected = false;
  global_svc_index = 0;

  uart_ble_rx_register_callback(uart_ble_at_client_rx_cb);
}


int ble_at_client_setup_and_run() {
  ble_at_client_init();

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  BSP_LED_Off(LED_GREEN);
  printf("--------------------------------------------\n");
  printf("Run ST BLE sensor application on your smartphone and connect to you device.\n");
  printf("--------------------------------------------\n");

  HAL_Delay(2000);
  uint8_t status = stm32wb_at_Init(&at_buffer[0], sizeof(at_buffer));
  status |= stm32wb_at_client_Init();

  if(status != 0)
  {
    printf("Failed to initialize AT module\n");
    return -1;
  }

  /* Test the UART communication link with BLE module */
  status = stm32wb_at_client_Query(BLE_TEST);
  if (status != 0) {
    printf("Failed to test the UART communication link with BLE module\n");
    return -2;
  }

  HAL_Delay(100);

  stm32wb_at_BLE_RST_t param_RST = { .reset = 1 };
  status = stm32wb_at_client_Set(BLE_RST, &param_RST);
  if (status != 0)
  {
    printf("Failed to reset BLE module\n");
    return -3;
  }

  HAL_Delay(100);

  stm32wb_at_BLE_RF_POWER_t param_PWR = { .tx_power = 3, .tx_power_code = 3 };
  status = stm32wb_at_client_Set(BLE_RF_POWER, &param_PWR);
  if (status != 0)
  {
    printf("Failed to set TX Power\n");
    return -4;
  }

  HAL_Delay(100);

  /* Send a BLE AT command to start the BLE P2P server application */
  printf("Starting the BLE P2P server application\n");
  stm32wb_at_BLE_SVC_t param_BLE_SVC;
  global_svc_index = 1;
  param_BLE_SVC.index = global_svc_index;
  status = stm32wb_at_client_Set(BLE_SVC, &param_BLE_SVC);
  if (status != 0)
  {
    printf("Failed to start the BLE P2P server application\n");
    return -5;
  }

  HAL_Delay(100);

  /* Query the BLE service to check if it is running */
  status = stm32wb_at_client_Query(BLE_SVC);
  if (status != 0)
  {
    printf("Failed to query BLE service\n");
    return -6;
  }

  HAL_Delay(100);

  uint32_t start = HAL_GetTick();
  while (HAL_GetTick() - start < 5000) {
    if (global_svc_index == 1) {
      break;
    }
  }

  if (global_svc_index == 1) {
    printf("BLE P2P server application is running\n");
  } else {
    printf("Failed to start the BLE P2P server application\n");
    return -7;
  }

  return 0;
}

bool ble_at_client_device_connected() {
  return global_device_connected;
}

/**
 * @brief Check if the device has disconnected after having connected
 *
 * @return true if the device has disconnected
 */
bool ble_at_client_check_disconnect() {
  if (global_device_has_connected) {
    if (!global_device_connected) {
      return true;
    }
  }

  return false;
}

/**
 * @brief Clear the disconnect flag, so disconnect check will only return true
 * once a client connects again
 */
void ble_at_client_clear_disconnect() {
  global_device_has_connected = false;
}
