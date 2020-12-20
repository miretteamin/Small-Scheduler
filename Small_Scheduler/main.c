/*
* Small_Scheduler.c
*
* Created: 11/22/2020 8:17:53 PM
* Author : Mirette
*/

#include "Scheduler.h"

void LED0_Init(void)
{
	DIO_SetPinDir(DIO_PORTC, DIO_PIN2, DIO_PIN_OUTPUT);
}

void LED1_Init(void)
{
	DIO_SetPinDir(DIO_PORTC, DIO_PIN7, DIO_PIN_OUTPUT);
}

void LED0_Toggle(void)
{
	DIO_TogglePin(DIO_PORTC, DIO_PIN2);
}

void LED1_Toggle(void)
{
	DIO_TogglePin(DIO_PORTC, DIO_PIN7);
}

int main(void)
{
	LED0_Init();
	LED1_Init();
	
	Scheduler_Init();
	
	Scheduler_TaskCreate(LED0_Toggle, 1000, 1, 10);
	Scheduler_TaskCreate(LED1_Toggle, 500, 2, 20);
	
	Scheduler_Run();
	
	while (1)
	{
	}
}

