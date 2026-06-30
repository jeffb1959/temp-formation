#include <WiFi.h>

#include "WifiManager.h"

namespace {

const char* wifiStatusToText(wl_status_t status) {
  switch (status) {
    case WL_CONNECTED:
      return "WL_CONNECTED";
    case WL_NO_SSID_AVAIL:
      return "WL_NO_SSID_AVAIL";
    case WL_CONNECT_FAILED:
      return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:
      return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:
      return "WL_DISCONNECTED";
    default:
      return "WL_UNKNOWN";
  }
}

const char* wifiAuthModeToText(wifi_auth_mode_t mode) {
  switch (mode) {
    case WIFI_AUTH_OPEN:
      return "OPEN";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return "WPA2_ENTERPRISE";
    case WIFI_AUTH_WPA3_PSK:
      return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return "WPA2/WPA3";
    case WIFI_AUTH_WAPI_PSK:
      return "WAPI_PSK";
    case WIFI_AUTH_OWE:
      return "OWE";
    case WIFI_AUTH_WPA3_ENT_192:
      return "WPA3_ENT_192";
    default:
      return "AUTRE";
  }
}

}  // namespace

bool WifiManager::connect(const char* ssid, const char* password,
                         unsigned long timeoutMs) {
  Serial.println("Connexion Wi-Fi: debut.");
  Serial.print("SSID utilise: ");
  Serial.println(ssid ? ssid : "(vide)");
  Serial.print("Duree max de connexion (ms): ");
  Serial.println(timeoutMs);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.disconnect(true);
  delay(500);

  Serial.println("Scan des reseaux Wi-Fi:");
  const int networksFound = WiFi.scanNetworks();
  bool targetFound = false;
  if (networksFound <= 0) {
    Serial.println("Aucun reseau Wi-Fi detecte.");
  } else {
    Serial.print("Reseaux trouves: ");
    Serial.println(networksFound);
    for (int i = 0; i < networksFound; ++i) {
      const String foundSsid = WiFi.SSID(i);
      const int foundRssi = WiFi.RSSI(i);
      const int foundChannel = WiFi.channel(i);
      const char* authText = wifiAuthModeToText(WiFi.encryptionType(i));
      Serial.print("  - SSID=");
      Serial.print(foundSsid);
      Serial.print(", RSSI=");
      Serial.print(foundRssi);
      Serial.print(" dBm, CH=");
      Serial.print(foundChannel);
      Serial.print(", AUTH=");
      Serial.println(authText);
      if (targetFound == false && ssid && foundSsid == ssid) {
        targetFound = true;
      }
    }
    if (targetFound) {
      Serial.println("SSID cible trouve durant le scan.");
    } else {
      Serial.println("SSID non trouve. Verifier que le reseau est en 2.4 GHz et visible.");
    }
  }

  _connected = false;
  WiFi.begin(ssid, password);

  const unsigned long startMs = millis();
  Serial.print("Connexion...");
  while (WiFi.status() != WL_CONNECTED && (millis() - startMs) < timeoutMs) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  const wl_status_t status = WiFi.status();
  Serial.print("WiFi.status() = ");
  Serial.print(status);
  Serial.print(" (");
  Serial.print(wifiStatusToText(status));
  Serial.println(")");

  if (status == WL_CONNECTED) {
    _connected = true;
    Serial.println("Connexion Wi-Fi: OK.");
    Serial.print("Adresse IP obtenue: ");
    Serial.println(WiFi.localIP().toString());
    Serial.print("RSSI (dBm): ");
    Serial.println(WiFi.RSSI());
    return true;
  }

  Serial.println("Connexion Wi-Fi: ECHEC (timeout).");
  if (!targetFound) {
    Serial.println(
        "SSID non trouve. Verifier que le reseau est en 2.4 GHz et visible.");
  } else {
    Serial.println(
        "SSID trouve mais connexion echouee. Verifier mot de passe, "
        "securite WPA et compatibilite 2.4 GHz.");
  }
  return false;
}

bool WifiManager::isConnected() const {
  return _connected && (WiFi.status() == WL_CONNECTED);
}

int WifiManager::getRssiDbm() const {
  return isConnected() ? WiFi.RSSI() : 0;
}
