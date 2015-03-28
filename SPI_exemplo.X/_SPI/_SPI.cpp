/* 
 * File:   _SPI.cpp
 * Author: Yure
 * 
 * Created on 16 de Março de 2015, 16:23
 */

#include "_SPI.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

struct IO_easy
{
    char* port;
    char _bit;
};

void _SPI::config_SSPin(char* p,char _bit)
{
    ss_pin._bit = _bit;
    ss_pin.port = p;
}

void _SPI::config_MOSIPin(char* p,char _bit)
{
    mosi_pin._bit = _bit;
    mosi_pin.port = p;
}

void ::config_MISOPin(char* p,char _bit)
{
    miso_pin._bit = _bit;
    miso_pin.port = p;
}

void _SPI::config_SCKPin(char* p, char _bit)
{
    sck_pin._bit = _bit;
    sck_pin.port = p;
}

void _SPI::setup_BitOrder(char _order)
{
    order = _order;
}

void _SPI::setup_edge(char val)
{
    edge = val;
}

char _SPI::transfer_byte(char data)
{
    char load, _return;
    load = data;
    if(order)
    {
        for(char i=0;i<8;i++)
        {
            *mosi_pin.port = (load & 0x80)<<(mosi_pin._bit);    //Escreva MSB
            _return = (*miso_pin.port & (1<<miso_pin._bit)) >> miso_pin._bit;      //Guarda o bit
            load = load << 1;
            _return = _return << 1;
            __delay_ms(1);
        }
    }
//    else
//    {
//        for(char i=0;i<8;i++)
//        {
//            *mosi_pin.port = (load & 0x80)<<(mosi_pin._bit);    //Escreva MSB
//            _return = (*miso_pin.port & (1<<miso_pin._bit)) >> miso_pin._bit;      //Guarda o bit
//            load = load << 1;
//            _return = _return << 1;
//            __delay_ms(1);
//        }
    }
}

