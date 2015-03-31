#include <xc.h>
#include <stdio.h>
#include "UART.c"
#include "SPI.c"
#include "RFID.c"
#define _XTAL_FREQ 4000000
/*****************************************************************************
 * FUSES
 ****************************************************************************/
// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
/*****************************************************************************/
void main()
{
    UART_init();
    SPI_init(0);
//    MFRC522_PCD_Init();
    while(1)
    {
        
    }
}

void interrupt _ISR()
{
    _ISR_UART();
}
