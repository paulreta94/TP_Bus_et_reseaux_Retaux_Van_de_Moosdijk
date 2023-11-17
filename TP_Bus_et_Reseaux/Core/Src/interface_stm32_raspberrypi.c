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
uint8_t set_k_command_received = 0;
uint8_t short_uartRxBuffer[SHORT_UART_RX_BUFFER_SIZE];
float K;
uint8_t angle = 90; // in theory it should be retrieved from the ADXL345

uint8_t temperature[] = "T=+12.50_C\r\n";

char *token;

/* This function enables to retrieve the orders of the Raspberry transmitted via the serial port.
 * We call the function HAL_UART_Receive_IT in the HAL_UART_RxCpltCallback to reset the interruption.
 */

void interface_stm32_raspberrypi(void) {

	if (uartRxReceived) {
		if (strcmp(short_uartRxBuffer, "GET_T") == 0) {
			bmp280_compensate_T_int32();
		}
		if (strcmp(short_uartRxBuffer, "GET_P") == 0) {
			bmp280_compensate_P_int64();
		}
		if (strcmp(short_uartRxBuffer, "SET_K") == 0) {
			set_k_command_received = 1;
		}
		if (strcmp(short_uartRxBuffer[0], "=") == 0) {
			if (set_k_command_received) {
				token = strtok(short_uartRxBuffer, "=");
				K = *token;
				printf("SET_K=OK\r\n");
			}
		}
		if (strcmp(short_uartRxBuffer, "GET_K") == 0) {
			printf("K=%.5f\r\n", K);
		}
		if (strcmp(short_uartRxBuffer, "GET_A") == 0) {
			printf("A=%d\r\n", angle);
		}
		uartRxReceived = 0;
		act_on_radiator(); // defined in USER CODE # 4 of the main.c file
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart1, short_uartRxBuffer, SHORT_UART_RX_BUFFER_SIZE);
}
