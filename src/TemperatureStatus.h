#pragma once

enum class TemperatureStatusCode {
  NORMAL,
  AVERTISSEMENT,
  CRITIQUE,
  ERREUR_SONDE
};

class TemperatureStatus {
public:
  TemperatureStatusCode evaluate(float temperatureC, bool readingOk);
  static const char* toString(TemperatureStatusCode status);
};
