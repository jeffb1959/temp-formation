# Projet Formation_temperature

## Objectif du projet

Former pas a pas la lecture d'une temperature avec une carte ESP32 et un DS18B20, avec un code modulaire et tres simple a expliquer.

L'approche se fait par petites phases testables.

## Phase 1.1 (validee)

- DS18B20 lu correctement toutes les 5 secondes.
- Pin capteur conservee: `D5` (`GPIO6`).
- Affichage serie actif avec:
  - demarrage du programme,
  - initialisation du capteur,
  - GPIO utilise,
  - temperature lue,
  - erreur si absence de sonde.

## Phase 1.2 (validee)

- Ajout d'un module d'affichage local: `LocalDisplay`.
- Ecran: e-paper monochrome 2.9" 296x128 en SPI.
- Le module affiche au minimum:
  - titre: `Formation temperature`
  - capteur: `DS18B20`
  - temperature en `°C`
  - etat: `Lecture OK` ou `Erreur sonde`
- Lecture serie conservee.
- L'ecran se met a jour apres une nouvelle lecture, pas en boucle.

## Phase 1.2a - Diagnostic e-paper
- DS18B20 OK sur `D5` (`GPIO6`).
- Avant correction pinMode: écran encore blanc.
- Compilation OK (diagnostic actif).
- Correction ajoutée : `pinMode` explicite sur CS/DC/RST/BUSY avant `display.init`.
- Log corrigé : `Pins e-paper -> CS:44 DC:10 RST:38 BUSY:4`.
- Inspection GxEPD2 (classes 2.9 trouvées): `GxEPD2_290`, `GxEPD2_290_T5`, `GxEPD2_290_T5D`, `GxEPD2_290_I6FD`, `GxEPD2_290_T94`, `GxEPD2_290_T94_V2`, `GxEPD2_290_BS`, `GxEPD2_290_M06`, `GxEPD2_290_GDEY029T94`, `GxEPD2_290_GDEY029T71H`.
- Modèle retenu pour ce diagnostic (inchangé): `GxEPD2_290_GDEY029T94`.
- Diagnostic écran actif:
  - logs série détaillés de l'initialisation et du rafraichissement,
  - pattern visuel `TEST EPAPER` + rectangle noir + température si dispo.
- En cours: `GxEPD2_290_GDEY029T94` et l'écran peut encore rester blanc selon le câblage/pile ou le timing.
- Pins EE05 v1.11 confirmées pour l'écran:
  - CS = 44
  - DC = 10
  - RST = 38
  - BUSY = 4
- Diagnostic driver en cours pour confirmer le bon modèle GxEPD2 2.9".
- Prochain essai prévu:
  - `GxEPD2_290_T94` (un seul modèle alternatif à tester ensuite).

Note: si `GxEPD2_290_T94` n'existe pas dans `GxEPD2_BW.h`, passer au suivant selon votre version de lib.

### Cablage DS18B20 (valide)

- VCC -> 3.3V
- GND -> GND
- DATA -> D5
- 4.7kOhm entre DATA et 3.3V

### Note sur le DS18B20

- `D15` n'a pas detecte la sonde dans les essais precedents.
- Ne jamais utiliser la pin D15 pour le capteur.
- La correspondance GPIO exacte reste a verifier plus tard si besoin.
- Le capteur reste obligatoirement sur `D5` pour cette phase.

### Configuration e-paper 2.9" (provisoire et ajustable)

Bibliotheques utilisees:
- `zinggjm/GxEPD2@^1.6.9`
- `adafruit/Adafruit GFX Library@^1.12.6`
- `adafruit/Adafruit BusIO@^1.17.4`

Modele d'ecran adapte au code:
- `GxEPD2_290_GDEY029T94` (monochrome 296x128)

Pins e-paper utilisées dans cette version 1.2:
- `CS=44`
- `DC=10`
- `RST=38`
- `BUSY=4`

SPI (si forcé):
- `SCK=13`
- `MISO=12`
- `MOSI=11`

