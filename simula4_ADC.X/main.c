#include<xc.h>
#include<stdio.h>
#define _XTAL_FREQ 4000000

void ADC_init()
{
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

    int adc_result = (ADRESH<<8)+ADRESL;
    return adc_result;
}
float cvt_toVoltage(int val)
{
    float ret = (float)val*(5.0/1023.0)*100.0;
    return ret;
}

inline char MOTOR_step_left(char val)
{
    val = val << 1;
    if(val == 24) val = 9;
    if(val == 18) val = 3;
    return val;
}
inline char MOTOR_step_right(char val)
{
    val = val >> 1;
    if(val == 1) val = 9;
    if(val == 4) val = 12;
    return val;
}
void corrige_motor(char* port,int step)
{
    //'aux' guarda posição atual em passos.
    //'output' guarda o nivel logico dos pinos.
    static char aux = 0,output = 9;
    if(step > aux)
    {
        for(int i=1;i<=step-aux;i++)
        {            
            output = MOTOR_step_right(output);
            *port = output;
            __delay_ms(5);
        }
        aux = step;
        return;
    }
    if(step < aux)
    {
        for(int i=1;i<=aux-step;i++)
        {
            output = MOTOR_step_left(output);
             *port = output;
            __delay_ms(5);
        }
        aux = step;
        return;
    }
}
void main()
{
    ADC_init();         //Inicializa o modulo ADC
    TRISD = 0xf0;       //Pinos para driver do motor como saidas;
    
    while(1)
    {
        //Faz a leitura e converte para temperatura
        float temperature = cvt_toVoltage(read_ADC(0));
        if((int)temperature > 80)
        {
            //Corrigir posição do motor de passo para 315°(200 passos)
            corrige_motor(&PORTD,175);
        }
        else if((int)temperature < 20)
        {
            //Corrigir posição do motor de passo para 0°(0 passos)
            corrige_motor(&PORTD,0);
        }
        else
        {
            //Dentro da faixa, ajuste o motor a cada 10° celsius
            corrige_motor(&PORTD,25*(-1+((int)temperature)/10));
        }
            for(int i=0;i<5;i++)__delay_ms(100);
    }
}

