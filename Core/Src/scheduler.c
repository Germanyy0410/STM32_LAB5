/*
 * scheduler.c
 *
 *  Created on: 21 thg 11, 2023
 *      Author: ProX
 */

#include "scheduler.h"

#define SCH_MAX_TASKS 40
#define NO_TASK_ID 0

sTask SCH_tasks_G[SCH_MAX_TASKS];

uint32_t current_index_task = 0;

void SCH_Init(void) {
	current_index_task = 0;
}

void SCH_Update(void) {
	for (int Index = 0; Index < current_index_task; Index++) {
		if (SCH_tasks_G[Index].Delay > 0) {
			SCH_tasks_G[Index].Delay--;
		} else {
			SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
			SCH_tasks_G[Index].RunMe++;
		}
	}
}

void SCH_Add_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period) {
	if(current_index_task < SCH_MAX_TASKS) {
		SCH_tasks_G[current_index_task].pTask = pFunction;
	    SCH_tasks_G[current_index_task].Delay = Delay;
	    SCH_tasks_G[current_index_task].Period = Period;
	    SCH_tasks_G[current_index_task].RunMe = 0;
	    SCH_tasks_G[current_index_task].TaskID = current_index_task;

	    current_index_task++;
	}
}

void SCH_Dispatcher_Task(void) {
	for(int Index = 0; Index < current_index_task; Index++) {
	    if(SCH_tasks_G[Index].RunMe > 0) {
	      (*SCH_tasks_G[Index].pTask)();
	      SCH_tasks_G[Index].RunMe--;
	      if(SCH_tasks_G[Index].Period == 0){
	    	  SCH_Delete_Task(Index);
	      }
	    }
	}
	// Enter low-power mode (Sleep mode). The MCU will wake up on the next interrupt
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void SCH_Delete_Task(const uint32_t TASK_INDEX) {
	if (TASK_INDEX >= current_index_task){ //
		return;
	} else {
		for (int i = TASK_INDEX; i < current_index_task - 1; i++){
			SCH_tasks_G[i].pTask  = SCH_tasks_G[i+1].pTask;
			SCH_tasks_G[i].Period = SCH_tasks_G[i+1].Period;
			SCH_tasks_G[i].RunMe  = SCH_tasks_G[i+1].RunMe;
			SCH_tasks_G[i].Delay  = SCH_tasks_G[i+1].Delay;
		}

		current_index_task--;
	}
}
