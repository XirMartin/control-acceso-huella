#include <Adafruit_Fingerprint.h>
#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "access_control.h"
#include "dy50.h"
#include "secrets.h"

static const int PIN_DY50_RX = 16;    // RX2 (ESP32 DevKit V1)
static const int PIN_DY50_TX = 17;    // TX2 (ESP32 DevKit V1)
static const int PIN_RELE = 18;       // D18 (ESP32 DevKit V1)
static const int PIN_TOUCH_OUT = 21;  // D21 (ESP32 DevKit V1)

static const bool TEST_FORCE_MATCH = false;
static const bool AUTO_MATCH_ENABLED = true;
bool autoMatchArmed = true;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
Dy50 sensor;
AccessControl ac;
WebServer server(80);

String enrollStatus = "Idle";
bool enrollRequested = false;
int enrollRequestedId = 0;

int enrollStep = 0;
bool enrollInProgress = false;
unsigned long enrollStepTs = 0;

bool enrollFingerprint(int id) {
  Serial.print("Enroll start id=");
  Serial.println(id);
  enrollStatus = "Esperando primer dedo";

  // 1) Esperar imagen
  while (finger.getImage() != FINGERPRINT_OK) {
    delay(50);
  }
  Serial.println("Image taken");
  enrollStatus = "Primera imagen tomada";

  // 2) Convertir imagen
  if (finger.image2Tz(1) != FINGERPRINT_OK) {
    Serial.println("image2Tz(1) failed");
    return false;
  }
  Serial.println("Image converted (slot 1)");

  // 3) Pedir que quite el dedo
  Serial.println("Remove finger");
  enrollStatus = "Retire el dedo";

  delay(1500);
  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(50);
  }

  // 4) Segunda imagen
  Serial.println("Place same finger again");
  enrollStatus = "Coloque el mismo dedo nuevamente";
  while (finger.getImage() != FINGERPRINT_OK) {
    delay(50);
  }
  Serial.println("Image taken (2)");
  enrollStatus = "Segunda imagen tomada";

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
  enrollStatus = "Modelo creado";

  // 7) Guardar
  if (finger.storeModel(id) != FINGERPRINT_OK) {
    Serial.println("storeModel failed");
    enrollStatus = "Error al guardar la huella";
    return false;
  }
  Serial.println("Stored!");
  enrollStatus = "Enrolado exitoso";

  return true;
}

