#ifndef RFID_h
#define RFID_h
#include "SPI.h"
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for setting up the PIC
	/////////////////////////////////////////////////////////////////////////////////////
    void RFID_init();
//    void _setSPIConfig();
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Basic interface functions for communicating with the 
	/////////////////////////////////////////////////////////////////////////////////////
    void _PCD_WriteRegister(unsigned char reg, unsigned char value);
    void _PCD_WriteRegisterVector(unsigned char reg, unsigned char count, unsigned char *values);
    unsigned char _PCD_ReadRegister(unsigned char reg);
    void _PCD_ReadRegisterVector(unsigned char reg, unsigned char count, unsigned char *values, unsigned char rxAlign = 0);
    void _setBitMask(unsigned char reg, unsigned char mask);
    void _PCD_SetRegisterBitMask(unsigned char reg, unsigned char mask);
    void _PCD_ClearRegisterBitMask(unsigned char reg, unsigned char mask);
    unsigned char _PCD_CalculateCRC(unsigned char *data, unsigned char length, unsigned char *result);

	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for manipulating the 
	/////////////////////////////////////////////////////////////////////////////////////
    void _PCD_Init();
    void _PCD_Reset();
    void _PCD_AntennaOn();
    void _PCD_AntennaOff();
    unsigned char _PCD_GetAntennaGain();
    void _PCD_SetAntennaGain(unsigned char mask);
    unsigned char _PCD_PerformSelfTest();
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with PICCs
	/////////////////////////////////////////////////////////////////////////////////////
    unsigned char _PCD_TransceiveData(unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned char *backLen, unsigned char *validBits = NULL, unsigned char rxAlign = 0, unsigned char checkCRC = false);
    unsigned char _PCD_CommunicateWithPICC(unsigned char command, unsigned char waitIRq, unsigned char *sendData, unsigned char sendLen, unsigned char *backData = NULL, unsigned char *backLen = NULL, unsigned char *validBits = NULL, unsigned char rxAlign = 0, unsigned char checkCRC = false);

    unsigned char _PICC_RequestA(unsigned char *bufferATQA, unsigned char *bufferSize);
    unsigned char _PICC_WakeupA(unsigned char *bufferATQA, unsigned char *bufferSize);
    unsigned char _PICC_REQA_or_WUPA(unsigned char command, unsigned char *bufferATQA, unsigned char *bufferSize);
    unsigned char _PICC_Select(Uid *uid, unsigned char validBits = 0);
    unsigned char _PICC_HaltA();
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Functions for communicating with MIFARE PICCs
	/////////////////////////////////////////////////////////////////////////////////////
    unsigned char _PCD_Authenticate(unsigned char command, unsigned char blockAddr, MIFARE_Key *key, Uid *uid);
    void _PCD_StopCrypto1();
    unsigned char _MIFARE_Read(unsigned char blockAddr, unsigned char *buffer, unsigned char *bufferSize);
    unsigned char _MIFARE_Write(unsigned char blockAddr, unsigned char *buffer, unsigned char bufferSize);
    unsigned char _MIFARE_Decrement(unsigned char blockAddr, long delta);
    unsigned char _MIFARE_Increment(unsigned char blockAddr, long delta);
    unsigned char _MIFARE_Restore(unsigned char blockAddr);
    unsigned char _MIFARE_Transfer(unsigned char blockAddr);
    unsigned char _MIFARE_Ultralight_Write(unsigned char page, unsigned char *buffer, unsigned char bufferSize);
    unsigned char _MIFARE_GetValue(unsigned char blockAddr, long *value);
    unsigned char _MIFARE_SetValue(unsigned char blockAddr, long value);

	/////////////////////////////////////////////////////////////////////////////////////
	// Support functions
	/////////////////////////////////////////////////////////////////////////////////////
    unsigned char PCD_MIFARE_Transceive(unsigned char *sendData, unsigned char sendLen, unsigned char acceptTimeout = false);
	// old function used too much memory, now name moved to flash; if you need char, copy from flash to memory
    //const char *GetStatusCodeName(unsigned char code);
    const __FlashStringHelper *GetStatusCodeName(unsigned char code);
    unsigned char PICC_GetType(unsigned char sak);
	// old function used too much memory, now name moved to flash; if you need char, copy from flash to memory
    //const char *PICC_GetTypeName(unsigned char type);
    const __FlashStringHelper *PICC_GetTypeName(unsigned char type);
	void PICC_DumpToSerial(Uid *uid);
    void PICC_DumpMifareClassicToSerial(Uid *uid, unsigned char piccType, MIFARE_Key *key);
    void PICC_DumpMifareClassicSectorToSerial(Uid *uid, MIFARE_Key *key, unsigned char sector);
	void PICC_DumpMifareUltralightToSerial();
    void MIFARE_SetAccessBits(unsigned char *accessBitBuffer, unsigned char g0, unsigned char g1, unsigned char g2, unsigned char g3);
    unsigned char MIFARE_OpenUidBackdoor(unsigned char logErrors);
    unsigned char MIFARE_SetUid(byte* newUid, unsigned char uidSize, unsigned char logErrors);
    unsigned char MIFARE_UnbrickUidSector(unsigned char logErrors);
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Convenience functions - does not add extra functionality
	/////////////////////////////////////////////////////////////////////////////////////
    unsigned char PICC_IsNewCardPresent();
    unsigned char PICC_ReadCardSerial();

    unsigned char MIFARE_TwoStepHelper(unsigned char command, unsigned char blockAddr, long data);


#endif