/*
 * encoder.h
 *
 *  Created on: 25 апр. 2022 г.
 *      Author: Rita-Margarita
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_
#include "main.h"

#define READ_ANGLE 0x3FFF // макросы команды для МОС�? из даташита
#define CLEAR_ERROR_FLAG 0x0001
#define READ_MOSI 0x4000 // подаем с МОС�? команду READ
#define WRITE_MOSI 0x0000 // подаем с МОС�? команду WRITE
#define PAR_MASK 0x8000 // бит четности
#define ERROR_FLAG 0x4000// бит ошибки
#define ANGLE_MASK 0x3FFF// маска, чтоб отбросить 15 и 14-ый биты
//#define DEBUG_KWA





typedef enum
{
	READ, WRITE
} en_MOSI_t; // перечисление команд на READ и WRITE от процессора

typedef enum
{
	OK, PAR_BIT, ERROR_FLAG_BIT
} en_error_t; // перечисление вида ошибок на прием


/**
  * @brief функция для обновления энкодера - периодическое событие
  * @param None
  * @retval None
  */
void encoder_update(void); // функция для обновления энкодера - периодическое событие

/**
 * @brief  Функция для того, чтоб узнать четность и выставить 15-ый бит;
 * @param package - пакет данных из 16 бит
 * @retval Возвращает четность (если четное, то 0 - остаток от деления на 2)
 */
uint8_t parity(uint32_t package);

/**
  * @brief функция для подготовки отправляемой команды
  * @param command - команда, которую мы посылаем на энкодер;
  * @param RWn - команда читает или записывает? это и отправляем
  * @retval package - пакет данных готовый к отправке
*/
uint32_t package_prep(uint32_t command, en_MOSI_t RWn);
/**
 * @brief
 * @param
 * @retval
 */
uint32_t get_angle(void);

/**
 * @brief обработчик прерывания
 * @param
 * @retval
 */
void int_package(void);



#endif /* INC_ENCODER_H_ */
