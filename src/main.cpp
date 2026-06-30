#include <Arduino.h>

#include "DeviceConfig.h"
#include "DS18B20Sensor.h"
#include "LocalDisplay.h"
#include "ThingsBoardClient.h"
#include "WifiManager.h"
#include "TemperatureStatus.h"
#include "TelemetryData.h"
#include "wifi_secrets.h"

constexpr uint8_t kSensorPin = D5;  // D5 / GPIO6

DS18B20Sensor temperatureSensor(kSensorPin);
WifiManager wifiManager;
ThingsBoardClient thingsBoardClient;
TemperatureStatus localStatus;
LocalDisplay localDisplay;
bool sensorFound = false;
bool displayReady = false;
bool wifiConnected = false;
bool mqttConnected = false;
bool mqttPublishOk = false;
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

  wifiConnected = wifiManager.connect(WIFI_SSID, WIFI_PASSWORD);
  if (wifiConnected) {
    Serial.print("RSSI Wi-Fi (dBm): ");
    Serial.println(wifiManager.getRssiDbm());
    mqttConnected = thingsBoardClient.connect();
    Serial.print("Connexion MQTT initiale: ");
    Serial.println(mqttConnected ? "OK" : "Non connecte");
  } else {
    Serial.println("Module sans connexion Wi-Fi pour le moment.");
    mqttConnected = false;
  }
}

void loop() {
  unsigned long now = millis();
  thingsBoardClient.loop();

  if (!sensorFound) {
    return;
  }

  if (now - lastReadMs >= DeviceConfig::READING_INTERVAL_MS) {
    lastReadMs = now;
    wifiConnected = wifiManager.isConnected();
    if (wifiConnected) {
      mqttConnected = thingsBoardClient.connect();
    } else {
      mqttConnected = false;
    }

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

    mqttPublishOk = false;
    const int wifiRssi = wifiConnected ? wifiManager.getRssiDbm() : 0;
    TelemetryData telemetry{
        DeviceConfig::DEVICE_ID, DeviceConfig::DEVICE_NAME,
        DeviceConfig::SENSOR_TYPE, temperatureC, status,
        DeviceConfig::READING_INTERVAL_S, wifiConnected, wifiRssi, mqttConnected,
        false, now};

    if (wifiConnected) {
      if (mqttConnected) {
        mqttPublishOk = thingsBoardClient.publishTelemetry(telemetry);
        telemetry.mqtt_publish_ok = mqttPublishOk;
        Serial.println(mqttPublishOk ? "MQTT publish OK" : "MQTT publish ECHEC");
      } else {
        Serial.println("MQTT publish ECHEC");
      }
    }
    printTelemetryJson(telemetry, Serial);

    if (displayReady) {
      localDisplay.showReading(temperatureC, status, wifiConnected, mqttConnected);
    }
  }
}
