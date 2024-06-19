#ifndef SINGLETON_H
#define SINGLETON_H

#include "structures.h"

typedef struct {
    GameInfo_t game;
    FSMState state;
    Piece current_piece;
    UserAction_t action;

} Singleton;

Singleton* get_instance();

#endif // SINGLETON_H
