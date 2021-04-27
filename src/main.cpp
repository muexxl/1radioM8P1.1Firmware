#include <main.h>
#include <config.h>
#include <Arduino.h>
#include <variant.h>

#include <SPI.h>
#include <mxsupport.h>
#include <mxtiming.h>

#include <radioconfig.h>
#include <configManager.h>
#include <mxsupport.h>
#include <radiolink.h>
#include <messagehandler.h>
#include <circularbuffer.h>
#include <RF24.h>
#include <ublox.h>
#include <setup.h>
#include <Wire.h>

 uint8_t testmsg1230[] = {0xff, 0xD3, 0x00, 0x06, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B};

 uint8_t testmsg1087[] = {0xff, 0xD3, 0x00, 0x33, 0x43, 0xF0, 0x00, 0x2E, 0xD5, 0xDA, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x69, 0x69, 0xA1, 0x2D, 0x19, 0x94, 0x17, 0x58, 0x5C, 0x85, 0xC9, 0x97, 0xB3, 0x47, 0x61, 0x45, 0xB8, 0x7E, 0xAB, 0xD0, 0x8A, 0x2A, 0x8A, 0x4E, 0x10, 0x81, 0x34, 0x70, 0xC9, 0xA0, 0x46, 0x6C, 0x9B};

 uint8_t testmsg1077[] = {0xff, 0xD3, 0x00, 0x5F, 0x43, 0x50, 0x00, 0x20, 0xDD, 0x44, 0xE2, 0x00, 0x00, 0x04, 0x00, 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x7D, 0x39, 0x35, 0x19, 0x15, 0x20, 0x00, 0x00, 0x19, 0xA0, 0xB6, 0xB7, 0xD0, 0xFF, 0xA0, 0x1B, 0x42, 0x63, 0x05, 0xFF, 0xEB, 0xAF, 0x98, 0xF5, 0xC8, 0x67, 0xB0, 0x78, 0x87, 0x7B, 0xFB, 0x8F, 0x3B, 0x4B, 0x2D, 0xA7, 0xD0, 0x58, 0xA3, 0xEB, 0x7E, 0x8C, 0x20, 0xAC, 0xEF, 0xE2, 0xB0, 0x04, 0x29, 0x19, 0xFD, 0x3E, 0x52, 0x94, 0x95, 0x29, 0x48, 0x03, 0x81, 0x48, 0x40, 0x0F, 0x83, 0xC0, 0x69, 0x7C, 0x41, 0x89, 0x42, 0x6F, 0x17, 0xB7, 0x40, 0xC0, 0xCF, 0x01, 0xD3};

 uint8_t testmsg3[] = {0xD3, 0x00, 0x06, 0x4C, 0xE0, 0x00, 0x88, 0x10, 0x97, 0xC2, 0x44, 0x8B,
                   0xD3, 0x00, 0x33, 0x43, 0xF0, 0x00,
                   0x2E, 0xD5, 0xDA, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0x00,
                   0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x69, 0x69, 0xA1, 0x2D,
                   0x19, 0x94, 0x17, 0x58, 0x5C, 0x85, 0xC9, 0x97, 0xB3, 0x47, 0x61,
                   0x45, 0xB8, 0x7E, 0xAB, 0xD0, 0x8A, 0x2A, 0x8A, 0x4E, 0x10, 0x81,
                   0x34, 0x70, 0xC9, 0xA0, 0x46, 0x6C, 0x9B,
                   0xD3, 0x00, 0x5F, 0x43, 0x50, 0x00, 0x20, 0xDD, 0x44, 0xE2, 0x00, 0x00, 0x04, 0x00, 0x80,
                   0x25, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x7D, 0x39,
                   0x35, 0x19, 0x15, 0x20, 0x00, 0x00, 0x19, 0xA0, 0xB6, 0xB7, 0xD0,
                   0xFF, 0xA0, 0x1B, 0x42, 0x63, 0x05, 0xFF, 0xEB, 0xAF, 0x98, 0xF5,
                   0xC8, 0x67, 0xB0, 0x78, 0x87, 0x7B, 0xFB, 0x8F, 0x3B, 0x4B, 0x2D,
                   0xA7, 0xD0, 0x58, 0xA3, 0xEB, 0x7E, 0x8C, 0x20, 0xAC, 0xEF, 0xE2,
                   0xB0, 0x04, 0x29, 0x19, 0xFD, 0x3E, 0x52, 0x94, 0x95, 0x29, 0x48,
                   0x03, 0x81, 0x48, 0x40, 0x0F, 0x83, 0xC0, 0x69, 0x7C, 0x41, 0x89,
                   0x42, 0x6F, 0x17, 0xB7, 0x40, 0xC0, 0xCF, 0x01, 0xD3};

