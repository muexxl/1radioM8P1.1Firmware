
#ifndef CUSTOM_MAIN_H
#define CUSTOM_MAIN_H

#include "main.h"

extern I2C_HandleTypeDef hi2c2;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim16;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

void setup();
void loop();
void flash_LED();

void regular_100Hz();
void regular_25Hz();
void regular_10Hz();
void regular_1Hz();
void regular_0_5Hz();

void send_Helloworld();
void regular_timer_control();

#endif // CUSTOM_MAIN_H
