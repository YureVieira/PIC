#include <xc.h>
#include "SPI.h"

/******************************************************************************/
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

void SPI_init()
{
    //Pinos(18F2550)
    TRISCbits.TRISC7 = 0;   //SDO
//    TRISCbits.TRISC7 = 0;   //SDO (18f4520)
    TRISBbits.TRISB1 = 0;     //SCK
//    TRISCbits.TRISC3 = 0;   //SCK (18f4520)
    TRISAbits.TRISA5 = 0;       //SS

    //Modo SPI
    SSPSTATbits.SMP = 1;
    SSPSTATbits.CKE = 1;    //Escrita da passagem ativa para inativa do clock
    SSPCON1bits.SSPEN = 1;    //Hardware ligado

    SSPCON1bits.CKP = 1;    //Estado inativo do clock é alto
    SSPCON1bits.SSPM = 0b0010;//(Fosc/64)
}

unsigned char SPI_transfer(unsigned char data)
{
    SSPBUF = data;
    while(!SSPSTATbits.BF);
    SSPSTATbits.BF = 0;
    return SSPBUF;
}
