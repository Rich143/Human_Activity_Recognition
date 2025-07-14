#include "motion_sensor_interface.h"

#include "b-u585i-iot02a-bsp/b_u585i_iot02a_motion_sensors.h"

int32_t get_function_instance(motion_sensor_type_t type, uint32_t *function, uint32_t *instance) {
    switch (type) {
        case MOTION_SENSOR_GYRO:
            *function = MOTION_GYRO;
            *instance = 0;
            break;
        case MOTION_SENSOR_ACCEL:
            *function = MOTION_ACCELERO;
            *instance = 0;
            break;
        case MOTION_SENSOR_MAG:
            *function = MOTION_MAGNETO;
            *instance = 1;
            break;
        default:
            *function = 1000;
            *instance = 1000;
            return -1;
    }

    return 0;
}

int32_t motion_sensor_get_axes(motion_sensor_type_t type, motion_sensor_axes_t *axes) {
    BSP_MOTION_SENSOR_Axes_t bsp_Axes;

    uint32_t function, instance;

    int32_t ret = get_function_instance(type, &function, &instance);

    if (ret != 0) {
        return ret;
    }

    int32_t status = BSP_MOTION_SENSOR_GetAxes(instance, function, &bsp_Axes);

    axes->xval = bsp_Axes.xval;
    axes->yval = bsp_Axes.yval;
    axes->zval = bsp_Axes.zval;

    return status;
}

int32_t motion_sensor_init(motion_sensor_type_t type) {
    uint32_t function, instance;

    int32_t ret = get_function_instance(type, &function, &instance);

    if (ret != 0) {
        return ret;
    }

    return BSP_MOTION_SENSOR_Init(instance, function);
}

int32_t motion_sensor_enable(motion_sensor_type_t type) {
    uint32_t function, instance;

    int32_t ret = get_function_instance(type, &function, &instance);

    if (ret != 0) {
        return ret;
    }

    return BSP_MOTION_SENSOR_Enable(instance, function);
}

int32_t motion_sensor_disable(motion_sensor_type_t type) {
    uint32_t function, instance;

    int32_t ret = get_function_instance(type, &function, &instance);

    if (ret != 0) {
        return ret;
    }

    return BSP_MOTION_SENSOR_Disable(instance, function);
}

int32_t motions_sensor_set_output_data_rate(motion_sensor_type_t type, float Odr)
{
    uint32_t function, instance;

    int32_t ret = get_function_instance(type, &function, &instance);

    if (ret != 0) {
        return ret;
    }

    return BSP_MOTION_SENSOR_SetOutputDataRate(instance, function, Odr);
}
