#include <unity.h>

void test_dummy_ok() {
  TEST_ASSERT_TRUE(true);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_dummy_ok);
  UNITY_END();
}

void loop() {}