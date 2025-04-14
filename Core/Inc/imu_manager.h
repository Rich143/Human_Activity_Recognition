/*
 * imu_manager.h
 *
 *  Created on: Apr 12, 2025
 *      Author: richardmatthews
 */

#ifndef INC_IMU_MANAGER_H_
#define INC_IMU_MANAGER_H_

#include "b-u585i-iot02a-bsp/b_u585i_iot02a_motion_sensors.h"

/**
 * IMU Sampling parameters. These must match the Model parameters
 */
//! Number of IMU samples per window
#define IMU_WINDOW_SIZE 24
//! IMU sampling frequency, this will correspond to 26 Hz for the ISM330DHCX
#define IMU_SAMPLING_FREQUENCY_HZ 20.0f

typedef struct {
BSP_MOTION_SENSOR_Axes_t window[IMU_WINDOW_SIZE];
} IMU_Window;

/**
 * @brief Initializes the IMU manager
 *
 * Initializes the IMU manager by initializing the IMU sensors
 *
 * @return BSP status.
 */
int imu_manager_init(void);

/**
 * @brief Reads in the IMU window from the sensors
 *
 * @param window Pointer to IMU window to write data into
 *
 * @return BSP status
 */
int imu_manager_read_window(IMU_Window* window);

#endif /* INC_IMU_MANAGER_H_ */
