/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "custom_main.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define nrf_IRQ_Pin GPIO_PIN_4
#define nrf_IRQ_GPIO_Port GPIOA
#define nrf_CE_Pin GPIO_PIN_1
#define nrf_CE_GPIO_Port GPIOB
#define MCU_LED_Pin GPIO_PIN_15
#define MCU_LED_GPIO_Port GPIOB
#define GPS_EXT_INT_Pin GPIO_PIN_9
#define GPS_EXT_INT_GPIO_Port GPIOA
#define GPS_RST_Pin GPIO_PIN_6
#define GPS_RST_GPIO_Port GPIOC
#define GPS_Fence_Pin GPIO_PIN_10
#define GPS_Fence_GPIO_Port GPIOA
#define RF98_RST_Pin GPIO_PIN_15
#define RF98_RST_GPIO_Port GPIOA
#define RF98_DIO0_Pin GPIO_PIN_8
#define RF98_DIO0_GPIO_Port GPIOB
#define RF_98DIO1_Pin GPIO_PIN_9
#define RF_98DIO1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
