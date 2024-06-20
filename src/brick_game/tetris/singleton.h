#ifndef SINGLETON_H
#define SINGLETON_H

#include "structures.h"


typedef struct {
    GameInfo_t game;
    FSMState state;
    Piece current_piece;
    UserAction_t action;
    int shape_curr;
} Singleton;

Singleton* get_instance();
void free_singleton();

#endif // SINGLETON_H
