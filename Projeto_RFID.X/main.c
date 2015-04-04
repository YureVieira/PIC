#include <xc.h>
#include <stdio.h>

#define SPI_TRIS TRISA
#define SPI_PORT PORTA
#define SPI_SCK 7
#define SPI_MOSI 6
#define SPI_MISO 5
#define RFID_CS 4
#define RFID_RESET 3

#include "UART.c"
#include "SPI.c"
#include "RFID.h"
#define _XTAL_FREQ 4000000


/*****************************************************************************
 * FUSES
 ****************************************************************************/
// CONFIG
////#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
////#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
////#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
////#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
////#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
////#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
////#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
//#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
/*****************************************************************************/
//MSSP_init()
//{
//    //Pinos(18F2550)
//    TRISCbits.TRISC7 = 0;   //SDO
////    TRISCbits.TRISC7 = 0;   //SDO (18f5420)
//    TRISBbits.TRISB1 = 0     //SCK
////    TRISCbits.TRISC3 = 0;   //SCK (18f5420)
//
//    //Modo SPI
//    SSPSTATbits.SMP = 0;
//    SSPSTATbits.CKE = 1;    //Escrita da passagem ativa para inativa do clock
//    SSPCON1bits.SSPEN = 1;    //Hardware ligado
//
//    SSPCON1bits.CKP = 1;    //Estado inativo do clock é alto
//    SSPCON1bits.SSPM = 0b0010;//(Fosc/64)
//}
//
//unsigned char MSSP_transfer(unsigned char data)
//{
//    SSPBUF = data;
//    while(!SSPSTATbits.BF);
//    SSPSTATbits.BF = 0;
//}

void main()
{
    UART_init();
    SPI_init(0);
    RFID_init();
//    MFRC522_PCD_Init();
    while(1)
    {
        inicio:
        // Look for new cards
        if ( PICC_IsNewCardPresent()!=0 )
        {
            goto inicio;
        }
        // Select one of the cards
        if (PICC_ReadCardSerial()!=0)
        {
            goto inicio;
        }
  //Mostra UID na serial
  printf("UID da tag :");
  for (unsigned char i = 0; i < uid.size; i++)
  {
    printf("%s",uid.uidByte[i] < 0x10 ? " 0" : " ");
    printf("%x",uid.uidByte[i]);
  }
  printf("\nMensagem : ");
    }
}

void interrupt _ISR()
{
    _ISR_UART();
}