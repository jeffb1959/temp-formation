#pragma once

#include <Arduino.h>

#include "TemperatureStatus.h"

struct TelemetryData {
  const char* device_id;
  const char* device_name;
  const char* sensor_type;
  float temperature_c;
  TemperatureStatusCode local_status;
  uint16_t reading_interval_s;
  bool wifi_connected;
  int wifi_rssi_dbm;
  unsigned long timestamp_ms;
};

void printTelemetryJson(const TelemetryData& data, Print& output);
