#ifndef BACKEND_H
#define BACKEND_H

#define _POSIX_C_SOURCE 200809L
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "structures.h"

int play_tetris(int ch);
int tetris_start();
GameInfo_t updateCurrentState();
int initialize_game();

bool tet_timer(clock_t const *current_time, clock_t const *last_update_time,
               int speed);
void free_game_resources();

UserAction_t keyboard_action(int *ch, int *pocket, bool *hold, bool test);
void userInput(UserAction_t action, bool hold);

void change_speed();
void init_piece();

void move_piece_down();

void move_piece_left();
void move_piece_right();
void move_piece_up();
void rotate_piece();

void clear_lines(int line);
int lines_cleared_score(int lines_cleared);

bool check_collision();
bool check_collision_end_game();
void attach_piece_to_field();
void check_for_complete_lines();

// void test_print(clock_t current_time, clock_t last_update_time);
// const char *getActionName(UserAction_t action);
// const char *getStateName(FSMState state);
#endif  // BACKEND_H