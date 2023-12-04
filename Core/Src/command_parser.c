/*
 * command_parser.c
 *
 *  Created on: Dec 2, 2023
 *      Author: ProX
 */

#include "command_parser.h"

void command_parser_fsm() {
	uint8_t s[] = "Message from Command Parser\r\n";
	HAL_UART_Transmit(&huart2, s, sizeof(s), 100);

	switch(parser_status) {
	case IDLING:
		if (temp == '!') {
			parser_status = RECEIVING;
			command_index = 0;
		}

		break;

	case RECEIVING:
		if (temp == '#') {
			parser_status = IDLING;
			command[command_index] = '\0';
			command_flag = 1;
		}
		else {
			if (temp == '!') {
				command_index = 0;
			}
			else {
				// Check for Backspace
				if (temp == 8 || temp == 127) {
					command_index--;
				} else {
					command[command_index++] = temp;

					if (command_index == MAX_BUFFER_SIZE) {
						command_index = 0;
					}
				}

			}
		}

		break;
	default:
		break;
	}
}
