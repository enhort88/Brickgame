#include "tetris.h"

#include "../../gui/cli/frontend.h"

int tetris_start() {
  int res = 0;
  Singleton *s = get_instance();
  s->state = SPAWN;
  res = initialize_game();
  int ch = '\0';
  int pocket = '\0';
  bool hold = false;
  clock_t last_update_time = clock();
  nodelay(stdscr, TRUE);
  while (s->state != GAME_OVER) {
    change_speed();
    clock_t current_time = clock();
    if (tet_timer(&current_time, &last_update_time, s->game.speed)) {
      if (!s->game.pause) {
        update_field(updateCurrentState());
      }
      last_update_time = current_time;
    }

    userInput(keyboard_action(&ch, &pocket, &hold), hold);
  //    test_print(current_time, last_update_time);  // del after all
    refresh();
    if (s->test==1) s->state=GAME_OVER;
  }
  nodelay(stdscr, FALSE);
  free_game_resources();
  free_singleton();
  if (s->test!=1){
  game_over_menu();}
  return res;
}

GameInfo_t updateCurrentState() {
  Singleton *s = get_instance();
  int ch = '\0';
  bool hold = false;
  int pocket = '\0';
  s->action = keyboard_action(&ch, &pocket, &hold);
  userInput(s->action, hold);
  check_state();
  return s->game;
}

int initialize_game() {
  int res = 0;
  Singleton *s = get_instance();
  s->game.field = (int **)calloc(HEIGHT, sizeof(int *));
  if (!s->game.field) {
    perror("Failed to allocate memory for game.field");
    res++;
  }
  for (int i = 0; i < HEIGHT && !res; i++) {
    s->game.field[i] = (int *)calloc(WIDTH, sizeof(int));
    if (!s->game.field[i]) {
      perror("Failed to allocate memory for game.field[i]");
      res++;
    }
  }
  if (!res) {
    s->game.next = (int **)calloc(4, sizeof(int *));
    if (!s->game.next) {
      perror("Failed to allocate memory for game.next");
      res++;
    }
    for (int i = 0; i < 4 && !res; i++) {
      s->game.next[i] = (int *)calloc(4, sizeof(int));
      if (!s->game.next[i]) {
        perror("Failed to allocate memory for game.next[i]");
        res++;
      }
    }
  }
  s->game.score = 0;
  s->game.high_score = read_high_score();
  s->game.level = 0;
  s->game.speed = 8000;

  init_piece();
  return res;
}

// void test_print(clock_t current_time, clock_t last_update_time) {
//   Singleton *s = get_instance();

//   mvprintw(19, 24, "Currtime: %ld", current_time);
//   mvprintw(20, 24, "LastUpT: %ld", last_update_time);
//   mvprintw(21, 24, "RES: %ld",
//            (current_time - last_update_time) * 10000 / CLOCKS_PER_SEC);
//   mvprintw(10, 24, "ACTION:%s\tSTATE:%s", getActionName(s->action),
//            getStateName(s->state));
//   //  draw_key(ch);  // del
//   refresh();
// }

bool tet_timer(clock_t *current_time, clock_t *last_update_time, int speed) {
  return (((*current_time - *last_update_time) * 10000 / CLOCKS_PER_SEC) >=
          speed);
}
void check_state() {
  Singleton *s = get_instance();
  transitionState(s->action);
  switch (s->state) {
    case SPAWN:
      init_piece();
      s->state = MOVING;
      break;
    case MOVING:
      move_piece_down();
      break;
    case ATTACHING:
      if (check_collision_end_game()) {
        s->state = GAME_OVER;
      } else {
        attach_piece_to_field();
        check_for_complete_lines();
        s->state = SPAWN;
      }
      break;
    default:
      break;
  }
}
int play_tetris(int ch) {
  Singleton * s =get_instance();
  s->test = (ch == 1) ? 0 : 1;
  clear();
  int menu_start_x = (WIDTH * 2) / 2 - 3;
  int menu_start_y = HEIGHT / 2 - 2 / 2;
  draw_board();
  int high_score = read_high_score();
  draw_score(0, high_score, 0);
  int x_cooord = 17;
  mvprintw(menu_start_y, menu_start_x - 1, "Tetris game");
  mvprintw(menu_start_y + 2, menu_start_x - 5, "Press \"S\" for Start");
  mvprintw(menu_start_y - 1, menu_start_x + x_cooord, "Press \"P\" for Pause");
  mvprintw(menu_start_y + 1, menu_start_x + x_cooord,
           "Press \"Down\" for Down");
  mvprintw(menu_start_y + 3, menu_start_x + x_cooord,
           "Press \"Left\" for Left");
  mvprintw(menu_start_y + 5, menu_start_x + x_cooord,
           "Press \"Right\" for Right");
  mvprintw(menu_start_y + 7, menu_start_x + x_cooord,
           "Press \"Space\" for Action");
  mvprintw(menu_start_y + 9, menu_start_x + x_cooord, "Press \"Q\" for Quit");
  while (ch != 'S' && ch != 's') {
    ch = GET_USER_INPUT;
  }
  return tetris_start();
}
void free_game_resources() {
  Singleton *s = get_instance();
  if (s->game.field != NULL) {
    for (int i = 0; i < HEIGHT; i++) {
      if (s->game.field[i] != NULL) {
        free(s->game.field[i]);
      }
    }
    free(s->game.field);
    s->game.field = NULL;
  }
  if (s->game.next != NULL) {
    for (int i = 0; i < 4; i++) {
      if (s->game.next[i] != NULL) {
        free(s->game.next[i]);
      }
    }
    free(s->game.next);
    s->game.next = NULL;
  }
}