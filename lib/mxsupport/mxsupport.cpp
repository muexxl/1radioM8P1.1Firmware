#include <mxsupport.h>


#ifdef ARDUINO

#include <Arduino.h>

void printObject(void *obj, int size)
{
    uint8_t *currentByte = reinterpret_cast<uint8_t *>(obj);
    int counter{0};
    while (size--)
    {
        //cout << std::dec << counter++ << ":";

        if (counter % 0x10 == 0)
        {
            if (counter)
            {
                Serial.print(F("\n"));
            }

            if (counter < 0x10)
            {
                Serial.print(0);
            }
            Serial.print(counter, HEX);
            Serial.print(F("  |  "));
            
        }

        if (int(*currentByte) < 0x10)
        {
            Serial.print(0);
        }
        Serial.print(int(*currentByte++), HEX);
        Serial.print(" ");
        counter++;
    }
    Serial.print("\n");
}

void writeToSerial(void *data, int size)
{
    char * chardata = reinterpret_cast<char*>(data);
    Serial.write(chardata,size);
}

void printHex(void *obj, int size)
{
    uint8_t *currentByte = reinterpret_cast<uint8_t *>(obj);
    while (size--)
    {
        if (int(*currentByte) < 0x10)
        {
            Serial.print(0);
        }
        Serial.print(int(*currentByte++), HEX);
        Serial.print(" ");
    }
}

bool isSerialAnswerY()
{
    while (!Serial.available())
    {
        ;
    }
    char answer = Serial.read();
    return (answer == 'y');
}
#else

#include <stdint.h>
#include <iostream>
using namespace std;

void printObject(void *obj, int size)
{
    uint8_t *currentByte = reinterpret_cast<uint8_t *>(obj);
    int counter{0};
    while (size--)
    {
        //cout << std::dec << counter++ << ":";

        if (counter % 0x10 == 0)
        {

            printf("\n%04X | ", counter);
        }
        printf("%02X ", int(*currentByte++));
        counter++;
    }
    printf("\n");
}

#endif

void copyBytewise(const void *source, void *target, int len)
{
    const char *sourceByte {reinterpret_cast<const char *>(source)};
    char *targetByte {reinterpret_cast<char *>(target)};
    while (len--)
    {
        *targetByte++ = *sourceByte++;
    }
}