bool matchFingerprint(uint16_t& outId) {
  if (TEST_FORCE_MATCH) {
    outId = 999;
    return true;
  }

  // Esperar a que haya dedo (bloqueante, como en enroll)
  while (finger.getImage() != FINGERPRINT_OK) {
    delay(50);
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

bool checkAuth() {
  if (!server.authenticate(WEB_USER, WEB_PASS)) {
    server.requestAuthentication();
    return false;
  }
  return true;
}

void handleRoot() {
  if (!checkAuth()) return;

  String html;
  html += "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<title>Control de acceso</title></head><body>";
  html += "<h1>Control de acceso activo</h1>";
  html += "<p>ID: <input id='fid' type='number' min='1' value='1'></p>";
  html +=
      "<p><button onclick=\"window.lastEnrollClick=Date.now(); "
      "document.getElementById('status').innerText='Solicitud de enroll enviada'; "
      "fetch('/enroll?id='+document.getElementById('fid').value)\">Enroll ID indicado</button></p>";
  html += "<p>Estado enroll: <span id='status'>Idle</span></p>";
  html +=
      "<p><button "
      "onclick=\"fetch('/"
      "delete?id='+document.getElementById('fid').value).then(r=>r.text()).then(t=>alert(t))\">"
      "Borrar ID indicado"
      "</button></p>";
  html +=
      "<p><button onclick=\"fetch('/match').then(r=>r.text()).then(t=>alert(t))\">Probar "
      "match</button></p>";
  html += "<script>";
  html += "window.lastEnrollClick=0;";
  html += "setInterval(async()=>{";
  html += "  if (Date.now()-window.lastEnrollClick < 3000) return;";
  html += "  const r = await fetch('/enroll-status');";
  html += "  const t = await r.text();";
  html += "  document.getElementById('status').innerText = t;";
  html += "}, 1000);";
  html += "</script>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleEnroll() {
  if (!checkAuth()) return;

  if (!server.hasArg("id")) {
    server.send(400, "text/plain", "Falta parametro id");
    return;
  }

  int id = server.arg("id").toInt();

  enrollRequestedId = id;
  enrollRequested = true;
  enrollInProgress = true;
  enrollStep = 1;
  enrollStepTs = millis();
  enrollStatus = "Solicitud de enroll enviada";

  server.send(200, "text/plain", "ENROLL START id=" + String(id));
}

void handleEnrollStatus() {
  if (!checkAuth()) return;
  server.send(200, "text/plain", enrollStatus);
}

void handleMatch() {
  if (!checkAuth()) return;
  enrollStatus = "Esperando dedo para match";
  uint16_t id = 0;

  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(50);
  }
  bool ok = matchFingerprint(id);

  if (ok) {
    digitalWrite(PIN_RELE, HIGH);
    delay(1000);
    digitalWrite(PIN_RELE, LOW);
    server.send(200, "text/plain", "MATCH OK id=" + String(id));
  } else {
    server.send(200, "text/plain", "MATCH FAIL");
  }
}

void handleDelete() {
  if (!checkAuth()) return;

  if (!server.hasArg("id")) {
    server.send(400, "text/plain", "Falta parametro id");
    return;
  }

  int id = server.arg("id").toInt();
  int r = finger.deleteModel(id);

  if (r == FINGERPRINT_OK) {
    server.send(200, "text/plain", "DELETE OK id=" + String(id));
  } else {
    server.send(500, "text/plain", "DELETE FAIL id=" + String(id) + " code=" + String(r));
  }
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
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi OK. IP: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/enroll", handleEnroll);
  server.on("/enroll-status", handleEnrollStatus);
  server.on("/match", handleMatch);
  server.on("/delete", handleDelete);
  server.on("/favicon.ico", []() { server.send(204); });
  server.begin();
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
    } else if (cmd.length() >= 2 && cmd[0] == 'd') {
      int id = cmd.substring(1).toInt();
      Serial.print("CMD delete id=");
      Serial.println(id);
      int r = finger.deleteModel(id);
      if (r == FINGERPRINT_OK) {
        Serial.println("DELETE OK");
      } else {
        Serial.print("DELETE FAIL code=");
        Serial.println(r);
      }

    } else if (cmd == "m") {
      Serial.println("CMD match");
      uint16_t id = 0;
      bool ok = matchFingerprint(id);
      if (ok) {
        Serial.print("MATCH OK id=");
        Serial.println(id);
        digitalWrite(PIN_RELE, HIGH);
        delay(1000);
        digitalWrite(PIN_RELE, LOW);
      } else {
        Serial.println("MATCH FAIL");
      }
    }
  }

  if (enrollRequested) {
    enrollRequested = false;
  }

  if (enrollInProgress && enrollStep == 1) {
    enrollStatus = "Esperando primer dedo";
    if (finger.getImage() == FINGERPRINT_OK) {
      enrollStatus = "Primera imagen tomada";
      enrollStep = 2;
      enrollStepTs = millis();
    }
  }

  if (enrollInProgress && enrollStep == 2) {
    if (finger.image2Tz(1) == FINGERPRINT_OK) {
      enrollStatus = "Retire el dedo";
      enrollStep = 3;
      enrollStepTs = millis();
    } else {
      enrollStatus = "Error en primera conversion";
      enrollInProgress = false;
      enrollStep = 0;
    }
  }

  if (enrollInProgress && enrollStep == 3) {
    if (finger.getImage() == FINGERPRINT_NOFINGER) {
      enrollStatus = "Coloque el mismo dedo nuevamente";
      enrollStep = 4;
      enrollStepTs = millis();
    }
  }

  if (enrollInProgress && enrollStep == 4) {
    if (finger.getImage() == FINGERPRINT_OK) {
      enrollStatus = "Segunda imagen tomada";
      enrollStep = 5;
      enrollStepTs = millis();
    }
  }

  if (enrollInProgress && enrollStep == 5) {
    if (finger.image2Tz(2) == FINGERPRINT_OK) {
      enrollStatus = "Creando modelo";
      enrollStep = 6;
      enrollStepTs = millis();
    } else {
      enrollStatus = "Error en segunda conversion";
      enrollInProgress = false;
      enrollStep = 0;
    }
  }

  if (enrollInProgress && enrollStep == 6) {
    if (finger.createModel() == FINGERPRINT_OK) {
      enrollStatus = "Guardando huella";
      enrollStep = 7;
      enrollStepTs = millis();
    } else {
      enrollStatus = "Error al crear modelo";
      enrollInProgress = false;
      enrollStep = 0;
    }
  }

  if (enrollInProgress && enrollStep == 7) {
    if (finger.storeModel(enrollRequestedId) == FINGERPRINT_OK) {
      enrollStatus = "Enrolado exitoso";
    } else {
      enrollStatus = "Error al guardar la huella";
    }
    enrollInProgress = false;
    enrollStep = 0;
  }

  if (finger.getImage() == FINGERPRINT_NOFINGER) {
    autoMatchArmed = true;
  }

  if (AUTO_MATCH_ENABLED && autoMatchArmed && !enrollInProgress) {
    uint16_t id = 0;
    if (matchFingerprint(id)) {
      autoMatchArmed = false;
      enrollStatus = "MATCH OK id=" + String(id);
      digitalWrite(PIN_RELE, HIGH);
      delay(1000);
      digitalWrite(PIN_RELE, LOW);
    }
  }

  server.handleClient();
  delay(20);
}