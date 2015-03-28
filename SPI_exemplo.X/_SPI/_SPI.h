/* 
 * File:   _SPI.h
 * Author: Yure
 *
 * Created on 16 de Março de 2015, 16:23
 */

#ifndef _SPI_H
#define	_SPI_H
#define RISING 0
#define FALLING 1

struct IO_easy
{
    char* port;
    char _bit;
};

struct _SPI {
    void config_SSPin(char* p,char _bit);
    void config_MOSIPin(char* p,char _bit);
    void config_MISOPin(char* p,char _bit);
    void config_SCKPin(char* p,char _bit);
    void setup_edge(char val);
    void setup_BitOrder(char _order);
    char transfer_byte(char data);
    IO_easy ss_pin;
    IO_easy mosi_pin;
    IO_easy miso_pin;
    IO_easy sck_pin;
    char order;
    char edge;
    char delay;
};

#endif	/* _SPI_H */

