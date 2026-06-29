#include <GxEPD2_BW.h>

#include "LocalDisplay.h"

namespace {
// Pins e-paper retenus pour la carte EE05 v1.11
constexpr uint8_t kEpaperCsPin = 44;   // CS
constexpr uint8_t kEpaperDcPin = 10;   // DC
constexpr uint8_t kEpaperRstPin = 38;  // RST
constexpr uint8_t kEpaperBusyPin = 4;  // BUSY

// Test diagnostic avec l'ecran du projet Boutons Memoire
using DisplayModel = GxEPD2_290_GDEY029T94;
using EpdDisplay = GxEPD2_BW<DisplayModel, DisplayModel::HEIGHT>;

EpdDisplay display(
    DisplayModel(kEpaperCsPin, kEpaperDcPin, kEpaperRstPin, kEpaperBusyPin));
bool initialized = false;
}  // namespace

bool LocalDisplay::begin() {
  Serial.println("Initialisation e-paper: debut.");
  Serial.println(
      "Modele GxEPD2 utilise: GxEPD2_290_GDEY029T94");
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

  showReading(0.0f, false);
  return initialized;
}

void LocalDisplay::showReading(float temperatureC, bool readingOk) {
  if (!initialized) {
    Serial.println("Affichage saute: e-paper non initialise.");
    return;
  }

  Serial.println("Debut rafraichissement e-paper.");

  char tempLine[24];
  if (readingOk) {
    snprintf(tempLine, sizeof(tempLine), "%.2f C", temperatureC);
  } else {
    snprintf(tempLine, sizeof(tempLine), "N/A");
  }

  char dimsLine[24];
  snprintf(dimsLine, sizeof(dimsLine), "W/H: %d x %d", display.width(),
           display.height());

  display.firstPage();
  do {
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(8, 10);
  display.print("Temp Module");
  display.setCursor(8, 34);
  display.print("ePaper 2.9");
  display.setCursor(8, 58);
  display.print(dimsLine);
  display.setCursor(8, 82);
    display.print("T:");
    display.print(tempLine);
  } while (display.nextPage());

  Serial.println("Fin rafraichissement e-paper.");
}
