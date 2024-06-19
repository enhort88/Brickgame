#include "tetris.h"

#include "../../gui/cli/frontend.h"

void tetris_start() {
  Singleton *s = get_instance();
  s->state = SPAWN;
  initialize_game();
  int ch = '\0';
  int pocket = '\0';
  bool hold = false;
  clock_t last_update_time;
  int update_interval = 8500;
  last_update_time = clock();
  nodelay(stdscr, TRUE);
  while (s->state != GAME_OVER) {
    clock_t current_time = clock();
    if (tet_timer(&current_time, &last_update_time, update_interval)) {
      if (!s->game.pause) {
        update_field(updateCurrentState());
      }
      last_update_time = current_time;
    }
    userInput(keyboard_action(&ch, &pocket, &hold), hold);
    //    test_print(current_time, last_update_time, ch);  // del after all
    refresh();
  }
  nodelay(stdscr, FALSE);
  if (s->game.field || s->game.next)
    free_game_resources();
  free_singleton();
  game_over_menu();
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
void initialize_game() {
  Singleton *s = get_instance();
  s->game.field = (int **)calloc(HEIGHT, sizeof(int *));
  if (!s->game.field) {
    perror("Failed to allocate memory for game.field");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < HEIGHT; i++) {
    s->game.field[i] = (int *)calloc(WIDTH, sizeof(int));
    if (!s->game.field[i]) {
      perror("Failed to allocate memory for game.field[i]");
      exit(EXIT_FAILURE);
    }
  }
  s->game.next = (int **)calloc(4, sizeof(int *));
  if (!s->game.next) {
    perror("Failed to allocate memory for game.next");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < 4; i++) {
    s->game.next[i] = (int *)calloc(4, sizeof(int));
    if (!s->game.next[i]) {
      perror("Failed to allocate memory for game.next[i]");
      exit(EXIT_FAILURE);
    }
  }
  s->game.score = 0;
  s->game.high_score = 0;
  s->game.level = 1;
  s->game.speed = 1;

  init_piece(); // Инициализация первой фигуры и следующей фигуры
}

void test_print(clock_t current_time, clock_t last_update_time, char ch) {
  Singleton *s = get_instance();

  mvprintw(19, 24, "Currtime: %ld", current_time);
  mvprintw(20, 24, "LastUpT: %ld", last_update_time);
  mvprintw(21, 24, "RES: %ld",
           (current_time - last_update_time) * 10000 / CLOCKS_PER_SEC);
  mvprintw(10, 24, "ACTION:%s\tSTATE:%s", getActionName(s->action),
           getStateName(s->state));
  //  draw_key(ch);  // del
  refresh();
}

void reset_game() { exitProgram(); }

bool tet_timer(clock_t *current_time, clock_t *last_update_time,
               int update_interval) {
  return (((*current_time - *last_update_time) * 10000 / CLOCKS_PER_SEC) >=
          update_interval);
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