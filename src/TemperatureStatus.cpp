#include "TemperatureStatus.h"

TemperatureStatusCode TemperatureStatus::evaluate(float temperatureC,
                                                bool readingOk) {
  if (!readingOk) {
    return TemperatureStatusCode::ERREUR_SONDE;
  }

  if (temperatureC > kCriticalThresholdC) {
    return TemperatureStatusCode::CRITIQUE;
  }
  if (temperatureC > kWarningThresholdC) {
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
