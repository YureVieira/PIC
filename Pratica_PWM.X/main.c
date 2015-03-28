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
    //Configuração de pinos
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    TXSTAbits.CSRC = 1;     //Master
    TXSTAbits.TXEN = 1;     //Habilita transmissão
    TXSTAbits.SYNC = 0;     //Modo assincrono
    //TXSTAbits.BRGH =

    RCSTAbits.SPEN = 1;     //Habilita a porta serial
    RCSTAbits.CREN = 1;     //Habilita recepção

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

void PWM_init()
{
    //TRISCbits.TRISC2 = 0;
    CCP1CON |= 0x0c;    //Configurado para pwm
    PR2 = 255;          //Periodo maximo

    //Timer 2(Sem prescaler)
    TMR2ON = 1;         //Liga timer
}

void write_PWM(unsigned char val)
{
    CCPR1L = val;
}
void ADC_init()
{
    ADCON2bits.ADFM = 1;
    ADCON1 = 0;
    ADCON2 = 0;
}

int read_ADC(char channel)
{
    ADCON0 &= 0xf0;
    ADCON0 |= channel;  //Seleciona canal

    __delay_us(20);
    
    ADCON0bits.ADON = 1;
    ADCON0bits.GODONE = 1;
    while(ADCON0bits.GODONE);
    ADCON0bits.ADON = 1;

    __delay_ms(20);

    //int adc_result = ((ADRESH <<8) + ADRESL);
    int adc_result = ADRESL;
    return adc_result;
}

void main()
{
    TRISC = 0xff;
    TRISB = 0xff;
    UART_init();
    PWM_init();
    ADC_init();
    unsigned char count = 0;
    printf("Ola mundo!");

    while(1)
    {
        printf("%d",read_ADC(0));
        _newLine();
        write_PWM(count);
        count++;
        __delay_ms(50);
    }
}
