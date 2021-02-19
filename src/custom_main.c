

#include "custom_main.h"
#include "main.h"

int increment_counter_100hz;
int counter_100hz;
int counter_25hz;
int counter_10hz;
int counter_1hz;
//UART_HandleTypeDef huart2;
uint8_t UART2_rxBuffer[16] = {"   Hello World\n"};
int counter;
int is_ledflash_due = 0;
void setup()
{
    HAL_GPIO_WritePin(GPS_RST_GPIO_Port, GPS_RST_Pin, GPIO_PIN_SET);
    flash_LED();
    //Start Timer
    HAL_TIM_Base_Start_IT(&htim16);
}

void loop()
{
    // flash_LED();
    //send_Helloworld();
    asm("NOP");

    regular_timer_control();

    //HAL_GPIO_WritePin(MCU_LED_GPIO_Port, MCU_LED_Pin, GPIO_PIN_SET);
    //HAL_UART_Receive (&huart2, (uint8_t*)UART2_rxBuffer, 1, 1000);
    // //HAL_UART_Transmit(&huart2, UART2_rxBuffer, 12, 100);
    // //HAL_UART_Transmit_DMA(&huart2,&UART2_rxBuffer[0],12);
}

void regular_timer_control()
{
    int ticks = increment_counter_100hz;
    increment_counter_100hz = 0;
 while (ticks--)
        {
            counter_100hz++;
            regular_100Hz();
            if (counter_100hz % 4 == 0)
            {
                regular_25Hz();
            }
            if (counter_100hz % 10 == 0)
            {
                regular_10Hz();
            }
            if (counter_100hz % 100 == 0)
            {
                regular_1Hz();
            }
            if (counter_100hz % 200 == 0)
            {
                regular_0_5Hz();
                counter_100hz = 0;
            }
        }
    }


void regular_100Hz()
{
    ;
}

void regular_25Hz()
{
    ;
}

void regular_10Hz()
{
    ;
}

void regular_1Hz()
{
    ;
}


void regular_0_5Hz()
{
    HAL_UART_Transmit(&huart2, UART2_rxBuffer, 16, 100);
    flash_LED();
}

void flash_LED()
{
    HAL_GPIO_WritePin(MCU_LED_GPIO_Port, MCU_LED_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(MCU_LED_GPIO_Port, MCU_LED_Pin, GPIO_PIN_SET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(&huart2, UART2_rxBuffer, 12, 100);
    HAL_UART_Receive_DMA(&huart2, UART2_rxBuffer, 12);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim16)
    {
        increment_counter_100hz++;
    }
}

void send_Helloworld()
{
    //sprintf(&UART2_rxBuffer[0],"%02d",counter++);
    //UART2_rxBuffer[2]=' ';
    //HAL_UART_Transmit(&huart2, (uint8_t*)UART2_rxBuffer, 15, 100);
    HAL_UART_Transmit_DMA(&huart2, (uint8_t *)UART2_rxBuffer, 15);
    //HAL_UART_Transmit_IT(&huart2,(uint8_t*)UART2_rxBuffer,15);
}
