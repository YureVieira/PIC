#include <xc.h>
#include <stdio.h>
#include <string.h>
#define _XTAL_FREQ 4000000
#define TMR1_20MS 0xb1df

/*****************************************************************************
 *Porta Serial
 ****************************************************************************/
char buffer[16];    //Buffer
int count=0;          //Quantidade de bytes do buffer
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
void UART_init(void)
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

    //Interrup??o
    INTCONbits.GIE = 1;     //Interru??es habilitadas
    INTCONbits.PEIE = 1;    //Interrup??es dos perif?ricos habilitada
    PIE1bits.RCIE = 1;      //Interrup??o de recep??o serial
}
int UART_available()
{
    return count;
}
char UART_read_byte()//Lê o proximo byte do buffer serial
{
    char data = buffer[0];  //Leia o primeiro valor do buffer
    count--;                //diminua o valor do tamanho

    //Deslocamento da fila
    for(int i=0;i<count;i++)
    {
        buffer[i]=buffer[i+1];
    }
    return data;
}
/******************************************************************************/
void set_CCP1(int val)
{
    char* p;
    p = &val;
    CCPR1L = *p;
    CCPR1H = *(p+1);
}
void SERVO_init()
{
    T1CONbits.RD16 = 1;
    T1CONbits.TMR1ON = 1;
    TMR1 = TMR1_20MS;          //Interromper a cada 20ms;
    GIE = 1;
    PEIE = 1;
    TMR1IE = 1;

    //Modulo CCP(compara??o)
    CCP1CON = 0x0a;     //force o pino para nivel baixo
    TRISCbits.TRISC2 = 0;
//    CCP1CONbits.CCP1M3 = 1;
//    CCP1CONbits.CCP1M0 = 1;
    CCP1IE = 1;
    set_CCP1(TMR1_20MS+1000);
//    CCPR1 = 0;


}
void SERVO_write(int val)
{
    int angle = 1000;   //Valor inicial para servo(1ms)
    if(val<=1000)
    {
        angle = val+1000;
        set_CCP1(TMR1_20MS+angle);
    }
}
void interrupt _ISR()
{
    if(RCIF)//Flag da recep??o serial
    {
        buffer[count] = RCREG;
        count++;
//        RCIF = 0;
    }
    if(CCP1IF)
    {
      PORTCbits.RC2 = 0;
      CCP1IF = 0;
    }
    if(TMR1IF)
    {
        TMR1 = TMR1_20MS;
        PORTCbits.RC0 = ~PORTCbits.RC0;
        PORTCbits.RC2 = 1;
        TMR1IF = 0;

    }
}
void DEVICES_set(char* port,char d1,char d2)
{
    char val=0;
    char msg1[30];
    char msg2[15];
    if(d1 != 'S')
    {
        switch(d2)
        {
            case '1':
                val = 1;
                strcpy(msg1,"Led 1 ");
                break;
            case '2':
                val = 2;
                strcpy(msg1 , "Led 2 ");
                break;
            case '3':
                val = 4;
                strcpy(msg1 , "Led 3 ");
                break;
            case 'T':
                val = 7;
                strcpy(msg1 , "Todos ");
                break;
            default:
                printf("segundo parametro invalido!\n");
                break;
        }
        if(d1=='L')
        {
            *port |= val;            //Seta pinos
            strcpy(msg2 , "aceso(s)");
            strcat(msg1,msg2);
            printf("%s\n",msg1);
        }
        else if(d1=='D')
        {
            *port &= ~(val);    //Limpa pinos
            strcpy(msg2 , "apagado(s)");
            strcat(msg1,msg2);
            printf("%s\n",msg1);
        }
        else printf("primeiro parametro invalido!\n");
    }
    else
    {
        switch(d2)
        {
            case '1':
                SERVO_write(0);
                printf("Servo em 1\n");
                break;
            case '2':
                SERVO_write(250);
                printf("Servo em 2\n");
                break;
            case '3':
                SERVO_write(500);
                printf("Servo em 3\n");
                break;
            case '4':
                SERVO_write(750);
                printf("Servo em 4\n");
                break;
            case '5':
                SERVO_write(1000);
                printf("Servo em 5\n");
                break;
            default:
                printf("Primeiro parametro invalido!\n");
                break;
        }
    }
}
void main()
{
    ADCON1 = 0x0f;
    TRISC = 0;
    TRISB = 0b11111000;
    
    UART_init();
    SERVO_init();

    printf("'L' e 'D' para ligar e desligar leds\n");
    printf("'1' a '3' junto ao comando anterior para escolher qual led\n");
    printf("'T' para aplicar a ação a todos os leds\n");
    printf("'S' + '1' a '5' para escolher posicionamento do servomotor\n");

    int num;
    while(1)
    {
        if(UART_available()>=2)
        {
            char fnc = UART_read_byte();
            char value = UART_read_byte();
            DEVICES_set(&PORTB,fnc,value);    //Aciona os dispositivos(leds,servo)
        }
    }
}