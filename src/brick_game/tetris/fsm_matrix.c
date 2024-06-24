#include "fsm_matrix.h"

#include <stdio.h>  //del

#include "../../gui/cli/frontend.h"

FSMState transitionMatrix[NUM_STATES][8] = {
    [START] = {[Start] = SPAWN,
               [Terminate] = GAME_OVER,
               [Pause] = START,
               [Left] = START,
               [Right] = START,
               [Up] = START,
               [Down] = START,
               [Action] = START},
    [SPAWN] = {[Action] = MOVING,
               [Terminate] = GAME_OVER,
               [Pause] = SPAWN,
               [Left] = SPAWN,
               [Right] = SPAWN,
               [Up] = SPAWN,
               [Down] = SPAWN,
               [Start] = SPAWN},
    [MOVING] = {[Action] = SHIFTING,
                [Terminate] = GAME_OVER,
                [Pause] = MOVING,
                [Left] = MOVING,
                [Right] = MOVING,
                [Up] = MOVING,
                [Down] = MOVING,
                [Start] = MOVING},
    [SHIFTING] = {[Action] = SHIFTING,
                  [Terminate] = GAME_OVER,
                  [Pause] = MOVING,
                  [Left] = MOVING,
                  [Right] = MOVING,
                  [Up] = MOVING,
                  [Down] = MOVING,
                  [Start] = START},
    [ATTACHING] = {[Action] = SPAWN,
                   [Terminate] = GAME_OVER,
                   [Pause] = ATTACHING,
                   [Left] = ATTACHING,
                   [Right] = ATTACHING,
                   [Up] = ATTACHING,
                   [Down] = ATTACHING,
                   [Start] = ATTACHING},
    [GAME_OVER] = {[Action] = GAME_OVER,
                   [Terminate] = GAME_OVER,
                   [Pause] = GAME_OVER,
                   [Left] = GAME_OVER,
                   [Right] = GAME_OVER,
                   [Up] = GAME_OVER,
                   [Down] = GAME_OVER,
                   [Start] = START}};

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  Singleton *s = get_instance();
  switch (action) {
    case Left:
      move_piece_left();
      update_field(s->game);
      break;
    case Right:
      move_piece_right();
      update_field(s->game);
      break;
    case Down:
      if (s->state == MOVING) {
        while (s->state != ATTACHING) move_piece_down();
        update_field(s->game);
      }
      break;
    case Up:
      move_piece_up();
      break;
    case Action:
      rotate_piece();
      update_field(s->game);
      break;
    case Pause:
      s->game.pause = !(s->game.pause);
      break;
    case Start:
      free_game_resources();
      free_singleton();
      tetris_start(0);
      break;
    case Terminate:
      s->state = GAME_OVER;
      break;
  }
}

UserAction_t keyboard_action(int *ch, int *pocket, bool *hold) {
    Singleton * s=  get_instance();

  *ch = GET_USER_INPUT;
  *hold = (*pocket == *ch) ? true : false;
  *pocket = *ch;
  if (s->test==1) {*ch = KEY_DOWN;
  s->game.speed=0;}
  switch (*ch) {
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_DOWN:
      return Down;
    case KEY_UP:
      return Up;
    case ' ':
      return Action;
    case 'p':
      return Pause;
    case 's':
      return Start;
    case 'q':
      return Terminate;
    default:
      return ERR;
  }
}

void transitionState(UserAction_t action) {
  Singleton *s = get_instance();
  if (action < 0 || action >= 8) {
    return;
  }
  s->state = transitionMatrix[s->state][action];
}

const char *getStateName(FSMState state) {
  switch (state) {
    case START:
      return "START";
    case SPAWN:
      return "SPAWN";
    case MOVING:
      return "MOVING";
    case SHIFTING:
      return "SHIFTING";
    case ATTACHING:
      return "ATTACHING";
    case GAME_OVER:
      return "GAME_OVER";
      // case PAUSE: return "PAUSE";
    default:
      return "UNKNOWN";
  }
}
const char *getActionName(UserAction_t action) {
  switch (action) {
    case Start:
      return "Start";
    case Pause:
      return "Pause";
    case Terminate:
      return "Terminate";
    case Left:
      return "Left";
    case Right:
      return "Right";
    case Up:
      return "Up";
    case Down:
      return "Down";
    case Action:
      return "Action";
    default:
      return "UNKNOWN";
  }
}