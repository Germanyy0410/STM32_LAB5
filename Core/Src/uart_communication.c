/*
 * uart_communication.c
 *
 *  Created on: Dec 2, 2023
 *      Author: ProX
 */

#include "uart_communication.h"


void uart_communiation_fsm_run() {
	switch(uart_status) {
	case WAIT_RST:
		if (command_flag == 1) {
			command_flag = 0;

			if (command[0] == 'R' && command[1] == 'S' && command[2] == 'T' && command_index == 3) {
				HAL_ADC_Start(&hadc1);
				ADC_value = HAL_ADC_GetValue(&hadc1);
				HAL_ADC_Stop(&hadc1);

				HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "\r\n"), 1000);
				uart_status = SEND_ADC;
				setTimer(1, 3000);
			}
		}

		break;

	case SEND_ADC:
		HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "!ADC=%lu#\r\n", ADC_value), 1000);
		uart_status = WAIT_OK;
		break;

	case WAIT_OK:
		if (command_flag == 1) {
			command_flag = 0;

			if (command[0] == 'O' && command[1] == 'K' && command_index == 2) {
				HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "\r\n"), 1000);
				uart_status = WAIT_RST;
				clearTimer(1);
			}
		}

		if (timer_flag[1] == 1) {
			uart_status = SEND_ADC;
			setTimer(1, 3000);
		}

		break;

	default:
		break;
	}
}
