#include <Arduino.h>
#include "dy50.h"
#include "access_control.h"

static const int PIN_DY50_RX = 16;    // RX2 (ESP32 DevKit V1)
static const int PIN_DY50_TX = 17;    // TX2 (ESP32 DevKit V1)
static const int PIN_RELE    = 18;    // D18 (ESP32 DevKit V1)
static const int PIN_TOUCH_OUT = 21;  // D21 (ESP32 DevKit V1)

Dy50 sensor;
AccessControl ac;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TOUCH_OUT, INPUT_PULLDOWN);
  Serial1.begin(57600, SERIAL_8N1, PIN_DY50_RX, PIN_DY50_TX);
  sensor.begin();
  ac.begin();
}

void loop() {
  static int last = -1;
  int now = digitalRead(PIN_TOUCH_OUT);
  if (now != last) {
    Serial.print("TOUCH_OUT=");
    Serial.println(now);
    last = now;
  }
  if (Serial1.available()) {
    int b = Serial1.read();
   Serial.print("UART1 byte: 0x");
   if (b < 16) Serial.print("0");
    Serial.println(b, HEX);
  }  
  delay(20);
}