#include <unity.h>

#include "access_control.h"

void setUp(void) {}

void tearDown(void) {}

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

void test_estado_interno_refleja_automatch_armado_y_desarmado() {
  AccessControl ac;
  ac.begin();

  TEST_ASSERT_TRUE(ac.isAutoMatchArmed());

  ac.handleMatchResult(true);

  TEST_ASSERT_FALSE(ac.isAutoMatchArmed());
}

void test_armAutoMatch_vuelve_a_armar_el_estado() {
  AccessControl ac;
  ac.begin();

  ac.handleMatchResult(true);
  TEST_ASSERT_FALSE(ac.isAutoMatchArmed());

  ac.armAutoMatch();
  TEST_ASSERT_TRUE(ac.isAutoMatchArmed());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_begin_habilita_automatch);
  RUN_TEST(test_no_intenta_match_si_hay_enroll_en_progreso);
  RUN_TEST(test_match_exitoso_desarma_automatch);
  RUN_TEST(test_match_fallido_no_desarma_automatch);
  RUN_TEST(test_estado_interno_refleja_automatch_armado_y_desarmado);
  RUN_TEST(test_armAutoMatch_vuelve_a_armar_el_estado);
  return UNITY_END();
}