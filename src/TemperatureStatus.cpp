#include "TemperatureStatus.h"
#include "DeviceConfig.h"

TemperatureStatusCode TemperatureStatus::evaluate(float temperatureC,
                                                bool readingOk) {
  if (!readingOk) {
    return TemperatureStatusCode::ERREUR_SONDE;
  }

  if (temperatureC > DeviceConfig::CRITICAL_THRESHOLD_C) {
    return TemperatureStatusCode::CRITIQUE;
  }
  if (temperatureC > DeviceConfig::WARNING_THRESHOLD_C) {
    return TemperatureStatusCode::AVERTISSEMENT;
  }
  return TemperatureStatusCode::NORMAL;
}

const char* TemperatureStatus::toString(TemperatureStatusCode status) {
  switch (status) {
    case TemperatureStatusCode::NORMAL:
      return "NORMAL";
    case TemperatureStatusCode::AVERTISSEMENT:
      return "AVERTISSEMENT";
    case TemperatureStatusCode::CRITIQUE:
      return "CRITIQUE";
    case TemperatureStatusCode::ERREUR_SONDE:
      return "ERREUR_SONDE";
    default:
      return "INCONNU";
  }
}
