#include <ncurses.h>

#include "./brick_game/tetris/backend.h"
#include "./gui/cli/frontend.h"

int main() {
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  cbreak();
  curs_set(0);  
  //play_tetris('S');
  main_menu_init();
  endwin();
  return 0;
}
