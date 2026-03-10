#include <Arduino.h>
#include <unity.h>

// RF-05: Si no hay coincidencia, no debe activarse el relé.
// En este test simulamos "no match" como: no hay dedo colocado durante la ventana de prueba.
// Entonces el firmware no debería activar el relé (no debería ponerse HIGH).

static const int PIN_RELE = 18;  // D18
static const int OBS_MS = 2000;  // ventana de observación

void test_rf05_rele_permanece_low_sin_dedo() {
  pinMode(PIN_RELE, INPUT);
  unsigned long t0 = millis();
  while (millis() - t0 < OBS_MS) {
    int v = digitalRead(PIN_RELE);
    if (v == HIGH) {
      TEST_FAIL_MESSAGE("Relé en HIGH sin match/dedo");
      return;
    }
    delay(10);
  }
  TEST_PASS();
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_rf05_rele_permanece_low_sin_dedo);
  UNITY_END();
}

void loop() {}