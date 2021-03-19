
#include <configManager.h>

#include <Arduino.h>
#include <radioconfig.h>
#include <mxsupport.h>

#include <EEPROM.h>




ConfigManager::ConfigManager() {
} 

void ConfigManager::begin()
{
    loadConfigFromEeprom();
    checkConfiguration();
  }
  
void ConfigManager::loadConfigFileViaSerial()
{
  Serial.println(F("Please upload config file"));
  Serial.print(F("Waiting for "));
  Serial.print(sizeof(config.data));
  Serial.print(F(" Bytes \n"));

  char *writeLocation;
  writeLocation = reinterpret_cast<char *>(&config);
  int size = sizeof(config.data);
  uint8_t buf;
  while (Serial.available())
  {
    buf = Serial.read();
  }
  //receive bytes for a complete config object
  while (size--)
  {
    while (!Serial.available())
    {
      ;
    }
    *writeLocation = Serial.read();
    Serial.print(F("received Byte: "));
    printHex(writeLocation, 1);
    Serial.print("\n");
    writeLocation++;
  }
}

void ConfigManager::updateConfig()
{
  char answer;
  loadConfigFileViaSerial();
  config.printDetails();
  Serial.print(F("\n Save Configuration to EEPROM (y/n)?"));
  while (!Serial.available())
  {
    ;
  }
  answer = Serial.read();
  if (answer == 'y')
  {
    saveConfigToEeprom();
  }
  else
  {
    updateConfig();
  }
}

void ConfigManager::saveConfigToEeprom()
{
  Serial.print(F("\n Saving Configuration to EEPROM\n"));
  EEPROM.put(0, config);
}
void ConfigManager::loadConfigFromEeprom()
{
  config.data = EEPROM.get(0, config.data);
}

void ConfigManager::checkConfiguration()
{
  if (config.isCRCcorrect())
  {
    // Serial.print(F("Valid Configuration found:\n"));
    // config.printDetails();
  }
  else
  {
    Serial.println(F("No valid Configuration found!\n"));
    Serial.println(F("Do you want to upload a configuration file? (y/n)"));
    if (isSerialAnswerY())
    {
      updateConfig();
    }
  }
}

RadioConfig ConfigManager::getConfig(){
    return config;
}