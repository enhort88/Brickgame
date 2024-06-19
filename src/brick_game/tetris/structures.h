#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>
#define GET_USER_INPUT getch()

typedef enum {
    START,
    SPAWN,
    MOVING,
    SHIFTING,
    ATTACHING,
    GAME_OVER,
    // PAUSE,
    NUM_STATES
} FSMState;

typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

typedef struct {
    int x, y; 
    int shape[4][4]; 
} Piece;

#include "singleton.h" 


#endif