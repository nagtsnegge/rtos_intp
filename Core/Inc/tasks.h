/*
 * tasks.h
 *
 *  Created on: Oct 20, 2022
 *      Author: nog8370
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "cmsis_os.h"

void DisplayTask();
void HandleUserButton();

extern osMessageQueueId_t displayQueue;
extern osSemaphoreId_t displaySem;

#endif /* INC_TASKS_H_ */
