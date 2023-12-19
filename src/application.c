#include "snake.h"
#include "terminal_interface.h"
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>

#include <stdio.h>

#define MAX(a, b) (a > b ? a : b)

// escape sequences for the arrow keys
#define UP_ESC "\e[A"
#define RIGHT_ESC "\e[C"
#define DOWN_ESC "\e[B"
#define LEFT_ESC "\e[D"

#define SNAKE "\u2588"
#define FOOD "X"
#define EMPTY "\u00B7"

struct timespec ts = {0, 50 * 1000000}; // dt = 200ms

int main() {
    // terminal size
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int scr_width = w.ws_col;
    int scr_height = w.ws_row;

    init_terminal();
    init_snake(scr_width, scr_height);

    clear_terminal();
    for (int x = 0; x < scr_width; x++) {
        for (int y = 0; y < scr_height; y++) {
            print_str(x, y, EMPTY);
        }
    }

    unsigned int snake_len = 1;
    cell_t *snake = get_snake_cells();
    cell_t *food = get_food_cell();

    char input[32];

    unsigned char quit = 0;

    while (!is_over() && !quit) {
        print_str(snake[snake_len - 1].x, snake[snake_len - 1].y, EMPTY);
        print_str(food->x, food->y, EMPTY);

        unsigned int len = read_input(input, 32);
        unsigned int cnt = 0;
        while (cnt < len) {
            if (len - cnt >= 3) {
                if (strncmp(input + cnt, UP_ESC, 3) == 0) {
                    set_movement_dir(UP);
                } else if (strncmp(input + cnt, RIGHT_ESC, 3) == 0) {
                    set_movement_dir(RIGHT);
                } else if (strncmp(input + cnt, DOWN_ESC, 3) == 0) {
                    set_movement_dir(DOWN);
                } else if (strncmp(input + cnt, LEFT_ESC, 3) == 0) {
                    set_movement_dir(LEFT);
                } else if (input[cnt] == '\e' || input[cnt] == 'q') {
                    quit = 1;
                    break;
                }
                cnt += 3;
            } else if (input[cnt] == '\e' || input[cnt] == 'q') {
                quit = 1;
                break;
            } else {
                cnt++;
            }
        }

        update_snake();

        snake_len = get_snake_length();
        print_str(snake[0].x, snake[0].y, SNAKE);
        print_str(snake[snake_len - 1].x, snake[snake_len - 1].y, SNAKE);
        print_str(food->x, food->y, FOOD);

        nanosleep(&ts, &ts);
    }

    restore_terminal();
}
