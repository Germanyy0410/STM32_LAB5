/*
 * scheduler.h
 *
 *  Created on: 21 thg 11, 2023
 *      Author: ProX
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include "global.h"

typedef struct {
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
} sTask;

void SCH_Init(void);
void SCH_Update(void);
void SCH_Add_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period);
void SCH_Dispatcher_Task(void);
void SCH_Delete_Task(const uint32_t TASK_INDEX);

#endif /* INC_SCHEDULER_H_ */
