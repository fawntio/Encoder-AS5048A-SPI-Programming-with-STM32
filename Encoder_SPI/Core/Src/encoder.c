#include "encoder.h"

#define SPI_RES 16
en_error_t error = OK;
float angle;
uint8_t buffer_Tx[2] =
{ 0 }; //потому что нам нужно хранить 16 бит, а по умолчанию 1 буфер максимум 8 бит
uint8_t buffer_Rx[2] =
{ 0 };

uint32_t tx_package = 0; //готовый к отправке пакет
uint32_t rx_package = 0; //готовый к получению пакет
extern SPI_HandleTypeDef hspi1;

void encoder_update(void) // функция для обновления энкодера - периодическое событие
{

	if (parity(rx_package)) //проверка пакета на четность
	{
		error = PAR_BIT;
		printf("ERROR: Parity bit\r\n");
	}

	if (rx_package & ERROR_FLAG) //проверка пакета на флаг ошибки
	{
		/*error = ERROR_FLAG_BIT;
		printf("ERROR: Error flag bit\r\n");*/
	}

	switch (error)
	{
	case ERROR_FLAG_BIT:
	{

		tx_package = package_prep(CLEAR_ERROR_FLAG, READ);

		break;
	}
	case PAR_BIT:
	{
		//printf("par bit\r\n");
		tx_package = package_prep(READ_ANGLE, READ);
		break;
	}
	default:
	{
		//printf("ok\r\n");
		//HAL_Delay(1);
		tx_package = package_prep(READ_ANGLE, READ);
		angle = (rx_package & ANGLE_MASK) * 0.0219; // отбрасываем 14 и 15-ый биты
		printf("Angle = %f\r\n", angle);
		break;
	}
	}

	HAL_GPIO_WritePin(CSn_GPIO_Port, CSn_Pin, GPIO_PIN_RESET); //ставим Ч�?П СЕЛЕКТ в низкий уровень (начинаем передачу)
	//HAL_Delay(1); // tL-задержка, ставим 1 (мс) потому что можно только целое число

	/* & - возвращает адрес (указатель)
	 * 1 - какой СПИ используем (у нас СПИ1);
	 2 - буфер для отправки Transmit;
	 3 - буфер для приема Receive;
	 4 - размер
	 */

	buffer_Tx[0] = (uint8_t) (tx_package & 0xFF); //записываем первые 8 бит
	buffer_Tx[1] = (uint8_t) (tx_package >> 8); //записываем последние 8 бит

	//memcpy(buffer_Tx, tx_package, 2); //эта функция копирует пакет в буфер (пакет размером 2 байта)
	HAL_SPI_TransmitReceive_IT(&hspi1, buffer_Tx, buffer_Rx, 1);

#ifdef DEBUG_KWA
	printf("while cycle \r\n");
#endif

	error = OK;

	return;
}

void int_package(void)
{
	rx_package = (uint16_t) buffer_Rx[1];//запись из буфера данных с энкодера
	rx_package = rx_package << 8;
	rx_package |= buffer_Rx[0];
	//memcpy(rx_package, buffer_Rx, 2); //эта функция копирует из буфера в пакет (пакет размером 2 байта)
	HAL_GPIO_WritePin(CSn_GPIO_Port, CSn_Pin, GPIO_PIN_SET); //ставим Ч�?П СЕЛЕКТ в высокий уровень (заканчиваем передачу)
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); // мигаем светодиодом
}

uint8_t parity(uint32_t package)
{
	uint8_t bit_sum = 0;
	uint32_t command_temp = package;

#ifdef DEBUG_KWA
 printf("\r\npackage = %li \r\n", package);
 #endif

	for (uint8_t i = 0; i < SPI_RES; i++) //считаем сумму единиц в команде
	{
		bit_sum += command_temp & 1;
		command_temp = command_temp >> 1;
#ifdef DEBUG_KWA
 printf("i=%i command temp = %li bit_sum = %i \r\n", i, command_temp,
 bit_sum);
 HAL_Delay(1);
 #endif

	}
#ifdef DEBUG_KWA
 printf("bit_sum_delit_na_2 = %i \r\n", bit_sum % 2);
 #endif

	return bit_sum % 2;

}

uint32_t package_prep(uint32_t command, en_MOSI_t RWn)
{
	uint32_t package = command;

	if (RWn == READ) // проверка на 14 бит
	{
		package |= READ_MOSI;
	} //выставляем 1 в 14-ый бит для чтения данных
	else
	{
		package |= WRITE_MOSI;
	}

	package |= (parity(package)) << 15; //выставляем 1 или 0 в 15-ый бит для PAR бита четности

	return package;
}

uint32_t get_angle(void)
{
	return angle;
}
