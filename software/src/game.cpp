/** 
 * @file game.cpp
 * @brief 
 * @author Michael Granberry
*/

#include "inc/game.hpp"

bool gameover = false;
bool is_board_printed = true;
bool winner = false;
char current_player = 'X';
int btn_press_count;
int8_t row;
int8_t col;
int8_t prev_row;
int8_t prev_col;
uint8_t number_of_moves = 0;
uint8_t led_state = 0;

char board[board_size][board_size] = {
        {clear_space, clear_space, clear_space},
        {clear_space, clear_space, clear_space},
        {clear_space, clear_space, clear_space}
    };


void init_gpio(gpio_config *gpio) {
    for (int i = 0; i < number_of_gpio; i++)
    {
        gpio_init(gpio[i].gpio_pin);
        gpio_set_dir(gpio[i].gpio_pin, gpio[i].pin_dir);
    }
}

void init_btn (btn *btn) {
    for (int i = 0; i < number_of_btns; i++)
    {
        gpio_init(btn[i].gpio_pin);
        btn[i].raw_btn_curr_state = 0;
        btn[i].raw_btn_prev_state = 0;
        btn[i].btn_press = 0;
    }
}

// Button debouncing
uint8_t debounce(btn &my_btn) {
    my_btn.raw_btn_curr_state = poll(my_btn.gpio_pin); // Read in push button signal
    if (my_btn.raw_btn_curr_state == 1) { 
        if(my_btn.raw_btn_prev_state == 0) {
            // Push button was pressed
            my_btn.btn_press = 1;
        } else {
            // Push button held down
            my_btn.btn_press = 0;
        }
    } else {
        // Push button was not pressed
        my_btn.btn_press = 0;
    }
    my_btn.raw_btn_prev_state = my_btn.raw_btn_curr_state;
    sleep_ms(debounce_time);
    return my_btn.btn_press;
}

// Get state of gpio pin conected to push button
uint8_t poll(uint8_t btn_pin) {
    uint8_t state = gpio_get(btn_pin);
    return state; // return gpio pin value
}

void init_game() {
    reset_row_col();
    set_is_board_printed();
    print_board();
    print_player_turn();
    printf("Press button 1 to select a position ---> #.\n");
    printf("Press button 2 to enter a position.\n");
    multicore_fifo_push_blocking(get_led_state());
}

void select_position() {
    board_position();
    update_select_position();
    print_board();
    btn_press_count++;
    print_player_turn();
    printf("Press button 1 to select a position ---> #.\n");
    // printf("Row: %d, Col: %d\n", row, col);
    printf("Press button 2 to enter a position.\n");
}

void enter_position() {
    number_of_moves++;
    // check position
    if (is_position_clear(row, col)) {
        // mark position
        mark_position(row, col);
        reset_row_col();
        // check for a win
        if (check_for_win()) {
            led_state = (current_player == X ) ? 2 : 3;
            player_won();
            winner = true;
        }
        // check for a tie
        else if (check_for_tie()) {
            led_state = 4;
            tie_game();
            winner = true;
        }
        else {
            // switch player
            current_player = (current_player == X ) ? O : X;
            led_state = (current_player == X ) ? 0 : 1;
            print_board();
            print_player_turn();
            printf("Press button 1 to select a position ---> #.\n");
            printf("Press button 2 to enter a position.\n");
            multicore_fifo_push_blocking(get_led_state());
        }
    } else {
        printf("Position is already taken!!!!!!\n");
    }
}

// check for a win
bool check_for_win(){
    bool is_match;
    for (u_int8_t i = 0; i <= board_size; i++) {
        switch (i)
        {
            // row win
        case 0:
            for (uint8_t i = 0; i < board_size; i++)
            {
                is_match = true;
                for (uint8_t j = 0; j < board_size; j++)
                {
                    is_match = is_match && ((board[i][j] == current_player) ? true : false);
                }
                if (is_match) {
                    printf("row win\n");
                    break;
                }
            }
            break;

            // col win
        case 1:
            
            for (uint8_t i = 0; i < board_size; i++)
            {
                is_match = true;
                for (uint8_t j = 0; j < board_size; j++)
                {
                    is_match = is_match && ((board[j][i] == current_player) ? true : false);
                }
                if (is_match) {
                    printf("col win\n");
                    break;
                }
            }
            break;

            // major diagonal win
        case 2:
            is_match = false;
            if ((board[0][0] == current_player) && (board[1][1] == current_player) && (board[2][2] == current_player)) {
                is_match = true;
                printf("man win\n");
            } 
            break;

            // minor diagonal win
        case 3:
            is_match = false;
            if ((board[0][2] == current_player) && (board[1][1] == current_player) && (board[2][0] == current_player)) {
                is_match = true;
                printf("min win\n");
            }
            break;

            // else reset board
        default:
            reset_game();
            break;
        }
        if (is_match) {
            printf("break is_match\n");
            break;
        }
    }
    return is_match;
}
    
