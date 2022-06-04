/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
	READ, WRITE
} en_MOSI_t; // перечисление команд на READ и WRITE от процессора

typedef enum
{
	OK, PAR_BIT, ERROR_FLAG_BIT
} en_error_t; // перечисление вида ошибок на прием

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define READ_ANGLE 0x3FFF // макросы команды для МОСИ из даташита
#define CLEAR_ERROR_FLAG 0x0001
#define READ_MOSI 0x4000 // подаем с МОСИ команду READ
#define WRITE_MOSI 0x0000 // подаем с МОСИ команду WRITE
#define PAR_MASK 0x8000 // бит четности
#define ERROR_FLAG 0x4000// бит ошибки
#define ANGLE_MASK 0x3FFF// маска, чтоб отбросить 15 и 14-ый биты
//#define DEBUG_KWA
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint32_t get_angle(void);
uint32_t SPI_package(uint32_t command, en_MOSI_t RWn);
uint8_t parity(uint32_t command);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CSn_Pin GPIO_PIN_4
#define CSn_GPIO_Port GPIOA
#define CLK_Pin GPIO_PIN_5
#define CLK_GPIO_Port GPIOA
#define MISO_Pin GPIO_PIN_6
#define MISO_GPIO_Port GPIOA
#define MOSI_Pin GPIO_PIN_7
#define MOSI_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
