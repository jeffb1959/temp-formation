#pragma once

#include <cstdint>

namespace DeviceConfig {

inline constexpr const char* DEVICE_ID = "prototype_temp_01";
inline constexpr const char* DEVICE_NAME = "Prototype temp";
inline constexpr const char* SENSOR_TYPE = "DS18B20";
inline constexpr float WARNING_THRESHOLD_C = 30.0f;
inline constexpr float CRITICAL_THRESHOLD_C = 60.0f;
inline constexpr unsigned long READING_INTERVAL_MS = 10000UL;
inline constexpr uint16_t READING_INTERVAL_S = 10;

}  // namespace DeviceConfig
