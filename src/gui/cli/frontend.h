#ifndef FRONTEND_H
#define FRONTEND_H

#include <unistd.h>
#include "../../brick_game/tetris/tetris.h"

#define WIDTH 10
#define HEIGHT 20

void play_tetris();


void main_menu_init();
void draw_menu();
void draw_field(GameInfo_t game_curr);
void draw_board();
void draw_next(GameInfo_t game); 
void draw_score(int score, int high_score, int level); 
void update_field(GameInfo_t game);
void draw_piece(Piece piece);
void free_game_resources();
void draw_key(char ch);  //del
void game_over_menu();           
#endif