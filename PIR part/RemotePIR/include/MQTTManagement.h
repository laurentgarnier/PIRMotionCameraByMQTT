#include <PubSubClient.h>

void connecterAuServeurMQTT(String nomDuDevice, String categorieDuDevice, String adresseMAC, PubSubClient *client)
{
    if (!client->connected())
    {
        Serial.println("");
        Serial.println("Connexion au serveur MQTT");
        if (client->connect(adresseMAC.c_str()))
        {
            Serial.println("Connexion au serveur OK");
            String topic = "Maison/" + categorieDuDevice + "/" + nomDuDevice + "/ToObject/EffetDemande" ;
            if (client->subscribe(topic.c_str()))
            {
                Serial.println("Abonnement au topic " + topic + " OK");
            }
            else
            {
                Serial.println("Abonnement au topic " + topic + "  KO");
            }
        }
    }
    Serial.println("");
}

boolean envoyerMessageDeVie(String nomDuDevice, String adresseMAC, IPAddress adresseIP, PubSubClient *client, String categorieDuDevice)
{
    if (!client->connected())
        connecterAuServeurMQTT(nomDuDevice, categorieDuDevice, adresseMAC, client);

    String payload = "Mac : " + adresseMAC + " - IP : " + adresseIP.toString() + " - Cpt : " + String(millis());
    String topic = "Maison/Discovery/" + categorieDuDevice + "/" + nomDuDevice;
    
    boolean result = client->publish(topic.c_str(), (char *)payload.c_str());
    Serial.println("Envoi du message de vie sur " + topic + " - Contenu " + payload + " - " + String(result));
    return result;
}

boolean publierMessage(String topic, String message, PubSubClient *client)
{
    boolean result = client->publish(topic.c_str(), message.c_str());
    Serial.println("Envoi du message " + message + " sur " + topic + " - " + String(result));
    return result;
}