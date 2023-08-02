/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define OLED_RS_Pin GPIO_PIN_5
#define OLED_RS_GPIO_Port GPIOC
#define OLED_CS_Pin GPIO_PIN_12
#define OLED_CS_GPIO_Port GPIOB
#define OLED_SCLK_Pin GPIO_PIN_13
#define OLED_SCLK_GPIO_Port GPIOB
#define OLED_RST_Pin GPIO_PIN_14
#define OLED_RST_GPIO_Port GPIOB
#define OLED_SDIN_Pin GPIO_PIN_15
#define OLED_SDIN_GPIO_Port GPIOB
#define CTRL_MOS_Pin GPIO_PIN_11
#define CTRL_MOS_GPIO_Port GPIOD
#define BR1_Pin GPIO_PIN_13
#define BR1_GPIO_Port GPIOD
#define BR2_Pin GPIO_PIN_15
#define BR2_GPIO_Port GPIOD
#define BR3_Pin GPIO_PIN_7
#define BR3_GPIO_Port GPIOC
#define BR4_Pin GPIO_PIN_9
#define BR4_GPIO_Port GPIOC
#define BC1_Pin GPIO_PIN_9
#define BC1_GPIO_Port GPIOA
#define BC2_Pin GPIO_PIN_11
#define BC2_GPIO_Port GPIOA
#define BC3_Pin GPIO_PIN_15
#define BC3_GPIO_Port GPIOA
#define BC4_Pin GPIO_PIN_11
#define BC4_GPIO_Port GPIOC
#define AR1_Pin GPIO_PIN_0
#define AR1_GPIO_Port GPIOD
#define AR2_Pin GPIO_PIN_2
#define AR2_GPIO_Port GPIOD
#define AR3_Pin GPIO_PIN_4
#define AR3_GPIO_Port GPIOD
#define AR4_Pin GPIO_PIN_6
#define AR4_GPIO_Port GPIOD
#define AC1_Pin GPIO_PIN_3
#define AC1_GPIO_Port GPIOB
#define AC2_Pin GPIO_PIN_6
#define AC2_GPIO_Port GPIOB
#define AC3_Pin GPIO_PIN_8
#define AC3_GPIO_Port GPIOB
#define AC4_Pin GPIO_PIN_0
#define AC4_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
