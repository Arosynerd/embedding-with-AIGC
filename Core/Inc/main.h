/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define Key_Shift_Pin GPIO_PIN_0
#define Key_Shift_GPIO_Port GPIOC
#define Key_Shift_EXTI_IRQn EXTI0_IRQn
#define Key_Left_Pin GPIO_PIN_1
#define Key_Left_GPIO_Port GPIOC
#define Key_Left_EXTI_IRQn EXTI1_IRQn
#define Key_Right_Pin GPIO_PIN_2
#define Key_Right_GPIO_Port GPIOC
#define Key_Right_EXTI_IRQn EXTI2_IRQn
#define DHT11_Pin GPIO_PIN_0
#define DHT11_GPIO_Port GPIOA
#define ST7735_BLK_Pin GPIO_PIN_8
#define ST7735_BLK_GPIO_Port GPIOE
#define ST7735_DC_Pin GPIO_PIN_15
#define ST7735_DC_GPIO_Port GPIOD
#define ST7735_CS_Pin GPIO_PIN_1
#define ST7735_CS_GPIO_Port GPIOD
#define ST7735_RST_Pin GPIO_PIN_4
#define ST7735_RST_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
