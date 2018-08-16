/* main program 
	written by Sarah Ngo
	August 2017 */
	
#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions */

#include "Timer.h"
//#include "Uart1Tx.h"
#include "adc.h"
#include "LED.h"

#define N_FIR 25		//number of coefficients

int main ()
{
//	char value[10];
	short LED = 0x00;
	float data [N_FIR] = {0.0};
	int i, newest = 0, data_index, LED_flag;
	float result = 0;
	float coeff[N_FIR]={-0.00045,   -0.0027,    0.0000,    0.0027,    0.0234,    0.0504,    0.0891,    0.1368,    0.1899,    0.2424,
				0.2871,    0.3159,    0.3267,    0.3159,    0.2871,    0.2424,    0.1899,    0.1368,    0.0891,    0.0504,
				0.0234,    0.0027,    0.0000,   -0.0027,   -0.0045};
		
	//initializtions
	adc_init();	
	Timer1_Init();
//	Uart1_init();
	LED_Init();
	
	T1TCR = 0x02;			//reset timer counter
	Timer1_SetMR0(1150);		//match register gives sampling freq
	TIMER1_ENABLE;				//Start timer
	
while(1)
 {
			if (getInterruptFlag() == 1)
			{		
				clearInterruptFlag();
				
				data[newest] = (float)(getAD_last());	//storing into an array		
				data_index = newest;
				
				for (result = 0, i = 0; i < N_FIR; i++)
				{
					result += coeff[i]*data[data_index];		//filter
					data_index--;			//go to next past data value
					if (data_index < 0)
					{
						data_index = N_FIR - 1;		//circle around when 0 is reached.
					}
				}				
				DACR = (result);  //send the value to the dac
				
				newest++;
				if (newest == N_FIR)		//if end is reached start over
				{
					newest = 0;
				}
				
				if (getTimer_iteration() >=10000)
				{
					if (LED_flag == 0)
					{
						LED |= 0xFF;
						LED_flag = 1;
					}
					else
					{
						LED &= 0x00;
						LED_flag = 0;
					}
					LED_Out(LED);
					resetTimer_iteration();		//reset counter
				}
			}
	}
}
