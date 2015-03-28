#include <stdio.h>
#include <xc.h>
#define _XTAL_FREQ 4000000
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
void UART_init()
{
    //Configura??o de pinos
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    TXSTAbits.CSRC = 1;     //Master
    TXSTAbits.TXEN = 1;     //Habilita transmiss?o
    TXSTAbits.SYNC = 0;     //Modo assincrono
    //TXSTAbits.BRGH =

    RCSTAbits.SPEN = 1;     //Habilita a porta serial
    RCSTAbits.CREN = 1;     //Habilita recep??o

    //Baudrate de 2400
    //BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    SPBRG = 25;
}
int UART_available()
{
    return count;
}
char UART_read_byte()//L? o proximo byte do buffer serial
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
void interrupt _ISR()
{
    if(RCIF)//Flag da recep??o serial
    {
        buffer[count] = RCREG;
        count++;
//        RCIF = 0;
    }
}
/******************************************************************************/
#define SPI_TRIS TRISB
#define SPI_PORT PORTB

#define SCK_PIN 7
#define MOSI_PIN 6
#define MISO_PIN 5
#define CS_PIN 4

inline void set_SCK(unsigned char a)
{
    a==1 ? SPI_PORT|=1<<SCK_PIN : SPI_PORT&=~(1<<SCK_PIN);
}
inline void set_CS(unsigned char a)
{
    a==1 ? SPI_PORT|=1<<CS_PIN : SPI_PORT&=~(1<<CS_PIN);
}
inline void set_MOSI(unsigned char a)
{
    a==1 ? SPI_PORT|=1<<MOSI_PIN : SPI_PORT&=~(1<<SCK_PIN);
}
inline unsigned char get_MISO()
{
    if(SPI_PORT & (1<<MISO_PIN)) return 1;
    else return 0;
}

void SPI_init()
{
    SPI_TRIS &= ~(1<<SCK_PIN | 1<<MOSI_PIN | 1<<CS_PIN);
    SPI_TRIS |= (1<<MISO_PIN);
    SPI_PORT |= (1<<SCK_PIN);             //Subida do clock
}
//unsigned char SPI_transfer(unsigned char data)
//{
//    unsigned char ret;
//    for(int i=0;i<8;i++)
//    {
//        //Coloca o bit no pino
//        if(data & 0x80)PORTB |= (1<<MOSI_PIN);
//        else PORTB &= ~(1<<MOSI_PIN);
//
//        PORTB &= ~(1<<SCK_PIN);             //Descida do clock
//        if(PORTB & (1<<MISO_PIN))ret|=1;    //Lê o pino miso
//        __delay_us(25);
//        PORTB |= (1<<SCK_PIN);             //Subida do clock
//
//        ret<<=1;
//        data<<=1;
//        __delay_us(5);
//    }
//    return ret;
//}
unsigned char SPI_transfer(unsigned char data)
{
    unsigned char ret;
    for(int i=0;i<8;i++)
    {

        if(SPI_PORT & (1<<MISO_PIN))ret|=1;    //Lê o pino miso
        SPI_PORT &= ~(1<<SCK_PIN);             //Descida do clock
         //Coloca o bit no pino
        if(data & 0x80)SPI_PORT |= (1<<MOSI_PIN);
        else SPI_PORT &= ~(1<<MOSI_PIN);
        __delay_us(25);
        SPI_PORT |= (1<<SCK_PIN);             //Subida do clock

        ret<<=1;
        data<<=1;
        __delay_us(5);
    }
    return ret;
}

//unsigned char SPI_transfer(unsigned char data)
//{
//    unsigned char ret;
//    for(int i=0;i<8;i++)
//    {
//        unsigned char aux = data;
//        ret += get_MISO();                  //Lê o pino miso
//        set_SCK(0);                         //Descida do clock
//         //Coloca o bit no pino
//        set_MOSI(aux>>7);
//        __delay_us(25);
//        set_SCK(1);             //Subida do clock
//
//        ret<<=1;
//        data<<=1;
//        __delay_us(5);
//    }
//    return ret;
//}

void main()
{
    UART_init();
    SPI_init();
    set_CS(1);
    SPI_transfer(0x80);
    SPI_transfer(0x04);
    set_CS(1);
    __delay_ms(100);

    while(1)
    {
        set_CS(1);
        SPI_transfer(0x02);
        unsigned char data = SPI_transfer(0x00);
        printf("Temperatura %d\n",data);
        set_CS(0);
//        for(int i=0;i<10;i++)__delay_ms(100);
        __delay_ms(100);

    }
}

