#include <Arduino.h>
#include <SPI.h>

#include <mxsupport.h>
#include <mxtiming.h>

#include <main.h>

#include <radioconfig.h>
#include <configManager.h>
#include <mxsupport.h>
#include <printf.h>
#include <radiolink.h>
#include <messagehandler.h>
#include <circularbuffer.h>


extern void SystemClock_Config(void);
extern bool is_due_2000ms;


CircularBuffer sendBuffer(config::SEND_BUFFER_SIZE);
int time1, time2;
uint8_t counter;
MessageHandler messageHandler;
Radiolink &radiolink{messageHandler.radiolink};
RadioConfig &radioConfig{radiolink.config};
RF24 &radio{radiolink.radio};



void setup () {
    //0pinMode(LED_BUILTIN, OUTPUT);
    SystemClock_Config();
    MX_TIM15_Init();
    initSerial();

    
}

void loop (){
    update_timers();
    if (is_due_2000ms) {
        is_due_2000ms = false;
        flash_LED(); // Heartbeat
    }
}


/*

void setup()
{

    initSerial();
    pinMode(LED_BUILTIN, OUTPUT);
    messageHandler.begin();
    messageHandler.sendBuffer = &sendBuffer;

    //initBuffer();
    radiolink.maxTimeBetweenContactInMillis = config::CONNECTION_TIMEOUT;
    messageHandler.sendRegistrationRequest();
    radio.printDetails();
}
void loop()
{
    //readRadioToBuffer(&data[0]);
    //delayMicroseconds(700);

    if (regularCheckRequired)
    {
        if (!radiolink.regularCheck())
        {
            //radiolink.radio.printDetails();
            messageHandler.sendRegistrationRequest();
            //Serial.print(F("Sending Registration request\n"));
        };
        regularCheckRequired = false;
    }
    messageHandler.checkRadioAndHandleMessage();
    loadFromSerialToSendBuffer();
}

bool loadFromSerialToSendBuffer()
{
    if (Serial.available())
    {
        if (sendBuffer.isFull() ){

            sendBuffer.dropFirstMAVMessage();
        }
        sendBuffer.put(Serial.read());
        return true;
    }
    return false;
}
*/

void initSerial()
{
    Serial.begin(115200);
    printf_begin(); // for radio.printDetails()
};

void initBuffer()
{
    for (int i = 0; i < 128; i++)
    {
        sendBuffer.put(i);
    }
};
