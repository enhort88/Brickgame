#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <ncurses.h>
#include "backend.h"
#include "fsm_matrix.h"



int tetris_start();
GameInfo_t updateCurrentState();
int initialize_game();
//void test_print(clock_t current_time, clock_t last_update_time);
bool tet_timer(clock_t *current_time, clock_t *last_update_time, int speed);
void check_state();

#endif 