#include "FS.h"
#include "SD_MMC.h"

//Create a dir in SD_MMC card
bool createDir(const char *path)
{
    SD_MMC.mkdir(path);
    if (SD_MMC.mkdir(path))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//delete a dir in SD_MMC card
bool removeDir(const char *path)
{
    SD_MMC.rmdir(path);
    if (SD_MMC.rmdir(path))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Write a file in SD_MMC card
bool writeFile(const char *path, uint8_t *buffer, size_t bufferLen)
{
    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file)
    {
        return false;
    }
    else
    {
        file.write(buffer, bufferLen);
    }
    file.close();
    return true;
}

size_t getFileSize(const char *filePath)
{
    size_t fileSize = 0;
    File file = SD_MMC.open(filePath, FILE_READ);

    if (file)
    {
        fileSize = file.size();
        file.close();
        return fileSize;
    }

    return -1;
}

bool readFile(const char *path, char *result)
{
    File file = SD_MMC.open(path, FILE_READ);
    if (!file)
        return false;

    file.readBytes((char *)result, file.size());
    file.close();
    return true;
}

//Delete a file in SD_MMC card
bool deleteFile(const char *path)
{
    SD_MMC.remove(path);
    if (SD_MMC.remove(path))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool mountSdCard()
{
    if (!SD_MMC.begin("/sdcard", true))
        return false;

    return true;
}

void unmontSdCard()
{
    SD_MMC.end();
}

bool isDirectoryExists(const char *directory)
{
    File root = SD_MMC.open(directory);
    if (!root)
    {
        return false;
    }

    if (!root.isDirectory())
    {
        return false;
    }
    return true;
}

bool isFileExists(const char *filePath)
{
    return SD_MMC.exists(filePath);
}

byte getNbFileForDirectory(String path)
{
    byte nbFile = 0;
    File root = SD_MMC.open(path);
    File file = root.openNextFile();
    while (file)
    {
        nbFile++;
        file.close();
        file = root.openNextFile();
    }
    return nbFile;
}