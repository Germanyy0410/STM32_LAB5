/*
 * software_timer.h
 *
 *  Created on: 3 thg 12, 2023
 *      Author: ProX
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "global.h"

#define NUM_OF_TIMERS 2

extern int TIME_CYCLE;
extern int timer_flag[NUM_OF_TIMERS];

void setTimer(int index, int duration);
void clearTimer(int index);
void timerRun();

#endif /* INC_SOFTWARE_TIMER_H_ */
