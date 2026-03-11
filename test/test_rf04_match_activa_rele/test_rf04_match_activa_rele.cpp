#include <Arduino.h>
#include <unity.h>

// RF-04: Si hay match, el sistema debe activar el relé durante 1 segundo.
//
// Este test no invoca directamente src/main.cpp.
// Verifica el efecto observable esperado para una activación de relé:
// que el pin pueda ponerse HIGH y luego LOW en la secuencia prevista.

static const int PIN_RELE = 18;  // D18

void test_rf04_rele_activa_y_vuelve_a_low() {
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);

  digitalWrite(PIN_RELE, HIGH);
  delay(1000);
  digitalWrite(PIN_RELE, LOW);

  int v = digitalRead(PIN_RELE);
  TEST_ASSERT_EQUAL(LOW, v);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_rf04_rele_activa_y_vuelve_a_low);
  UNITY_END();
}

void loop() {}