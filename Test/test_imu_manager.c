#include "unity.h"

#include <stdlib.h>

#include "motion_sensor_interface.h"
#include "accel_data_type.h"
#include "imu_manager.h"

#include "mock_motion_sensor_interface.h"
#include "mock_stm32u5xx_hal.h"

accel_data_t window;

void init_imu_manager_test(void) {
    motion_sensor_init_ExpectAndReturn(MOTION_SENSOR_ACCEL, BSP_ERROR_NONE);
    motion_sensor_enable_ExpectAndReturn(MOTION_SENSOR_ACCEL, BSP_ERROR_NONE);
    motions_sensor_set_output_data_rate_ExpectAndReturn(MOTION_SENSOR_ACCEL, IMU_SAMPLING_FREQUENCY_HZ, BSP_ERROR_NONE);
    imu_manager_init();
}

void setUp(void) {
    window.num_samples = IMU_WINDOW_SIZE;

    window.x = (float32_t *)malloc(sizeof(float32_t) * IMU_WINDOW_SIZE);
    window.y = (float32_t *)malloc(sizeof(float32_t) * IMU_WINDOW_SIZE);
    window.z = (float32_t *)malloc(sizeof(float32_t) * IMU_WINDOW_SIZE);

    HAL_GetTick_IgnoreAndReturn(0);

    init_imu_manager_test();
}

void tearDown(void) {
    free(window.x);
    free(window.y);
    free(window.z);
}

float helper_mg_to_ms2(int32_t mg) {
    return mg * (9.81f / 1000.0f);
}

void test_read_window(void) {
    motion_sensor_axes_t axes[IMU_WINDOW_SIZE];
    float x_data[IMU_WINDOW_SIZE];
    float y_data[IMU_WINDOW_SIZE];
    float z_data[IMU_WINDOW_SIZE];

    for (int i = 0; i < IMU_WINDOW_SIZE; i++) {
        axes[i].xval = i*100;
        axes[i].yval = i*100 + IMU_WINDOW_SIZE;
        axes[i].zval = i*100 + IMU_WINDOW_SIZE * 2;

        x_data[i] = helper_mg_to_ms2(axes[i].xval);
        y_data[i] = helper_mg_to_ms2(axes[i].yval);
        z_data[i] = helper_mg_to_ms2(axes[i].zval);
    }

    // Reverse order expectations since they are queued
    for (int i = 0; i < IMU_WINDOW_SIZE; i++) {
        motion_sensor_get_axes_ExpectAndReturn(MOTION_SENSOR_ACCEL, NULL, 0);
        motion_sensor_get_axes_IgnoreArg_axes();
        motion_sensor_get_axes_ReturnMemThruPtr_axes(&axes[i], sizeof(motion_sensor_axes_t));

        HAL_Delay_Expect(1000.0f / IMU_SAMPLING_FREQUENCY_HZ);
    }

    int32_t status = imu_manager_read_window(&window);
    TEST_ASSERT_EQUAL_INT(BSP_ERROR_NONE, status);

    for (int i = 0; i < IMU_WINDOW_SIZE; i++) {
        char buf_x[200];
        sprintf(buf_x, "x_data[%d]: %f, window.x[%d]: %f", i, x_data[i], i, window.x[i]);

        char buf_y[200];
        sprintf(buf_y, "y_data[%d]: %f, window.y[%d]: %f", i, y_data[i], i, window.y[i]);

        char buf_z[200];
        sprintf(buf_z, "z_data[%d]: %f, window.z[%d]: %f", i, z_data[i], i, window.z[i]);

        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(1e-4f, x_data[i], window.x[i], buf_x);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(1e-4f, y_data[i], window.y[i], buf_y);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(1e-4f, z_data[i], window.z[i], buf_z);
    }
}
