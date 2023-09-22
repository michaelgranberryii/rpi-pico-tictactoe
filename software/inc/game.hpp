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

// Const
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

// Struct for configuring gpio pins and direction
struct gpio_config {
    uint8_t gpio_pin;
    uint8_t pin_dir;
}; typedef gpio_config gpio_config;

// Struct for configuring buttons
struct btn {
    uint8_t gpio_pin;
    uint8_t raw_btn_curr_state;
    uint8_t raw_btn_prev_state;
    uint8_t btn_press;
}; typedef btn btn;

// Struct for configuring LEDs
struct led {
    uint8_t gpio_pin;
}; typedef led led;

/**
 * @brief Intialized game
*/
void init_game();

/**
 * @brief Lets user select a position on board
*/
void select_position();

/**
 * @brief Lets user enter in a position on board
*/
void enter_position();

/**
 * @brief Resets the game
*/
void reset_game();

/**
 * @brief Displays which player won and flashes corresponding LED
*/
void player_won();

/**
 * @brief Displays that the game was a tie and flashes both LEDs
*/
void tie_game();

/**
 * @brief Handles the traversing of baord only to open positions. Displays a '#' as current selected position
*/
void board_position();

/**
 * @brief Checks to see if current selected position is not taken
 * @param row Current row
 * @param col Current column
*/
bool is_position_clear(uint8_t row, uint8_t col);

/**
 * @brief Marks position with current players letter
 * @param row Current row
 * @param col Current column
*/
void mark_position(uint8_t row, uint8_t col);

/**
 * @brief Checks for a row, col, and diagonal win
*/
bool check_for_win();

/**
 * @brief Checks for a tie. No win and number of entries equals 9.
*/
bool check_for_tie();

/**
 * @brief Replace '#' with ' '
*/
void update_select_position();

/**
 * @brief Prints board
*/
void print_board();

/**
 * @brief Prints a statement to indicate players turn
*/
void print_player_turn();

/**
 * @brief Prints winner
*/
void print_winner();

/**
 * @brief Sets boolean variable 'gameover' to gameover_arg
 * @param gameover_arg Updated gameover variable
*/
bool set_gameover(bool gameover_arg);

/**
 * @brief Returns 'gameover' value
*/
bool get_gameover();

/**
 * @brief Toggles 'is_board_printed" variable
*/
bool set_is_board_printed();

/**
 * @brief Returns the value of is_board_printed
*/
bool get_is_board_printed();

/**
 * @brief Resets row = -1 and col = 0
*/
void reset_row_col();

/**
 * @brief Configures btn array structure with default values
 * @param btn Pointer to first element of btn struct array
*/
void init_btn (btn *btn);

/**
 * @brief Configures gpio array structure with gpio pin numbers and pin direction
 * @param gpio Pointer to first element of gpio struct array
*/
void init_gpio(gpio_config *gpio);

/**
 * @brief Debounces the button
 * @param my_btn A reference to btn a struct
*/
uint8_t debounce(btn &my_btn);

/**
 * @brief Checks state of button
 * @param btn_pin The gpio pin of a button
*/
uint8_t poll(uint8_t btn_pin);

/**
 * @brief Selects a LED pattern to play
 * @param sel Use as the selector in case statement
 * @param led LED array pointer 
*/
void led_pattern(uint8_t sel,led *led);

/**
 * @brief Returns the led pattern selector variable value
*/
uint8_t get_led_state();
#endif