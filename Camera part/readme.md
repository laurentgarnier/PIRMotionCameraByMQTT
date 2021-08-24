# Soft pour la camera 

# WARNING
Pour envoyer le mail avec les pièces jointes stockées sur la carte SD, il faut paramétrer la librairie ESP Mail client
libdeps\esp32cam\ESP Mail Client\src\ESP_Mail_FS.h
#include <SD_MMC.h>
#define ESP_Mail_DEFAULT_SD_FS SD_MMC //For ESP32 SDMMC
#define CARD_TYPE_SD_MMC 1

//#define ESP_Mail_DEFAULT_SD_FS SD //For ESP32 SDMMC
//#define CARD_TYPE_SD 1

Modification également dans \libdeps\esp32cam\ESP Mail Client\src\ESP_Mail_Client.cpp

bool ESP_Mail_Client::sdMMCBegin(const char *mountpoint, bool mode1bit, bool format_if_mount_failed)
{
#if defined(ESP32)
#if defined(CARD_TYPE_SD_MMC)
  _sdConfigSet = true;
  sd_mmc_mountpoint = mountpoint;
  sd_mmc_mode1bit = mode1bit;
  sd_mmc_format_if_mount_failed = format_if_mount_failed;
  return ESP_MAIL_SD_FS.begin(mountpoint, mode1bit);//, format_if_mount_failed);
#endif
#endif
  return false;
}