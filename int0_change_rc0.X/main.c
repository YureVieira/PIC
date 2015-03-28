/*
 * File:   main.c
 * Author: Aluno
 *
 * Created on 12 de Fevereiro de 2015, 10:39
 */
#include<xc.h>
#define _XTAL_FREQ 4000000
#define pin_C 0
//
//void set_int0();
//int main() {
//    TRISCbits.RC0 = 0;       //Saida
//    TRISA = 0xff;           //Tudo entrada
//    PORTCbits.RC0 == 1;
//    //set_int0();
//    while(1)
//    {
//        PORTCbits.RC0 == 1;
//        __delay_ms(100);
//        PORTCbits.RC0 == 0;
//        __delay_ms(100);
//   // if(PORTBbits.RB0 == 0)
//    //    PORTCbits.RC0 == 1;
//   // else
//   //    PORTCbits.RC0 == 0;
//    }
//}
//
void set_int0()
{
    INTCONbits.GIE = 1;     //Interruções habilitadas
    INTCONbits.PEIE = 1;    //Interrupções dos periféricos habilitada
    INTCONbits.INT0E = 1;   //Interrupção externa 0 habilitada
    INTCON2bits.INTEDG0 = 1;//Borda de subida
}
void interrupt int0_interrupt()
{
    //PORTC ^= (1<<pin_C);    //Muda o pino de estado
    //if(PORT)
    //PORTCbits.RC0 = 1;
    INTCONbits.INT0F = 0;   //Limpando a flag
}


void main(void) {
 //TRISB &= ~(1<<LED);    //Define o bit 3 da porta B como saida
 TRISB = 0xff;
 TRISA = 0xff;
 TRISC = 0;
 TRISD = 0;
 PORTB = 0;
 CMCON	= 0x07;
 //set_int0();
 //Loop infinito
 while(1)
 {
     if(LB0==0)
     {
        PORTC ^= (1<<pin_C);
     }
     __delay_ms(100);
 }
}



