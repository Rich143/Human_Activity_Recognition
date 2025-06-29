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

    delay_status_t status;

    for (int i = 0; i < signal->num_samples; i++) {
        cb_status_t rc = f32_cb_push(&delay_signal->x_cb, signal->x[i]);
        if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
            return status;
        }

        rc = f32_cb_push(&delay_signal->y_cb, signal->y[i]);
        if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
            return status;
        }

        rc = f32_cb_push(&delay_signal->z_cb, signal->z[i]);
        if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
            return status;
        }
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

    uint32_t delayed_values = f32_cb_get_count(&delay_signal->x_cb);

    if (delay_amount >= delayed_values) {
        return DELAY_STATUS_ERROR_DELAY_TOO_LARGE;
    }

    int min_delay_amount = delay_amount - len + 1;

    for (int32_t delay = delay_amount; delay >= min_delay_amount; delay--) {
        uint32_t idx = delay_amount - delay;

        cb_status_t rc = f32_cb_get_delayed(&delay_signal->x_cb, delay, &signal->x[idx]);
        if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
            return status;
        }

        rc = f32_cb_get_delayed(&delay_signal->y_cb, delay, &signal->y[idx]);
        if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
            return status;
        }

        rc = f32_cb_get_delayed(&delay_signal->z_cb, delay, &signal->z[idx]);
        if ((status = check_cb_error(rc)) != DELAY_STATUS_OK) {
            return status;
        }
    }

    return DELAY_STATUS_OK;
}
