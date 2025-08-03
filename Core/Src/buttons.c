/*
 * buttons.c
 *
 *  Created on: Aug 1, 2025
 *      Author: richardmatthews
 */

#include "buttons.h"
#include "main.h"

#define BUTTON_DEBOUNCE_MS 150

bool button_user_pressed() {
    if (HAL_GPIO_ReadPin(USER_Button_GPIO_Port, USER_Button_Pin) == GPIO_PIN_SET) {
        HAL_Delay(BUTTON_DEBOUNCE_MS);

        if (HAL_GPIO_ReadPin(USER_Button_GPIO_Port, USER_Button_Pin) == GPIO_PIN_SET) {
            return true;
        }
    }

    return false;
}
