/*
 * Timer.c
 *
 * Created: 17/07/2020 12:28:31 م
 *  Author: Ali
 */ 

#include <avr/interrupt.h>
//#include "Timer.h"
#include "BIT_Math.h"
#include "STD.h"

 static void(*timer0_ptr_func)(void);
  static void(*timer0_ptr_func_2)(void);

 static uint8 CounterRegister_InitValue = 0;
 static uint32 Number_OverFlows = 0;


static void(*timer1_ptr_func)(void);
uint32 Num_CompMatch = 0 ;


static uint32 Timer1_Number_OverFlows = 0;


 volatile uint32 ICU_ReadCapturedVal_1 =0;
 volatile uint32 ICU_ReadCapturedVal_2 =0;
 volatile uint32 ICU_ReadCapturedVal_3 =0;
 volatile uint8 Cap_Counter = 0;
  

void Timer_Init(void)
{
	/*Initilize Timer 0  as Normal mode */
	TCCR0 |= 0x00 ;
	
	TIMSK |= 0x01;
	
	SREG |= 0x80;
	
}

void Timer_Start(void)
{
	
	/* start timer with prescaler   1024 */
	TCCR0 |= 0x05;
	
}

void Timer_Stop(void)
{
	
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
	
}


void Timer_SetDelay(uint32 Delay_Ms)
{
	/* tick time in micro second */
	uint8 Tick_Time_us = (1024 / 16);
	
	uint32 Total_Ticks = (Delay_Ms * 1000) / Tick_Time_us;
	
    Number_OverFlows = Total_Ticks / 256 ;
	
	CounterRegister_InitValue = 256 - (Total_Ticks % 256) ; //256 - ((  ((float) Total_Ticks / 256 ) - Number_OverFlows ) * 256 );
	
	
	TCNT0 = CounterRegister_InitValue ;
	
	Number_OverFlows ++;
	
}

void TIMER0_SetCallBack_1(void(*ptr)(void))
{
	
	timer0_ptr_func  =  ptr;
}

void TIMER0_SetCallBack_2(void(*ptr)(void))
{
	
	timer0_ptr_func_2 = ptr;
}


ISR(TIMER0_OVF_vect)
{
	
	static uint32 counter = 0;
	
	counter++;
	
	if(counter == Number_OverFlows)
	{
		TCNT0 = CounterRegister_InitValue;
		
		counter = 0;
		
		(*timer0_ptr_func)();
		(*timer0_ptr_func_2)();
		
	}
	
	
}


void Timer1_Init(void)
{
	TCCR1B |= 0x08 ;
	TIMSK |= 0X10;
	SREG  |= 0x80;
}

void Timer1_Start(void)
{
	TCCR1B |= 0x04 ;
}

void Timer1_Stop(void)
{
	
CLR_BIT(TCCR1B,0);
CLR_BIT(TCCR1B,1);
CLR_BIT(TCCR1B,2);

}

void Timer1_SetDelay(uint32 Delay_Ms)
{
	if(Delay_Ms < 4000)
	{
	
	uint8 Tick_Time = 1024 / 16;
	
	uint32 Num_Ticks = (Delay_Ms * 1000) / Tick_Time;
	
	OCR1A = Num_Ticks -1 ;
	
	Num_CompMatch = 1;
	
	}
}


void TIMER1_SetCallBack(void(*ptr)(void))
{
	timer1_ptr_func = ptr;
	
}



ISR(TIMER1_COMPA_vect)
{
	static uint32 counter = 0;
	
	counter ++;
	
	if( counter == Num_CompMatch  )
	{
		counter = 0;
		
		(*timer1_ptr_func)();
	}
	
	
}



void PWM0_Init(void)
{
	DDRB |= 0X08;
	//setting fast pwm
	TCCR0 |= 0x48;
	
	//setting non inverted mode
	
	TCCR0 |= 0x20;
	
}

void PWM0_Generate(uint16 DutyCycle)
{
	
	OCR0 = ((DutyCycle * 256 ) /100 ) - 1;
}

void PWM0_Start(void)
{
	TCCR0 |= 0x03 ;
}



void PWM1_Init(void)
{
		TCCR1A |= 0x82 ;
		TCCR1B |= 0x18 ;
		
		ICR1 = 1250;
}


void PWM1_Generate(uint32 DutyCycle)
{
	OCR1A = ((DutyCycle * 65535 ) /100 ) - 1;
}


void PWM1_Start(void)
{
	TCCR1B |= 0x04;
}




void ICU_Init(void)
{
	SET_BIT(DDRB , 7);
	CLR_BIT(DDRD, 6);
	
	ICR1 = 0x0000;
	TCCR1B |= 0x41 ;
	
	TIMSK |= 0X24;
	SREG |= 0x80;
}




ISR(TIMER1_CAPT_vect)
{
	Cap_Counter ++;
	
	if( Cap_Counter == 1)
	{
		ICU_ReadCapturedVal_1 = ICR1 ;
		
		Timer1_Number_OverFlows = 0;
		
		CLR_BIT(TCCR1B , 6);
	}
	
	else if(Cap_Counter == 2)
	{
		ICU_ReadCapturedVal_2 = ICR1 + (Timer1_Number_OverFlows * 65535);
		
		SET_BIT(TCCR1B , 6);
		
	}
	
	else if(Cap_Counter == 3)
	{
		ICU_ReadCapturedVal_3 = ICR1 + (Timer1_Number_OverFlows * 65535);
		//SET_BIT(PORTB , 7);
		CLR_BIT(TCCR1B , 6) ;
		CLR_BIT(TIMSK , 5) ;
		
	}
	
}


ISR(TIMER1_OVF_vect)
{
	Timer1_Number_OverFlows ++;
}