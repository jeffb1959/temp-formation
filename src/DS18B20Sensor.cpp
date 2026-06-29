#include "DS18B20Sensor.h"

DS18B20Sensor::DS18B20Sensor(uint8_t pin)
    : _pin(pin), _oneWire(pin), _sensors(&_oneWire) {}

bool DS18B20Sensor::begin() {
  _sensors.begin();
  return _sensors.getDeviceCount() > 0;
}

bool DS18B20Sensor::readTemperature(float& temperatureC) {
  _sensors.requestTemperatures();
  temperatureC = _sensors.getTempCByIndex(0);

  if (temperatureC == DEVICE_DISCONNECTED_C) {
    return false;
  }
  return true;
}

uint8_t DS18B20Sensor::getPin() const {
  return _pin;
}
