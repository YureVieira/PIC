#include<xc.h>
#define _XTAL_FREQ 4000000
#define TMR1_20MS 0xb1df

void config_timer()
{
    T1CONbits.RD16 = 1;
    T1CONbits.TMR1ON = 1;
    TMR1 = TMR1_20MS;          //Interromper a cada 20ms;
    GIE = 1;
    PEIE = 1;
    TMR1IE = 1;

    //Modulo CCP(comparação)
    CCP1CON = 0x0a;     //force o pino para nivel baixo
    TRISCbits.TRISC2 = 0;
//    CCP1CONbits.CCP1M3 = 1;
//    CCP1CONbits.CCP1M0 = 1;
    CCP1IE = 1;
//    CCPR1 = 0;


}
void set_CCP1(int val)
{
    char* p;
    p = &val;
    CCPR1L = *p;
    CCPR1H = *(p+1);
}
void write_servo(int val)
{
    int angle = 1000;
    if(val<=1000)
    {
        angle = val+1000;
        set_CCP1(angle);
    }
}
void interrupt _ISR()
{
    if(CCP1IF)
    {
      //set_CCP1(0xb1e0);
      PORTCbits.RC2 = 0;
      CCP1IF = 0;
    }
    if(TMR1IF)
    {
//        printf("%d",CCPR1H);
//        _newLine();
//        TMR1 = 0xb1df;          //Interromper a cada 20ms
//        set_CCP1(1000);
        PORTCbits.RC0 = ~PORTCbits.RC0;
        PORTCbits.RC2 = 1;
        TMR1IF = 0;

    }
}
void main()
{
    TRISCbits.TRISC0 = 0;
    config_timer();
    //write_servo(1000);
    int num;
    while(1)
    {
        write_servo(num);
        __delay_ms(10);
        num++;
        if(num>1000)num=0;
    }

}

