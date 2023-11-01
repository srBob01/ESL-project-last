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

void turn_on_led(int i){
    nrf_gpio_pin_write(leds[i], 0);
}

void turn_off_led(int i){
    nrf_gpio_pin_write(leds[i], 1);
}

bool is_press(){
    return !nrf_gpio_pin_read(MY_SW_1);
}

bool is_not_press(){
    return !is_press();
}

void task_1(){
    for(int i = 0; i < LEDS_NUMBER; ++i){
        for(int j = 0; j < number_blinks[i]; ++j){
            if(is_not_press()){
                --j;
            }
            else{
                turn_on_led(i);
                for(uint32_t k = 0; k < WAIT_TIME; k+=10){
                    if(is_not_press()){
                        turn_off_led(i);
                        --j;
                        break;
                    }
                    nrf_delay_ms(10);
                }
                turn_off_led(i);
                nrf_delay_ms(WAIT_TIME);
            }
        }
    }
}

void task_2(){
    for(int i = 0; i < LEDS_NUMBER; ++i){
        for(int j = 0; j < number_blinks[i]; ++j){
            if(is_not_press()){
                --j;
            }
            else{
                turn_on_led(i);
                for(uint32_t k = 0; k < WAIT_TIME; k+=10){
                    if(is_not_press()){
                        nrf_delay_ms(WAIT_TIME - k);
                        turn_off_led(i);
                        break;
                    }
                    nrf_delay_ms(10);
                }
                turn_off_led(i);
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
