#pragma once

#include <cstdint>

namespace DeviceConfig {

// DEVICE_ID:
// - Identifiant unique de ce module.
// - A conserver court, stable et sans espace si possible.
// - Sert plus tard pour la passerelle, ThingsBoard et l'historique.
// - Exemples: "compresseur_30_forces", "pompe_vide_axyz".
inline constexpr const char* DEVICE_ID = "prototype_temp_01";

// DEVICE_NAME:
// - Nom lisible affiché sur l'e-paper et dans les logs.
// - Peut contenir des espaces.
// - Exemple: "Compresseur 30 forces".
inline constexpr const char* DEVICE_NAME = "Prototype temp";

// SENSOR_TYPE:
// - Type de capteur branché sur ce module.
// - Pour le prototype: "DS18B20".
// - Peut evoluer plus tard en "PT100_MAX31865" ou autre.
inline constexpr const char* SENSOR_TYPE = "DS18B20";

// WARNING_THRESHOLD_C:
// - Seuil local d'avertissement (degres Celsius).
// - Utilise uniquement l'affichage local pour l'etat.
// - Les alertes reelles et emails seront traites plus tard par ThingsBoard.
inline constexpr float WARNING_THRESHOLD_C = 30.0f;

// CRITICAL_THRESHOLD_C:
// - Seuil local critique (degres Celsius).
// - Utilise uniquement l'affichage local pour l'etat.
// - Sert en labo pour valider le changement d'etat avec de l'eau chaude.
inline constexpr float CRITICAL_THRESHOLD_C = 60.0f;

// READING_INTERVAL_S:
// - Intervalle entre deux lectures, en secondes.
// - Cette valeur est simple a lire/modifier en formation.
// - Exemple: 10 = une lecture toutes les 10 secondes.
inline constexpr uint16_t READING_INTERVAL_S = 60;

// READING_INTERVAL_MS:
// - Meme intervalle, converti en millisecondes pour le programme.
// - Le code interne utilise millis(), donc on travaille en ms.
// - NE PAS modifier directement cette valeur.
// - Seul point important:
//   - READING_INTERVAL_S est la source de verite a modifier.
//   - READING_INTERVAL_MS est calcule automatiquement.
//   - Le code utilise READING_INTERVAL_MS pour la boucle.
//   - La telemetrie JSON expose READING_INTERVAL_S.
//   - Cela evite d'avoir deux valeurs differentes.
inline constexpr unsigned long READING_INTERVAL_MS =
    static_cast<unsigned long>(READING_INTERVAL_S) * 1000UL;

}  // namespace DeviceConfig
