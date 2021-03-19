
#ifndef RADIOCONFIG_H
#define RADIOCONFIG_H
#include <stdint.h>

#pragma pack(push, 1)
struct ConfigData
{
  uint8_t cePin;
  uint8_t csnPin;
  uint16_t ownIdentifier;
  uint16_t crc16;
};

class RadioConfig
{

public:
  bool isCRCcorrect();
  void setCRC();
  void printDetails();
  uint8_t getCePin();
  uint8_t getCsnPin();
  uint8_t getChannel();
  uint32_t getOwnAddress();
  uint32_t getMasterAddress();
  uint32_t getBroadcastAddress();
  ConfigData data;
  uint16_t getIdentifierIntFromAddressPointer(void *);
  uint32_t getAddressFromIdentifier(const uint16_t & identfier);

private:
  uint8_t addressPrefix = 0xe3;
  uint8_t addressPostfix = 0xe7;
  uint32_t broadcastAddress = 0xc3b2a1c7;
  uint8_t channel = 0x64;
};

#pragma pack(pop)
#endif