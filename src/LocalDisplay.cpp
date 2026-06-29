#include <SPI.h>
#include <GxEPD2_BW.h>

#include "LocalDisplay.h"

namespace {
// Pins e-paper retenus pour la carte EE05 v1.11
constexpr uint8_t kEpaperCsPin = 44;   // CS
constexpr uint8_t kEpaperDcPin = 10;   // DC
constexpr uint8_t kEpaperRstPin = 38;  // RST
constexpr uint8_t kEpaperBusyPin = 4;  // BUSY
constexpr uint8_t kSpiSckPin = 13;     // SCK
constexpr uint8_t kSpiMisoPin = 12;    // MISO
constexpr uint8_t kSpiMosiPin = 11;    // MOSI

// Diagnostic temporaire: meme modele que Boutons Memoire
using DisplayModel = GxEPD2_213_GDEY0213B74;
constexpr const char* kDisplayModelName =
    "GxEPD2_213_GDEY0213B74 - test diagnostic avec ecran Boutons Memoire";

using EpdDisplay = GxEPD2_BW<DisplayModel, DisplayModel::HEIGHT>;

EpdDisplay display(
    DisplayModel(kEpaperCsPin, kEpaperDcPin, kEpaperRstPin, kEpaperBusyPin));
bool initialized = false;
}  // namespace

bool LocalDisplay::begin() {
  Serial.println("Initialisation e-paper: debut.");
  Serial.print("Modele GxEPD2 utilise: ");
  Serial.println(kDisplayModelName);
  Serial.print("Pins e-paper -> CS:");
  Serial.print(static_cast<int>(kEpaperCsPin));
  Serial.print(" DC:");
  Serial.print(static_cast<int>(kEpaperDcPin));
  Serial.print(" RST:");
  Serial.print(static_cast<int>(kEpaperRstPin));
  Serial.print(" BUSY:");
  Serial.println(static_cast<int>(kEpaperBusyPin));
  pinMode(kEpaperCsPin, OUTPUT);
  pinMode(kEpaperDcPin, OUTPUT);
  pinMode(kEpaperRstPin, OUTPUT);
  pinMode(kEpaperBusyPin, INPUT);
  digitalWrite(kEpaperCsPin, HIGH);
  digitalWrite(kEpaperDcPin, HIGH);
  digitalWrite(kEpaperRstPin, HIGH);
  SPI.begin(kSpiSckPin, kSpiMisoPin, kSpiMosiPin, kEpaperCsPin);
  display.init(115200);
  Serial.print("Largeur e-paper: ");
  Serial.print(display.width());
  Serial.print(" / Hauteur e-paper: ");
  Serial.println(display.height());
  display.setRotation(3);
  Serial.println("Rotation e-paper: 3.");
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

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.fillRect(5, 5, display.width() - 10, display.height() - 10,
                     GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setTextSize(2);
    display.setCursor(12, 40);
    display.print("TEST EPAPER");

    display.setTextColor(GxEPD_BLACK);
    display.setTextSize(2);
    display.setCursor(8, display.height() - 34);
    display.print("T:");
    display.print(tempLine);
    display.setCursor(8, display.height() - 16);
    display.setTextSize(1);
    display.print(readingOk ? "Lecture OK" : "Erreur sonde");
  } while (display.nextPage());

  Serial.println("Fin rafraichissement e-paper.");
}
