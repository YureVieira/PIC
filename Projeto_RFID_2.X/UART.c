#include "UART.h"
#include <xc.h>
/*****************************************************************************
 *Porta Serial
 ****************************************************************************/
char buffer[16];    //Buffer
int count=0;          //Quantidade de bytes do buffer
void putch(char data)
{
    if(data == 10)
    {
        while(!TXIF)
        continue;
        TXREG = 10;
        while(!TXIF)
        continue;
        TXREG = 13;
        return;
    }
    while(!TXIF)
        continue;
    TXREG = data;
}
void UART_init()
{
    //Configura??o de pinos
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    TXSTAbits.CSRC = 1;     //Master
    TXSTAbits.TXEN = 1;     //Habilita transmiss?o
    TXSTAbits.SYNC = 0;     //Modo assincrono
    //TXSTAbits.BRGH =

    RCSTAbits.SPEN = 1;     //Habilita a porta serial
    RCSTAbits.CREN = 1;     //Habilita recep??o

    //Baudrate de 2400
    //BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    SPBRG = 25;
}
int UART_available()
{
    return count;
}
char UART_read_byte()//L? o proximo byte do buffer serial
{
    char data = buffer[0];  //Leia o primeiro valor do buffer
    count--;                //diminua o valor do tamanho

    //Deslocamento da fila
    for(int i=0;i<count;i++)
    {
        buffer[i]=buffer[i+1];
    }
    return data;
}
/******************************************************************************/
inline void _ISR_UART()
{
    if(RCIF)//Flag da recep??o serial
    {
        buffer[count] = RCREG;
        count++;
    }
}
/******************************************************************************/
