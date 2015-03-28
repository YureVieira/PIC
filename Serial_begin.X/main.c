#include<xc.h>
#define _XTAL_FREQ 4000000

#define CONFIG_LED TRISBbits.RB0
#define LED PORTBbits.RB0
#define ON 1
#define OFF 0
#define OUTPUT 0
#define INPUT 1

#define CONFIG_PIN_DEBUG TRISAbits.RA0
#define PIN_DEBUG PORTAbits.RA0



/******************************************************************************
 * Funções para comunicação serial assincrona
 *****************************************************************************/
//class Serial
//{
//    private:
//    char buffer[16];
//    int count;
//    int index;
//    void _EUSART_receiver();
//
//    public:
//    void config_9600();
//    int available();
//    void write_byte(char data);
//    char read_byte();
//};
////void interrupt
//void Serial::config_9600()
//{
//    //ADCON1 = 0x0f;          //Sem ad
//    count = 0;
//    index = 0;
//
//    //Configuração de pinos
//    TRISCbits.RC6 = 1;
//    TRISCbits.RC7 = 1;
//
//    TXSTAbits.CSRC = 1;     //Master
//    TXSTAbits.TXEN = 1;     //Habilita transmissão
//    TXSTAbits.SYNC = 0;    //Modo assincrono
//    //TXSTAbits.BRGH =
//
//    RCSTAbits.SPEN = 1;     //Habilita a porta serial
//    RCSTAbits.CREN = 1;     //Habilita recepção
//
//    //Baudrate de 9600
//    BAUDCONbits.BRG16 = 0;
//    TXSTAbits.BRGH = 0;
//    SPBRG = 6;
//}
//
//int Serial::available()
//{
//    return count;
//}
//
//void Serial::write_byte(char data)//Escreve um byte
//{
//    TXSTAbits.TRMT = 0;
//    TXREG = data;
//    while(!TXSTAbits.TRMT);
//}
//
//char Serial::read_byte()//Lê o proximo byte do buffer serial
//{
//    char data = buffer[0];
//    count--;
//
//    //Deslocamento da fila
//    for(int i=0;i<count-1;i++)
//    {
//        buffer[i]=buffer[i+1];
//    }
//}
//
//void Serial::_EUSART_receiver()
//{
//    buffer[count] = RCREG;
//    count++;
//}
    char buffer[16];    //Buffer
    int count;          //Quantidade de bytes do buffer
    //int index;          //Indice do buffer
//void interrupt
void Serial_config_2400()
{
    //ADCON1 = 0x0f;          //Sem ad
    count = 0;
    //index = 0;

    //Configuração de pinos
    TRISCbits.RC6 = 1;
    TRISCbits.RC7 = 1;

    TXSTAbits.CSRC = 1;     //Master
    TXSTAbits.TXEN = 1;     //Habilita transmissão
    TXSTAbits.SYNC = 0;     //Modo assincrono
    //TXSTAbits.BRGH =

    RCSTAbits.SPEN = 1;     //Habilita a porta serial
    RCSTAbits.CREN = 1;     //Habilita recepção

    //Baudrate de 2400
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.BRGH = 0;
    SPBRG = 25;

    //Interrupção
    INTCONbits.GIE = 1;     //Interruções habilitadas
    INTCONbits.PEIE = 1;    //Interrupções dos periféricos habilitada
    PIE1bits.RCIE = 1;      //Interrupção de recepção serial
}

int Serial_available()
{
    return count;
}

void Serial_write_byte(char data)//Escreve um byte
{
    TXSTAbits.TRMT = 0;
    TXREG = data;
    while(!TXSTAbits.TRMT);
}

char Serial_read_byte()//Lê o proximo byte do buffer serial
{
    char data = buffer[0];  //Leia o primeiro valor do buffer
    count--;                //diminua o valor do tamanho

    //Deslocamento da fila
    for(int i=0;i<count-1;i++)
    {
        buffer[i]=buffer[i+1];
    }
    return data;
}
void Serial_write_msg(char *str)
{
    while(*str)
   {
        if(*str == 10)
        {
            Serial_write_byte(10);
            Serial_write_byte(13);
        }
        else Serial_write_byte(*str);
        str++;
   }
}

void interrupt _EUSART_receiver()
{
    if(RCIF)//Flag da recepção serial
    {
        buffer[count] = RCREG;
        count++;
//        RCIF = 0;
    }
}

/******************************************************************************/
void main()
{
//    Serial ser;                   //Instancia
    Serial_config_2400();                  //Configurações
    ADCON1 = 0x0f;                  //Entradas digitais
    PORTB = 0;

//    CONFIG_PIN_DEBUG = OUTPUT;
    CONFIG_LED = OUTPUT;            //Pino do ligado a led habilitado

    //Transmita a mensagem
//   for(int i=0;msg[i]!=0;i++)
//   {
//        Serial_write_byte(msg[i]);
//   }

   Serial_write_msg("'L' para Ligar, e 'D' para desligar.\n");
   //Nova linha

    while(1)
    {
        if(Serial_available()>0)
        {
            char data = Serial_read_byte();
            if(data == 'L' || data == 'l')
            {
                LED = ON;
                Serial_write_msg("Led ligado\n");
            }
            else if(data == 'D' || data == 'd')
            {
                LED = OFF;
                Serial_write_msg("Led desligado\n");
            }
            else
            {
                LED = OFF;
                Serial_write_msg("Erro, caractere invalido!\n");
            }
        }
    }
}
