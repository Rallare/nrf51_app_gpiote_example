/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
* @defgroup blinky_example_pca10001_main main.c
* @{
* @ingroup blinky_example_pca10001
*
* @brief Blinky Example Application main file.
*
* This file contains the source code for a sample application using GPIO to drive LEDs.
*
*/

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_gpiote.h"

#define APP_GPIOTE_MAX_USERS    1 /** Change this to match amount ot GPIOTE users, app_button uses one */

app_gpiote_user_id_t m_app_gpiote_my_id;

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    NVIC_SystemReset();
}

void gpiote_event_handler(uint32_t event_pins_low_to_high, uint32_t event_pins_high_to_low)
{
    if (event_pins_high_to_low & (1 << BUTTON_0))
        nrf_gpio_pin_set(LED_0);
    
    if (event_pins_low_to_high & (1 << BUTTON_0))
        nrf_gpio_pin_clear(LED_0);
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    uint32_t err_code;
    // Configure LED-pins as outputs
    nrf_gpio_cfg_output(LED_0);
    nrf_gpio_cfg_output(LED_1);
    nrf_gpio_cfg_sense_input(BUTTON_0, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
    
    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);    
    err_code = app_gpiote_user_register(&m_app_gpiote_my_id, (1 << BUTTON_0), (1 << BUTTON_0), gpiote_event_handler);
    APP_ERROR_CHECK(err_code);
    
    err_code = app_gpiote_user_enable(m_app_gpiote_my_id);
    APP_ERROR_CHECK(err_code);
    while(true)
    {
        nrf_gpio_pin_set(LED_1);

        nrf_delay_ms(500);

        nrf_gpio_pin_clear(LED_1);   

        nrf_delay_ms(500);
    }
}

/**
 *@}
 **/
