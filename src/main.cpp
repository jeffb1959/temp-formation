#include <Arduino.h>

#include "DS18B20Sensor.h"
#include "LocalDisplay.h"
#include "TemperatureStatus.h"
#include "TelemetryData.h"

constexpr uint8_t kSensorPin = D5;  // D5 / GPIO6
constexpr unsigned long kReadIntervalMs = 10000;
constexpr uint16_t kReadIntervalS = 10;
constexpr const char* kDeviceId = "prototype_temp_01";
constexpr const char* kDeviceName = "Prototype temp";
constexpr const char* kSensorType = "DS18B20";

DS18B20Sensor temperatureSensor(kSensorPin);
TemperatureStatus localStatus;
LocalDisplay localDisplay;
bool sensorFound = false;
bool displayReady = false;
unsigned long lastReadMs = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("\n=== Demarrage du programme ===");
  Serial.print("GPIO utilise pour le DS18B20: ");
  Serial.println(temperatureSensor.getPin());

  Serial.println("Initialisation du capteur DS18B20...");
  sensorFound = temperatureSensor.begin();
  if (!sensorFound) {
    Serial.println("Erreur: aucune sonde DS18B20 detectee.");
    Serial.println(
        "Verifie le cablage, la resistance de 4.7k entre DATA et 3.3V, puis reessaie.");
  } else {
    Serial.println("Capteur DS18B20 detecte et initialise.");
  }

  displayReady = localDisplay.begin();
  if (!displayReady) {
    Serial.println("Erreur: e-paper non disponible.");
  } else {
    Serial.println("Ecran e-paper pret.");
  }
}

void loop() {
  unsigned long now = millis();

  if (!sensorFound) {
    return;
  }

  if (now - lastReadMs >= kReadIntervalMs) {
    lastReadMs = now;

    float temperatureC = 0.0f;
    TemperatureStatusCode status;
    if (temperatureSensor.readTemperature(temperatureC)) {
      status = localStatus.evaluate(temperatureC, true);
      Serial.print("Temperature lue: ");
      Serial.print(temperatureC);
      Serial.println(" C");
    } else {
      status = localStatus.evaluate(0.0f, false);
      Serial.println("Erreur: sonde non lue (deconnexion, mauvais cablage ou perte de contact).");
      temperatureC = 0.0f;
    }

    Serial.print("Etat local: ");
    Serial.println(TemperatureStatus::toString(status));

    TelemetryData telemetry{
        kDeviceId, kDeviceName, kSensorType, temperatureC, status, kReadIntervalS,
        now};
    printTelemetryJson(telemetry, Serial);

    if (displayReady) {
      localDisplay.showReading(temperatureC, status);
    }
  }
}