bool check_for_tie() {
    bool is_tie = false;
    is_tie = (number_of_moves == (board_size*board_size)) ? true : false;
    return is_tie;
}

// check position
bool is_position_clear(uint8_t row, uint8_t col) {
    if (board[row][col] == '#') {
        return true;
    } else {
        return false;
    } 
}

void player_won() {
    print_board();
    printf("Player %c won!\n", current_player);
    printf("Press button 3 to reset game.\n");
    set_gameover(true);
    multicore_fifo_push_blocking(get_led_state());
}

void tie_game() {
    print_board();
    printf("Tie game!\n");
    printf("Press button 3 to reset game.\n");
    set_gameover(true);
    multicore_fifo_push_blocking(get_led_state());
}

// makr position
void mark_position(uint8_t row, uint8_t col) {
    board[row][col] = current_player;
}

void print_board() {
    // printf("\e[1;1H\e[23]");
    printf("\033[H\033[J"); 
    printf(" %c | %c | %c \n", board[0][0], board[0][1],board[0][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1],board[1][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1],board[2][2]);
}

void reset_game() {
    for (uint8_t i = 0; i < board_size; i++) {
        for (uint8_t j = 0; j < board_size; j++) {
            board[i][j] = clear_space;
        }
    }
    print_board();
    number_of_moves = 0;
    reset_row_col();
    set_gameover(false);
    current_player = (current_player == X ) ? O : X;
    led_state = (current_player == X ) ? 0 : 1;
    print_player_turn();
    printf("Press button 1 to select a position ---> #.\n");
    printf("Press button 2 to enter a position.\n");
    multicore_fifo_push_blocking(get_led_state());
}

void board_position() {
    prev_row = row;
    prev_col = col;
    printf("prev_row: %d, prev_col: %d\n", prev_row, prev_col);
    bool is_not_empty = true;
    while (is_not_empty) {
        if (btn_press_count == 9) {
            reset_row_col();
        }
        col = btn_press_count % board_size;
        if ((btn_press_count % board_size) == 0) {
            row++;
        }

        if (board[row][col] == ' ') {
            board[row][col] = '#';
            break;
        }
        btn_press_count++;
    }

}

void update_select_position() {
    if (prev_row != -1) {
        board[prev_row][prev_col] = ' ';
    }
    
}

bool set_gameover(bool gameover_arg) {
    gameover = gameover_arg;
    return gameover;
}

bool get_gameover() {
    return gameover;
}

bool set_is_board_printed() {
    is_board_printed = (is_board_printed == true) ? false : true;
    return is_board_printed;
}

bool get_is_board_printed() {
    return is_board_printed;
}

uint8_t get_led_state() {
    return led_state;
}

void reset_row_col() {
    row = -1;
    col = 0;
    btn_press_count = 0;
}

void print_player_turn() {
    printf("It's player %c's turn.\n", current_player);
}

void led_pattern(uint8_t sel, led *led) {
        switch (sel)
        {
        // x move
        case 0:
            gpio_put(led[0].gpio_pin, high);
            gpio_put(led[1].gpio_pin, low);
            break;

        // o move
        case 1:
            gpio_put(led[1].gpio_pin, high);
            gpio_put(led[0].gpio_pin, low);
            break;

        // x won
        case 2:
            gpio_put(led[0].gpio_pin, high);
            sleep_ms(500);
            gpio_put(led[0].gpio_pin, low);
            sleep_ms(500);
            break;

        // o won
        case 3:
            gpio_put(led[1].gpio_pin, high);
            sleep_ms(500);
            gpio_put(led[1].gpio_pin, low);
            sleep_ms(500);
            break;
        
        // tie
        case 4:
            gpio_put(led[0].gpio_pin, high);
            gpio_put(led[1].gpio_pin, high);
            sleep_ms(500);
            gpio_put(led[0].gpio_pin, low);
            gpio_put(led[1].gpio_pin, low);
            sleep_ms(500);
            break;
        
        // led off
        default:
            gpio_put(led[0].gpio_pin, low);
            gpio_put(led[1].gpio_pin, low);
            break;
        }
}
