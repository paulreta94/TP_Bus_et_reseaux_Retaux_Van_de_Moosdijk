/*
 * interface_stm32_raspberrypi.c
 *
 *  Created on: Oct 17, 2023
 *      Author: peret
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal_uart.h"

uint8_t uartRxReceived = 0;
uint8_t short_uartRxBuffer[SHORT_UART_RX_BUFFER_SIZE];
uint8_t long_uartRxBuffer[LONG_UART_RX_BUFFER_SIZE];
uint8_t K;
extern uint8_t angle;

uint8_t temperature[] = "T=+12.50_C\r\n";

char *token;

void interface_stm32_raspberrypi(void) {

	if (uartRxReceived) {
		if (strcmp(short_uartRxBuffer, "GET_T") == 0) {
			//bmp280_compensate_T_int32();
			HAL_UART_Transmit(&huart1, temperature, sizeof(temperature), 100);
		}
		if (strcmp(short_uartRxBuffer, "GET_P") == 0) {
			bmp280_compensate_P_int64();
		}
//		if (strcmp(short_uartRxBuffer, "SET_K") == 0) {
//			uartRxReceived=0;
//		}
		if (strcmp(short_uartRxBuffer[0], "=") == 0) {
			token = strtok(short_uartRxBuffer,"=");
			K = *token;
			printf("K=%d\r\n",K);
		}
		if (strcmp(short_uartRxBuffer, "GET_A") == 0) {
			printf("A=%d\r\n",angle);
		}
		uartRxReceived = 0;
	}
}
//BMP280_get_angle();
/*case ASCII_K_CHARACTER:
 printf("test OK");
 factor_K_buffer[0] = uartRxBuffer[0];
 if (strlen(factor_K_buffer) > 4) {
 factor_K = strtok(factor_K_buffer, "=");
 //BMP280_set_factor_K(factor_K);
 } else {
 printf("test OK");
 //BMP280_get_factor_K();
 }
 case ASCII_P_CHARACTER:
 printf("test OK");
 //BMP280_get_pressure();
 case ASCII_T_CHARACTER:
 printf("test OK");


 break;
 default:
 printf("Command not found");
 }
 uartRxReceived = 0;
 }

 }*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart1, short_uartRxBuffer, SHORT_UART_RX_BUFFER_SIZE);
}

//void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
//	uartRxReceived = 1;
//	HAL_UART_Receive_IT(&huart1, long_uartRxBuffer, LONG_UART_RX_BUFFER_SIZE);
//}
