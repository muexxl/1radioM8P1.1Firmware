#include <Arduino.h>
#include <variant.h>

#include <SPI.h>

#include <mxsupport.h>
#include <mxtiming.h>

#include <main.h>

#include <radioconfig.h>
#include <configManager.h>
#include <mxsupport.h>
#include <radiolink.h>
#include <messagehandler.h>
#include <circularbuffer.h>
#include <RF24.h>
#include <ublox.h>
#include <setup.h>

extern bool is_due_2000ms;
 bool loadFromSerialToSendBuffer();

CircularBuffer sendBuffer(config::SEND_BUFFER_SIZE);
CircularBuffer rx_buffer_broadcast(200);
CircularBuffer rx_rtcm_buffer(config::RTCM_BUFFER_SIZE);

I2C_HandleTypeDef hi2c2;

UBlox ublox;
MessageHandler messageHandler;

int time1, time2;
uint8_t counter;
Radiolink &radiolink{messageHandler.radiolink};
RadioConfig &radioConfig{radiolink.config};
RF24 &radio{radiolink.radio};

uint8_t data[]= " Hello World!";
u_int16_t len{10};
HAL_StatusTypeDef answer;
void setup () {
    //0pinMode(LED_BUILTIN, OUTPUT);
    SystemClock_Config();
    MX_TIM15_Init(); //for flashing LED
    MX_I2C2_Init();
    initSerial();
    messageHandler.begin();
    //messageHandler.sendBuffer = &sendBuffer;
    radiolink.maxTimeBetweenContactInMillis = config::CONNECTION_TIMEOUT;
    //messageHandler.sendRegistrationRequest();    
    data[0] = 0xf0; //register address ?!
    data[0] = 0xf1; //register address ?!
    data[0] = 0xf2; //register address ?!
    //
}

void loop (){
    update_timers();
    if (is_due_2000ms) {
        is_due_2000ms = false;
        flash_LED(); // Heartbeat
        answer = HAL_I2C_Master_Transmit_IT(&hi2c2,0x42<<1,&data[0],len);
        answer = HAL_BUSY;
        if (!radiolink.regularCheck())
        {
            messageHandler.sendRegistrationRequest();
            #ifdef VERBOSE
            
            Serial.print(("Sending Registration request\n"));
            #endif // VERBOSE
        };
    }
    //messageHandler.checkRadioAndHandleMessage();
    //loadFromSerialToSendBuffer();
    // Wire.beginTransmission(0x42);
    // Wire.write(0xff);
    // Wire.endTransmission();
    
}

// int _write (int file, char * ptr, int len)
// {
//   if ( (file != 1) && (file != 2) )
//     return 0;
//   else
//     Serial.write(ptr, len);
//   return len;
// }
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
*/
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

void initSerial()
{
    Serial.begin(115200);
    //printf_begin(); // for radio.printDetails()
};

void initBuffer()
{
    for (int i = 0; i < 128; i++)
    {
        sendBuffer.put(i);
    }
};

void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef * hi2c2)
{
	answer = HAL_BUSY;
	//flash_LED();// TX Done .. Do Something!

}