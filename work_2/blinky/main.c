/**
 *@}
 **/

/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 * This file contains the source code for a sample application to blink LEDs.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#define WAIT_TIME 1000

#define MY_LED_1 NRF_GPIO_PIN_MAP(0, 6)

#define MY_LED_2_R NRF_GPIO_PIN_MAP(0, 8)

#define MY_LED_2_G NRF_GPIO_PIN_MAP(1, 9)

#define MY_LED_2_B NRF_GPIO_PIN_MAP(0, 12)

#define MY_SW_1 NRF_GPIO_PIN_MAP(1, 6)

static const int number_blinks[LEDS_NUMBER] = {6, 6, 1, 1};/*device id 6610, last 1 for clarity.*/

static const uint32_t leds[LEDS_NUMBER] = {MY_LED_1, MY_LED_2_R, MY_LED_2_G, MY_LED_2_B};/*pins in order*/

void set_leds(){
    for(int i = 0;i < LEDS_NUMBER; ++i){
        nrf_gpio_cfg_output(leds[i]);
    }
}

void set_sw(){
    nrf_gpio_cfg_input(MY_SW_1, NRF_GPIO_PIN_PULLUP);
}

void task_1(){
    for(int i = 0; i < LEDS_NUMBER; ++i){
        for(int j = 0; j < number_blinks[i]; ++j){
            if(nrf_gpio_pin_read(MY_SW_1)){
                --j;
            }
            else{
                nrf_gpio_pin_write(leds[i], 0);
                for(uint32_t k = 0; k < WAIT_TIME; k+=10){
                    if(nrf_gpio_pin_read(MY_SW_1)){
                        nrf_gpio_pin_write(leds[i], 1);
                        --j;
                        break;
                    }
                    nrf_delay_ms(10);
                }
                nrf_gpio_pin_write(leds[i], 1);
                nrf_delay_ms(WAIT_TIME);
            }
        }
    }
}

void task_2(){
    for(int i = 0; i < LEDS_NUMBER; ++i){
        for(int j = 0; j < number_blinks[i]; ++j){
            if(nrf_gpio_pin_read(MY_SW_1)){
                --j;
            }
            else{
                nrf_gpio_pin_write(leds[i], 0);
                for(uint32_t k = 0; k < WAIT_TIME; k+=10){
                    if(nrf_gpio_pin_read(MY_SW_1)){
                        nrf_delay_ms(WAIT_TIME - k);
                        nrf_gpio_pin_write(leds[i], 1);
                        break;
                    }
                    nrf_delay_ms(10);
                }
                nrf_gpio_pin_write(leds[i], 1);
                nrf_delay_ms(WAIT_TIME);
            }
        }
    }
}

/**
 * @brief Function for application main entry.
 */
int main(void){
    /* Configure board. */
    bsp_board_init(BSP_INIT_LEDS);
    bsp_board_leds_off();
    set_leds();
    set_sw();
    /* Toggle LEDs. */
    while (true){
        task_2();/*or task_1()*/
    }
}

/**
 *@}
 **/
