
#include "Arduino.h"
#include "driver/rtc_io.h"

#include "SDCardManagement.h"
#include "WifiManagement.h"
#include "CameraManagement.h"
#include "MailManagement.h"
#include "Configuration.h"
#include "TraceDebug.h"
#include "MQTTManagement.h"

RTC_DATA_ATTR int bootCount = 0;

String configFile = "/Configuration/Config.json";
String picturesDirectory = "/Pictures";
String pictureName = "Picture";
int nbImagesAAcquerir = 10;

IPAddress adresseIP;
WiFiClient clientWifi;
String adresseMAC;

String serveurMqtt = "192.168.10.200";
String nomDevice = "CameraMobile";
String categorieDevice = "Capteurs";

int timingDernierEnvoiDesDonnees;
const int periodeEnvoiMessageDeVieEnMs = 60000; // Toutes les mn

PubSubClient *clientMqtt;

void AcquerirEtEnvoyerMail(String topic, byte *message, unsigned int length);

void setup()
{
  InitDebugSerie();

  // Gestion de la carte SD pour stockage temporaire des images acquises
  mountSdCard();
  if (isDirectoryExists(picturesDirectory.c_str()))
    removeDir(picturesDirectory.c_str());

  createDir(picturesDirectory.c_str());

  // Récupération de la configuration sur la carte SD
  if (isFileExists(configFile.c_str()))
  {
    size_t fileSize = getFileSize(configFile.c_str());
    char *fileData = (char *)malloc(fileSize);
    readFile(configFile.c_str(), fileData);
    decodeConfigFile(String((char *)fileData));
  }

  adresseIP = connectToWifi(ssid, password);
  // Récupération de l'adresse MAC du device
  uint8_t mac[6];
  WiFi.macAddress(mac);
  adresseMAC = macToStr(mac);

  TraceDebug("Init camera");
  // initialisation de la caméra
  initCamera();

  TraceDebug("Init OK");

  clientMqtt = new PubSubClient(serveurMqtt.c_str(), 1883, AcquerirEtEnvoyerMail, clientWifi);

  connecterAuServeurMQTT(nomDevice, categorieDevice, adresseMAC, clientMqtt);
  
  envoyerMessageDeVie(nomDevice, adresseMAC, adresseIP, clientMqtt, categorieDevice);

  timingDernierEnvoiDesDonnees = millis();
}

void AcquerirEtEnvoyerMail(String topic, byte *message, unsigned int length)
{
  // Lancement des acquisitions
  for (int indexAcquisition = 0; indexAcquisition < nbImagesAAcquerir; indexAcquisition++)
  {
    TraceDebug("Acquisition " + String(indexAcquisition));
    String nomImage = picturesDirectory + String("/") + pictureName + String(indexAcquisition) + ".jpg";
    acquisitionResult acquisition = takePicture();
    TraceDebug("Ecriture du fichier " + nomImage);
    if (!writeFile(nomImage.c_str(), acquisition.buffer, acquisition.bufferLength))
      TraceDebug("Erreur ecriture du fichier " + nomImage);
    free(acquisition.buffer);
    // 1 seconde entre chaque acquisition
    delay(1000);
  }

  if(!clientWifi.connected())
    // Connexion Wi-fi
    adresseIP = connectToWifi(ssid, password);

  // Envoi du mail maintenant que tout est OK
  sendMail(picturesDirectory, pictureName, nbImagesAAcquerir);
}

void loop()
{
  int timingCourant = millis();

  if ((timingCourant - timingDernierEnvoiDesDonnees) > periodeEnvoiMessageDeVieEnMs)
  {
    envoyerMessageDeVie(nomDevice, adresseMAC, adresseIP, clientMqtt, categorieDevice);
    timingDernierEnvoiDesDonnees = timingCourant;
  }
  clientMqtt->loop();
}