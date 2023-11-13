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

uint8_t uartRxReceived;
uint8_t uartRxBuffer[1];
char factor_K_buffer[1];

char *factor_K;

void interface_stm32_raspberrypi(void) {

	if (uartRxReceived) {
		printf("Com OK\r\n");
		/*switch (uartRxBuffer[4]) {
		case ASCII_A_CHARACTER:
			printf("test OK");
			//BMP280_get_angle();
		case ASCII_K_CHARACTER:
			printf("test OK");
			/*factor_K_buffer[0] = uartRxBuffer[0];
			if (strlen(factor_K_buffer) > 4) {
				factor_K = strtok(factor_K_buffer, "=");
				//BMP280_set_factor_K(factor_K);
			}
			else{
				printf("test OK");
			//BMP280_get_factor_K();
				}
		case ASCII_P_CHARACTER:
			printf("test OK");
			//BMP280_get_pressure();
		case ASCII_T_CHARACTER:
			printf("test OK");
			//BMP280_get_temperature();

			break;
		default:
			printf("Command not found");
		}*/
		uartRxReceived = 0;
	}
}
/*void Shell_Init(void) {
 memset(argv, (int) NULL, MAX_ARGS * sizeof(char*));
 memset(cmdBuffer, 0, CMD_BUFFER_SIZE * sizeof(char));
 memset(uartRxBuffer, 0, UART_RX_BUFFER_SIZE * sizeof(char));
 memset(uartTxBuffer, 0, UART_TX_BUFFER_SIZE * sizeof(char));

 HAL_UART_Receive_IT(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE);
 HAL_UART_Transmit(&huart2, prompt, strlen((char*) prompt), HAL_MAX_DELAY);
 }

 void Shell_Loop(void) {
 if (uartRxReceived) {
 switch (uartRxBuffer[0]) {
 case ASCII_CR: // Nouvelle ligne, instruction à traiter
 HAL_UART_Transmit(&huart2, newline, sizeof(newline), HAL_MAX_DELAY);
 cmdBuffer[idx_cmd] = '\0';
 argc = 0;
 token = strtok(cmdBuffer, " ");
 while (token != NULL) {
 argv[argc++] = token;
 token = strtok(NULL, " ");
 }
 idx_cmd = 0;
 newCmdReady = 1;
 break;
 case ASCII_BACK: // Suppression du dernier caractère
 cmdBuffer[idx_cmd--] = '\0';
 HAL_UART_Transmit(&huart2, backspace, sizeof(backspace),
 HAL_MAX_DELAY);
 break;

 default: // Nouveau caractère
 cmdBuffer[idx_cmd++] = uartRxBuffer[0];
 HAL_UART_Transmit(&huart2, uartRxBuffer, UART_RX_BUFFER_SIZE,
 HAL_MAX_DELAY);
 }
 uartRxReceived = 0;
 }

 if (newCmdReady) {
 if (strcmp(argv[0], "GET_T") == 0) {
 int uartTxStringLength = snprintf((char*) uartTxBuffer,
 UART_TX_BUFFER_SIZE, "T=12.5 _C\r\n");
 HAL_UART_Transmit(&huart2, uartTxBuffer, uartTxStringLength,
 HAL_MAX_DELAY);
 } else {
 HAL_UART_Transmit(&huart2, cmdNotFound, sizeof(cmdNotFound),
 HAL_MAX_DELAY);
 }
 HAL_UART_Transmit(&huart2, prompt, sizeof(prompt), HAL_MAX_DELAY);
 newCmdReady = 0;
 }
 }*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&huart4, uartRxBuffer, UART_RX_BUFFER_SIZE);
}
