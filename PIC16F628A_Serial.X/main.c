#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
/*****************************************************************************
 *Porta Serial
 ****************************************************************************/
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
    //Configuração de pinos
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    TXSTAbits.CSRC = 1;     //Master
    TXSTAbits.TXEN = 1;     //Habilita transmissão
    TXSTAbits.SYNC = 0;     //Modo assincrono
    //TXSTAbits.BRGH =

    RCSTAbits.SPEN = 1;     //Habilita a porta serial
    RCSTAbits.CREN = 1;     //Habilita recepção

    //Baudrate de 2400
    //BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    SPBRG = 25;
    //SPBRG = 207;    //baudrate = 300
}
/******************************************************************************/
//void trisa_set(char a){TRISA|=(1<<a);}
//void trisa_reset(char a){TRISA&=~(1<<a);}
//void trisb_set(char a){TRISB|=(1<<a);}
//void trisb_reset(char a){TRISB&=~(1<<a);}
//void trisc_set(char a){TRISC|=(1<<a);}
//void trisc_reset(char a){TRISC&=~(1<<a);}
void main()
{
    PCONbits.OSCF = 1;
    char* p;
    p = &TRISB;
    UART_init();
    while(1)
    {
        printf("Address PORTB = 0x%X\n",p);
        for(int i=0;i<50;i++)__delay_ms(100);
    }
}

