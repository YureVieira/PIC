#ifndef UART_H
#define UART_H

void putch(char data);
inline void UART_init();
int UART_available();
char UART_read_byte();
inline void _ISR_UART();
#endif // UART_H
