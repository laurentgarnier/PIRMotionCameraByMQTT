# Description du projet

Le projet permet l'envoi de mail avec des photos lorsqu'un mouvement est détecté.
La capture d'images est faite à l'aide d'un module ESP32-CAM équipé d'une carte SD.
La détection de mouvement est faite via un module ESP8266 ESP 01 équipé d'un capteur PIR.

La communication entre les 2 modules se fait via un broker MQTT. 
Le module PIR envoie un message MQTT lorsqu'il détecte un mouvement. Le module caméra est abonné au topic et déclenche alors les
acquisitions et l'envoi de mail.

Le module caméra est paramétrable à travers la carte SD. 