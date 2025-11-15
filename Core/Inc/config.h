/*
 * config.h
 *
 *  Created on: Aug 3, 2025
 *      Author: richardmatthews
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <stdbool.h>


#define LOAD_IMU_DATA_FROM_FILE             0
#define LOAD_IMU_DATA_FROM_FILE_NAME        "Recorded_Data/TEST_DATA_WALKING_OCT_21_input.h"
#define LOAD_IMU_DATA_FROM_FILE_NUM_SAMPLES TEST_DATA_WALKING_OCT_21_INPUT_LEN
#define LOAD_IMU_DATA_FROM_FILE_X_DATA      TEST_DATA_WALKING_OCT_21_input_x
#define LOAD_IMU_DATA_FROM_FILE_Y_DATA      TEST_DATA_WALKING_OCT_21_input_y
#define LOAD_IMU_DATA_FROM_FILE_Z_DATA      TEST_DATA_WALKING_OCT_21_input_z

#define LOG_PRINT_CSV_INCLUDE_MODEL_OUTPUT 0

#define CONFIG_LOGGING_ENABLED_DEFAULT 1

#define CONFIG_CLI_ENABLE_KEY_PRESSES 10

#define CONFIG_LOG_USE_TEST_DATA 0

#define CONFIG_PREDICTIONS_ENBLED_DEFAULT 0

#define CONFIG_FLASH_DATA_LOG_SECTOR_START 0
#define CONFIG_FLASH_DATA_LOG_SIZE_SECTORS 1020

#define CONFIG_FLASH_ERROR_LOG_SECTOR_START \
    (CONFIG_FLASH_DATA_LOG_SECTOR_START + CONFIG_FLASH_DATA_LOG_SIZE_SECTORS)

#define CONFIG_FLASH_ERROR_LOG_SIZE_SECTORS \
    (NOR_FLASH_NUM_SECTORS - CONFIG_FLASH_DATA_LOG_SIZE_SECTORS)

void config_init();

void config_set_logging_enabled(bool enabled);

void config_set_print_csv_data(bool enabled);

bool config_get_logging_enabled();

bool config_get_print_csv_enabled();

bool config_get_cli_enabled();

void config_set_cli_enabled(bool enabled);

void config_set_predictions_enabled(bool enabled);

bool config_get_predictions_enabled();

#endif /* INC_CONFIG_H_ */
