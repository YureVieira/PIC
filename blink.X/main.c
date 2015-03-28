/* 
 * File:   main.c
 * Autor: Yure Vieira Sampaio Albuquerque
 *
 * Piscar um led atraves do timer 0(sem interrupção)em frequencia de 1Hz
 */

#include <xc.h>
#define _XTAL_FREQ 4000000

#define LED 1

//Prototipos
void delay_0(int _val);
/*******************************************/
//Função principal
void main() {
 //TRISB &= ~(1<<LED);    //Define o bit 3 da porta B como saida
 TRISB = 0;
 TRISA = 0;
 //Loop infinito
 while(1)
 {
     //PORTB ^=(1<<LED);  //Troca o estado do pino
     //delay_0(500);
     PORTBbits.RB1 = 1 ;
     delay_0(500);
     //__delay_ms(100);
     PORTBbits.RB1 = 0;
     delay_0(500);
     //__delay_ms(100);
 }
}
/*******************************************/
//Funções
inline void config_timer()
{
    ///Configuração do timer 0
 T0CONbits.T08BIT = 0;  //timer de 16 bits
 T0CONbits.T0CS = 0;    //Fonte sendo o clock/4
 T0CONbits.PSA = 1;     //Sem uso de prescaler
 T0CONbits.T0PS = 0;    //Prescaler de 2 
}
/**/
inline void set_timer0()
{
    //Senpre escrever parte alta primeiro
     //TMR0H = 0xf8;
     //TMR0L = 0x30;
     //1000 contagens para estouro
     TMR0H = 0xfc;
     TMR0L = 0x17;
}
/**/
inline void timer0_on()
{
    T0CONbits.TMR0ON=1;    //Liga timer
    //Debug
    //PORTA = T0CON;
}
/**/
void delay_0(int _val)
{
    int count=0;
    config_timer();
    while(count<_val)    //Cada ciclo corresponde a 1ms
    {
        set_timer0();   //Carrega o valor para contagem no timer
        timer0_on();
        //Debug
        //PORTBbits.RB0 = 1;
        //
        while(!TMR0IF); //Loop enquanto flag for falsa;
        TMR0IF = 0;     //Limpa flag
        count++;
        //
        //PORTBbits.RB0 = 0;
        //
    }
}
/*******************************************/