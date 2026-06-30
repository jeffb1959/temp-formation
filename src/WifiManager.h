#pragma once

#include <Arduino.h>

class WifiManager {
public:
  bool connect(const char* ssid, const char* password,
               unsigned long timeoutMs = 30000UL);
  bool isConnected() const;
  int getRssiDbm() const;

private:
  bool _connected = false;
};
