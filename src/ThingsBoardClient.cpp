#include <Arduino.h>

#include "DeviceConfig.h"
#include "ThingsBoardClient.h"
#include "thingsboard_secrets.h"

ThingsBoardClient::ThingsBoardClient() : _mqttClient(_wifiClient) {}

const char* ThingsBoardClient::mqttStateToText(int state) {
  switch (state) {
    case MQTT_CONNECTION_TIMEOUT:
      return "MQTT_CONNECTION_TIMEOUT";
    case MQTT_CONNECTION_LOST:
      return "MQTT_CONNECTION_LOST";
    case MQTT_CONNECT_FAILED:
      return "MQTT_CONNECT_FAILED";
    case MQTT_DISCONNECTED:
      return "MQTT_DISCONNECTED";
    case MQTT_CONNECTED:
      return "MQTT_CONNECTED";
    case MQTT_CONNECT_BAD_PROTOCOL:
      return "MQTT_CONNECT_BAD_PROTOCOL";
    case MQTT_CONNECT_BAD_CLIENT_ID:
      return "MQTT_CONNECT_BAD_CLIENT_ID";
    case MQTT_CONNECT_UNAVAILABLE:
      return "MQTT_CONNECT_UNAVAILABLE";
    case MQTT_CONNECT_BAD_CREDENTIALS:
      return "MQTT_CONNECT_BAD_CREDENTIALS";
    case MQTT_CONNECT_UNAUTHORIZED:
      return "MQTT_CONNECT_UNAUTHORIZED";
    default:
      return "MQTT_UNKNOWN";
  }
}

bool ThingsBoardClient::connect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connexion MQTT: impossible (Wi-Fi non connecte).");
    return false;
  }

  if (_connected && _mqttClient.connected()) {
    return true;
  }

  Serial.println("Connexion MQTT: debut.");
  Serial.print("MQTT host: ");
  Serial.print(THINGSBOARD_HOST);
  Serial.print(":");
  Serial.println(THINGSBOARD_PORT);

  _mqttClient.setServer(THINGSBOARD_HOST, THINGSBOARD_PORT);
  const bool connected =
      _mqttClient.connect(DeviceConfig::DEVICE_ID, THINGSBOARD_ACCESS_TOKEN, "");
  _connected = connected;

  if (connected) {
    Serial.println("Connexion MQTT: OK.");
    return true;
  }

  const int state = _mqttClient.state();
  Serial.print("Connexion MQTT: ECHEC. Code etat: ");
  Serial.print(state);
  Serial.print(" (");
  Serial.print(mqttStateToText(state));
  Serial.println(")");
  return false;
}

void ThingsBoardClient::loop() {
  if (_mqttClient.connected()) {
    _mqttClient.loop();
  } else {
    _connected = false;
  }
}

bool ThingsBoardClient::publishTelemetry(const TelemetryData& data) {
  if (!_connected || !_mqttClient.connected()) {
    return false;
  }

  const char* statusText = TemperatureStatus::toString(data.local_status);
  char payload[220];
  snprintf(payload, sizeof(payload),
           "{\"temperature_c\":%.2f,"
           "\"local_status\":\"%s\","
           "\"wifi_rssi_dbm\":%d,"
           "\"device_name\":\"%s\","
           "\"sensor_type\":\"%s\"}",
           data.temperature_c, statusText, data.wifi_rssi_dbm, data.device_name,
           data.sensor_type);

  return _mqttClient.publish("v1/devices/me/telemetry", payload);
}

bool ThingsBoardClient::isConnected() {
  return _connected && _mqttClient.connected();
}
