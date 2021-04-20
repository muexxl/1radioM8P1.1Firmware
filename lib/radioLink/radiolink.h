#ifndef RADIOLINK_H
#define RADIOLINK_H

#include <radioconfig.h>
#include <configManager.h>
#include <RF24.h>
#include <clientmanager.h>
class Radiolink
{
private:
    /* data */
    uint8_t sendBuffer1[32];
    uint8_t sendBuffer2[32];
    void swapSendBufferAndBackupBuffer();

public:
    uint8_t recvBuffer[32];

    uint8_t *pSendBuffer;
    uint8_t sendBufferLen{0};

    uint8_t *pBackupBuffer;
    uint8_t backupBufferLen{0};

    RF24 radio = RF24(0x37, 0x36);
    RadioConfig config;

    uint32_t address;
    unsigned long lastMessageReceived;
    bool health = true;
    int maxTimeBetweenContactInMillis = 1000;
    bool regularCheck();

    bool sendToMaster(void *message, int len);
    bool sendToIdentifier(uint16_t, void *, int);
    bool sendFromSendBuffer();
    bool send(const void *, int);
    int getAck(void *ackBuffer);

    uint8_t checkRadio(void * buffer, uint8_t * pipeNo);
    uint8_t checkRadio(uint8_t *pipeNo);
    uint8_t checkRadio();

    void sendRegistrationRequest();
    void checkRadioAndRespond();
    void writeSendBufferToAckPayloadBuffer();
    void writeBackupBufferToAckPayloadBuffeR();
    void setAddresses();
    bool printSelfCheck();
    bool initRadio();
    void begin();

    void restoreSendBufferFromBackupBuffer();
    void shiftSendBufferToBackupBuffer();
    void restore_ack_payload_buffer();
    

    void openReadingPipe(int, uint32_t);
    void openWritingPipe(uint32_t);
    void openReadingAndWritingPipe(uint32_t &address);
    void openReadingAndWritingPipe(uint16_t &identfier);
    void openReadingAndWritingPipe(const Client * client);
    void openMasterPipe();
    Radiolink(/* args */);
    ~Radiolink();
};
#endif