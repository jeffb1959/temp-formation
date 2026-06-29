#include <Arduino.h>

#include "DS18B20Sensor.h"

constexpr uint8_t kSensorPin = 6;  // D5
constexpr unsigned long kReadIntervalMs = 5000;

DS18B20Sensor temperatureSensor(kSensorPin);
bool sensorFound = false;
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
    } else {
      Serial.println("Erreur: sonde non lue (déconnexion, mauvais câblage ou perte de contact).");
    }
  }
}
