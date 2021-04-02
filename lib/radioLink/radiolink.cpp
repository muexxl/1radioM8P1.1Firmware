#include <radiolink.h>
#include <mxsupport.h>

Radiolink::Radiolink()
{
    pSendBuffer = &sendBuffer1[0];
    pBackupBuffer = &sendBuffer2[0];
}

Radiolink::~Radiolink()
{
    //Destruction of Radiolink
}

void Radiolink::begin()
{
    ConfigManager cfgMgr;
    cfgMgr.begin();
    config = cfgMgr.getConfig();
    initRadio();

    for (size_t i = 0; i < 32; i++)
    {
        pSendBuffer[i] = i;
        recvBuffer[i] = 0;
    }
}

bool Radiolink::initRadio()
{

    radio = RF24(config.getCePin(), config.getCsnPin());
    radio.begin();
    radio.setChannel(0x64);
    radio.setAutoAck(1);
    radio.setCRCLength(RF24_CRC_16);
    radio.setDataRate(RF24_2MBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.setRetries(4, 2);
    radio.enableDynamicPayloads();
    radio.enableAckPayload();
    radio.setAddressWidth(4);
    setAddresses();

    radio.startListening();
    return radio.isChipConnected();
}
void Radiolink::openReadingPipe(int pipe, uint32_t address)
{

    radio.openReadingPipe(pipe, reinterpret_cast<uint8_t *>(&address));
}
void Radiolink::openWritingPipe(uint32_t address)
{
    radio.openWritingPipe(reinterpret_cast<uint8_t *>(&address));
}

void Radiolink::openReadingAndWritingPipe(uint32_t &address)
{
    radio.openReadingPipe(0, reinterpret_cast<uint8_t *>(&address));
    radio.openWritingPipe(reinterpret_cast<uint8_t *>(&address));
}

void Radiolink::openReadingAndWritingPipe(uint16_t &identfier)
{
    uint32_t address = config.getAddressFromIdentifier(identfier);
    openReadingAndWritingPipe(address);
}

void Radiolink::openReadingAndWritingPipe(const Client *client)
{
    uint32_t address = config.getAddressFromIdentifier(client->identifier);
    openReadingAndWritingPipe(address);
}

void Radiolink::setAddresses()
{
    uint32_t address;
    //radio.enableAckPayload();
    address = config.getOwnAddress();
    radio.openReadingPipe(0, reinterpret_cast<uint8_t *>(&address));
    radio.openWritingPipe(reinterpret_cast<uint8_t *>(&address));

    address = config.getBroadcastAddress();
    radio.openReadingPipe(1, reinterpret_cast<uint8_t *>(&address));
    radio.setAutoAck(1,false);
}

bool Radiolink::printSelfCheck()
{
    health = radio.isChipConnected();
    Serial.print('\n');
    if (health)
    {
        Serial.println(F("Chip connected"));
    }
    else
    {
        Serial.println(F("Chip NOT connected"));
    }
    return health;
}

bool Radiolink::sendToMaster(void *message, int len)
{
    uint32_t address = config.getMasterAddress();
    radio.openWritingPipe(reinterpret_cast<uint8_t *>(&address));
    radio.stopListening();

    bool success = radio.write(message, len, 1);
    openReadingAndWritingPipe(config.data.ownIdentifier);
    radio.startListening();
    return success;
}
bool Radiolink::sendToIdentifier(uint16_t identifier, void *message, int len)
{
    uint32_t address = config.getAddressFromIdentifier(identifier);
    bool success;
    radio.openWritingPipe(reinterpret_cast<uint8_t *>(&address));
    radio.stopListening();
    success = radio.write(message, len);
    return success;
};

// bool Radiolink::send(void *message, int len)
// {
//     return radio.write(message, len);;
// };

bool Radiolink::send(const void *message, int len)
{
    return radio.write(message, len);
    ;
};

bool Radiolink::sendFromSendBuffer()
{
    return radio.write(pSendBuffer, sendBufferLen);
}

int Radiolink::getAck(void *ackBuffer)
{
    int len{0};
    if (radio.available())
    {
        len = radio.getDynamicPayloadSize();
        radio.read(ackBuffer, len);
    }

    return len;
};

bool Radiolink::regularCheck()
{
    unsigned long timePassed{0};
    health = true;
    if (!radio.isChipConnected())
    {
        health = false;
        return health;
    }
    timePassed = millis() - lastMessageReceived;
    //printf("\n %d ms since last contact\n", timePassed);
    if (timePassed > maxTimeBetweenContactInMillis)
    {
        health = false;
    }

    return health;
}

void Radiolink::checkRadioAndRespond()
{
    uint8_t pipeNo = 1;
    while (radio.available(&pipeNo))
    {
        uint8_t len = radio.getDynamicPayloadSize();
        radio.read(&recvBuffer[0], len);
        printObject(&recvBuffer, len);
        if (len > 0 && pipeNo == 0) //only reset counter for data that has been received on the OWN address!
        {
            lastMessageReceived = millis();
        }
    }
}

void Radiolink::writeSendBufferToAckPayloadBuffer()
{
    radio.writeAckPayload(0, pSendBuffer, sendBufferLen); 
}

uint8_t Radiolink::checkRadio(void *buffer, uint8_t *pipeNo)
{
    uint8_t len{0};
    if (radio.available(pipeNo))
    {
        len = radio.getDynamicPayloadSize();
        radio.read(buffer, len);
    }
    if (len>0 && *pipeNo == 0){
        lastMessageReceived = millis();
    }
    return len;
}

uint8_t Radiolink::checkRadio(uint8_t *pipeNo)
{
    uint8_t len{0};
    if (radio.available(pipeNo))
    {
        len = radio.getDynamicPayloadSize();
        radio.read(&recvBuffer[0], len);
    }
    if (len>0 && *pipeNo == 0){
        lastMessageReceived = millis();
    }
    return len;
}

uint8_t Radiolink::checkRadio()
{
    uint8_t pipeNo;
    return checkRadio(&pipeNo);
}


void Radiolink::openMasterPipe()
{
    uint32_t masterAddress = config.getMasterAddress();
    radio.openReadingPipe(2, masterAddress);
}

void Radiolink::restoreSendBufferFromBackupBuffer()
{
    swapSendBufferAndBackupBuffer();
    backupBufferLen = 0;
}
void Radiolink::shiftSendBufferToBackupBuffer()
{
    swapSendBufferAndBackupBuffer();
    sendBufferLen = 0;
}

void Radiolink::swapSendBufferAndBackupBuffer()
{
    uint8_t *oldPSendBuffer{pSendBuffer};
    uint8_t oldSendBufferLen{sendBufferLen};
    pSendBuffer = pBackupBuffer;
    sendBufferLen = backupBufferLen;
    pBackupBuffer = oldPSendBuffer;
    backupBufferLen = oldSendBufferLen;
}