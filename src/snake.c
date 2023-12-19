#include "snake.h"
#include <stdlib.h>
#include <time.h>

int width, height;
unsigned int snake_length;
cell_t snake[1024];
dir_t snake_dir;
cell_t food;
unsigned char over;

void random_food();

void init_snake(int scr_width, int scr_height) {
    width = scr_width;
    height = scr_height;
    snake_length = 1;
    snake[0].x = 0;
    snake[0].y = 0;
    snake_dir = RIGHT;
    random_food();
    over = 0;

    srand((unsigned int) time(NULL));
}

void set_movement_dir(dir_t dir) {
    if (snake_dir - 2 != dir && snake_dir + 2 != dir) {
        snake_dir = dir;
    }
}

void update_snake() {
    if (over) return;

    int xdif = (-(snake_dir & 2) + 1) * (snake_dir & 1);
    int ydif = (-(~snake_dir & 2) + 1) * (~snake_dir & 1);
    cell_t next_cell = {snake[0].x + xdif, snake[0].y + ydif};

    if (next_cell.x < 0 || next_cell.x >= width || next_cell.y < 0 || next_cell.y >= height) {
        over = 1;
        return;
    }

    if (next_cell.x == food.x && next_cell.y == food.y) {
        snake_length++;
        random_food();
    }

    for (int i = snake_length - 1; i >= 1; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = next_cell;

    for (int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            over = 1;
            return;
        }
    }
}

void random_food() {
    cell_t random;
    unsigned char failed = 0;
    do {
        random.x = rand() % width;
        random.y = rand() % height;

        failed = (random.x == food.x && random.y == food.y); // don't chose the same spot again
        for (int i = 0; i < snake_length && !failed; i++) {
            failed = (random.x == snake[i].x && random.y == snake[i].y);
        }
    } while(failed);
    food = random;
}

unsigned int get_snake_length() {
    return snake_length;
}

cell_t *get_snake_cells() {
    return snake;
}

cell_t *get_food_cell() {
    return &food;
}

unsigned char is_over() {
    return over;
}