Modele utilise dans le projet Boutons Memoire:
- `GxEPD2_213_GDEY0213B74` (e-paper 2.13")

Ici la difference:
- meme librairie GxEPD2,
- modele differents de panneau (2.13" vs 2.9"),
- ecran et broches d'affichage independantes de la phase 1.1.

### Methodologie (formation)

1. Etre module par fonctionnalite.
2. Garder le code lisible et court.
3. Tester chaque phase avant de passer a la suivante.
4. Utiliser des messages serie clairs pour expliquer chaque etat.

## Version actuelle

- `0.7.0` - Envoi MQTT vers ThingsBoard.

## Historique des versions

- `0.7.0` - Envoi MQTT vers ThingsBoard.
- `0.6.0` - Connexion Wi-Fi locale du module.
- `0.5.0` - Configuration centralisée du module.
- `0.4.0` - Phase 1.4: structure de télémétrie locale JSON série.
- `0.3.0` - Phase 1.3: ajout logique locale de statut temperature pour test laboratoire.
- `0.2.0` - Phase 1.2: affichage local e-paper 2.9" du capteur DS18B20 en plus du serie.
- `0.1.0` - Phase 1.1: lecture DS18B20 fonctionnelle sur D5.

## Phase 2.0 - Connexion Wi-Fi locale du module

- Ajout d'un module dédié: `WifiManager` (`src/WifiManager.h`, `src/WifiManager.cpp`).
- Le module initialise la connexion Wi-Fi, affiche les logs de:
  - démarrage connexion,
  - SSID utilisé (sans mot de passe),
  - succès connexion,
  - adresse IP reçue,
  - échec si la connexion ne s'établit pas dans un délai raisonnable.
- Nouveau fichier local:
  - [`include/wifi_secrets.h`](include/wifi_secrets.h) (`WIFI_SSID`, `WIFI_PASSWORD`)
  - version d'exemple: [`include/wifi_secrets.example.h`](include/wifi_secrets.example.h)
- `wifi_secrets.h` contient des credentials locales de développement et **ne doit pas être poussé sur GitHub** (`.gitignore`).
- Architecture validée pour la suite:
  - `ESP32 -> Wi-Fi -> ThingsBoard` (sans passerelle à ce stade).
- La passerelle a été écartée pour garder une architecture directe et simple vers ThingsBoard.
- La lecture capteur/affichage e-paper/télémétrie locale reste inchangée (10 s entre lectures).
- La télémétrie JSON ajoute un statut réseau lisible:
  - `wifi_connected`
  - `wifi_rssi_dbm` (si connecté).
- L'écran e-paper affiche un indicateur court:
  - `WiFi OK` ou `WiFi NOK`.

## Phase 2.0a - Diagnostic Wi-Fi

- Ajout d'un diagnostic de connexion Wi-Fi pour faciliter le dépannage.
- Au démarrage, le module effectue un scan et affiche:
  - le nombre de réseaux détectés,
  - pour chaque réseau: SSID, RSSI, canal, type d'authentification.
- Le code loggue désormais:
  - `Duree max de connexion (ms)`,
  - un point à chaque tentative,
  - `WiFi.status()` (valeur + texte: `WL_CONNECTED`, `WL_NO_SSID_AVAIL`, `WL_CONNECT_FAILED`, `WL_CONNECTION_LOST`, `WL_DISCONNECTED`, `WL_UNKNOWN`),
  - l'adresse IP quand connecté,
  - le RSSI quand connecté.
- Si le SSID est introuvable au scan:
  - `SSID non trouve. Verifier que le reseau est en 2.4 GHz et visible.`
- Si le SSID est trouvé mais la connexion échoue:
  - `SSID trouve mais connexion echouee. Verifier mot de passe, securite WPA et compatibilite 2.4 GHz.`
- Rappel important ESP32:
  - le Wi-Fi est en 2.4 GHz; certains routeurs avec SSID combiné 2.4/5 GHz peuvent rendre la liaison instable.
- Test recommandé:
  - créer un hotspot 2.4 GHz avec un SSID simple, visible, et tester la connexion sur ce réseau.

## Phase 2.2 - Envoi de télémétrie MQTT vers ThingsBoard

- Ajout d'un module dédié: `ThingsBoardClient` (`src/ThingsBoardClient.h`, `src/ThingsBoardClient.cpp`).
- Référence MQTT ThingsBoard:
  - host: `mqtt.thingsboard.cloud`
  - port: `1883`
  - topic: `v1/devices/me/telemetry`
- L'authentification MQTT utilise le token du device ThingsBoard comme username.
- Rappel sécurité: si un token apparait dans une capture d’écran ou sur GitHub, il doit être régénéré immédiatement.
- Le client MQTT utilise `WiFiClient` et `PubSubClient`.
- Connexion MQTT:
  - tentée seulement si le Wi-Fi est connecté,
  - logs de début, host/port, succès, échec avec code d’état PubSubClient.
- Authentification MQTT:
  - `THINGSBOARD_ACCESS_TOKEN` est utilisé comme username,
  - password vide.
- Credentials locale:
  - `include/thingsboard_secrets.h`
  - version d'exemple: `include/thingsboard_secrets.example.h`
- `thingsboard_secrets.h` reste local et doit être ignoré par Git (`.gitignore`).
- Télémétrie locale JSON ajoutée:
  - `mqtt_connected`
  - `mqtt_publish_ok`
- Le JSON envoyé à ThingsBoard reste simple et n'inclut jamais de token ni de mot de passe Wi-Fi.
- Cette étape envoie uniquement de la télémétrie: pas d'alarmes ThingsBoard, pas de courriel, pas de logique avancée.

## Phase 1.5 - Configuration centralisée du module

- Ajout d’un fichier de configuration dédié: [`DeviceConfig.h`](src/DeviceConfig.h), chargé des constantes module:
  - `DEVICE_ID = "prototype_temp_01"`
  - `DEVICE_NAME = "Prototype temp"`
  - `SENSOR_TYPE = "DS18B20"`
  - `WARNING_THRESHOLD_C = 30.0`
  - `CRITICAL_THRESHOLD_C = 60.0`
  - `READING_INTERVAL_S = 10`
  - `READING_INTERVAL_MS = READING_INTERVAL_S * 1000`
- `DeviceConfig.h` est maintenant documenté pour la formation: chaque constante a un commentaire clair (usage, contenu attendu, exemples quand c’est utile).
- Principe d’une seule source de vérité pour la cadence:
  - `READING_INTERVAL_S` est la valeur à modifier pour la formation.
  - `READING_INTERVAL_MS` est recalculée automatiquement en interne.
  - Le programme utilise `READING_INTERVAL_MS` avec `millis()`.
  - Le JSON continue d’exposer `reading_interval_s`.
  - Cela évite d'avoir deux valeurs qui divergent.
- Centralisation utile pour préparer un projet multi-équipement plus tard: chaque module peut partager la même architecture logicielle avec une configuration propre (identité, capteur, seuils, cadence).
- Aucun comportement fonctionnel n’a été changé:
  - la télémétrie JSON continue d’exposer `device_id`, `device_name`, `sensor_type` et `reading_interval_s`,
  - la logique `TemperatureStatus` utilise désormais ces constantes,
  - l’affichage continue de montrer le nom du module, la température et l’état,
  - la fréquence de lecture reste à 10 secondes.

## Phase 1.4 - Préparer la télémétrie locale

- Ajout du module `TelemetryData` (`TelemetryData.h`, `TelemetryData.cpp`) pour préparer les données utiles à un futur envoi vers passerelle/ThingsBoard.
- Une ligne JSON est maintenant affichée dans le moniteur série à chaque lecture, pour test local.
- Structure prévue réutilisable plus tard (MQTT/HTTP/ESP-NOW):
  - `device_id`
  - `device_name`
  - `sensor_type`
  - `temperature_c`
  - `local_status`
  - `reading_interval_s`
  - `timestamp_ms`
- Valeurs temporaires utilisées:
  - `device_id = "prototype_temp_01"`
  - `device_name = "Prototype temp"`
  - `sensor_type = "DS18B20"`
  - `reading_interval_s = 10`
- Les seuils restent les mêmes que la phase 1.3 (30/60 pour labo).
- Rappel:
  - pas de Wi-Fi,
  - pas de ThingsBoard,
  - pas d'e-mail,
  - pas de passerelle pour l’instant.
- Phase 1.4 garde `kReadIntervalMs = 10000` (10 secondes) et prépare l'évolution sans changer la logique actuelle de capteur/affichage.

### Diagnostic e-paper 1.2 (écran croisé)

- Compilation OK, DS18B20 OK, écran 2.9" toujours blanc avec `GxEPD2_290_GDEY029T94` après correction des pins et logs.
- Test temporaire en cours avec `GxEPD2_213_GDEY0213B74` (mêmes broches EE05), pour isoler :
  - la validite du bus SPI / connecteur EE05,
  - et un probleme potentiel de modele/driver 2.9".
- Objectif: confirmer que le matériel EE05 fonctionne avec un écran deja connu (Boutons Memoire), sans changer la logique DS18B20.

## Test 1.2b - Initialisation identique Boutons Mémoire

- Conserve le mode `GxEPD2_213_GDEY0213B74`.
- Utilise toujours les pins EE05 validées: CS=44, DC=10, RST=38, BUSY=4.
- Init logicielle alignée sur `DisplayManager.cpp` de Boutons Mémoire:
  - `display.init(115200);`
  - `display.setRotation(3);`
  - `display.setFullWindow();`
- Suppression temporaire de l'init SPI manuelle pour ce test:
  - `#include <SPI.h>`
  - `pinMode(...)`
  - `digitalWrite(...)`
  - `SPI.begin(...)`
- Rendu test simplifié:
  - `fillScreen(GxEPD_WHITE)`
  - `setTextColor(GxEPD_BLACK)`
  - textes `Memoire EE05`, `ePaper OK`, et `W/H`.
- Succès: affichage texte + température validé sur écran 2.13" avec cette initialisation.
- Retour au grand écran 2.9": même initialisation Boutons Mémoire (sans `SPI.begin`), modèle changé en `GxEPD2_290_GDEY029T94`.

## Phase 1.3 - Etat local de temperature (laboratoire)

- Nouveau module ajouté: `TemperatureStatus` (`TemperatureStatus.h` / `TemperatureStatus.cpp`).
- Etats locaux affichés localement:
  - `NORMAL`
  - `AVERTISSEMENT`
  - `CRITIQUE`
  - `ERREUR_SONDE`
- Seuils temporaires de test:
  - avertissement: `30.0 C`
  - critique: `60.0 C`
- L’affichage e-paper affiche maintenant:
  - nom equipement: `Prototype temp`
  - temperature courante
  - etat local
  - seuil d’avertissement
  - seuil critique
- Intervalle de lecture conservé: `10 secondes`.
- Pas de Wi-Fi, pas de ThingsBoard, pas de courriel, pas de logique de "3 lectures consecutives" dans cette phase.
- Rappel important: les alarmes, actions et notifications officielles (dont ThingsBoard et e-mails) seront ajoutées dans une phase suivante.
- Leçon apprise:
  - valider d’abord l’écran avec un e-paper déjà connu (ex. Boutons Mémoire / `GxEPD2_213_GDEY0213B74`) pour isoler plus vite les causes.

## A verifier

- Carte: XIAO ESP32-S3 (ESP32 EE05)
- No Wi-Fi, pas de ThingsBoard, pas de passerelle, pas d'alerte courriel, pas d'affichage e-paper additionnel.

Note: en Phase 1.4, correction de compilation appliquée pour ESP32-S3 USB CDC:
`TelemetryData::printTelemetryJson` accepte maintenant `Print&` au lieu de `HardwareSerial&`.

