#include "frontend.h"

void main_menu_init() {
  clear();
  draw_board();
  draw_score(0, 0, 0);
  int choice = 0;
  int ch;
  char *choices[] = {"Tetris", "Quit"};
  int n_choices = sizeof(choices) / sizeof(char *);

  while (true) {
    int menu_start_x = (WIDTH * 2) / 2 - 3;
    int menu_start_y = HEIGHT / 2 - n_choices / 2;
    for (int i = 0; i < n_choices; ++i) {
      if (i == choice)
        attron(A_REVERSE);
      mvprintw(menu_start_y + i, menu_start_x, "%s", choices[i]);
      if (i == choice)
        attroff(A_REVERSE);
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
        play_tetris();
      } else if (choice == 1) {
        exitProgram();
      }
      return;
    }
  }
}

void play_tetris() {
  clear();
  int menu_start_x = (WIDTH * 2) / 2 - 3;
  int menu_start_y = HEIGHT / 2 - 2 / 2;
  draw_board();
  draw_score(0, 0, 0);

  mvprintw(menu_start_y, menu_start_x - 1, "Tetris game");
  mvprintw(menu_start_y + 1, menu_start_x - 5, "Press \"S\" for Start");

  int ch = 0;
  while (ch != 'S' && ch != 's') {
    ch = GET_USER_INPUT;
    //  draw_key(ch);  // del
  }
  tetris_start();
}

void exitProgram() {
  Singleton *s = get_instance();
  if (s->game.field || s->game.next)
    free_game_resources();
  free_singleton();
  clear();
  endwin();
  exit(0);
}

void update_field(GameInfo_t game) {
  Singleton *s = get_instance();
  clear();
  draw_board();
  draw_field(game);
  draw_score(game.score, game.high_score, game.level);
  draw_next(s->game);
  draw_piece(s->current_piece);
  refresh();
}
void draw_field(GameInfo_t game_curr) {
  //  if (game.field)
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (game_curr.field[y][x] == 0) {
        mvprintw(y + 1, x * 2 + 1, ".");
      } else {
        mvprintw(y + 1, x * 2 + 1, "[]");
      }
    }
  }
  // else
  // for (int y = 0; y < HEIGHT; y++) {
  //       for (int x = 0; x < WIDTH; x++) {
  //           mvprintw(y + 1, x * 2 + 1, ".");
  //       }
  //   }
  refresh();
}

void draw_score(int score, int high_score, int level) {
  // Singleton* s = get_instance();
  int info_start_x = WIDTH * 2 + 4;
  // if (high_score < score) high_score = score;
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
  int base_x = WIDTH + 2; // Пример координаты x для отображения следующей
                          // фигуры (справа от игрового поля)
  int base_y =
      7; // Пример координаты y для отображения следующей фигуры (вверху экрана)

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
  if (ch != -1)
    mvprintw(8, info_start_x, "CHAR:%c INT %d", ch, ch);
  refresh();
}
void game_over_menu() {
  clear();
  int menu_start_x = (WIDTH * 2) / 2 - 3;
  int menu_start_y = HEIGHT / 2 - 2 / 2;
  draw_board();
  draw_score(0, 0, 0);
  mvprintw(menu_start_y, menu_start_x - 1, "GAME OVER");
  mvprintw(menu_start_y + 1, menu_start_x - 2, "Press \"Space\"");
  int ch = 0;
  while (ch != 32 && ch != 32) {
    ch = GET_USER_INPUT;
  }
  main_menu_init();
}