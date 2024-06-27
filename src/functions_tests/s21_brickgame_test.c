#include "s21_brickgame_test.h"

START_TEST(s21_brick_game_test_1) {
  int res = play_tetris('S');
  ck_assert_int_eq(0, res);
}
END_TEST

START_TEST(s21_brick_game_test_2) {
  int ch1 = KEY_LEFT;
  int ch2 = KEY_RIGHT;
  int ch3 = KEY_DOWN;
  int ch4 = ' ';
  int ch5 = 'p';
  int ch6 = 's';
  int ch7 = 'q';
  int pocket = '\0';
  bool hold = false;
  UserAction_t test_KEY_LEFT = (keyboard_action(&ch1, &pocket, &hold, true));
  ck_assert_int_eq(Left, test_KEY_LEFT);
  UserAction_t test_KEY_RIGHT = (keyboard_action(&ch2, &pocket, &hold, true));
  ck_assert_int_eq(Right, test_KEY_RIGHT);
  UserAction_t test_KEY_DOWN = (keyboard_action(&ch3, &pocket, &hold, true));
  ck_assert_int_eq(Down, test_KEY_DOWN);
  UserAction_t test_KEY_ACT = (keyboard_action(&ch4, &pocket, &hold, true));
  ck_assert_int_eq(Action, test_KEY_ACT);
  UserAction_t test_KEY_PAUSE = (keyboard_action(&ch5, &pocket, &hold, true));
  ck_assert_int_eq(Pause, test_KEY_PAUSE);
  UserAction_t test_KEY_START = (keyboard_action(&ch6, &pocket, &hold, true));
  ck_assert_int_eq(Start, test_KEY_START);
  UserAction_t test_KEY_QUIT = (keyboard_action(&ch7, &pocket, &hold, true));
  ck_assert_int_eq(Terminate, test_KEY_QUIT);
}
END_TEST
START_TEST(s21_brick_game_test_3) {
  Singleton *s = get_instance();
  s->current_piece.y = 5;  // Исходная позиция
  move_piece_down();
  ck_assert_int_eq(s->current_piece.y, 6);
}
END_TEST
START_TEST(s21_brick_game_test_4) {
  Singleton *s = get_instance();
  s->current_piece.x = 5;  // Исходная позиция
  s->state = MOVING;
  move_piece_left();
  ck_assert_int_eq(s->current_piece.x, 4);
}
END_TEST
START_TEST(s21_brick_game_test_5) {
  Singleton *s = get_instance();
  s->current_piece.x = 5;  // Исходная позиция
  s->state = MOVING;
  move_piece_right();
  ck_assert_int_eq(s->current_piece.x, 6);
}
END_TEST

START_TEST(s21_brick_game_test_6) {
  GameInfo_t game_info = updateCurrentState();
  ck_assert_int_eq(game_info.score, 0);
}
END_TEST
START_TEST(s21_brick_game_test_7) {
  int res = initialize_game();
  ck_assert_int_eq(0, res);
}
END_TEST

START_TEST(s21_brick_game_test_8) {
  Singleton *s = get_instance();
  initialize_game();
  s->state = SPAWN;
  GameInfo_t game_info = updateCurrentState();
  ck_assert_int_eq(game_info.score, 0);
  s->state = MOVING;
  game_info = updateCurrentState();
  ck_assert_int_eq(game_info.score, 0);
  s->state = ATTACHING;
  game_info = updateCurrentState();
  ck_assert_int_eq(game_info.score, 0);
}
END_TEST

START_TEST(s21_brick_game_test_9) { rotate_piece(); }
END_TEST
START_TEST(s21_brick_game_test_10) {
  bool collision = check_collision();
  ck_assert_int_eq(collision, false);
}
END_TEST
START_TEST(s21_brick_game_test_11) {
  bool collision_end_game = check_collision_end_game();
  ck_assert_int_eq(collision_end_game, false);
}
END_TEST
START_TEST(s21_brick_game_test_12) { attach_piece_to_field(); }

