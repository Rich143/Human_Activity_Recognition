/*
 * imu_manager.c
 *
 *  Created on: Apr 12, 2025
 *      Author: richardmatthews
 */

#include "imu_manager.h"
#include "accel_data_type.h"
#include "stm32u5xx_hal.h"

#define IMU_SAMPLING_PERIOD_MS (1000 / IMU_SAMPLING_FREQUENCY_HZ)

int imu_manager_init(void) {
  int32_t status = BSP_ERROR_NONE;

  status = motion_sensor_init(MOTION_SENSOR_ACCEL);
  if (status != BSP_ERROR_NONE) return status;

  status = motion_sensor_enable(MOTION_SENSOR_ACCEL);
  if (status != BSP_ERROR_NONE) return status;

  status = motions_sensor_set_output_data_rate(MOTION_SENSOR_ACCEL,
                                               IMU_SAMPLING_FREQUENCY_HZ);
  if (status != BSP_ERROR_NONE) return status;

  return status;
}

/* Need to convert from IMU data to accel_data_t units */
float mg_to_ms2(int32_t mg) {
    return mg * (9.81f / 1000.0f);
}

int imu_manager_read_window(accel_data_t *window) {
  int32_t status = BSP_ERROR_NONE;
  uint32_t sample_time = 0;

  if (window->num_samples != IMU_WINDOW_SIZE) return -1;

  for (int i = 0; i < IMU_WINDOW_SIZE; i++) {
    motion_sensor_axes_t axes;

    sample_time = HAL_GetTick();

    status = motion_sensor_get_axes(MOTION_SENSOR_ACCEL, &axes);
    if (status != BSP_ERROR_NONE) return status;

    window->x[i] = mg_to_ms2(axes.xval);
    window->y[i] = mg_to_ms2(axes.yval);
    window->z[i] = mg_to_ms2(axes.zval);

    HAL_Delay(IMU_SAMPLING_PERIOD_MS - (HAL_GetTick() - sample_time));
  }

  return status;
}