extern bool is_due_2000ms;
extern bool is_due_1000ms;
extern bool is_due_500ms;
extern bool is_due_200ms;
bool loadFromSerialToSendBuffer();
int counter{0};

CircularBuffer sendBuffer(config::SEND_BUFFER_SIZE);
CircularBuffer rx_buffer_broadcast(200);
CircularBuffer rx_rtcm_buffer(config::RTCM_BUFFER_SIZE);

I2C_HandleTypeDef hi2c2;

UBlox ublox;
MessageHandler messageHandler;

int time1, time2;
Radiolink &radiolink{messageHandler.radiolink};
RadioConfig &radioConfig{radiolink.config};
RF24 &radio{radiolink.radio};

uint8_t data[] = " Hello World!";
u_int16_t len{10};
HAL_StatusTypeDef answer;
HAL_I2C_StateTypeDef status;

void send_test_msg();

void setup()
{
    //0pinMode(LED_BUILTIN, OUTPUT);
    SystemClock_Config();
    MX_TIM15_Init(); //for flashing LED
    MX_I2C2_Init();
    initSerial();
    messageHandler.begin();
    
    ;
    //
}

void loop()
{
    update_timers();
    //ublox.send(&data[0], len);
    if (is_due_2000ms)
    {
        is_due_2000ms = false;
        //flash_LED(); // Heartbeat

        //         if (!radiolink.regularCheck())
        //         {
        //             //messageHandler.sendRegistrationRequest();
        // #ifdef VERBOSE

        //             Serial.print(("Sending Registration request\n"));
        // #endif // VERBOSE
        //         };
    }
    if (is_due_500ms)
    {
        send_test_msg();
        send_test_msg();
        is_due_500ms = false;
    }
    if (is_due_1000ms)
    {
        //send_test_msg();
        is_due_1000ms = false;
        //send_test_msg();
    }

    //messageHandler.checkRadioAndHandleMessage();
    //loadFromSerialToSendBuffer();
    ublox.check_rtcm_buffer_and_send_via_i2c();
}

void send_test_msg()
{
    // if (hi2c2.State == HAL_I2C_STATE_READY)
    // {

    //     if (counter%3 ==0){
    //         HAL_I2C_Master_Transmit_IT(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg1230[0], 12);
    //     }else if (counter%3==1)
    //     {
    //         HAL_I2C_Master_Transmit_IT(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg1[0], 57);
    //     }else if (counter%3==2)
    //     {
    //         HAL_I2C_Master_Transmit_IT(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg2[0], 101);
    //     }

    //     counter++;
    //     //HAL_I2C_Master_Transmit_IT(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg3[0], 170);
    // }
    //HAL_I2C_Master_Transmit_IT(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg3[0], 170);
    // HAL_I2C_Master_Transmit(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg1230[0], 14, 100);
    // HAL_Delay(5);

    //HAL_I2C_Master_Transmit(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg1230[0], 13, 100);
    //HAL_I2C_Master_Transmit(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg1230[0], 13, 100);
    //HAL_I2C_Master_Transmit(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg1087[0], 58, 100);
    //HAL_I2C_Master_Transmit(&hi2c2, 0x42 << 1, (uint8_t *)&testmsg1077[0], 102, 100);

    ublox.add_to_RTCM_buffer(&testmsg3[0], 170);
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
        if (sendBuffer.isFull())
        {

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
