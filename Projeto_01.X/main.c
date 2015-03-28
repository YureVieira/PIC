/*
 * Objetivo: Mostrar a matricula em 4 displays de 7 segmentos
 * e com um bot�o mudar a dire��o de avan�o do texto
 */

/*
 * Tarefas:
 * -Fazer um vetor com os numeros de matricula;
 * -Fazer uma rota��o pelos elementos do vetor;
 * -Rotina que receba um inteiro de 0 a 9 e o mostre no display;
 * -Rotina que muda sentido da rota��o;
 */
/******************************************************************************/
inline void setRegister_TMR1(int _high,int _low)
{
    /*Timer para 1ms*/
    TMR1H = _high;   //0xFC;
    TMR1L = _low;    //0x17;
}
void config_timer()
{
    //Usarei timer 1
    T1CONbits.RD16 = 1;     //Opera��es de 16 bits
    //T1CONbits.T1OSCEN = 1;
    T1CONbits.TMR1CS = 1;   //Liga o timer ao clock do uC(clk/4)
    T1CONbits.TMR1ON = 1;   //Liga timer
    /*Timer para 1ms*/
    setRegister_TMR1(0xFC,0x17);
    /*Configura��o da interrup��o*/
    INTCONbits.GIEH = 1;     //Habilita��o global
    INTCONbits.GIEL = 1;    //Interrup��o dos perifericos
    PIE1bits.TMR1IE = 1;    //Habilita interrup��o do timer1
    /*Colocar o timer 1 como alta prioridade*/
}
void interrupt process()
{
    /*Tratamento da interrup��o do timer 1*/
    if(TMR1IF)
    {
        //Incrementar endere�o
    }
    setRegister_TMR1(0xFC,0x17);
    TMR1IF = 0;             //Limpa flag de interrup��o do TMR1
}
void send_bit(short int _bit,int _address){}
/******************************************************************************/
#include <xc.h>

#define _XTAL_FREQ 4000000

//#pragma config FOSC=INTRCIO, WDTE=OFF, PWRTE=OFF, MCLRE=ON, CP=OFF, \
//                CPD=OFF, BOREN=OFF, IESO=OFF, FCMEN=OFF

void main()
{
  TRISB0 = 0; //RB0 as Output PIN
  TRISB1 = 1; //RD0 as Input PIN
  ADCON1 |= 0b1111;
  RB0 = 0; //LED Off

  while(1)
  {
  }
//  return 0;
}
