#ifndef FSM_MATRIX_H
#define FSM_MATRIX_H

#include "structures.h"
#include "backend.h"

UserAction_t keyboard_action(int *ch, int *pocket, bool *hold);
void userInput(UserAction_t action, bool hold);
void transitionState(UserAction_t action);
const char* getActionName(UserAction_t action);
const char* getStateName(FSMState state);

#endif 