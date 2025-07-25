
/**
  ******************************************************************************
  * @file    app_x-cube-ai.c
  * @author  X-CUBE-AI C code generator
  * @brief   AI program body
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

 /*
  * Description
  *   v1.0 - Minimum template to show how to use the Embedded Client API
  *          model. Only one input and one output is supported. All
  *          memory resources are allocated statically (AI_NETWORK_XX, defines
  *          are used).
  *          Re-target of the printf function is out-of-scope.
  *   v2.0 - add multiple IO and/or multiple heap support
  *
  *   For more information, see the embeded documentation:
  *
  *       [1] %X_CUBE_AI_DIR%/Documentation/index.html
  *
  *   X_CUBE_AI_DIR indicates the location where the X-CUBE-AI pack is installed
  *   typical : C:\Users\<user_name>\STM32Cube\Repository\STMicroelectronics\X-CUBE-AI\7.1.0
  */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#if defined ( __ICCARM__ )
#elif defined ( __CC_ARM ) || ( __GNUC__ )
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"
#include "network.h"
#include "network_data.h"

/* USER CODE BEGIN includes */
#include "imu_manager.h"
#include "preprocess.h"
#include "accel_data_type.h"
#include "ai_input_data_type.h"
#include "ai_output_data_type.h"
#include <assert.h>
_Static_assert(IMU_WINDOW_SIZE == AI_NETWORK_IN_1_HEIGHT, "IMU window size must match network input height");
/* USER CODE END includes */

/* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_in_1[AI_NETWORK_IN_1_SIZE_BYTES];
ai_i8* data_ins[AI_NETWORK_IN_NUM] = {
data_in_1
};
#else
ai_i8* data_ins[AI_NETWORK_IN_NUM] = {
NULL
};
#endif

#if !defined(AI_NETWORK_OUTPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_out_1[AI_NETWORK_OUT_1_SIZE_BYTES];
ai_i8* data_outs[AI_NETWORK_OUT_NUM] = {
data_out_1
};
#else
ai_i8* data_outs[AI_NETWORK_OUT_NUM] = {
NULL
};
#endif

/* Activations buffers -------------------------------------------------------*/

AI_ALIGNED(32)
static uint8_t pool0[AI_NETWORK_DATA_ACTIVATION_1_SIZE];

ai_handle data_activations0[] = {pool0};

/* AI objects ----------------------------------------------------------------*/

static ai_handle network = AI_HANDLE_NULL;

static ai_buffer* ai_input;
static ai_buffer* ai_output;

static void ai_log_err(const ai_error err, const char *fct)
{
  /* USER CODE BEGIN log */
  if (fct)
    printf("TEMPLATE - Error (%s) - type=0x%02x code=0x%02x\r\n", fct,
        err.type, err.code);
  else
    printf("TEMPLATE - Error - type=0x%02x code=0x%02x\r\n", err.type, err.code);

  do {} while (1);
  /* USER CODE END log */
}

static int ai_boostrap(ai_handle *act_addr)
{
  ai_error err;

  /* Create and initialize an instance of the model */
  err = ai_network_create_and_init(&network, act_addr, NULL);
  if (err.type != AI_ERROR_NONE) {
    ai_log_err(err, "ai_network_create_and_init");
    return -1;
  }

  ai_input = ai_network_inputs_get(network, NULL);
  ai_output = ai_network_outputs_get(network, NULL);

#if defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-inputs" option is used, memory buffer can be
   *  used from the activations buffer. This is not mandatory.
   */
  for (int idx=0; idx < AI_NETWORK_IN_NUM; idx++) {
	data_ins[idx] = ai_input[idx].data;
  }
#else
  for (int idx=0; idx < AI_NETWORK_IN_NUM; idx++) {
	  ai_input[idx].data = data_ins[idx];
  }
#endif

#if defined(AI_NETWORK_OUTPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-outputs" option is used, memory buffer can be
   *  used from the activations buffer. This is no mandatory.
   */
  for (int idx=0; idx < AI_NETWORK_OUT_NUM; idx++) {
	data_outs[idx] = ai_output[idx].data;
  }
#else
  for (int idx=0; idx < AI_NETWORK_OUT_NUM; idx++) {
	ai_output[idx].data = data_outs[idx];
  }
#endif

  return 0;
}

static int ai_run(void)
{
  ai_i32 batch;

  batch = ai_network_run(network, ai_input, ai_output);
  if (batch != 1) {
    ai_log_err(ai_network_get_error(network),
        "ai_network_run");
    return -1;
  }

  return 0;
}

/* USER CODE BEGIN 2 */
float x_in[IMU_WINDOW_SIZE];
float y_in[IMU_WINDOW_SIZE];
float z_in[IMU_WINDOW_SIZE];

float x_scratch[IMU_WINDOW_SIZE];
float y_scratch[IMU_WINDOW_SIZE];
float z_scratch[IMU_WINDOW_SIZE];

static accel_data_t input = {
  .num_samples = IMU_WINDOW_SIZE,
  .x = x_in,
  .y = y_in,
  .z = z_in
};

static accel_data_t scratch = {
  .num_samples = IMU_WINDOW_SIZE,
  .x = x_scratch,
  .y = y_scratch,
  .z = z_scratch
};

preprocess_t preprocess;

