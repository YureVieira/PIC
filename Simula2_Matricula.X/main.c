#include <xc.h>
#include <stdio.h>
#include <stdarg.h>
#define _XTAL_FREQ 4000000

volatile bit direction;

/*****************************************************************************
 *Debug Serial
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
void init_uart(void)
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
/******************************************************************************/
void config_display()
{
    //
    TRISD &= ~(0x0f);   //Pinos para cada segmento
    TRISC &= ~(0x0f);    //Pinos de habilitação dos displays
}
void print_display(char _index,char _num)
{
//    PORTC &= ~(0x0f);       //Apaga todos os displays
    PORTC |= (0x0f);       //Apaga todos os displays
    PORTD = _num;           //Prepara os leds para exibirem o numero
//    PORTC |= (1<<_index);   //Liga o pino de habilitação escolhido
    PORTC &= ~(1<<_index);   //Liga o pino de habilitação escolhido
}
void print_vetor(char* _vector,char _index)
{
    char* _start;
    _start = _vector;
    _vector += _index;
    for(char i=0;i<4;i++)
    {
        if(*_vector < 10)
        {
//            printf("|%d",*_vector);
            print_display(i,*_vector);
            _vector++;
        }
        else
        {
            _vector = _start;
//            printf("|%d",*_vector);
            print_display(i,*_vector);
            _vector++;
        }
    }
//    printf("|");
//    _newLine();
}
void set_int0()
{
    INTCONbits.GIE = 1;     //Interruções habilitadas
    INTCONbits.PEIE = 1;    //Interrupções dos periféricos habilitada
    INTCONbits.INT0E = 1;   //Interrupção externa 0 habilitada
    INTCON2bits.INTEDG0 = 1;//Borda de subida
}
void interrupt _interrupt()
{
    if(INT0F)
    {
        direction = ~direction;
        INT0F = 0;   //Limpando a flag
    }
}
//Função principal
void main()
{
    //Configrações basicas
    ADCON1 = 0x0f;
    TRISB = 0xff;

    //Variaveis
    signed char index;
    char matricula[]={2,0,1,0,2,5,2,5,0,0,5,2,10};
    direction = 1;
    
    //Debug pela serial
    init_uart();
    printf("PIC ativado/n");

    //Inicializando os displays
    config_display();
    set_int0();
    //print_vetor(matricula,0);

    //Laço
    while(1)
    {
        print_vetor(matricula,index);
        if(direction == 1)
        {
            index++;
            if(index==12)index=0;
        }
        else
        {
            index--;
            if(index<0)index=11;
        }
        for(char i=0;i<5;i++)__delay_ms(100);
     printf("index = %d\n",index);
    }
}
