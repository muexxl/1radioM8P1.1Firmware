#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#define MAX_CLIENTS 20
#include <stdint.h>
#include <circularbuffer.h>


#define CLIENTMANAGER_MAX_FAILS 2

struct Client
{
    uint16_t identifier;
    uint8_t fails;
    uint8_t lastMessageID;
    CircularBuffer *pSendBuffer;
    Client(uint16_t id)
        : identifier{id},
          fails{0},
          lastMessageID{0xa0}
    {
    };
    Client()
        : identifier{0},
          fails{0},
          lastMessageID{0xa0}
    {
    }
    ~Client(){
        delete pSendBuffer;
    }
};


class ClientManager
{
private:
    int currentClientPosition{0};
    int registeredClients{0};

public:
    static const int max_clients = MAX_CLIENTS;
    Client clients[max_clients];
    Client *pCurrentClient{&clients[0]};
    bool removeClient(uint16_t);
    bool addClient(uint16_t);
    bool isRegisteredClient(uint16_t);
    void resetClient(uint16_t identifier);
    void printClients();
    uint16_t getNextClientIdentifier();
    void removeCurrentClient();
    bool nextClient();
    int getRegisteredClients();
    void reportFail();
    bool isCurrentClientValid();
    void resetFails();
    void printCurrentClient();
    bool setCurrentClient(uint16_t identifier);
};

#endif