/*
 * Timer.h
 *
 * Created: 17/07/2020 12:28:44 م
 *  Author: Ali
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "ATMEGA32_Regs.h"

void Timer_Init(void);

void Timer_Start(void);

void Timer_Stop(void);

void Timer_SetDelay(uint32 Delay_Ms);

void TIMER0_SetCallBack_1(void(*ptr)(void));
void TIMER0_SetCallBack_2(void(*ptr)(void));



void Timer1_Init(void);

void Timer1_Start(void);

void Timer1_Stop(void);

void Timer1_SetDelay(uint32 Delay_Ms);

void TIMER1_SetCallBack(void(*ptr)(void));



void PWM0_Init(void);

void PWM0_Generate(uint8 DutyCycle);

void PWM0_Start(void);



void PWM1_Init(void);

void PWM1_Generate(uint32 DutyCycle);

void PWM1_Start(void);


void ICU_Init(void);


//uint32 ICU_Capture(void);



#endif /* TIMER_H_ */