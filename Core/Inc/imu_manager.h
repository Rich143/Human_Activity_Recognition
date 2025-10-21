/*
 * imu_manager.h
 *
 *  Created on: Apr 12, 2025
 *      Author: richardmatthews
 */

#ifndef INC_IMU_MANAGER_H_
#define INC_IMU_MANAGER_H_

#include "High_Level/motion_sensor_interface.h"
#include "accel_data_type.h"

/**
 * IMU Sampling parameters. These must match the Model parameters
 */
//! Number of IMU samples per window
#define IMU_WINDOW_SIZE 24
//! IMU sampling frequency, this will correspond to 104 Hz for the ISM330DHCX
#define IMU_SAMPLING_FREQUENCY_HZ 100.0F
//! IMU Full Scale (in g), set to 4g which should avoid clipping for all HAR activities
#define IMU_FULL_SCALE_G 4


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
int imu_manager_read_window(accel_data_t* window);

#endif /* INC_IMU_MANAGER_H_ */
