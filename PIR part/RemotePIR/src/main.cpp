#include <Arduino.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include "WifiManagement.h"

const char *ssidWifi = "CG";
const char *passwordWifi = "12ImpEglAnt69";
const char *serveurMqtt = "192.168.10.200";

String nomDevice = "MobilePIR";
String categorieDevice = "Capteurs";

WiFiClient clientWifi;
IPAddress adresseIP;

PubSubClient clientMqtt(clientWifi);

Ticker ticker;

void blink()
{
  //Inversion de l'état de la led
  int state = digitalRead(LED_BUILTIN); // récupération de son état
  digitalWrite(LED_BUILTIN, !state);    // on le positionne en opposition d'état
}

void connecterAuServeurMQTT()
{
  if (clientMqtt.connect(nomDevice.c_str()))
  {
    Serial.println("Connected to server");
  }
  else
    Serial.println("I think connection failed!");
}

void envoyerLeMessageMQTT()
{
  Serial.println(F("Envoi message MQTT"));

  String topic = "Maison/" + categorieDevice + "/" + nomDevice + "/FromObject/State";
  if (!clientMqtt.connected())
  {
    connecterAuServeurMQTT();
  }
  Serial.println(F("Publication ON"));
  clientMqtt.publish(topic.c_str(), "1");
  delay(2000);
  Serial.println(F("Publication OFF"));
  clientMqtt.publish(topic.c_str(), "0");
}

void setup()
{
  // Fait clignoter la LED intégré durant la connexion au réseau WiFi - Blink Bleu Led during WiFi connexion
  pinMode(LED_BUILTIN, OUTPUT);
  ticker.attach(0.5, blink);

  Serial.begin(115200);

  adresseIP = connectToWifi(ssidWifi, passwordWifi);
  clientMqtt.setServer(serveurMqtt, 1883);

  envoyerLeMessageMQTT();

  Serial.println("Je m'endors");
  ticker.detach();
  digitalWrite(LED_BUILTIN, LOW);

  ESP.deepSleep(0);
}

void loop()
{
}