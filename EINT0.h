#ifndef EINT0
#define EINT0

void EINT0_init(void);		// Function that initializes interrupt
extern __irq void EINT0_IRQHandler (void);
extern int key_flag;

#endif
