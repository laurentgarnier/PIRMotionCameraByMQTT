#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "TraceDebug.h"

String ssid = "ssid";
String password = "pwd";

String emailSenderAccount = "##############";
String emailSenderPassword = "##############";
String smtpServer = "##############";
int smtpServerPort = 000;
String emailSender = "##############";
String emailSubject = "##############";
String emailMessage = "##############";
String emailRecipient = "##############";
int tempsEntreDeuxEnvoisEnMinutes = 1;

void decodeConfigFile(String data)
{
    TraceDebug("Decodage configuration");
    TraceDebug(data);

    // Les données arrivent en JSON sous la forme
    // {
    //     "ssid": "[SSID name]",
    //     "ssidPwd": "SSID_PWD",
    //     "emailSenderAccount": "[email Sender Account]",
    //     "emailSenderPassword": "[email Sender Password]",
    //     "smtpServer": "[smtp Server address]",
    //     "smtpServerPort": [smtp server port],
    //     "emailSender": "[email Sender shown by recipients]",
    //     "emailSubject": "[email Subject]",
    //     "emailMessage": "[email Message]",
    //     "emailRecipient": "[email Recipients separates by ,]",
    //     "tempsEntreDeuxEnvoisDeMailEnMinutes":5
    // }
    DynamicJsonDocument json(1024);
    DeserializationError error = deserializeJson(json, data);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        TraceDebug(error.c_str());
        return;
    }

    JsonObject obj = json.as<JsonObject>();

    ssid = obj["ssid"].as<String>();
    password = obj["ssidPwd"].as<String>();
    emailSenderAccount = obj["emailSenderAccount"].as<String>();
    emailSenderPassword = obj["emailSenderPassword"].as<String>();
    smtpServer = obj["smtpServer"].as<String>();
    smtpServerPort = obj["smtpServerPort"];
    emailSender = obj["emailSender"].as<String>();
    emailSubject = obj["emailSubject"].as<String>();
    emailMessage = obj["emailMessage"].as<String>();
    emailRecipient = obj["emailRecipient"].as<String>();
    tempsEntreDeuxEnvoisEnMinutes = obj["tempsEntreDeuxEnvoisDeMailEnMinutes"];

    TraceDebug("Config : " + ssid + " - " + password);
}

#endif