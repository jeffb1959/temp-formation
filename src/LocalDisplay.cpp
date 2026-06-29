#include <GxEPD2_BW.h>

#include "DeviceConfig.h"
#include "LocalDisplay.h"

namespace {
// Pins e-paper retenus pour la carte EE05 v1.11
constexpr uint8_t kEpaperCsPin = 44;   // CS
constexpr uint8_t kEpaperDcPin = 10;   // DC
constexpr uint8_t kEpaperRstPin = 38;  // RST
constexpr uint8_t kEpaperBusyPin = 4;  // BUSY

using DisplayModel = GxEPD2_290_GDEY029T94;
using EpdDisplay = GxEPD2_BW<DisplayModel, DisplayModel::HEIGHT>;

EpdDisplay display(
    DisplayModel(kEpaperCsPin, kEpaperDcPin, kEpaperRstPin, kEpaperBusyPin));
bool initialized = false;
}  // namespace

bool LocalDisplay::begin() {
  Serial.println("Initialisation e-paper: debut.");
  Serial.println("Modele GxEPD2 utilise: GxEPD2_290_GDEY029T94");
  Serial.print("Pins e-paper -> CS:");
  Serial.print(static_cast<int>(kEpaperCsPin));
  Serial.print(" DC:");
  Serial.print(static_cast<int>(kEpaperDcPin));
  Serial.print(" RST:");
  Serial.print(static_cast<int>(kEpaperRstPin));
  Serial.print(" BUSY:");
  Serial.println(static_cast<int>(kEpaperBusyPin));

  display.init(115200);
  Serial.print("Largeur e-paper: ");
  Serial.print(display.width());
  Serial.print(" / Hauteur e-paper: ");
  Serial.println(display.height());
  display.setRotation(3);
  Serial.println("Rotation e-paper: 3.");
  display.setFullWindow();
  initialized = true;
  Serial.println("Initialisation e-paper: OK.");

  showReading(0.0f, TemperatureStatusCode::ERREUR_SONDE);
  return initialized;
}

void LocalDisplay::showReading(float temperatureC, TemperatureStatusCode status) {
  if (!initialized) {
    Serial.println("Affichage saute: e-paper non initialise.");
    return;
  }

  Serial.println("Debut rafraichissement e-paper.");

  char tempLine[24];
  if (status != TemperatureStatusCode::ERREUR_SONDE) {
    snprintf(tempLine, sizeof(tempLine), "%.2f C", temperatureC);
  } else {
    snprintf(tempLine, sizeof(tempLine), "N/A");
  }

  const char* statusText = TemperatureStatus::toString(status);

  char warningLine[30];
  char criticalLine[30];
  char dimsLine[24];
  snprintf(warningLine, sizeof(warningLine), "Seuil A: > %.1f C",
           DeviceConfig::WARNING_THRESHOLD_C);
  snprintf(criticalLine, sizeof(criticalLine), "Seuil C: > %.1f C",
           DeviceConfig::CRITICAL_THRESHOLD_C);
  snprintf(dimsLine, sizeof(dimsLine), "W/H: %d x %d", display.width(),
           display.height());

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setTextSize(2);
    display.setCursor(8, 10);
    display.print(DeviceConfig::DEVICE_NAME);
    display.setCursor(8, 34);
    display.print("T:");
    display.print(tempLine);
    display.setCursor(8, 58);
    display.print("Etat:");
    display.print(statusText);
    display.setTextSize(1);
    display.setCursor(8, 84);
    display.print(warningLine);
    display.setCursor(8, 100);
    display.print(criticalLine);
    display.setCursor(8, 116);
    display.print(dimsLine);
  } while (display.nextPage());

  Serial.println("Fin rafraichissement e-paper.");
}
