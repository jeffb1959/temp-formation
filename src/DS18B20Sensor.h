#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

class DS18B20Sensor {
public:
  explicit DS18B20Sensor(uint8_t pin);

  bool begin();
  bool readTemperature(float& temperatureC);
  uint8_t getPin() const;

private:
  uint8_t _pin;
  OneWire _oneWire;
  DallasTemperature _sensors;
};
