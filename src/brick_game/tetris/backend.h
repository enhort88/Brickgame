#ifndef BACKEND_H
#define BACKEND_H

#include "tetris.h"
#include <string.h>


void init_piece();
void reset_game();
void spawn_piece();
void move_piece_down();
void down();
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