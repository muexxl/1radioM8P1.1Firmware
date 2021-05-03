#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <clientmanager.h>
#include <radiolink.h>
#include <radioconfig.h>
#include <circularbuffer.h>

//#define VERBOSE
#define PACKAGE_CONTROL_OFF

#define BROADCAST_PIPE 0x01

namespace CommCodes
{
    const uint8_t CONFIGMSG = 0xff;
    const uint8_t REGISTRATION_REQUEST = 0x01;
    const uint8_t DEAUTH_REQUEST = 0x02;
    const uint8_t RESET_REQUEST = 0x03;
    const uint8_t PING = 0x04;
    const uint8_t PONG = 0x05;
    const uint8_t TIME_MSG = 0x06;
    const uint32_t STARTSEQUENCE = 0x07060504;
    const uint8_t HELLO = 0xA0;

    const uint8_t MAX_MSG_ID = 0x40;
    const uint8_t GPS_MSG = 0x41;
    const uint8_t GPS_RTCM_MSG_MIN = 0xd0;
    const uint8_t GPS_RTCM_MSG_MAX = 0xdf;
    const uint8_t HEARTBEAT = 0x48;
}; // namespace CommCodes

class MessageHandler
{
private:
    /* data */
    bool master = false;
    uint8_t lastMessageID{0};
    uint8_t messageID{0};
    uint8_t last_rtcm_msg_id{0};
public:

    int lost_rtcm_packets{0};
    ClientManager clientManager;
    Radiolink radiolink;
    RadioConfig & config {radiolink.config};
    //CircularBuffer *sendBuffer;
    
    void handleMessage(uint8_t *msg, int len);
    void handleBroadcast(uint8_t *msg, int len);
    void handle_RTCM_Message(uint8_t *msg, int len);


    void handleConfigMessage(uint8_t *msg, int len);
    void handleRegistrationMessage(uint8_t *msg, int len);
    void begin();
    void handleDataMessage(uint8_t * msg, int len);
    void sendPong(uint8_t * msg, int len);
    void checkRadioAndHandleMessage();
    void loadNextMessageToAckBuffer();
    void loadConfigMessageToAckBuffer(uint8_t msgCode);
    void openMasterPipe();
    void convertToMaster();
    void openConversation();
    void sendRegistrationRequest();
    void talkToCurrentClient();
    void fillRadioSendBufferFromCurrentClient();
    void checkHeader(uint8_t * header);
    void readSerial();
    void dropMAVMessageFromSendBuffer();
    MessageHandler();
    ~MessageHandler();
};


#endif