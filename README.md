# Projet Formation_temperature

## Objectif du projet

Ce projet permet d'apprendre pas à pas la lecture d'un capteur de temperature DS18B20 avec une carte ESP32 (XIAO ESP32-S3), en gardant un code simple et modulaire.

L'idee de la phase 1.1 est d'avoir une base stable:

- un capteur DS18B20 lisible;
- un code séparé en petit modules (`main.cpp` + un module `DS18B20Sensor`);
- des logs serie clairs pour la formation.

## Phase 1.1 - Lecture DS18B20 fonctionnelle

La phase 1.1 est validée.

- Le capteur est installe sur breadboard.
- Le capteur lit une temperature valide.
- La temperature change quand la sonde est tenue dans la main (test manuelle).
- La lecture est envoyee toutes les 5 secondes en serie.
- Un message d'erreur est affiche si la sonde n'est pas detectee.

### Cablage DS18B20 valide

- DS18B20 VCC -> 3.3V
- DS18B20 GND -> GND
- DS18B20 DATA -> D5
- Resistance 4.7 kOhm entre DATA et 3.3V

### Pin utilisee

- Pin capteur: `D5` (`GPIO6`).
- Note: un test precedent sur `D15` n'a pas detecte la sonde.
- La correspondance exacte GPIO pour la carte doit rester a verifier plus tard.

### Methode de travail

Le projet suit une progression par petites phases testables:

1. Mettre en place un code minimal de lecture du capteur.
2. Verifier le cablage et le fonctionnement de base.
3. Valider en serial la lecture periodique.
4. Ajouter la prochaine petite fonctionnalite par phase.

## Version actuelle

- `0.1.0` - Lecture DS18B20 fonctionnelle sur `D5`.

## Historique des versions

- `0.1.0` (Phase 1.1) : Lecture DS18B20 fonctionnelle en serie (5s), module `DS18B20Sensor`, pin `D5`.

## A faire (prochaines etapes)

- Etape suivante: ajouter la logique d'autres phases (selon objectif de formation), sans Wi-Fi, sans ThingsBoard, sans affichage e-paper et sans passerelle pour cette sequence.
