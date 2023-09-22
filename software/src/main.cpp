/** 
 * @file main.hpp
 * @brief 
 * @author Michael Granberry
*/

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "inc/game.hpp"
#include <iostream>

// GPIO pin numbers
const uint8_t BTN1_PIN = 18;
const uint8_t BTN2_PIN = 17;
const uint8_t BTN3_PIN = 16;
const uint8_t LED1_PIN = 15;
const uint8_t LED2_PIN = 14;

// core1
void core1_entry() {
    uint32_t led_state_core1;

    // configure leds
    led led[number_of_leds] = {
        {.gpio_pin = LED1_PIN},
        {.gpio_pin = LED2_PIN}
    };

    while (true)
    {
        if (multicore_fifo_rvalid()) {
            led_state_core1 = multicore_fifo_pop_blocking();
        }

        // play led pattern
        led_pattern(led_state_core1, led);
    }
}


// core0
int main() {
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    sleep_ms(1000); // If I don't delay my program initially, I won't see the board print at the start of the game. It's weird.

    // configure gpio pins
    gpio_config my_gpio[number_of_gpio] = {
        {.gpio_pin = BTN1_PIN, .pin_dir = GPIO_IN},
        {.gpio_pin = BTN2_PIN, .pin_dir = GPIO_IN},
        {.gpio_pin = BTN3_PIN, .pin_dir = GPIO_IN},
        {.gpio_pin = LED1_PIN, .pin_dir = GPIO_OUT},
        {.gpio_pin = LED2_PIN, .pin_dir = GPIO_OUT}
    };
    init_gpio(my_gpio);

    // configure buttons
    btn btn[number_of_btns] = {
        {.gpio_pin = BTN1_PIN},
        {.gpio_pin = BTN2_PIN},
        {.gpio_pin = BTN3_PIN}
    };
    init_btn(btn);

    // super loop
    while (true)
    {     
        // while game is not over, play game.
        if(!get_gameover()) {
            
            // initiate game
            if (get_is_board_printed()) {
                init_game();
            }
            // select position
            if (debounce(btn[0])) {
                select_position();
            }
            // enter position
            else if (debounce(btn[1])) {
                enter_position();
            }
        }
        else {
            // reset game
            if (debounce(btn[2])) {
                reset_game();
            }
        }
    }
}