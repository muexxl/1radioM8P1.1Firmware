

#ifndef MXTIMING_H
#define MXTIMING_H

#include <Arduino.h>

void MX_TIM15_Init();

void update_timers();
void flash_LED(int repetition = 1);
void every_1000_ms();



#endif // !MXTIMING_H