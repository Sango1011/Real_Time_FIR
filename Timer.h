#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER1_ENABLE				(T1TCR = 0x01)
#define TIMER1_INT_ENABLE		(VICIntEnable |= 1 << 5)

//extern unsigned int timer_iteration;
//extern int interrupt_flag;
//extern short AD_last;

extern __irq void Timer1_Handler(void);

int getInterruptFlag(void);
void clearInterruptFlag(void);
void Timer1_Init(void);
void Timer1_SetMR0(unsigned long value);
short getAD_last(void);
unsigned int getTimer_iteration(void);
void resetTimer_iteration(void);

#endif
