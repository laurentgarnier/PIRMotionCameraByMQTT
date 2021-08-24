#include <ESP8266WiFi.h>

IPAddress connectToWifi(char* ssid, char* password)
{
    Serial.println("");
    Serial.println("Connexion au Wifi " + String(ssid));
    
    WiFi.begin(ssid, password);

    // Tant que non connecté
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // Connexion réussie
    IPAddress adresseIP = WiFi.localIP();
    Serial.println("");
    Serial.println("WiFi OK");
    Serial.print("Adresse IP : ");
    Serial.println(adresseIP);
    Serial.println("");
    return adresseIP;
}