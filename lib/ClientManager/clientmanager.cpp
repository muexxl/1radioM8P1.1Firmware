#include <clientmanager.h>
#include <Arduino.h>
#include <circularbuffer.h>


void ClientManager::printClients()
{

    int saveCurrentClientPosition = currentClientPosition;
    Serial.print("\n");
    for (int i = 0; i < max_clients; i++)
    {
        currentClientPosition=i;
        pCurrentClient=&clients[i];

        printCurrentClient();
    }
    currentClientPosition=saveCurrentClientPosition;
    pCurrentClient =&clients[currentClientPosition] ;
}

void ClientManager::printCurrentClient(){
        Serial.print(currentClientPosition);
        Serial.print(F(" has Memory Address: "));
        
        uint32_t address = reinterpret_cast<uint32_t>(pCurrentClient);
        
        Serial.print(address, HEX);
        Serial.print(F(" has ID: "));
        Serial.print(pCurrentClient->identifier, HEX);
        Serial.print(F(" has Fails: "));
        Serial.print(pCurrentClient->fails);
        Serial.print(F(" has lastMsgID: "));
        Serial.print(pCurrentClient->lastMessageID);
        Serial.print(" \n");
}


bool ClientManager::removeClient(uint16_t identifier)
{
    bool returnvalue = false;
    for (Client &client : clients)
    {
        if (client.identifier == identifier)
        {
            client.identifier = 0;
            delete client.pSendBuffer;

            registeredClients--;
            returnvalue = true;
            break;
        }
    }
    return returnvalue;
}


bool ClientManager::setCurrentClient(uint16_t identifier)
{
    bool returnvalue = false;
    for (Client &client : clients)
    {
        if (client.identifier == identifier)
        {
            pCurrentClient = &client;
            returnvalue = true;
            break;
        }
    }
    return returnvalue;
}

void ClientManager::removeCurrentClient()
{
    if (pCurrentClient->identifier)
    {
         pCurrentClient->identifier = 0x00;
        delete (pCurrentClient->pSendBuffer);
        registeredClients--;
    }
}

bool ClientManager::addClient(uint16_t identifier)
{
    bool returnValue = {false};
    if (isRegisteredClient(identifier))
    {
        resetClient(identifier);
        return false;
    }
    if (registeredClients >=  MAX_CLIENTS) {
        return false;
    }
    for (Client &c : clients)
    {
        if (c.identifier == 0x0000)
        {
             c.identifier = identifier;
            c.lastMessageID = 0xa0;
             c.pSendBuffer = new CircularBuffer(128);
            registeredClients++;
            returnValue = true;
            break;
        }
    }
    return returnValue;
}

bool ClientManager::isRegisteredClient(uint16_t identifier)
{
    bool exists = false;
    for (Client &client : clients)
    {
        if (client.identifier == identifier)
        {
            exists = true;
            break;
        }
    }
    
    return exists;
}


void ClientManager::resetClient(uint16_t identifier)
{
     for (Client &client : clients)
    {
        if (client.identifier == identifier)
        {
            client.lastMessageID  = 0xa0;
            break;
        }
    }
}

uint16_t ClientManager::getNextClientIdentifier()
{
    if (!registeredClients)
    {
        return 0; //currently no Clients registers :/
    }
    uint16_t identifier{0};
    pCurrentClient = &clients[++currentClientPosition];
    identifier = pCurrentClient->identifier;
    if (!identifier)
    {
        identifier = getNextClientIdentifier();
    }
    return identifier;
}

bool ClientManager::nextClient()
{
    if (!registeredClients)
    {
        return 0; //currently no Clients registered :/
    }
    do
    {
        ++currentClientPosition %= MAX_CLIENTS;
        pCurrentClient = &clients[currentClientPosition];
        
    } while (!pCurrentClient->identifier);
    
    return 1;
}
int ClientManager::getRegisteredClients()
{
    return registeredClients;
}

bool ClientManager::isCurrentClientValid()
{
    return pCurrentClient->identifier;
}

void ClientManager::resetFails()
{
    pCurrentClient->fails = 0;
}
void ClientManager::reportFail()
{
    
    if (++(pCurrentClient->fails) > CLIENTMANAGER_MAX_FAILS)
    {
        removeCurrentClient();
    }
}
