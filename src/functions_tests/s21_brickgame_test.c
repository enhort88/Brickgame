#include"s21_brickgame_test.h"

START_TEST(s21_brick_game_test_1) {
  int res = play_tetris('S');
  ck_assert_int_eq(0, res);
}
END_TEST

Suite *suite(void) {
  Suite *suite = suite_create("\033[32m s21_brick_game_test \033[0m");
  TCase *tcase = tcase_create("core");

  tcase_add_test(tcase, s21_brick_game_test_1);
  

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

  return fail_count;
}
