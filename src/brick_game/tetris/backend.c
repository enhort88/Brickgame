#include "backend.h"

#include "../../gui/cli/frontend.h"

static FSMState transitionMatrix[NUM_STATES][8] = {
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

int play_tetris(int ch) {
  Singleton *s = get_instance();
  s->test = (ch == 1) ? 0 : 1;
  clear();
  int menu_start_x = (WIDTH * 2) / 2 - 3;
  int menu_start_y = HEIGHT / 2 - 1;
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
  int res = tetris_start();
  if (s->test != 1) {
    game_over_menu();
  }
  free_game_resources();
  free_singleton();
  return res;
}

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
    userInput(keyboard_action(&ch, &pocket, &hold, false), hold);
    //    test_print(current_time, last_update_time);
    refresh();
    if (s->test == 1) s->state = GAME_OVER;
  }
  nodelay(stdscr, FALSE);
  return res;
}

GameInfo_t updateCurrentState() {
  Singleton *s = get_instance();
  int ch = '\0';
  bool hold = false;
  int pocket = '\0';
  s->action = keyboard_action(&ch, &pocket, &hold, false);
  userInput(s->action, hold);
  if (s->action > 0 && s->action < 8) {
    s->state = transitionMatrix[s->state][s->action];
  }
  switch (s->state) {
    case SPAWN:
      init_piece();
      s->state = MOVING;
      break;
    case MOVING:
      move_piece_down();
      //      s->state = MOVING;
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

bool tet_timer(clock_t const *current_time, clock_t const *last_update_time,
               int speed) {
  return (((*current_time - *last_update_time) * 10000 / CLOCKS_PER_SEC) >=
          speed);
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
void init_piece() {
  Singleton *s = get_instance();
  struct timespec tv;
  clock_gettime(CLOCK_MONOTONIC, &tv);
  unsigned int seed = tv.tv_sec * 1000000 + tv.tv_nsec / 1000;
  srand(seed);
  // Инициализация новой фигуры в верхней части экрана
  s->current_piece.x = WIDTH / 2 - 2;
  s->current_piece.y = 0;
  // Определение различных форм фигур
  int shapes[7][4][4] = SHAPES;
  // Инициализация генератора случайных чисел
  srand(time(NULL));
  int shape_next = rand() % 7;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      s->game.next[i][j] = shapes[shape_next][i][j];
    }
  }
  if (s->shape_curr == -1) {
    s->shape_curr = rand() % 7;
  }
  memcpy(s->current_piece.shape, shapes[s->shape_curr],
         sizeof(shapes[s->shape_curr]));
  s->current_piece.type = s->shape_curr;
  s->shape_curr = shape_next;
}

void move_piece_down() {
  Singleton *s = get_instance();
  s->current_piece.y++;
  if (check_collision()) {
    s->current_piece.y--;
    s->state = ATTACHING;
  }
}
void move_piece_left() {
  Singleton *s = get_instance();
  if (s->state == MOVING) {
    s->current_piece.x--;
    if (check_collision()) {
      s->current_piece.x++;
    }
  }
}

void move_piece_right() {
  Singleton *s = get_instance();
  if (s->state == MOVING) {
    s->current_piece.x++;
    if (check_collision()) {
      s->current_piece.x--;
    }
  }
}

void move_piece_up() {}

void copy_shape(int dest[4][4], int src[4][4]) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      dest[x][y] = src[y][x];
    }
  }
}

void rotate_temp_shape(int shape[4][4], int temp_shape[4][4]) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      shape[x][y] = temp_shape[x][3 - y];
    }
  }
}

bool attempt_move(Singleton *s, int dx, int dy) {
  s->current_piece.x += dx;
  s->current_piece.y += dy;
  if (!check_collision()) {
    return true;
  }
  s->current_piece.x -= dx;
  s->current_piece.y -= dy;
  return false;
}

bool resolve_collision(Singleton *s, int temp_shape[4][4]) {
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (attempt_move(s, dx, dy)) {
        return true;
      }
    }
  }

  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      if (s->current_piece.shape[y][x] != 0) {
        if (s->current_piece.x + x < 0) s->current_piece.x++;
        if (s->current_piece.x + x >= WIDTH) s->current_piece.x--;
        if (s->current_piece.y + y >= HEIGHT) s->current_piece.y--;
      }
    }
  }

  for (int dx = -2; dx <= 2; dx++) {
    if (attempt_move(s, dx, 0)) {
      return true;
    }
  }

  copy_shape(s->current_piece.shape, temp_shape);
  return false;
}

void rotate_piece() {
  Singleton *s = get_instance();
  if (s->state != MOVING || s->current_piece.type == 1) return;
  int temp_shape[4][4] = {0};
  copy_shape(temp_shape, s->current_piece.shape);
  rotate_temp_shape(s->current_piece.shape, temp_shape);

  if (check_collision() && !resolve_collision(s, temp_shape)) {
    copy_shape(s->current_piece.shape, temp_shape);
  }
}

