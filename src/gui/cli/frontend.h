#ifndef FRONTEND_H
#define FRONTEND_H

#include <unistd.h>
#include "../../brick_game/tetris/backend.h"
#define HIGH_SCORE_FILE "highscore.txt"

#define WIDTH 10
#define HEIGHT 20

void main_menu_init();
void draw_field(GameInfo_t game_curr);
void draw_board();
void draw_next(GameInfo_t game); 
void draw_score(int score, int high_score, int level); 
void update_field(GameInfo_t game);
void draw_piece(Piece piece);
void game_over_menu();
int read_high_score();         
void write_high_score(int high_score);  
#endif