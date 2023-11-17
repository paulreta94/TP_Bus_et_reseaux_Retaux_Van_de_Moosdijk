/*
 * interface_STM32_RaspberryPi.h
 *
 *  Created on: Oct 17, 2023
 *      Author: peret
 */

#ifndef INC_INTERFACE_STM32_RASPBERRYPI_H_
#define INC_INTERFACE_STM32_RASPBERRYPI_H_

#include "main.h"
#include <stdlib.h>
#include <stdio.h>

#define SHORT_UART_RX_BUFFER_SIZE 5

void interface_stm32_raspberrypi(void);

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

#endif /* INC_INTERFACE_STM32_RASPBERRYPI_H_ */