START_TEST(s21_brick_game_test_13) {
  Singleton *s = get_instance();
  initialize_game();
  s->action = Left;
  userInput(s->action, false);
  ck_assert_int_eq(3, s->action);
  s->action = Right;
  userInput(s->action, false);
  ck_assert_int_eq(4, Right);
  s->state = MOVING;
  s->action = Down;
  userInput(s->action, false);
  ck_assert_int_eq(6, Down);
  s->action = Up;
  userInput(s->action, false);
  ck_assert_int_eq(5, Up);
  s->action = Action;
  userInput(s->action, false);
  ck_assert_int_eq(7, Action);
  s->action = Pause;
  userInput(s->action, false);
  ck_assert_int_eq(1, Pause);
  s->action = Start;
  userInput(s->action, false);
  ck_assert_int_eq(0, Start);
  s->action = Terminate;
  userInput(s->action, false);
  ck_assert_int_eq(2, Terminate);
}
END_TEST
START_TEST(s21_brick_game_test_14) {
  initialize_game();
  clear_lines(4);
}
END_TEST

START_TEST(s21_brick_game_test_15) {
  Singleton *s = get_instance();
  initialize_game();
  initialize_piece();
  for (int i = 4; i < HEIGHT && s->game.field; i++) {
    for (int j = 0; j < WIDTH; j++) {
      s->game.field[i][j] = 1;
    }
  }
  s->state = MOVING;
  s->current_piece.type = 2;
  rotate_piece();
}
END_TEST
START_TEST(s21_brick_game_test_16) {
  int src[4][4] = {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int dest[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int check[4][4] = {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};
  copy_shape(dest, src);
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      ck_assert_int_eq(dest[y][x], check[y][x]);
    }
  }
}
END_TEST
START_TEST(s21_brick_game_test_17) {
  Singleton s;
  s.current_piece.x = 0;
  s.current_piece.y = 0;
  bool result = attempt_move(&s, 1, 1);

  ck_assert_int_eq(result, true);
  ck_assert_int_eq(s.current_piece.x, 1);
  ck_assert_int_eq(s.current_piece.y, 1);
}
END_TEST

Suite *suite(void) {
  Suite *suite = suite_create("\033[32m s21_brick_game_test \033[0m");
  TCase *tcase = tcase_create("core");

  tcase_add_test(tcase, s21_brick_game_test_1);
  tcase_add_test(tcase, s21_brick_game_test_2);
  tcase_add_test(tcase, s21_brick_game_test_3);
  tcase_add_test(tcase, s21_brick_game_test_4);
  tcase_add_test(tcase, s21_brick_game_test_5);
  tcase_add_test(tcase, s21_brick_game_test_6);
  tcase_add_test(tcase, s21_brick_game_test_7);
  tcase_add_test(tcase, s21_brick_game_test_8);
  tcase_add_test(tcase, s21_brick_game_test_9);
  tcase_add_test(tcase, s21_brick_game_test_10);
  tcase_add_test(tcase, s21_brick_game_test_11);
  tcase_add_test(tcase, s21_brick_game_test_12);
  tcase_add_test(tcase, s21_brick_game_test_13);
  tcase_add_test(tcase, s21_brick_game_test_14);
  tcase_add_test(tcase, s21_brick_game_test_15);
  tcase_add_test(tcase, s21_brick_game_test_16);
  tcase_add_test(tcase, s21_brick_game_test_17);

  suite_add_tcase(suite, tcase);
  return suite;
}

void case_test_runner(Suite *suite, int *fail_count) {
  SRunner *suite_runner = srunner_create(suite);
  srunner_run_all(suite_runner, CK_NORMAL);
  *fail_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
}

int main(void) {
  int fail_count = 0;
  case_test_runner(suite(), &fail_count);
  free_game_resources();
  free_singleton();
  return fail_count;
}

void initialize_piece() {
  Singleton *s = get_instance();
  // Пример начальной фигуры (например, I-образная фигура)
  int shape[4][4] = {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      s->current_piece.shape[y][x] = shape[y][x];
    }
  }
}