#include "frontend.h"

void main_menu_init() {
  clear();
  draw_board();
  int high_score = read_high_score();
  draw_score(0, high_score, 0);
  int choice = 0;
  int ch;
  char *choices[] = {"Tetris", "Quit"};
  int n_choices = sizeof(choices) / sizeof(char *);

  while (true) {
    int menu_start_x = (WIDTH * 2) / 2 - 3;
    int menu_start_y = HEIGHT / 2 - n_choices / 2;
    for (int i = 0; i < n_choices; ++i) {
      if (i == choice) attron(A_REVERSE);
      mvprintw(menu_start_y + i, menu_start_x, "%s", choices[i]);
      if (i == choice) attroff(A_REVERSE);
    }
    refresh();
    ch = GET_USER_INPUT;
    switch (ch) {
      case KEY_UP:
        choice = (choice == 0) ? n_choices - 1 : choice - 1;
        break;
      case KEY_DOWN:
        choice = (choice == n_choices - 1) ? 0 : choice + 1;
        break;
      case 10:
        if (choice == 0) {
          play_tetris(0);
        } else if (choice == 1) {
          return;
        }
    }
  }
}

int play_tetris(int ch) {
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

void update_field(GameInfo_t game) {
  Singleton *s = get_instance();
  clear();
  draw_board();
  draw_field(game);
  s->game.high_score = read_high_score();
  if (s->game.score > s->game.high_score) {
    write_high_score(s->game.score);
  }
  draw_score(game.score, game.high_score, game.level);
  draw_next(game);
  draw_piece(s->current_piece);
  refresh();
}
void draw_field(GameInfo_t game_curr) {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (game_curr.field[y][x] == 0) {
        mvprintw(y + 1, x * 2 + 1, ".");
      } else {
        mvprintw(y + 1, x * 2 + 1, "[]");
      }
    }
  }
  refresh();
}

void draw_score(int score, int high_score, int level) {
  int info_start_x = WIDTH * 2 + 4;
  mvprintw(0, info_start_x, "High Score:%d", high_score);
  mvprintw(2, info_start_x, "Score:%d", score);
  mvprintw(4, info_start_x, "Level:%d", level);

  refresh();
}

void draw_board() {
  for (int y = 0; y < HEIGHT + 2; y++) {
    for (int x = 0; x < WIDTH * 2 + 2; x++) {
      if (y == 0 || y == HEIGHT + 1 || x == 0 || x == WIDTH * 2 + 1) {
        mvprintw(y, x, "#");
      }
    }
  }
}
void draw_next(GameInfo_t game) {
  int base_x = WIDTH + 2;
  int base_y = 7;
  mvprintw(6, 24, "Next:");
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (game.next[y][x] != 0) {
        mvprintw(base_y + y + 1, (base_x + x) * 2 + 1, "[]");
      }
    }
  }
  refresh();
}

void draw_piece(Piece piece) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (piece.shape[y][x] != 0) {
        mvprintw(piece.y + y + 1, (piece.x + x) * 2 + 1, "[]");
      }
    }
  }
  refresh();
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
void draw_key(char ch) {
  int info_start_x = WIDTH * 2 + 4;
  if (ch != -1) mvprintw(8, info_start_x, "CHAR:%c INT %d", ch, ch);
  refresh();
}
void game_over_menu() {
  clear();
  int menu_start_x = (WIDTH * 2) / 2 - 3;
  int menu_start_y = HEIGHT / 2 - 2 / 2;
  draw_board();
  int high_score = read_high_score();
  draw_score(0, high_score, 0);
  mvprintw(menu_start_y, menu_start_x - 1, "GAME OVER");
  mvprintw(menu_start_y + 1, menu_start_x - 2, "Press \"Space\"");
  int ch = 0;
  while (ch != 32 && ch != 32) {
    ch = GET_USER_INPUT;
  }
  clear();
  draw_board();
  draw_score(0, high_score, 0);
  return;
}

int read_high_score() {
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  if (!file) {
    return 0;
  }

  int high_score = 0;
  fscanf(file, "%d", &high_score);
  fclose(file);

  return high_score;
}

void write_high_score(int high_score) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}