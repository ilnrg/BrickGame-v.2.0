#define _GNU_SOURSE
#include <check.h>

#include "../brick_game/common/common_back.h"

START_TEST(setTime_test) {
  {
    long long current_time = setTime();
    time_t expected_time = time(NULL);
    ck_assert_int_lt(current_time, (expected_time + 1) * SECOND);
    ck_assert_int_gt(current_time, (expected_time - 1) * SECOND);
  }
  {
    long long current_time = setTime();
    long long expected_time = (time(NULL) * SECOND);
    ck_assert_int_lt(current_time, expected_time + SECOND);
    ck_assert_int_gt(current_time, expected_time);
  }
}
END_TEST

START_TEST(setSpeed_test) {
  {
    GameInfo_t stats;
    stats.level = 1;
    setSpeed(&stats.level, &stats.speed);
    ck_assert_int_eq(stats.speed, START_SPEED);
  }
  {
    GameInfo_t stats;
    stats.level = 3;
    setSpeed(&stats.level, &stats.speed);
    ck_assert_double_eq(stats.speed,
                        START_SPEED * (1 - (stats.level - 1) * 0.1));
  }
  {
    GameInfo_t stats;
    stats.level = 10;
    setSpeed(&stats.level, &stats.speed);
    ck_assert_double_eq(stats.speed, START_SPEED * 0.05);
  }
}
END_TEST

Suite *test_suite() {
  Suite *s = suite_create("common_back_tests");
  TCase *test = tcase_create("common_back_tests");

  tcase_add_test(test, setTime_test);
  tcase_add_test(test, setSpeed_test);

  suite_add_tcase(s, test);
  return s;
}

int main() {
  printf("\nSTARTING COMMON BACKEND FUNCS TESTS\n");
  Suite *s;
  s = test_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int nf = srunner_ntests_failed(sr);
  srunner_free(sr);
  printf("COMMON BACKEND FUNCS TESTS ARE OVER\n\n");
  return nf == 0 ? 0 : 1;
}