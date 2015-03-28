#include<xc.h>
#include <stdlib.h>
#define _XTAL_FREQ 4000000

#define ON 1
#define OFF 0

#define LED_1 0
#define LED_2 1
#define LED_3 2
#define LED_4 3

#define SW_1 PORTBbits.RA0
#define SW_2 PORTBbits.RA1
#define SW_3 PORTBbits.RA2
#define SW_4 PORTBbits.RA3



void print_correct()
{
    PORTC = 0b00111111;
}
void print_error()
{
    PORTC = 0b01110110;
}
void clear_display()
{
    PORTC = 0;
}

void main()
{
    //Configrações basicas
    ADCON1 = 0x0f;
    TRISB = 0xff;
    
    TRISA &= ~(0x0f);
    TRISC = 0;
    int num;
    
    while(1)
    {
        num = rand()%4;

        PORTA = 0;
        PORTA |= 1 << num;
        //Se acertar a tecla
        if((PORTB&0xf) == (PORTA&0xf))
        {
           print_correct();
           for(int i=0;i<4;i++) __delay_ms(100);    //400ms
        }
        //Se alguma tecla for pressionada e for a errada
        else if((PORTB&0xf) != (PORTA&0xf) && (PORTB&0x0f)!=0 )
        {
           print_error();
           for(int i=0;i<4;i++)__delay_ms(100);     //400ms
        }

        __delay_ms(100);
        clear_display();
    }
}

