#include <Arduino.h>

#include "DS18B20Sensor.h"
#include "LocalDisplay.h"

constexpr uint8_t kSensorPin = D5;  // D5 / GPIO6
constexpr unsigned long kReadIntervalMs = 10000;

DS18B20Sensor temperatureSensor(kSensorPin);
LocalDisplay localDisplay;
bool sensorFound = false;
bool displayReady = false;
unsigned long lastReadMs = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("\n=== Démarrage du programme ===");
  Serial.print("GPIO utilisé pour le DS18B20: ");
  Serial.println(temperatureSensor.getPin());

  Serial.println("Initialisation du capteur DS18B20...");
  sensorFound = temperatureSensor.begin();
  if (!sensorFound) {
    Serial.println("Erreur: aucune sonde DS18B20 détectée.");
    Serial.println("Vérifie le câblage, la résistance de 4.7k entre DATA et 3.3V, puis réessaie.");
  } else {
    Serial.println("Capteur DS18B20 détecté et initialisé.");
  }

  displayReady = localDisplay.begin();
  if (!displayReady) {
    Serial.println("Erreur: e-paper non disponible.");
  } else {
    Serial.println("Ecran e-paper prêt.");
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
    if (temperatureSensor.readTemperature(temperatureC)) {
      Serial.print("Température lue: ");
      Serial.print(temperatureC);
      Serial.println(" °C");
      if (displayReady) {
        localDisplay.showReading(temperatureC, true);
      }
    } else {
      Serial.println("Erreur: sonde non lue (déconnexion, mauvais câblage ou perte de contact).");
      if (displayReady) {
        localDisplay.showReading(0.0f, false);
      }
    }
  }
}
