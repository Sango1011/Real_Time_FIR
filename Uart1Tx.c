
#include <LPC23xx.H>
#include "Uart1Tx.h"


//Function Prototypes   -------------------------------------------------------
__irq void sio_irq(void);


char serial_char;

/* Initialize Serial Interface   	*/
void Uart1_init(void)
{

  PINSEL0 |= 0x40000000;               	/* Enable TxD1                       */
  PINSEL1 |= 0x00000001;               	/* Enable RxD1                       */
  
//  PINSEL4 |= 0xA;			//1010   (not working eventhough the manaul 
												//		  states that they can be the pins

  U1FDR    = 0x10;			/* Fractional divider not used    	*/
  U1LCR    = 0x83;      /* (8-N-1) 8 bits, no Parity, 1 Stop bit  	*/
  U1DLL    = 78;      /* 9600 Baud Rate @ 12.0 MHZ PCLK  */
  //U1DLL    = 13; 				//57600
  U1DLM    = 0;         /* High divisor latch = 0          */
  U1LCR    = 0x03;      /* DLAB = 0                        */

  /*Goal: UART1 interrupt is ONLY used for RECEIVING a character */	
  /* 1) Hook the serial interrupt routine to its VIC Vector Address */
  /* 2) Enable UART1 Interrupt	 */
  /* 3) Enable UART1 RX Interrupt	*/										
  VICVectAddr7  = (unsigned long) sio_irq;	  
  VICIntEnable |= 1 << 7; 				 
  U1IER = 0x1;		
}


void sendChar(char c)
{
	//If Transmitter Holding Register (THR) is not empty, 
	//wait in the loop until it is empty
	while ((U1LSR & 0x20) != 0x20);		//0X20 denotes U1THR is empty 
		   
	U1THR = c;
}

void sendString(char* str)
{
	while(*str != '\0') {
		sendChar(*str);
		str++;
	}
}

/* Interrupt service routine for Serial Comm. (UART1)				*/
__irq void sio_irq(void)
{
	// Check U1IIR register if you have "Receive Data Available" interrupt
	// If it is, get data, reverse its case (lowercase to uppercase or vice versa)
	// send it back
	// At the last step, you should acknowledge Interrupt

	if ((U1IIR & 0x01) == 0)		/* There is at least one interrupt source (Table 361)*/
	{
	  switch (U1IIR & 0x0E)
		{ 	/*  the numbers for case cames from Table 361 (Bits 3:1) */	
			case 0x04:		   		/* Receive Data Available (RDA) -> Bits 3:1 = 010 */
				/* get data available in U1RBR register */
				//while ((U1LSR & 0x01) != 0x01);		//when U1LSR = 1, U1RBR contains valid data
				serial_char = U1RBR;
		  	break;
			//case 0x04:
			default:break;
		}
	}
	
	VICVectAddr = 0; 		/* Acknowledge Interrupt */
}
