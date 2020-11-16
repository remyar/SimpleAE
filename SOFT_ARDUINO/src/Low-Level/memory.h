#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <EEPROM.h>

#define ADDR_TR_MIN     0
#define SIZE_TR_MIN     64


void MEMORY_Init(void);

uint8_t MEMORY_ReadByte(uint8_t address);
void MEMORY_WriteByte(uint8_t address, uint8_t val);
uint16_t MEMORY_ReadUnsignedInt(uint8_t address);
void MEMORY_WriteUnsignedInt(uint8_t address, uint16_t val);
float MEMORY_ReadFloat(uint16_t address);
void MEMORY_WriteFloat(uint16_t address, float val);
#endif