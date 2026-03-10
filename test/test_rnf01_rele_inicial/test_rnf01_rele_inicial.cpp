#include <Arduino.h>
#include <unity.h>

// Importante: este test corre "on-target" en el ESP32

// Debe coincidir con src/main.cpp
static const int PIN_RELE = 18;  // D18 (ESP32 DevKit V1)

void test_rnf01_rele_inicia_en_low() {
  pinMode(PIN_RELE, INPUT);  // leer estado sin forzar salida
  int v = digitalRead(PIN_RELE);

  // Aceptamos LOW como estado seguro.
  // Si el hardware invierte la lógica por transistor, esto se ajusta.
  TEST_ASSERT_EQUAL(LOW, v);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_rnf01_rele_inicia_en_low);
  UNITY_END();
}

void loop() {}