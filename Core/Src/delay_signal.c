/*
 *  delay_signal.c
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#include <math.h>
#include <stdio.h>

#include "delay_signal.h"

delay_status_t check_cb_error(cb_status_t status) {
    switch (status) {
        case cb_status_ok:
            return DELAY_STATUS_OK;
        case cb_status_error_delay_too_large:
            return DELAY_STATUS_ERROR_DELAY_TOO_LARGE;
        case cb_status_error_null:
            return DELAY_STATUS_ERROR_NULL;
        default:
            return DELAY_STATUS_ERROR_OTHER;
    }
}

delay_status_t delay_signal_init(delay_signal_t *delay_signal) {
    if (!delay_signal) {
        return DELAY_STATUS_ERROR_NULL;
    }

    delay_status_t status;

    cb_status_t rc = f32_cb_init(&delay_signal->x_cb,
                                 delay_signal->x_buf,
                                 DELAY_MAX_LEN);
    if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
        return status;
    }

    rc = f32_cb_init(&delay_signal->y_cb, delay_signal->y_buf,
                     DELAY_MAX_LEN);
    if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
        return status;
    }

    rc = f32_cb_init(&delay_signal->z_cb, delay_signal->z_buf,
                     DELAY_MAX_LEN);
    if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
        return status;
    }

    return DELAY_STATUS_OK;
}

delay_status_t delay_signal_push_signal(delay_signal_t *delay_signal, AccelData *signal) {
    if (!delay_signal || !signal) {
        return DELAY_STATUS_ERROR_NULL;
    }

    for (int i = 0; i < signal->num_samples; i++) {
      f32_cb_push(&delay_signal->x_cb, signal->x[i]);
      f32_cb_push(&delay_signal->y_cb, signal->y[i]);
      f32_cb_push(&delay_signal->z_cb, signal->z[i]);
    }

    return DELAY_STATUS_OK;
}

delay_status_t delay_signal_get_delay_range(delay_signal_t *delay_signal,
                                            AccelData *signal,
                                            uint32_t delay_amount,
                                            uint32_t len) {
    if (!delay_signal || !signal) {
        return DELAY_STATUS_ERROR_NULL;
    }

    delay_status_t status;

    cb_status_t rc = f32_cb_get_delayed(&delay_signal->x_cb, delay_amount, &signal->x[0]);
    if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
        return status;
    }

    rc = f32_cb_get_delayed(&delay_signal->y_cb, delay_amount, &signal->y[0]);
    if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
        return status;
    }

    rc = f32_cb_get_delayed(&delay_signal->z_cb, delay_amount, &signal->z[0]);
    if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
        return status;
    }

    return DELAY_STATUS_OK;
}
