#ifndef _SPI_H_
#define _SPI_H_
#include "SPI.c"

void SPI_init(unsigned char mode);
unsigned char SPI_transfer(unsigned char data);

#endif // SPI_H
