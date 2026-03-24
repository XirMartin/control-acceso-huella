#include <unity.h>

#include "access_control.h"

void test_begin_habilita_automatch() {
  AccessControl ac;
  TEST_ASSERT_TRUE(ac.begin());
  TEST_ASSERT_TRUE(ac.canTryAutoMatch(false));
}

void test_no_intenta_match_si_hay_enroll_en_progreso() {
  AccessControl ac;
  ac.begin();
  TEST_ASSERT_FALSE(ac.canTryAutoMatch(true));
}

void test_match_exitoso_desarma_automatch() {
  AccessControl ac;
  ac.begin();

  TEST_ASSERT_TRUE(ac.handleMatchResult(true));
  TEST_ASSERT_FALSE(ac.canTryAutoMatch(false));
}

void test_match_fallido_no_desarma_automatch() {
  AccessControl ac;
  ac.begin();

  TEST_ASSERT_FALSE(ac.handleMatchResult(false));
  TEST_ASSERT_TRUE(ac.canTryAutoMatch(false));
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_begin_habilita_automatch);
  RUN_TEST(test_no_intenta_match_si_hay_enroll_en_progreso);
  RUN_TEST(test_match_exitoso_desarma_automatch);
  RUN_TEST(test_match_fallido_no_desarma_automatch);
  UNITY_END();
}

void loop() {}