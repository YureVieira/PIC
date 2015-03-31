#include <xc.h>
#include "SPI.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif
/******************************************************************************/
#ifndef SPI_TRIS
#define SPI_TRIS TRISB
#endif
#ifndef SPI_PORT
#define SPI_PORT PORTB
#endif

#ifndef SCK_PIN
#define SCK_PIN 7
#endif
#ifndef MOSI_PIN
#define MOSI_PIN 6
#endif
#ifndef MISO_PIN
#define MISO_PIN 5
#endif


#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

unsigned char _mode;
void SPI_init(unsigned char mode)
{
    _mode = mode;
    SPI_TRIS &= ~(1<<SCK_PIN | 1<<MOSI_PIN);
    SPI_TRIS |= (1<<MISO_PIN);
    if(mode == 2 || mode == 3)
        SPI_PORT |= (1<<SCK_PIN);             //Repouso em alto
    else
        SPI_PORT &= ~(1<<SCK_PIN);             //Repouso em baixo
}
unsigned char SPI_transfer(unsigned char data)
{
    unsigned char ret;
    if(_mode==1 || _mode==3)
    {
    for(int i=0;i<8;i++)
    {
        //Coloca o bit no pino
        if(data & 0x80)SPI_PORT |= (1<<MOSI_PIN);
        else SPI_PORT &= ~(1<<MOSI_PIN);

        SPI_PORT ^= (1<<SCK_PIN);
        if(SPI_PORT & (1<<MISO_PIN))ret|=1;    //Lê o pino miso
        __delay_us(10);
        SPI_PORT ^= (1<<SCK_PIN);

        ret<<=1;
        data<<=1;
        __delay_us(10);
    }
    }
    else
    {
        unsigned char ret;
        for(int i=0;i<8;i++)
        {

           if(SPI_PORT & (1<<MISO_PIN))ret|=1;    //Lê o pino miso
           SPI_PORT ^= (1<<SCK_PIN);

           //Coloca o bit no pino
           if(data & 0x80)SPI_PORT |= (1<<MOSI_PIN);
           else SPI_PORT &= ~(1<<MOSI_PIN);
           __delay_us(10);
           SPI_PORT ^= (1<<SCK_PIN);

           ret<<=1;
           data<<=1;
          __delay_us(10);
            }
    }
    return ret;
}
