/* main program 
	written by Sarah Ngo
	August 2017 */
	
#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions */

#include "Timer.h"
//#include "Uart1Tx.h"
#include "adc.h"
#include "LED.h"

#define N_FIR 100		//number of coefficients

int main ()
{
//	char value[10];
	short LED = 0x00;
	float data [N_FIR] = {0.0};
	int i, newest = 0, data_index, LED_flag;
	float result = 0;
	float coeff[N_FIR]={-0.0020,   -0.0020,   -0.0020,   -0.0019,   -0.0017,   -0.0015,   -0.0011,   -0.0006,    0.0001,    0.0010,
			0.0020,    0.0032,    0.0046,    0.0060,    0.0074 ,   0.0087 ,   0.0098,    0.0106 ,   0.0110 ,   0.0110,
			0.0104 ,   0.0092 ,   0.0073 ,   0.0048 ,   0.0017,  -0.0019  , -0.0058  , -0.0101  ,-0.0143  , -0.0185,
			-0.0222 ,  -0.0254  , -0.0278 ,  -0.0293  , -0.0297  , -0.0289 ,  -0.0269 ,  -0.0237  , -0.0193 ,  -0.0140,
			-0.0079,   -0.0012  ,  0.0059 ,   0.0129  ,  0.0196 ,   0.0258   , 0.0311  ,  0.0352 ,   0.0382 ,   0.0396,
			0.0396 ,   0.0382  ,  0.0352 ,   0.0311  ,  0.0258  ,  0.0196 ,   0.0129  ,  0.0059 ,  -0.0012,   -0.0079,
			-0.0140 ,  -0.0193,   -0.0237 ,  -0.0269  , -0.0289  , -0.0297 ,  -0.0293 ,  -0.0278 ,  -0.0254 ,  -0.0222,
			-0.0185 ,  -0.0143 ,  -0.0101,   -0.0058 ,  -0.0019 ,   0.0017  ,  0.0048  ,  0.0073  ,  0.0092  ,  0.0104,
			0.0110 ,   0.0110 ,   0.0106,    0.0098  ,  0.0087 ,   0.0074 ,   0.0060  ,  0.0046 ,   0.0032 ,   0.0020,
			0.0010,    0.0001,   -0.0006  , -0.0011  , -0.0015  , -0.0017  , -0.0019   ,-0.0020  , -0.0020  , -0.0020};

	//initializtions
	adc_init();	
	Timer1_Init();
//	Uart1_init();
	LED_Init();
	
	T1TCR = 0x02;			//reset timer counter
	Timer1_SetMR0(12000);		//match register gives sampling freq
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
				
				result += 5500.0;			//msking positivie values bc the dac flips the negative values.
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
