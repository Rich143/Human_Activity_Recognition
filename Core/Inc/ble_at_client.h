/**
  ******************************************************************************
  * @file    ble_at_client.h
  * @brief   This file contains all the function prototypes for
  *          the ble_at_client.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLE_AT_CLIENT_H__
#define __BLE_AT_CLIENT_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern uint8_t global_svc_index;
extern uint8_t at_buffer[64];

void ble_at_client_init();
int ble_at_client_setup_and_run();
int8_t ble_at_client_notify();
int8_t ble_at_client_set_service(uint8_t svc_index);

bool ble_at_client_device_connected();

#ifdef __cplusplus
}
#endif

#endif /* __BLE_AT_CLIENT_H__ */
