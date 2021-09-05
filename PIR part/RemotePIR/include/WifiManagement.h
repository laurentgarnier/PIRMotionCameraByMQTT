#include <ESP8266WiFi.h>

IPAddress connectToWifi(const char* ssid,const char* password)
{
    Serial.println("");
    Serial.println(F("Connexion au Wifi ") + String(ssid));
    
    WiFi.begin(ssid, password);

    // Tant que non connecté
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    // Connexion réussie
    IPAddress adresseIP = WiFi.localIP();
    Serial.println("");
    Serial.println(F("Connexion WiFi OK"));
    Serial.print(F("Adresse IP : "));
    Serial.println(adresseIP);
    Serial.println("");
    return adresseIP;
}