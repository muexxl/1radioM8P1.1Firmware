#include "radioconfig.h"
#include "crc.h"
#include "mxsupport.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void RadioConfig::setCRC()
{
    data.crc16 = calcCRC(&data, sizeof(data) - 2);
}

bool RadioConfig::isCRCcorrect()
{
    return (data.crc16 == calcCRC(&data, sizeof(data) - 2));
}

uint32_t RadioConfig::getOwnAddress()
{
    return getAddressFromIdentifier(data.ownIdentifier);
}

uint16_t RadioConfig::getIdentifierIntFromAddressPointer(void *address)
{
    uint16_t *p16 = reinterpret_cast<uint16_t *>(address + 1);
    return *p16;
}

uint32_t RadioConfig::getAddressFromIdentifier(const uint16_t & identfier){
    uint32_t address{0};
    uint8_t *p8 = reinterpret_cast<uint8_t *>(&address);
    uint16_t *p16 = reinterpret_cast<uint16_t *>(p8 + 1);

    p8[0] = addressPostfix;   //1st byte LSB -e3
    p16[0] = identfier; //2nd + 3rd byte
    p8[3] = addressPrefix;    // 4th byte-e7
    return address;
}


uint32_t RadioConfig::getMasterAddress()
{
    return broadcastAddress +1 ;
}

uint32_t RadioConfig::getBroadcastAddress()
{
    return broadcastAddress;
}

uint8_t RadioConfig::getCePin()
{
    return data.cePin;
}
uint8_t RadioConfig::getCsnPin()
{
    return data.csnPin;
}
uint8_t RadioConfig::getChannel()
{
    return channel;
}

#ifdef ARDUINO
void RadioConfig::printDetails()
{
    Serial.println(F("\nDetails of Radio Config"));
    Serial.print("---------------------------------\n");

    Serial.print(F("\nCE Pin:\t"));
    printHex(&data.cePin, sizeof(data.cePin));

    Serial.print(F("\nCSN Pin:\t"));
    printHex(&data.csnPin, sizeof(data.csnPin));

    Serial.print(F("\nOwn Identifier:\t"));
    printHex(&data.ownIdentifier, sizeof(data.ownIdentifier));

    Serial.print(F("\nBroadcastAddress :\t"));
    printHex(&broadcastAddress, sizeof(broadcastAddress));

    Serial.print(F("\nCRC16 Checksum:\t"));
    printHex(&data.crc16, sizeof(data.crc16));
    Serial.print(F(" -> "));
    isCRCcorrect() ? Serial.print("Ok") : Serial.print("NOT OK");
    Serial.print(F("\n---------------------------------\n"));
}

#endif