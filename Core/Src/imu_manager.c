/*
 * imu_manager.c
 *
 *  Created on: Apr 12, 2025
 *      Author: richardmatthews
 */

#include "imu_manager.h"
#include "stm32u5xx_hal.h"

#define IMU_SAMPLING_PERIOD_MS (1000 / IMU_SAMPLING_FREQUENCY_HZ)

int imu_manager_init(void) {
  int32_t status = BSP_ERROR_NONE;

  status = BSP_MOTION_SENSOR_Init(0, MOTION_ACCELERO);
  if (status != BSP_ERROR_NONE) return status;

  status = BSP_MOTION_SENSOR_Enable(0, MOTION_ACCELERO);
  if (status != BSP_ERROR_NONE) return status;

  status = BSP_MOTION_SENSOR_SetOutputDataRate(0, MOTION_ACCELERO, IMU_SAMPLING_FREQUENCY_HZ);
  if (status != BSP_ERROR_NONE) return status;

  return status;
}

int imu_manager_read_window(IMU_Window *window) {
  int32_t status = BSP_ERROR_NONE;
  uint32_t sample_time = 0;

  for (int i = 0; i < IMU_WINDOW_SIZE; i++) {
    BSP_MOTION_SENSOR_Axes_t *axes = &(window->window[i]);

    sample_time = HAL_GetTick();

    status = BSP_MOTION_SENSOR_GetAxes(0, MOTION_ACCELERO, axes);
    if (status != BSP_ERROR_NONE) return status;

    HAL_Delay(IMU_SAMPLING_PERIOD_MS - (HAL_GetTick() - sample_time));
  }

  return status;
}
