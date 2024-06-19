#include <ncurses.h>

#include "../../brick_game/tetris/tetris.h"
#include "frontend.h"

int main() {

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  cbreak();
  curs_set(0); // ?
  main_menu_init();
  endwin();
  return 0;
}
