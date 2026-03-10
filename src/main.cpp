#include <Arduino.h>
#include "dy50.h"
#include "access_control.h"

static const int PIN_DY50_RX = 16;
static const int PIN_DY50_TX = 17;
static const int PIN_RELE    = 18;

Dy50 sensor;
AccessControl ac;

void setup() {
  Serial.begin(115200);
  sensor.begin();
  ac.begin();
}

void loop() {
  delay(1000);
}