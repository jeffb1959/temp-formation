#pragma once

#include <WiFi.h>
#include <PubSubClient.h>

#include <Arduino.h>

#include "TelemetryData.h"

class ThingsBoardClient {
public:
  ThingsBoardClient();
  bool connect();
  void loop();
  bool publishTelemetry(const TelemetryData& data);
  bool isConnected();

private:
  const char* mqttStateToText(int state);
  WiFiClient _wifiClient;
  PubSubClient _mqttClient;
  bool _connected = false;
};
