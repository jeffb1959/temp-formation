#pragma once

#include <Arduino.h>

class LocalDisplay {
public:
  bool begin();
  void showReading(float temperatureC, bool readingOk);
};
