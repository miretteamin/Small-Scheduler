/*
* Scheduler.h
*
* Created: 11/22/2020 8:53:26 PM
*  Author: Mirette
*/


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Timer.h"
#include "DIO.h"

#define Total_System_Tasks  2

typedef enum
{
	Waiting,
	Running,
	Ready,
	Deleted
}Task_State;

typedef struct
{
	void(*task_ptr)(void);
	Task_State status;
	uint16 Priodicity;
	uint16 Priority;
	uint16 Task_ID;
	uint16 Time;
}TaskControlBlock;

TaskControlBlock System_Tasks[Total_System_Tasks];

void Scheduler_Init(void);

void Scheduler_TaskCreate(void(*ptr)(void), uint16 Priodicity, uint16 Priority, uint16 ID);

void Scheduler_DeleteTask(uint16 ID);

void Scheduler_LongTerm(void); //CallBack

void Scheduler_Run(void);

#endif /* SCHEDULER_H_ */