/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SPI_RES 16
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
en_error_t error = OK;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	uint32_t angle;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USB_DEVICE_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		angle = get_angle();
		HAL_Delay(1);

#ifdef DEBUG_KWA
		printf("while cycle \r\n");
#endif

		switch (error)
		{
		case ERROR_FLAG_BIT:
		{
			uint32_t flag_response = 0;
			while (error == ERROR_FLAG_BIT)
			{
				HAL_GPIO_WritePin(CSn_GPIO_Port, CSn_Pin, GPIO_PIN_RESET); //ставим Ч�?П СЕЛЕКТ в низкий уровень (начинаем передачу)
				HAL_Delay(1); // tL-задержка, ставим 1 (мс) потому что можно только целое число

				//SPI_package(CLEAR_ERROR_FLAG, READ); //объявляем переменную result
				printf("  CLEAR FLAG COOMAND \r\n");
				flag_response = SPI_package(CLEAR_ERROR_FLAG, READ);
				//SPI_package(READ_ANGLE, READ); //объявляем переменную result

				HAL_Delay(1); // tH-задержка
				HAL_GPIO_WritePin(CSn_GPIO_Port, CSn_Pin, GPIO_PIN_SET); //ставим Ч�?П СЕЛЕКТ в высокий уровень (заканчиваем передачу)

				if (flag_response & ERROR_FLAG) // проверка на флаг - исправился или нет?
				{
					HAL_Delay(10);
				}
				else
				{
					error = OK;
				}

			}

			break;
		}
		case PAR_BIT:
		{
			printf("par bit\r\n");
			break;
		}
		default:
		{
			printf("ok\r\n");
			break;
		}
		}

		error = OK;
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1); //для того, чтоб мигать светодиодом
		HAL_Delay(10); // задержка для светодиода

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 | CSn_Pin | CLK_Pin | MOSI_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : PA1 CSn_Pin CLK_Pin MOSI_Pin */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | CSn_Pin | CLK_Pin | MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : MISO_Pin */
	GPIO_InitStruct.Pin = MISO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(MISO_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
uint8_t parity(uint32_t command)
{
	uint8_t bit_sum = 0;
	uint32_t command_temp = command;

#ifdef DEBUG_KWA
	printf("\r\ncommand = %li \r\n", command);
#endif

	for (uint8_t i = 0; i < SPI_RES; i++) //считаем сумму единиц в команде
	{
		bit_sum += command_temp & 1;
		command_temp = command_temp >> 1;
#ifdef DEBUG_KWA
		printf("i=%i command temp = %li bit_sum = %i \r\n", i, command_temp,
				bit_sum);
#endif

		HAL_Delay(1);
	}
#ifdef DEBUG_KWA
	printf("bit_sum_delit_na_2 = %i \r\n", bit_sum % 2);
#endif

	return bit_sum % 2;

}
/**
 * @brief  Функция для того, чтоб получать пакет данных с энкодера (16 бит) и посылать команды с контроллера;
 * SPI_package - название функции, а в скобках - что она принимает на вход, если ничего, то пишем void
 * @retval Возвращает пакет данных 16 бит
 */
uint32_t SPI_package(uint32_t command, en_MOSI_t RWn)
{
	uint32_t result = 0; //объявляем переменную result
	uint8_t result_bit = 0; //

	if (RWn == READ) // проверка на 14 бит
	{
		command |= READ_MOSI;
	} //выставляем 1 в 14-ый бит для чтения данных
	else
	{
		command |= WRITE_MOSI;
	}

	command |= (parity(command)) << 15; //выставляем 1 или 0 в 15-ый бит для PAR бита четности

	for (uint8_t i = 0; i < SPI_RES; i++) //счетчик битов/клоков
	{
		if (command & (1 << (SPI_RES - 1))) //умножаем на 100...00 в зависимости от размера SPI
		{
			HAL_GPIO_WritePin(MOSI_GPIO_Port, MOSI_Pin, GPIO_PIN_SET); //тут мы ставим 1, если разраяд команды равен 1
		}
		else
		{
			HAL_GPIO_WritePin(MOSI_GPIO_Port, MOSI_Pin, GPIO_PIN_RESET); //тут мы ставим 0, если разраяд команды равен 0
		}
		command = command << 1; //сдвигаем команду на 1, чтоб начать работу с новым разрядом

		HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET); //ставим клок
		HAL_Delay(1); //tcklH
		HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET); //ставим клок
		result = result << 1; // сдвигаем число на 1 влево побитово
		result_bit = HAL_GPIO_ReadPin(MISO_GPIO_Port, MISO_Pin);
		result |= result_bit; // побитово складываем число (побитовое �?Л�?) со значением М�?СО
		HAL_Delay(1); //tcklL

		printf("%c  ", result_bit + 0x30); //для COM-порта

		//мы добавляем 0x30 в 16-тиричной системе, чтобы числовые значения корректно отображались на экране (ASCII-таблица)

	}

	return result; //функция коончилась - возвращаем переменную angle
}

/**
 * @brief  Функция для того, чтоб получать угол с энкодера; uint16_t - это тип числа и какого оно размера (16 бит);
 * angle - название функции, а в скобках - что она принимает на вход, если ничего, то пишем void
 * @retval Возвращает угол angle
 */
uint32_t get_angle(void)
{
	HAL_GPIO_WritePin(CSn_GPIO_Port, CSn_Pin, GPIO_PIN_RESET); //ставим Ч�?П СЕЛЕКТ в низкий уровень (начинаем передачу)
	HAL_Delay(1); // tL-задержка, ставим 1 (мс) потому что можно только целое число

	uint32_t result = SPI_package(READ_ANGLE, READ); //объявляем переменную result

	HAL_Delay(1); // tH-задержка
	HAL_GPIO_WritePin(CSn_GPIO_Port, CSn_Pin, GPIO_PIN_SET); //ставим Ч�?П СЕЛЕКТ в высокий уровень (заканчиваем передачу)

	if (parity(result)) //проверка пакета на четность
	{
		error = PAR_BIT;
		printf("ERROR: Parity bit\r\n");
		return 0;
	}

	if (result & ERROR_FLAG) //проверка пакета на флаг ошибки
	{
		error = ERROR_FLAG_BIT;
		printf("ERROR: Error flag bit\r\n");
		return 0;
	}

	result = result & ANGLE_MASK; // отбрасываем 14 и 15-ый биты
	printf("%f", result * 0.0219);

	printf("\r\n");
	return result;

}

int _write(int file, char *ptr, int len)
{
	CDC_Transmit_FS((uint8_t*) ptr, len);
	return len;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
