#pragma once

#include <Arduino.h>
#include "TemperatureStatus.h"

class LocalDisplay {
public:
  bool begin();
  void showReading(float temperatureC, TemperatureStatusCode status,
                  bool wifiConnected, bool mqttConnected);
};
