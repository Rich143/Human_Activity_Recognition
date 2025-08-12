/*
 * config.h
 *
 *  Created on: Aug 3, 2025
 *      Author: richardmatthews
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_


#define LOAD_IMU_DATA_FROM_FILE             0
#define LOAD_IMU_DATA_FROM_FILE_NAME        "Recorded_Data/pamap2_cycling_1.h"
#define LOAD_IMU_DATA_FROM_FILE_NUM_SAMPLES PAMAP2_CYCLING_1_NUM_SAMPLES
#define LOAD_IMU_DATA_FROM_FILE_X_DATA      pamap2_cycling_1_x
#define LOAD_IMU_DATA_FROM_FILE_Y_DATA      pamap2_cycling_1_y
#define LOAD_IMU_DATA_FROM_FILE_Z_DATA      pamap2_cycling_1_z

#define LOGGING_ENABLED 1
#define LOG_PRINT_CSV 1
#define LOG_PRINT_CSV_INCLUDE_MODEL_OUTPUT 0

#define CONFIG_LOG_MAX_SAVED_ROWS 1000000

#define CONFIG_CLI_ENABLE_KEY_PRESSES 10

#endif /* INC_CONFIG_H_ */
