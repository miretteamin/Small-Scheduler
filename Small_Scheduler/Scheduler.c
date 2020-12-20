/*
* Scheduler.c
*
* Created: 11/22/2020 8:53:07 PM
*  Author: Mirette
*/

#include "Scheduler.h"

void Scheduler_Init(void)
{
	Timer_Init();
	Timer_SetDelay(1);
	TIMER0_SetCallBack_1(Scheduler_LongTerm);
	Timer_Start();
}

void Scheduler_TaskCreate(void(*ptr)(void), uint16 Priodicity, uint16 Priority, uint16 ID)
{
	static uint8 task_nb = 0;
	
	System_Tasks[task_nb].task_ptr = ptr;
	System_Tasks[task_nb].Priodicity = Priodicity;
	System_Tasks[task_nb].Priority = Priority;
	System_Tasks[task_nb].Task_ID = ID;
	System_Tasks[task_nb].Time = Priodicity;
	System_Tasks[task_nb].status = Ready;
	
	task_nb++;
}

void Scheduler_DeleteTask(uint16 ID)
{
	uint8 task_nb = 0;
	
	for (task_nb = 0; task_nb < Total_System_Tasks; task_nb++)
	{
		if(System_Tasks[task_nb].Task_ID == ID)
		{
			System_Tasks[task_nb].status = Deleted;
			break;
		}
	}
}

void Scheduler_LongTerm(void)//CallBack
{
	uint8 task_cnt = 0;
	
	for (task_cnt = 0; task_cnt < Total_System_Tasks; task_cnt++)
	{
		if(System_Tasks[task_cnt].status == Waiting)
		{
			System_Tasks[task_cnt].Time--;
			if(System_Tasks[task_cnt].Time == 0)
			{
				System_Tasks[task_cnt].status = Ready;
				System_Tasks[task_cnt].Time = System_Tasks[task_cnt].Priodicity;
			}
		}
	}
}


void Scheduler_Run(void)
{
	uint8 task_cnt = 0;
	static uint8 maxPriority = 0;
	uint8 executed_task = 0;
	
	while(1)
	{
		for (task_cnt = 0; task_cnt < Total_System_Tasks; task_cnt++)
		{
			if(System_Tasks[task_cnt].status == Ready)
			{
				if(maxPriority <= System_Tasks[task_cnt].Priority)
				{
					maxPriority = System_Tasks[task_cnt].Priority;
					executed_task =	task_cnt;
				}
			}
		}
		System_Tasks[executed_task].task_ptr();
		System_Tasks[executed_task].status = Waiting;
		
		maxPriority = 0;
	}
}