bool check_collision() {
  Singleton *s = get_instance();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (s->current_piece.shape[y][x] != 0) {
        int new_y = s->current_piece.y + y;
        int new_x = s->current_piece.x + x;
        if (new_y >= HEIGHT || new_x < 0 || new_x >= WIDTH ||
            s->game.field[new_y][new_x] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}
bool check_collision_end_game() {
  Singleton *s = get_instance();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (s->current_piece.shape[y][x] != 0) {
        int new_y = s->current_piece.y + y;
        int new_x = s->current_piece.x + x;
        if (new_y >= HEIGHT || new_x < 0 || new_x >= WIDTH) {
          continue;
        }
        if (s->game.field[new_y][new_x] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}
void attach_piece_to_field() {
  Singleton *s = get_instance();
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (s->current_piece.shape[y][x] != 0) {
        int field_y = s->current_piece.y + y;
        int field_x = s->current_piece.x + x;
        if (field_y >= 0 && field_y < HEIGHT && field_x >= 0 &&
            field_x < WIDTH) {
          s->game.field[field_y][field_x] = s->current_piece.shape[y][x];
        }
      }
    }
  }
}

void check_for_complete_lines() {
  Singleton *s = get_instance();
  int lines_cleared = 0;
  for (int y = 0; y < HEIGHT; y++) {
    bool complete = true;
    for (int x = 0; x < WIDTH; x++) {
      if (s->game.field[y][x] == 0) {
        complete = false;
        break;
      }
    }
    if (complete) {
      clear_lines(y);
      lines_cleared++;
      y--;
    }
  }
  s->game.score += lines_cleared_score(lines_cleared);
  s->state = SPAWN;
}
int lines_cleared_score(int lines_cleared) {
  switch (lines_cleared) {
    case 1:
      return 100;
      break;
    case 2:
      return 300;
      break;
    case 3:
      return 700;
      break;
    case 4:
      return 1500;
      break;
    default:
      return 0;
  }
}

void clear_lines(int line) {
  Singleton *s = get_instance();
  for (int y = line; y > 0; y--) {
    for (int x = 0; x < WIDTH; x++) {
      s->game.field[y][x] = s->game.field[y - 1][x];
    }
  }
  for (int x = 0; x < WIDTH; x++) {
    s->game.field[0][x] = 0;
  }
}
void change_speed() {
  Singleton *s = get_instance();
  int const speed_levels[] = {7200, 6400, 5600, 4800, 4000,
                              3200, 2400, 1600, 800,  200};
  int const score_thresholds[] = {600,  1200, 1800, 2400, 3000,
                                  3600, 4200, 4800, 5400, 6000};
  int num_levels = sizeof(speed_levels) / sizeof(speed_levels[0]);
  for (int i = num_levels - 1; i >= 0; i--) {
    if (s->game.score >= score_thresholds[i]) {
      s->game.speed = speed_levels[i];
      s->game.level = i + 1;
      break;
    }
  }
}
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
      while (s->state == MOVING) move_piece_down();
      update_field(s->game);
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
      if (s->state == GAME_OVER) {
        free_game_resources();
        free_singleton();
        tetris_start();
      }
      break;
    case Terminate:
      s->state = GAME_OVER;
      break;
  }
}

UserAction_t keyboard_action(int *ch, int *pocket, bool *hold, bool test) {
  Singleton *s = get_instance();
  if (!test) *ch = GET_USER_INPUT;
  *hold = (*pocket == *ch) ? true : false;
  *pocket = *ch;
  if (s->test == 1) {
    *ch = KEY_DOWN;
    s->game.speed = 0;
  }
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
// const char *getActionName(UserAction_t action) {
//   switch (action) {
//     case Start:
//       return "Start";
//     case Pause:
//       return "Pause";
//     case Terminate:
//       return "Terminate";
//     case Left:
//       return "Left";
//     case Right:
//       return "Right";
//     case Up:
//       return "Up";
//     case Down:
//       return "Down";
//     case Action:
//       return "Action";
//     default:
//       return "UNKNOWN";
//   }
// }
// const char *getStateName(FSMState state) {
//   switch (state) {
//     case START:
//       return "START";
//     case SPAWN:
//       return "SPAWN";
//     case MOVING:
//       return "MOVING";
//     case SHIFTING:
//       return "SHIFTING";
//     case ATTACHING:
//       return "ATTACHING";
//     case GAME_OVER:
//       return "GAME_OVER";
//       // case PAUSE: return "PAUSE";
//     default:
//       return "UNKNOWN";
//   }
// }
