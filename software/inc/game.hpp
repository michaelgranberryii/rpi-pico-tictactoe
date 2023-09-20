/** 
 * @file game.hpp
 * @brief 
 * @author Michael Granberry
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "pico/stdlib.h"
#include "pico/multicore.h"

const char clear_space = ' ';
const uint8_t board_size = 3;
const uint8_t number_of_gpio = 5;
const uint8_t number_of_btns = 3;
const uint8_t number_of_leds = 2;
const uint8_t high = 1;
const uint8_t low = 0;
const char X = 'X';
const char O = 'O';
const uint8_t debounce_time = 50;

struct gpio_config {
    uint8_t gpio_pin;
    uint8_t pin_dir;
}; typedef gpio_config gpio_config;

struct btn {
    uint8_t gpio_pin;
    uint8_t raw_btn_curr_state;
    uint8_t raw_btn_prev_state;
    uint8_t btn_press;
}; typedef btn btn;

struct led {
    uint8_t gpio_pin;
}; typedef led led;

void init_game();
void select_position();
void enter_position();
void reset_game();
void player_won();
void tie_game();

void board_position();
bool is_position_clear(uint8_t row, uint8_t col);
void mark_position(uint8_t row, uint8_t col);
bool check_for_win();
bool check_for_tie();

void update_select_position();
void print_board();
void print_player_turn();
void print_winner();

bool set_gameover(bool gameover_arg);
bool get_gameover();
bool set_is_board_printed();
bool get_is_board_printed();
void reset_row_col();

void init_btn (btn *btn);
void init_gpio(gpio_config *gpio);
uint8_t debounce(btn &my_btn);
uint8_t poll(uint8_t btn_pin);
void led_pattern(uint8_t sel,led *led);
uint8_t get_led_state();
#endif