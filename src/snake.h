#ifndef SNAKE_H
#define SNAKE_H

typedef struct cell_t cell_t;
typedef enum dir_t dir_t;

struct cell_t {
    int x;
    int y;
};

enum dir_t {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

void init_snake(int scr_width, int scr_height);

void set_movement_dir(dir_t dir);
void update_snake();

unsigned int get_snake_length();
cell_t *get_snake_cells();
cell_t *get_food_cell();

unsigned char is_over();

#endif
