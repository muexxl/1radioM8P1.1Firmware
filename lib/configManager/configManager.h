
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <radioconfig.h>

class ConfigManager {

private:
public:
    RadioConfig config;
    ConfigManager();
    RadioConfig getConfig();
    void loadConfigFromEeprom();
    void loadConfigFileViaSerial();
    void saveConfigToEeprom();
    void updateConfig();
    void checkConfiguration();
    void begin();
};

#endif