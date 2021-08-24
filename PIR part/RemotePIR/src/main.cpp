#include <Arduino.h>
#include "MQTTManagement.h"
#include "WifiManagement.h"

String ssidWifi = "CG";
String passwordWifi = "12ImpEglAnt69";
String serveurMqtt = "192.168.10.200";
String nomDevice = "MobilePIR";
String categorieDevice = "Capteurs";

WiFiClient clientWifi;
IPAddress adresseIP;
String adresseMAC;
PubSubClient *clientMqtt;

const int PIR_PIN = 2;

int timingDernierEnvoiMessageMQTT;
int intervalEntreDeuxMessagesMQTTEnSecondes = 300; // 5mn
int timingDernierEnvoiMessageDeVie;
int periodeEnvoiMessageDeVieEnSecondes = 30;

void envoyerLeMessageMQTT()
{
  Serial.println("Envoi message MQTT");

  String payload = "PIR ON";
  String topic = "Maison/FromDevice/" + categorieDevice + "/" + nomDevice;

  publierMessage(topic, payload, clientMqtt);
}

void GererActivationPIR()
{
  digitalWrite(LED_BUILTIN, HIGH);
  int timingCourant = millis();
  if ((timingCourant - timingDernierEnvoiMessageMQTT) > (intervalEntreDeuxMessagesMQTTEnSecondes * 1000))
  {
    envoyerLeMessageMQTT();
    timingDernierEnvoiMessageMQTT = timingCourant;
  }
  digitalWrite(LED_BUILTIN, LOW);
}

String macToStr(const uint8_t *mac)
{
  String result;
  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  char bufferSSID[ssidWifi.length()];
  ssidWifi.toCharArray(bufferSSID, ssidWifi.length());

  char bufferPassword[passwordWifi.length()];
  passwordWifi.toCharArray(bufferPassword, passwordWifi.length());

  adresseIP = connectToWifi(bufferSSID, bufferPassword);
  // Récupération de l'adresse MAC du device
  uint8_t mac[6];
  WiFi.macAddress(mac);
  adresseMAC = macToStr(mac);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(PIR_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIR_PIN), GererActivationPIR, FALLING);

  char bufferServeurMQTT[serveurMqtt.length()];
  serveurMqtt.toCharArray(bufferServeurMQTT, serveurMqtt.length());
  clientMqtt = new PubSubClient(bufferServeurMQTT, 1883, clientWifi);

  connecterAuServeurMQTT(nomDevice, categorieDevice, adresseMAC, clientMqtt);
  envoyerMessageDeVie(nomDevice, adresseMAC, adresseIP, clientMqtt, categorieDevice);

  timingDernierEnvoiMessageMQTT = millis();
  timingDernierEnvoiMessageDeVie = millis();
}

void loop()
{
  int timingCourant = millis();
  if ((timingCourant - timingDernierEnvoiMessageDeVie) > (periodeEnvoiMessageDeVieEnSecondes * 1000))
  {
    envoyerMessageDeVie(nomDevice, adresseMAC, adresseIP, clientMqtt, categorieDevice);
    timingDernierEnvoiMessageDeVie = timingCourant;
  }
  if (digitalRead(PIR_PIN) == HIGH)
    GererActivationPIR();
}
