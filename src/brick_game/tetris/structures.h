#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>
#define GET_USER_INPUT getch()

#define SHAPE_I {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_O {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_T {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_Z {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_S {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_L {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPE_J {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
#define SHAPES {SHAPE_I, SHAPE_O, SHAPE_T, SHAPE_Z, SHAPE_S, SHAPE_L, SHAPE_J}



typedef enum {
    START,
    SPAWN,
    MOVING,
    SHIFTING,
    ATTACHING,
    GAME_OVER,
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
    int type;
} Piece;



#include "singleton.h" 



#endif