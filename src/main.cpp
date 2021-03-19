#include <Arduino.h>
#include <SPI.h>

#include <mxtiming.h>

#include <main.h>


extern void SystemClock_Config(void);

extern bool is_due_2000ms;

void setup () {
    //0pinMode(LED_BUILTIN, OUTPUT);
    SystemClock_Config();
    Serial.begin(115200);
    MX_TIM15_Init();
    
}

void loop (){
    update_timers();
    if (is_due_2000ms) {
        is_due_2000ms = false;
        flash_LED(); // Heartbeat
    }
}

