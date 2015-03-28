#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
#define SCK_PIN 4
#define MOSI_PIN 5
#define MISO_PIN 6
#define SS_PIN 7
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3
#define DELAY __delay_us(100)
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
    //Configuração de pinos
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;

    TXSTAbits.CSRC = 1;     //Master
    TXSTAbits.TXEN = 1;     //Habilita transmissão
    TXSTAbits.SYNC = 0;     //Modo assincrono
    //TXSTAbits.BRGH =

    RCSTAbits.SPEN = 1;     //Habilita a porta serial
    RCSTAbits.CREN = 1;     //Habilita recepção

    //Baudrate de 2400
    //BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    SPBRG = 25;
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
char _mode;
void SPI_init(unsigned char mode)
{
    TRISB &= ~(1<<SCK_PIN | 1<<MOSI_PIN | 1<<SS_PIN);   //Pinos de saida
    TRISB |= 1<<MISO_PIN;                               //Pino de entrada
//    PORTB |= (1<<SCK_PIN);

    //Definir estado de repouso do pino de clock
    if(mode & 0x01)PORTB |= (1<<SCK_PIN);
    else PORTB &= ~(1<<SCK_PIN);
    _mode = mode;
}

void SPI_setSS(unsigned char d)
{
    if(d)PORTB |= (1<<SS_PIN);
    else PORTB &= ~(1<<SS_PIN);
}

char SPI_transfer(unsigned char data)
{
    unsigned char ret = 0x00 ; // Retorno do SPI.
    unsigned char SPI_byte;
    SPI_byte = data;
    for( unsigned char i=0;i<8;i++ )
    {
        if(_mode & 0x02)
        {
            // Escreve BIT no MOSI.
            if(SPI_byte & 0x80)PORTB |= (1<<MOSI_PIN);
            else PORTB &= ~(1<<MOSI_PIN);

            SPI_byte <<= 1 ;                // Faz o shift para o próximo bit.
            ret      <<= 1 ;

            PORTB ^= (1<<SCK_PIN);          //Muda estado do clock
            DELAY;
            if(PORTB & (1<<MISO_PIN))ret |= 1;    //Lê o pino MISO)
            PORTB ^= (1<<SCK_PIN);          //Muda estado do clock
            DELAY;
        }
        else
        {
            PORTB ^= (1<<SCK_PIN);          //Muda estado do clock
            if(PORTB & (1<<MISO_PIN))ret |= 1;    //Lê o pino MISO)
            DELAY;
            // Escreve BIT no MOSI.
            if(SPI_byte & 0x80)PORTB |= (1<<MOSI_PIN);
            else PORTB &= ~(1<<MOSI_PIN);

            SPI_byte <<= 1 ;                // Faz o shift para o próximo bit.
            ret      <<= 1 ;

            PORTB ^= (1<<SCK_PIN);          //Muda estado do clock
            DELAY;
        }
    }
    return( ret ) ; // Retorna o byte recebido.
}
/******************************************************************************/
void main()
{
    UART_init();
    SPI_init(SPI_MODE3);
    unsigned char temp=0;

    SPI_setSS(1);
    SPI_transfer(0x80);
    SPI_transfer(0x04);
    __delay_ms(100);
    SPI_setSS(0);

    while(1)
    {
        //Lê a temperatura
        SPI_setSS(0);
        SPI_transfer(0x02);
        temp = SPI_transfer(0x00);
        SPI_setSS(1);
        printf("%d\n", temp);
        for(char i=0;i<10;i++)__delay_ms(100);
    }
}
/******************************************************************************/
/******************************************************************************/

