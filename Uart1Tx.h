#ifndef UART1TX_H_
#define UART1TX_H_

extern char serial_char;

extern __irq void sio_irq(void);

void Uart1_init(void);
void sendChar(char c);
void sendString(char* str);
#endif