ai_input_data_t network_input_buffer;

void print_imu_csv(const accel_data_t *unproc,
                   const accel_data_t *lpf,
                   const ai_input_data_t *proc,
                   int window_size) {
    for (int i = 0; i < window_size; i++) {
        printf("%.6f,%.6f,%.6f,", unproc->x[i], unproc->y[i], unproc->z[i]);
        printf("%.6f,%.6f,%.6f,", lpf->x[i], lpf->y[i], lpf->z[i]);
        printf("%.6f,%.6f,%.6f\n",
               AI_INPUT_GET_X(proc->data_array, i),
               AI_INPUT_GET_Y(proc->data_array, i),
               AI_INPUT_GET_Z(proc->data_array, i));
    }
}

ai_input_data_t *get_network_input_buffer(ai_i8* data[]) {
    /* Format for access is data[H][W][C==1] */
    float (*data_array)[AI_NETWORK_IN_1_WIDTH][AI_NETWORK_IN_1_CHANNEL] =
        (float (*)[AI_NETWORK_IN_1_WIDTH][AI_NETWORK_IN_1_CHANNEL])data[0];

    network_input_buffer.data_array = data_array;

    return &network_input_buffer;
}

preprocess_status_t acquire_and_process_data(ai_i8* data[])
{
    /*printf("Acquiring window\n");*/

    ai_input_data_t *output = get_network_input_buffer(data);

    int32_t status = imu_manager_read_window(&input);
    if (status != BSP_ERROR_NONE) {
        printf("Failed to read window from IMU\n");
        return PREPROCESS_STATUS_ERROR_OTHER;
    } else {
        /*printf("IMU Window:\n");*/
        /*for (int i = 0; i < IMU_WINDOW_SIZE; i++) {*/
            /*printf("X: %f Y: %f Z: %f\n", input.x[i], input.y[i], input.z[i]);*/
        /*}*/
    }

    preprocess_status_t preprocess_status =
        gravity_suppress_rotate(&preprocess, &input,
                                &scratch, output);

    if (preprocess_status != PREPROCESS_STATUS_OK) {
        if (preprocess_status == PREPROCESS_STATUS_ERROR_BUFFERING) {
            printf("Buffering IMU data for preprocessing\n");
            return preprocess_status;
        } else {
            printf("Failed to preprocess\n");
            return preprocess_status;
        }
    }

    /*printf("Processed Window:\n");*/
    /*for (int i = 0; i < IMU_WINDOW_SIZE; i++) {*/
        /*printf("X: %f Y: %f Z: %f\n", output->x[i], output->y[i], output->z[i]);*/
    /*}*/

    /*print_imu_csv(&input, &scratch, output, IMU_WINDOW_SIZE);*/

    return PREPROCESS_STATUS_OK;
}

int get_max_value(float *data_array, int len) {
    int max_idx = 0;
    for (int i = 0; i < len; i++) {
        if (data_array[i] > data_array[max_idx]) {
            max_idx = i;
        }
    }

    return max_idx;
}

int post_process(ai_i8* data[])
{
    float *data_array = (float *)data[0];

    printf("Model output: %lf, %lf, %lf, %lf\n", data_array[0], data_array[1],
         data_array[2], data_array[3]);

    int max_idx = get_max_value(data_array, 4);
    printf("Max index: %d, label: ", max_idx);
    switch (max_idx) {
        case AI_OUTPUT_DATA_TYPE_STATIONARY:
            printf("Stationary\n");
            break;
        case AI_OUTPUT_DATA_TYPE_WALKING:
            printf("Walking\n");
            break;
        case AI_OUTPUT_DATA_TYPE_RUNNING:
            printf("Running\n");
            break;
        case AI_OUTPUT_DATA_TYPE_CYCLING:
            printf("Cycling\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }

    return 0;
}
/* USER CODE END 2 */

/* Entry points --------------------------------------------------------------*/

void MX_X_CUBE_AI_Init(void)
{
    /* USER CODE BEGIN 5 */
  printf("\r\nTEMPLATE - initialization\r\n");

  preprocess_status_t status = preprocess_init(&preprocess);
  if (status != PREPROCESS_STATUS_OK) {
      printf("Failed to initialize preprocess\n");
      while(1);
  }

  ai_boostrap(data_activations0);
    /* USER CODE END 5 */
}

void MX_X_CUBE_AI_Process(void)
{
    /* USER CODE BEGIN 6 */
  int res = -1;

  printf("TEMPLATE - run - main loop\r\n");

  if (network) {

      do {
          /* 1 - acquire and pre-process input data */
          preprocess_status_t status = acquire_and_process_data(data_ins);
          /* 2 - process the data - call inference engine */
          if (status == PREPROCESS_STATUS_OK) {
              res = ai_run();
              /* 3- post-process the predictions */
              if (res == 0) {
                  res = post_process(data_outs);
              }
          } else if (status != PREPROCESS_STATUS_ERROR_BUFFERING) {
              res = status;
          }
      } while (res==0);
  }

  if (res) {
    ai_error err = {AI_ERROR_INVALID_STATE, AI_ERROR_CODE_NETWORK};
    ai_log_err(err, "Process has FAILED");
  }
    /* USER CODE END 6 */
}
#ifdef __cplusplus
}
#endif
