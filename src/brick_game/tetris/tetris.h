#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <ncurses.h>
#include "backend.h"
#include "fsm_matrix.h"



void tetris_start();
GameInfo_t updateCurrentState();
void initialize_game();
void reset_game();
void test_print(clock_t current_time, clock_t last_update_time, char ch);
bool tet_timer(clock_t *current_time, clock_t *last_update_time, int update_interval);
void check_state();
#endif 