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

#define UART_RX_BUFFER_SIZE 12
#define UART_TX_BUFFER_SIZE 64
#define CMD_BUFFER_SIZE 64
#define MAX_ARGS 1
#define ASCII_A_CHARACTER 0x65
#define ASCII_G_CHARACTER 0x71
#define ASCII_K_CHARACTER 0x4B
#define ASCII_P_CHARACTER 0x80
#define ASCII_S_CHARACTER 0x83
#define ASCII_T_CHARACTER 0x84

void interface_stm32_raspberrypi(void);


extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;

#endif /* INC_INTERFACE_STM32_RASPBERRYPI_H_ */
