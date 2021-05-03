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
    flash_LED();
}

void loop()
{
    update_timers();
    if (is_due_1000ms)
    {
        is_due_1000ms = false;
        //flash_LED(); // Heartbeat

        //ublox.rx_rtcm_buffer.print();
        //ublox.rx_rtcm_buffer.print_sorted();
        //ublox.rx_rtcm_buffer.reset();
        Serial.printf("Lost Packets %d \n",messageHandler.lost_rtcm_packets);
        if (!radiolink.regularCheck())
        {
            //messageHandler.sendRegistrationRequest();
#ifdef VERBOSE

            Serial.print(("Sending Registration request\n"));
#endif // VERBOSE
        };
    }

    messageHandler.checkRadioAndHandleMessage();
    loadFromSerialToSendBuffer();
    ublox.check_rtcm_buffer_and_send_via_i2c();
}

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
