#include<xc.h>
#include<stdio.h>
#define _XTAL_FREQ 4000000

/******************************************************/
void putch(char data)
{
    while(!TXIF)
        continue;
    TXREG = data;
}
void UART_init()
{
    //Configura??o de pinos
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    TXSTAbits.CSRC = 1;     //Master
    TXSTAbits.TXEN = 1;     //Habilita transmiss?o
    TXSTAbits.SYNC = 0;     //Modo assincrono
    //TXSTAbits.BRGH =

    RCSTAbits.SPEN = 1;     //Habilita a porta serial
    RCSTAbits.CREN = 1;     //Habilita recep??o

    //Baudrate de 2400
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    SPBRG = 25;
}
void _newLine()
{
    putch(10);
    putch(13);
}
/******************************************************************************/

void ADC_init()
{
    //PBADEN = 1;
    ADCON2bits.ADFM = 1;
    ADCON1 = 0;
    ADCON2bits.ACQT = 0b101;    //12 TDA
    ADCON2bits.ADCS = 0b010;
}
int read_ADC(char channel)
{
    ADCON0 = 0;
    ADCON0 |= channel;  //Seleciona canal

    __delay_us(20);

    ADCON0bits.ADON = 1;
    ADCON0bits.GODONE = 1;
    while(ADCON0bits.GODONE);
    ADCON0bits.ADON = 1;

    __delay_ms(20);

    //int adc_result = ((ADRESH <<8) + ADRESL);
    int adc_result = (ADRESH<<8)+ADRESL;
    return adc_result;
}

void main()
{
    UART_init();
    ADC_init();
    while(1)
    {
        printf("%d",read_ADC(0));
        _newLine();
        for(int i=0;i<5;i++)__delay_ms(100);
    }
}
