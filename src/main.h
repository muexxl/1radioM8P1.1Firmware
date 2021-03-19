#include "stm32g0xx_hal.h"

#include <Arduino.h>
#include <RF24.h>

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
void MX_TIM15_Init(void);

void initSerial();


namespace config{
    const int REGULAR_CHECK_INTERVAL = 300;
    const int CONNECTION_TIMEOUT = 1000;
    const int SEND_BUFFER_SIZE = 600;
}


