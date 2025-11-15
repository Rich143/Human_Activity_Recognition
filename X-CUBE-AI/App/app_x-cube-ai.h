
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_AI_H
#define __APP_AI_H
#ifdef __cplusplus
extern "C" {
#endif
/**
  ******************************************************************************
  * @file    app_x-cube-ai.h
  * @author  X-CUBE-AI C code generator
  * @brief   AI entry function definitions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ai_platform.h"

void MX_X_CUBE_AI_Init(void);
/* USER CODE BEGIN includes */
typedef enum {
  AI_STATUS_OK = 0,
  AI_STATUS_BUFFERING,
  AI_STATUS_PREPROCESS_ERROR,
  AI_STATUS_INFERENCE_ERROR,
  AI_STATUS_POSTPROCESS_ERROR,
  AI_STATUS_NOT_INITIALIZED_ERROR,
  AI_STATUS_END_OF_RECORDED_DATA,
  AI_STATUS_ERROR,
} ai_status_t;

ai_status_t MX_X_CUBE_AI_Process(void);

/* USER CODE END includes */
#ifdef __cplusplus
}
#endif
#endif /*__STMicroelectronics_X-CUBE-AI_8_1_0_H */
