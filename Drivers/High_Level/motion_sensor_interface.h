#pragma once

#include <stdint.h>

#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MOTION_SENSOR_GYRO = 1U,
    MOTION_SENSOR_ACCEL = 2U,
    MOTION_SENSOR_MAG = 4U,
} motion_sensor_type_t;

typedef struct
{
  int32_t xval;
  int32_t yval;
  int32_t zval;
} motion_sensor_axes_t;

int32_t motion_sensor_init(motion_sensor_type_t type);

int32_t motion_sensor_enable(motion_sensor_type_t type);
int32_t motion_sensor_disable(motion_sensor_type_t type);

int32_t motion_sensor_set_output_data_rate(motion_sensor_type_t type, float Odr);

int32_t motion_sensor_get_full_scale(motion_sensor_type_t type, int32_t
                                     *full_scale);

int32_t motion_sensor_set_full_scale(motion_sensor_type_t type,
                                     int32_t full_scale);

int32_t motion_sensor_get_axes(motion_sensor_type_t type, motion_sensor_axes_t *axes);
#ifdef __cplusplus
}
#endif
