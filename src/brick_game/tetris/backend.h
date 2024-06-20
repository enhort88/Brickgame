#ifndef BACKEND_H
#define BACKEND_H

#include "tetris.h"
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SHAPE_I {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_O {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_T {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_Z {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_S {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_L {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_J {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPES {SHAPE_I, SHAPE_O, SHAPE_T, SHAPE_Z, SHAPE_S, SHAPE_L, SHAPE_J}

void init_piece();
void reset_game();
void spawn_piece();
void move_piece_down();

void move_piece_left();
void move_piece_right();
void move_piece_up();
void rotate_piece();

void clear_lines(int line);

bool check_collision();
bool check_collision_end_game();
void attach_piece_to_field();
void check_for_complete_lines();

void update_score(); 
void save_high_score(); 
int load_high_score(); 

#endif