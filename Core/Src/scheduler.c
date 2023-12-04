/*
 * scheduler.c
 *
 *  Created on: 21 thg 11, 2023
 *      Author: ProX
 */

#include "scheduler.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"

#define SCH_MAX_TASKS 40

uint8_t taskList[SCH_MAX_TASKS], mutex_lock;

struct sTask {
	struct sTask *next, *prev;

	void (*Task)();
	uint32_t Delay;
	uint32_t Period;
	uint8_t Delay_Flag;
	uint32_t TaskID;
}*SCH_tasks_G = 0;

void SCH_Arrange_Task(struct sTask *index) {
	if (index == SCH_tasks_G) return;

	struct sTask *tempTask = SCH_tasks_G;
	uint8_t check = 0;

	while (tempTask->Delay <= index->Delay) {
		if (tempTask == SCH_tasks_G) check += 1;
		if (check == 2) break;
		index->Delay -= tempTask->Delay;
		tempTask = tempTask->next;
	}

	if (index->Delay <= 0) index->Delay_Flag = 1;

	index->next = tempTask;
	index->prev = tempTask->prev;
	tempTask->prev->next = index;
	tempTask->prev = index;

	if (check == 0) {
		tempTask->Delay -= index->Delay;
		if (tempTask->Delay <= 0) tempTask->Delay_Flag = 1;
		SCH_tasks_G = index;
	} else if (check == 1) {
		tempTask->Delay -= index->Delay;
		if (tempTask->Delay <= 0) tempTask->Delay_Flag = 1;
	}

}

void SCH_Add_Task(void (*pFunctionTask)(), uint32_t Delay, uint32_t Period) {
	if (!pFunctionTask) return;

	uint32_t index = 0;
	while ((index < SCH_MAX_TASKS) && (taskList[index])) {
		index += 1;
	}
	if (index == SCH_MAX_TASKS) {
		return;
	} else {
		taskList[index] = 1;
	}

	mutex_lock = 1;

	struct sTask *newTask = (struct sTask*)malloc(sizeof(struct sTask));
	newTask->Delay = Delay;
	newTask->Period = Period;
	newTask->Task = pFunctionTask;
	newTask->TaskID = index;
	newTask->Delay_Flag = 0;

	if (SCH_tasks_G) {
		SCH_Arrange_Task(newTask);
	} else {
		newTask->prev = SCH_tasks_G;
		newTask->next = SCH_tasks_G;
		SCH_tasks_G = newTask;
	}

	mutex_lock = 0;


}

void SCH_Update(void) {
	if ((SCH_tasks_G) && (!SCH_tasks_G->Delay_Flag) && (!mutex_lock)) {
		if (SCH_tasks_G->Delay > 0) {
			SCH_tasks_G->Delay -= 1;
		}
		if (SCH_tasks_G->Delay <= 0) {
			SCH_tasks_G->Delay_Flag = 1;
		}
	}
}

void SCH_Delete_Task(uint32_t TaskID) {
	if (taskList[TaskID]) {
		taskList[TaskID] = 0;

		struct sTask *toDelete = SCH_tasks_G;
		while (toDelete->TaskID != TaskID) {
			toDelete = toDelete->next;
		}

		toDelete->next->prev = toDelete->prev;
		toDelete->prev->next = toDelete->next;

		free(toDelete);
	}
}

void SCH_Dispatch_Tasks(void) {
	mutex_lock = 1;

	while ((SCH_tasks_G) && (SCH_tasks_G->Delay_Flag)) {
		(*SCH_tasks_G->Task)();
		if (SCH_tasks_G->Period == 0) {
			struct sTask *cur = SCH_tasks_G;
			SCH_tasks_G = SCH_tasks_G->next;
			SCH_Delete_Task(cur->TaskID);
		} else {
			struct sTask *cur = SCH_tasks_G;
			SCH_tasks_G->next->prev = SCH_tasks_G->prev;
			SCH_tasks_G->prev->next = SCH_tasks_G->next;
			SCH_tasks_G = SCH_tasks_G->next;
			cur->Delay_Flag = 0;
			cur->Delay = cur->Period;
			SCH_Arrange_Task(cur);
		}
	}

	mutex_lock = 0;

	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}


