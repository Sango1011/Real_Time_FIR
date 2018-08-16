#include <LPC23xx.H>                    /* LPC23xx definitions                */

#include "Timer.h"
//#include "adc.h"
#include "LED.h"
#include "Uart1tx.h"

unsigned int timer_iteration;
volatile int interrupt_flag;    

short AD_last;

short getAD_last(void)
{
	return AD_last;
}

unsigned int getTimer_iteration(void)
{
	return timer_iteration;
}

void resetTimer_iteration(void)
{
	timer_iteration = 0;
}

void clearInterruptFlag(void)
{
	interrupt_flag = interrupt_flag - 10000;
}

int getInterruptFlag(void)
{
	return interrupt_flag;
}

void Timer1_Init(void)
{
	VICVectAddr5 = (unsigned long) Timer1_Handler;		//points to timer 1 interrupt handler
	T1MCR |= 0x01;				//Interrupt on MR0 
	TIMER1_INT_ENABLE;		//interrupt timer1 interrupt
}

void Timer1_SetMR0(unsigned long value)
{
	T1MR0 = value;
}

__irq void Timer1_Handler(void)
{	
	AD_last = AD0DR0;     //Shifting value in AD0DR0 by 6 bit 
	
	T1TCR = 0x02;		//reset timer counter
	
	timer_iteration++;

	AD0CR |= 0x01000000;		//start conversion
	T1IR  = 0x01;		//reset interrupt flag	
	VICVectAddr = 0;	//Acknowledge interrupt
	T1TCR = 0x01;		//Start timer
	interrupt_flag = 1;
}
