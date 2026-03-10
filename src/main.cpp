#include <Arduino.h>
#include "dy50.h"
#include "access_control.h"
#include <Adafruit_Fingerprint.h>

static const int PIN_DY50_RX = 16;    // RX2 (ESP32 DevKit V1)
static const int PIN_DY50_TX = 17;    // TX2 (ESP32 DevKit V1)
static const int PIN_RELE    = 18;    // D18 (ESP32 DevKit V1)
static const int PIN_TOUCH_OUT = 21;  // D21 (ESP32 DevKit V1)

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
Dy50 sensor;
AccessControl ac;

bool enrollFingerprint(int id) {
  Serial.print("Enroll start id=");
  Serial.println(id);

  // 1) Esperar imagen
  while (finger.getImage() != FINGERPRINT_OK) {
    delay(50);
  }
  Serial.println("Image taken");

  // 2) Convertir imagen
  if (finger.image2Tz(1) != FINGERPRINT_OK) {
    Serial.println("image2Tz(1) failed");
    return false;
  }
  Serial.println("Image converted (slot 1)");

  // 3) Pedir que quite el dedo
  Serial.println("Remove finger");
  delay(1500);
  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(50);
  }

  // 4) Segunda imagen
  Serial.println("Place same finger again");
  while (finger.getImage() != FINGERPRINT_OK) {
    delay(50);
  }
  Serial.println("Image taken (2)");

  // 5) Convertir segunda imagen
  if (finger.image2Tz(2) != FINGERPRINT_OK) {
    Serial.println("image2Tz(2) failed");
    return false;
  }
  Serial.println("Image converted (slot 2)");

  // 6) Crear modelo
  if (finger.createModel() != FINGERPRINT_OK) {
    Serial.println("createModel failed (finger mismatch?)");
    return false;
  }
  Serial.println("Model created");

  // 7) Guardar
  if (finger.storeModel(id) != FINGERPRINT_OK) {
    Serial.println("storeModel failed");
    return false;
  }
  Serial.println("Stored!");

  return true;
}

bool matchFingerprint(uint16_t &outId) {
  // Espera imagen
  if (finger.getImage() != FINGERPRINT_OK) {
    return false;
  }
  // Convierte imagen
  if (finger.image2Tz() != FINGERPRINT_OK) {
    return false;
  }
  // Busca coincidencia
  if (finger.fingerSearch() != FINGERPRINT_OK) {
    return false;
  }

  outId = finger.fingerID;
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TOUCH_OUT, INPUT_PULLDOWN);

  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);

  Serial2.begin(57600, SERIAL_8N1, PIN_DY50_RX, PIN_DY50_TX);

  finger.begin(57600);
  delay(100);

  if (finger.verifyPassword()) {
    Serial.println("DY50 OK: verifyPassword() true");
  } else {
    Serial.println("DY50 FAIL: verifyPassword() false");
  }

  sensor.begin();
  ac.begin();

  Serial.println("Comandos: e<id> enroll | d<id> delete | m match");
}

void loop() {
  static int lastTouch = 0;
  int touch = digitalRead(PIN_TOUCH_OUT);

  // Flanco ascendente: cuando pasa de 0 a 1
  if (touch == 1 && lastTouch == 0) {
    Serial.println("TOUCH detectado");
  }

  lastTouch = touch;

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.length() >= 2 && cmd[0] == 'e') {
    int id = cmd.substring(1).toInt();
    Serial.print("CMD enroll id=");
    Serial.println(id);
    bool ok = enrollFingerprint(id);
    Serial.println(ok ? "ENROLL OK" : "ENROLL FAIL");
    } else if (cmd == "m") {
    Serial.println("CMD match");
    uint16_t id = 0;
    bool ok = matchFingerprint(id);
    if (ok) {
      Serial.print("MATCH OK id=");
      Serial.println(id);
    } else {
      Serial.println("MATCH FAIL");
    }
  }
}

  delay(20);
}