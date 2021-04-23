#include <messagehandler.h>

#include <Arduino.h>
#include <mxsupport.h>
#include <mxtiming.h>
#include <ublox.h>

extern CircularBuffer sendBuffer;
extern CircularBuffer rx_buffer_broadcast;
extern UBlox ublox;

//#define VERBOSEublox

MessageHandler::MessageHandler()
{
}
MessageHandler::~MessageHandler()
{
}

void MessageHandler::begin()
{
    radiolink.begin();
}
void MessageHandler::handle_RTCM_Message(uint8_t *msg, int len)
{
    ublox.send(++msg, --len);
    
    flash_LED();
    //printObject(msg, len);
}

void MessageHandler::handleBroadcast(uint8_t *msg, int len)
{
    //radiolink.restore_ack_payload_buffer();
    uint8_t msgID = msg[0];
    if (msgID == CommCodes::GPS_RTCM_MSG)
    {
        handle_RTCM_Message(msg,len);
    }
    else if (msgID == CommCodes::HEARTBEAT)
        {
            flash_LED();
        }
    
}
void MessageHandler::handleMessage(uint8_t *msg, int len)
{
    if (len == 0)
    {
#ifdef VERBOSE
        Serial.print(F("\n"));
        Serial.print(F("BLANK PAYLOAD RECEIVED\n"));
        Serial.print(F(" \n"));
#endif //VERBOSE
        clientManager.reportFail();
    }
    uint8_t msgID = msg[0];
    if (msgID <= CommCodes::MAX_MSG_ID)
    {

        handleDataMessage(msg, len);
    }
    else if (msgID == CommCodes::CONFIGMSG)
    {
        handleConfigMessage(msg, len);
    }
    else if (msgID == CommCodes::HELLO)
    {
        handleDataMessage(msg, len);
    }

    else
    {
#ifdef VERBOSE
        Serial.print(F("Unknown Message Received\n"));
        printObject(msg, len);
#endif //VERBOSE
    }
}

void MessageHandler::handleConfigMessage(uint8_t *msg, int len)
{
    switch (msg[1])
    {

    case CommCodes::PING:
        sendPong(msg, len);
        break;

    default:
#ifdef VERBOSE
        Serial.println(F("Unkown Config Message received"));
        printObject(msg, len);
#endif //VERBOSE

        break;
    }
}

void MessageHandler::sendPong(uint8_t *msg, int len)
{
    uint8_t answer[len];
    copyBytewise(msg, &answer, len);
    answer[1] = CommCodes::PONG;
    radiolink.radio.writeAckPayload(0, &answer, len);
}

//Client Routine
void MessageHandler::checkRadioAndHandleMessage()
{
    uint8_t pipeNo{0xff};
    uint8_t len = radiolink.checkRadio(&pipeNo);
    if (len > 0)
    {
#ifdef VERBOSE
#warning VERBOSE MESSAGING ENABLED
        Serial.printf(F("Received message\n on pipe # %d"), pipeNo);

        printObject(&radiolink.recvBuffer, len);

#endif //VERBOSE
        if (pipeNo == BROADCAST_PIPE)
        {
            handleBroadcast(&radiolink.recvBuffer[0], len);
        }
        else
        {
            handleMessage(&radiolink.recvBuffer[0], len);
        }
    }
}

void MessageHandler::handleDataMessage(uint8_t *msg, int len)
{
    uint8_t recvMessageID = msg[0];
    if (len > 1)
    {
        Serial.write(&radiolink.recvBuffer[1], len - 1);
    }
    loadNextMessageToAckBuffer();
}

void MessageHandler::loadConfigMessageToAckBuffer(uint8_t msgCode)
{
    radiolink.pSendBuffer[0] = CommCodes::CONFIGMSG;
    radiolink.pSendBuffer[1] = msgCode;
    radiolink.sendBufferLen = 2;
    radiolink.writeSendBufferToAckPayloadBuffer();
}
void MessageHandler::loadNextMessageToAckBuffer()
{
    //uint8_t dummydata[] = {31, 32, 33}; //source
    ++messageID %= CommCodes::MAX_MSG_ID;
    //radiolink.shiftSendBufferToBackupBuffer();
    radiolink.pSendBuffer[0] = messageID;
    radiolink.sendBufferLen = 1;
    for (int i = 1; i < 32; i++)
    {
        if (!sendBuffer.isEmpty())
        {
            radiolink.pSendBuffer[i] = sendBuffer.get();
            ++radiolink.sendBufferLen;
        }
        else
        {
            break;
        }
    }
#ifdef VERBOSE
    Serial.print(F("Loading to Ack PL\n"));
    printObject(radiolink.pSendBuffer, radiolink.sendBufferLen);
#endif //VERBOSE
    radiolink.writeSendBufferToAckPayloadBuffer();
}

void MessageHandler::sendRegistrationRequest()
{
    uint8_t msg[6];
    uint32_t address;
    msg[0] = CommCodes::CONFIGMSG;
    msg[1] = CommCodes::REGISTRATION_REQUEST;

    address = config.getOwnAddress();

    copyBytewise(&address, &msg[2], 4);
#ifdef VERBOSE
    Serial.print(F("\nSending Registration request\n"));
    printObject(msg, 6);
#endif //VERBOSE
    radiolink.sendToMaster(&msg, 6);
}

#ifdef PACKAGE_CONTROL_OFF

void MessageHandler::readSerial()
{
    Client *c = clientManager.pCurrentClient;

    while (Serial.available() && c->pSendBuffer->isNotFull())
    {
        c->pSendBuffer->put(Serial.read());
    }

    // Serial.print(F("Size of Sendbuffer before after reading Serial: "));
    // Serial.print(c->pSendBuffer->size());
}
#else //PACKAGE_CONTROL_OFF
void MessageHandler::readSerial()
{
    uint8_t gotByte;
    const uint8_t *startSequence = reinterpret_cast<const uint8_t *>(&CommCodes::STARTSEQUENCE);
    uint8_t msgHeader[8];
    uint16_t len, identifier;
    bool validHeaderFound;

    while (Serial.available() > 8 && !validHeaderFound)
    {
        gotByte = Serial.read();
        if (gotByte == startSequence[0])
        {
            msgHeader[0] = gotByte;
            for (int i = 0; i < 8; i++)
            {
                msgHeader[i] = Serial.read();
            }
            len = *reinterpret_cast<uint16_t *>(&msgHeader[4]);
            identifier = *reinterpret_cast<uint16_t *>(&msgHeader[6]);
            validHeaderFound = CommCodes::STARTSEQUENCE == *reinterpret_cast<uint32_t *>(msgHeader);
        }
    }

    if (validHeaderFound)
    {

        bool clientValid = clientManager.setCurrentClient(identifier);
        if (clientValid)
        {
            while (len > clientManager.pCurrentClient->pSendBuffer->available())
            {
                //immediate communication!!
                talkToCurrentClient();
            }
            while (len)
            {
                if (Serial.available())
                {
                    clientManager.pCurrentClient->pSendBuffer->put(Serial.read());
                    --len;
                }
            }
        }
    }
}

#endif //PACKAGE_CONTROL_OFF

void dropMAVMessageFromSendBuffer()
{

    ;
}